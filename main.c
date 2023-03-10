/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC 4: MSCLP CAPSENSE&trade; Low Power
*              Example for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2021-2022 Cypress Semiconductor $
*******************************************************************************/

/*******************************************************************************
 * Include header files
 ******************************************************************************/

#include "cy_pdl.h"
#include "cybsp.h"
#include "cycfg.h"
#include "cycfg_capsense.h"
#include "SpiMaster.h"
#include "LEDcontrol.h"

/*******************************************************************************
* User configurable Macros
********************************************************************************/

/* Enable this, if Serial LED needs to be enabled */
#define ENABLE_SPI_SERIAL_LED            (1u)

/* Enable this, if Tuner needs to be enabled */
#define ENABLE_TUNER                     (1u)

/* Enable this, if Debugging needs to be enabled -
 *    See Readme for more details.
 *    SWD and Tuner cannot be enabled together as the pins are MUXed in CY8CKIT-040T */
#define SWD_DEBUG_ENABLE                 (0u)

/* 128Hz Refresh rate in Active mode */
#define ACTIVE_MODE_REFRESH_RATE         (128u)

/* 32Hz Refresh rate in Active-Low Refresh rate(ALR) mode */
#define ALR_MODE_REFRESH_RATE            (32u)

/* Timeout to move from ACTIVE mode to ALR mode if there is no user activity */
#define ACTIVE_MODE_TIMEOUT_SEC          (10u)

/* Timeout to move from ALR mode to WOT mode if there is no user activity */
#define ALR_MODE_TIMEOUT_SEC             (5u)

/* Active mode Scan time in us ~= 16us */
#define ACTIVE_MODE_FRAME_SCAN_TIME      (16u)
/* Active mode Processing time in us ~= 25us with Serial LED and Tuner disabled
 * and 179us with Serial LED and Tuner enabled */
#define ACTIVE_MODE_PROCESS_TIME         (25u)

/* ALR mode Scan time in us ~= 16us */
#define ALR_MODE_FRAME_SCAN_TIME         (16u)
/* ALR mode Processing time in us ~= 25us with Serial LED and Tuner disabled
 * and 179us with Serial LED and Tuner enabled */
#define ALR_MODE_PROCESS_TIME            (25u)

/*******************************************************************************
* Macros
********************************************************************************/
#define CAPSENSE_MSC0_INTR_PRIORITY      (3u)
#define CY_ASSERT_FAILED                 (0u)

/* Setting self-cap sensors raw-count calibration percentage to 70% */
#define CSD_CALIBRATION_LEVEL            (70u)

/* setting recommended CDAC Dither scale value. Default is '0u' */
#define    CDAC_DITHER_SCALE             (1u)

/* setting recommended CDAC Dither seed value. Default is '255u' */
#define    CDAC_DITHER_SEED              (15u)

/* setting recommended CDAC Dither poly value. Default is '142u' */
#define    CDAC_DITHER_POLY              (9u)

/* Setting SWD_DEBUG_ENABLE to 0 enabled EZI2C */
#define ENABLE_EZI2C                     (!SWD_DEBUG_ENABLE)

#if ENABLE_EZI2C
/* EZI2C interrupt priority must be higher than CAPSENSE&trade; interrupt. */
#define EZI2C_INTR_PRIORITY              (2u)
#endif

#define ILO_FREQ                         (40000u)
#define TIME_IN_US                       (1000000u)

#define MINIMUM_TIMER                    (TIME_IN_US / ILO_FREQ)
#if ((TIME_IN_US / ACTIVE_MODE_REFRESH_RATE) > (ACTIVE_MODE_FRAME_SCAN_TIME + ACTIVE_MODE_PROCESS_TIME))
    #define ACTIVE_MODE_TIMER            (TIME_IN_US / ACTIVE_MODE_REFRESH_RATE - \
                                            (ACTIVE_MODE_FRAME_SCAN_TIME + ACTIVE_MODE_PROCESS_TIME))
#elif
    #define ACTIVE_MODE_TIMER            (MINIMUM_TIMER)
#endif

#if ((TIME_IN_US / ALR_MODE_REFRESH_RATE) > (ALR_MODE_FRAME_SCAN_TIME + ALR_MODE_PROCESS_TIME))
    #define ALR_MODE_TIMER               (TIME_IN_US / ALR_MODE_REFRESH_RATE - \
                                            (ALR_MODE_FRAME_SCAN_TIME + ALR_MODE_PROCESS_TIME))
#elif
    #define ALR_MODE_TIMER               (MINIMUM_TIMER)
#endif

#define ACTIVE_MODE_TIMEOUT              (ACTIVE_MODE_REFRESH_RATE * ACTIVE_MODE_TIMEOUT_SEC)

#define ALR_MODE_TIMEOUT                 (ALR_MODE_REFRESH_RATE * ALR_MODE_TIMEOUT_SEC)

#define TIMEOUT_RESET                    (0u)

/*****************************************************************************
* Finite state machine states for device operating states
*****************************************************************************/
typedef enum
{
    ACTIVE_MODE = 0x01u,    /* Active mode - All the sensors are scanned in this state
                            * with highest refresh rate */
    ALR_MODE = 0x02u,       /* Active-Low Refresh Rate (ALR) mode - All the sensors are
                            * scanned in this state with low refresh rate */
    WOT_MODE = 0x03u        /* Wake on Touch (WoT) mode - Low Power sensors are scanned
                            * in this state with lowest refresh rate */
} APPLICATION_STATE;

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void initialize_capsense(void);
static void set_dither_parameters(void);
static void capsense_msc0_isr(void);

#if ENABLE_EZI2C
static void ezi2c_isr(void);
static void initialize_capsense_tuner(void);
#endif

#if ENABLE_SPI_SERIAL_LED
void led_control();
#endif

/* Deep Sleep Callback function */
void register_callback(void);
cy_en_syspm_status_t deep_sleep_callback(cy_stc_syspm_callback_params_t *callbackParams,
                                         cy_en_syspm_callback_mode_t mode);

/*******************************************************************************
* Global Definitions
*******************************************************************************/

/* Variables holds the current low power state [ACTIVE, ALR or WOT] */
APPLICATION_STATE capsense_state;

#if ENABLE_EZI2C
cy_stc_scb_ezi2c_context_t ezi2c_context;
#endif

#if ENABLE_SPI_SERIAL_LED
extern cy_stc_scb_spi_context_t CYBSP_MASTER_SPI_context;
stc_serial_led_context_t led_context;
#endif

/* Callback parameters for custom, EzI2C, SPI */

#if ENABLE_EZI2C
/* Callback parameters for EzI2C */
cy_stc_syspm_callback_params_t ezi2cCallbackParams =
{
    .base       = SCB1,
    .context    = &ezi2c_context
};
#endif

#if ENABLE_SPI_SERIAL_LED
/* Callback parameters for SPI */
cy_stc_syspm_callback_params_t spiCallbackParams =
{
    .base       = SCB0,
    .context    = &CYBSP_MASTER_SPI_context
};
#endif

/* Callback parameters for custom callback */
cy_stc_syspm_callback_params_t deepSleepCallBackParams = {
    .base       =  NULL,
    .context    =  NULL
};

#if ENABLE_EZI2C
/* Callback declaration for EzI2C Deep Sleep callback */
cy_stc_syspm_callback_t ezi2cCallback =
{
    .callback       = (Cy_SysPmCallback)&Cy_SCB_EZI2C_DeepSleepCallback,
    .type           = CY_SYSPM_DEEPSLEEP,
    .skipMode       = 0UL,
    .callbackParams = &ezi2cCallbackParams,
    .prevItm        = NULL,
    .nextItm        = NULL,
    .order          = 0
};
#endif

#if ENABLE_SPI_SERIAL_LED
/* Callback declaration for SPI Deep Sleep callback */
cy_stc_syspm_callback_t spiCallback =
{
    .callback       = (Cy_SysPmCallback)&Cy_SCB_SPI_DeepSleepCallback,
    .type           = CY_SYSPM_DEEPSLEEP,
    .skipMode       = 0UL,
    .callbackParams = &spiCallbackParams,
    .prevItm        = NULL,
    .nextItm        = NULL,
    .order          = 1
};
#endif

/* Callback declaration for Custom Deep Sleep callback */
cy_stc_syspm_callback_t deepSleepCb =
{
    .callback       = &deep_sleep_callback,
    .type           = CY_SYSPM_DEEPSLEEP,
    .skipMode       = 0UL,
    .callbackParams = &deepSleepCallBackParams,
    .prevItm        = NULL,
    .nextItm        = NULL,
    .order          = 2
};

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  System entrance point. This function performs
*  - initial setup of device
*  - initialize CAPSENSE&trade;
*  - initialize tuner communication
*  - scan touch input continuously at 3 different power modes
*  - serial RGB LED for touch indication
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint32_t capsense_state_timeout;
    uint32_t interruptStatus;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(CY_ASSERT_FAILED);
    }

    /* Enable global interrupts */
    __enable_irq();

    #if ENABLE_EZI2C
    /* Initialize EZI2C */
    initialize_capsense_tuner();
    #endif

    #if ENABLE_SPI_SERIAL_LED
    /* Initialize SPI master */
    result = init_spi_master();

    /* Initialization failed. Stop program execution */
    if(result != INIT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    #else

    /* SPI pins drive mode to Analog HighZ */
    Cy_GPIO_SetDrivemode(CYBSP_SERIAL_LED_PORT, CYBSP_SERIAL_LED_NUM, CY_GPIO_DM_ANALOG);
    #endif

    /* Register callbacks */
    register_callback();

    /* Define initial state of the device and the corresponding refresh rate*/
    capsense_state = ACTIVE_MODE;
    capsense_state_timeout = ACTIVE_MODE_TIMEOUT;

    /* Initialize MSC CAPSENSE&trade; */
    initialize_capsense();

    /* Measures the actual ILO frequency and compensate MSCLP wake up timers */
    Cy_CapSense_IloCompensate(&cy_capsense_context);

    /* Configure the MSCLP wake up timer as per the ACTIVE mode refresh rate */
    Cy_CapSense_ConfigureMsclpTimer(ACTIVE_MODE_TIMER, &cy_capsense_context);

    for (;;)
    {
        switch(capsense_state)
        {
            case ACTIVE_MODE:

                Cy_CapSense_ScanAllSlots(&cy_capsense_context);

                interruptStatus = Cy_SysLib_EnterCriticalSection();

                while (Cy_CapSense_IsBusy(&cy_capsense_context))
                {
                    Cy_SysPm_CpuEnterDeepSleep();

                    Cy_SysLib_ExitCriticalSection(interruptStatus);

                    /* This is a place where all interrupt handlers will be executed */
                    interruptStatus = Cy_SysLib_EnterCriticalSection();
                }

                Cy_SysLib_ExitCriticalSection(interruptStatus);

                Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);

                /* Scan, process and check the status of the all Active mode sensors */
                if(Cy_CapSense_IsAnyWidgetActive(&cy_capsense_context))
                {
                    capsense_state_timeout = ACTIVE_MODE_TIMEOUT;
                }
                else
                {
                    capsense_state_timeout--;

                    if(TIMEOUT_RESET == capsense_state_timeout)
                    {
                        capsense_state = ALR_MODE;
                        capsense_state_timeout = ALR_MODE_TIMEOUT;

                        /* Configure the MSCLP wake up timer as per the ALR mode refresh rate */
                        Cy_CapSense_ConfigureMsclpTimer(ALR_MODE_TIMER, &cy_capsense_context);
                    }
                }

                #if ENABLE_SPI_SERIAL_LED
                /* Serial LED control for showing the CAPSENSE&trade; touch status and power mode */
                led_control();
                #endif

                break;
                /* End of ACTIVE_MODE */

            /* Active Low Refresh-rate Mode */
            case ALR_MODE :

                Cy_CapSense_ScanAllSlots(&cy_capsense_context);

                interruptStatus = Cy_SysLib_EnterCriticalSection();

                while (Cy_CapSense_IsBusy(&cy_capsense_context))
                {
                    Cy_SysPm_CpuEnterDeepSleep();

                    Cy_SysLib_ExitCriticalSection(interruptStatus);

                    /* This is a place where all interrupt handlers will be executed */
                    interruptStatus = Cy_SysLib_EnterCriticalSection();
                }

                Cy_SysLib_ExitCriticalSection(interruptStatus);

                Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);

                /* Scan, process and check the status of the all Active mode sensors */
                if(Cy_CapSense_IsAnyWidgetActive(&cy_capsense_context))
                {
                    capsense_state = ACTIVE_MODE;
                    capsense_state_timeout = ACTIVE_MODE_TIMEOUT;

                    /* Configure the MSCLP wake up timer as per the ACTIVE mode refresh rate */
                    Cy_CapSense_ConfigureMsclpTimer(ACTIVE_MODE_TIMER, &cy_capsense_context);
                }
                else
                {
                    capsense_state_timeout--;

                    if(TIMEOUT_RESET == capsense_state_timeout)
                    {
                        capsense_state = WOT_MODE;

                        #if ENABLE_SPI_SERIAL_LED
                        /* Serial LED control for showing the CAPSENSE&trade; power mode change to WOT */
                        led_control();
                        #endif
                    }
                }

                break;
                /* End of Active-Low Refresh Rate(ALR) mode */

            /* Wake On Touch Mode */
            case WOT_MODE :

                Cy_CapSense_ScanAllLpSlots(&cy_capsense_context);

                interruptStatus = Cy_SysLib_EnterCriticalSection();

                while (Cy_CapSense_IsBusy(&cy_capsense_context))
                {
                    Cy_SysPm_CpuEnterDeepSleep();

                    Cy_SysLib_ExitCriticalSection(interruptStatus);

                    /* This is a place where all interrupt handlers will be executed */
                    interruptStatus = Cy_SysLib_EnterCriticalSection();
                }

                Cy_SysLib_ExitCriticalSection(interruptStatus);

                if (Cy_CapSense_IsAnyLpWidgetActive(&cy_capsense_context))
                {
                    capsense_state = ACTIVE_MODE;
                    capsense_state_timeout = ACTIVE_MODE_TIMEOUT;

                    /* Configure the MSCLP wake up timer as per the ACTIVE mode refresh rate */
                    Cy_CapSense_ConfigureMsclpTimer(ACTIVE_MODE_TIMER, &cy_capsense_context);
                }
                else
                {
                    capsense_state = ALR_MODE;
                    capsense_state_timeout = ALR_MODE_TIMEOUT;

                    /* Configure the MSCLP wake up timer as per the ALR mode refresh rate */
                    Cy_CapSense_ConfigureMsclpTimer(ALR_MODE_TIMER, &cy_capsense_context);

                    #if ENABLE_SPI_SERIAL_LED
                    /* Serial LED control for showing the CAPSENSE&trade; power mode change to ALR */
                    led_control();
                    #endif
                }

                break;
                /* End of "WAKE_ON_TOUCH_MODE" */

            default:
                /**  Unknown power mode state. Unexpected situation.  **/
                CY_ASSERT(CY_ASSERT_FAILED);
                break;
        }
        #if ENABLE_TUNER
        /* Establishes synchronized communication with the CAPSENSE&trade; Tuner tool */
        Cy_CapSense_RunTuner(&cy_capsense_context);
        #endif
    }
}

/*******************************************************************************
* Function Name: initialize_capsense
********************************************************************************
* Summary:
*  This function initializes the CAPSENSE&trade; and configures the CAPSENSE&trade;
*  interrupt.
*
*******************************************************************************/
static void initialize_capsense(void)
{
    cy_capsense_status_t status = CY_CAPSENSE_STATUS_SUCCESS;

    /* CAPSENSE&trade; interrupt configuration MSCLP 0 */
    const cy_stc_sysint_t capsense_msc0_interrupt_config =
    {
        .intrSrc = CY_MSCLP0_LP_IRQ,
        .intrPriority = CAPSENSE_MSC0_INTR_PRIORITY,
    };

    /* Capture the MSC HW block and initialize it to the default state. */
    status = Cy_CapSense_Init(&cy_capsense_context);

    if (CY_CAPSENSE_STATUS_SUCCESS == status)
    {
        /* Initialize CAPSENSE&trade; interrupt for MSC 0 */
        Cy_SysInt_Init(&capsense_msc0_interrupt_config, capsense_msc0_isr);
        NVIC_ClearPendingIRQ(capsense_msc0_interrupt_config.intrSrc);
        NVIC_EnableIRQ(capsense_msc0_interrupt_config.intrSrc);

        /* Setting calibration percentage*/
        Cy_CapSense_SetCalibrationTarget(CSD_CALIBRATION_LEVEL, CY_CAPSENSE_CSD_GROUP, &cy_capsense_context);

        /* setting Dither parameter
         * Must be called after Cy_CapSense_Init() and before Cy_CapSense_Enable()
         */
        set_dither_parameters();

        status = Cy_CapSense_Enable(&cy_capsense_context);
    }

    if(status != CY_CAPSENSE_STATUS_SUCCESS)
    {
        /* This status could fail before tuning the sensors correctly.
         * Ensure that this function passes after the CAPSENSE&trade; sensors are tuned
         * as per procedure give in the Readme.md file */
    }
}

/*******************************************************************************
* Function Name: capsense_msc0_isr
********************************************************************************
* Summary:
*  Wrapper function for handling interrupts from CAPSENSE&trade; MSC0 block.
*
*******************************************************************************/
static void capsense_msc0_isr(void)
{
    Cy_CapSense_InterruptHandler(CY_MSCLP0_HW, &cy_capsense_context);
}

/*******************************************************************************
* Function Name: set_dither_parameters
********************************************************************************
* Summary:
*  This functions sets the below CDAC Dither parameters to achive better performance
*  1. CDAC_Dither_Scale
*       - Default value is '0'
*       - Recommended value defined in macro 'CDAC_DITHER_SCALE'
*  2. CDAC_Dither_poly
*       - Default value is '142'
*       - Recommended value defined in macro 'CDAC_DITHER_POLY'
*  3. CDAC_Dither_Seed
*       - Default value is '255'
*       - Recommended value defined in macro 'CDAC_DITHER_SEED'
*
*  Note : Must be called after Cy_CapSense_Init() and before Cy_CapSense_Enable
*
*  Refer CE Readme for more details
*  Parameters:  void
*  Return:  void
*******************************************************************************/
static void set_dither_parameters(void)
{
    uint32_t wdIndex;

    /* set Dither scale for each widgets*/
    for (wdIndex = 0u; wdIndex < CY_CAPSENSE_TOTAL_WIDGET_COUNT; wdIndex++)
    {
        cy_capsense_context.ptrWdContext[wdIndex].cdacDitherValue = CDAC_DITHER_SCALE;
    }

    /* set Dither poly for all widgets*/
    cy_capsense_context.ptrInternalContext->cdacDitherPoly = CDAC_DITHER_POLY;

    /* set Dither seed for all widgets*/
    cy_capsense_context.ptrInternalContext->cdacDitherSeed = CDAC_DITHER_SEED;
}

#if ENABLE_EZI2C
/*******************************************************************************
* Function Name: initialize_capsense_tuner
********************************************************************************
* Summary:
*  EZI2C module to communicate with the CAPSENSE&trade; Tuner tool.
*
*******************************************************************************/
static void initialize_capsense_tuner(void)
{
    cy_en_scb_ezi2c_status_t status = CY_SCB_EZI2C_SUCCESS;

    /* EZI2C interrupt configuration structure */
    const cy_stc_sysint_t ezi2c_intr_config =
    {
        .intrSrc = CYBSP_EZI2C_IRQ,
        .intrPriority = EZI2C_INTR_PRIORITY,
    };

    /* Initialize the EzI2C firmware module */
    status = Cy_SCB_EZI2C_Init(CYBSP_EZI2C_HW, &CYBSP_EZI2C_config, &ezi2c_context);

    if(status != CY_SCB_EZI2C_SUCCESS)
    {
        CY_ASSERT(CY_ASSERT_FAILED);
    }

    Cy_SysInt_Init(&ezi2c_intr_config, ezi2c_isr);
    NVIC_EnableIRQ(ezi2c_intr_config.intrSrc);

    /* Set the CAPSENSE&trade; data structure as the I2C buffer to be exposed to the
     * master on primary slave address interface. Any I2C host tools such as
     * the Tuner or the Bridge Control Panel can read this buffer but you can
     * connect only one tool at a time.
     */
    Cy_SCB_EZI2C_SetBuffer1(CYBSP_EZI2C_HW, (uint8_t *)&cy_capsense_tuner,
                            sizeof(cy_capsense_tuner), sizeof(cy_capsense_tuner),
                            &ezi2c_context);

    Cy_SCB_EZI2C_Enable(CYBSP_EZI2C_HW);

}

/*******************************************************************************
* Function Name: ezi2c_isr
********************************************************************************
* Summary:
*  Wrapper function for handling interrupts from EZI2C block.
*
*******************************************************************************/
static void ezi2c_isr(void)
{
    Cy_SCB_EZI2C_Interrupt(CYBSP_EZI2C_HW, &ezi2c_context);
}

#endif

#if ENABLE_SPI_SERIAL_LED
/*******************************************************************************
* Function Name: led_control
********************************************************************************
* Summary:
*  Control LED1 in the kit to show the button status:
*    No touch - LED1 == OFF
*    Touch - LED1 == RED
*
*  Control LED2 in the kit to show the device power mode:
*    Active mode - LED2 == RED
*    ALR mode - LED2 == YELLOW
*    WOT mode - LED2 == GREEN
*
*  LED3 is not used and always OFF
*
* Parameters:
*  DEVICE_STATE - device power mode state
*******************************************************************************/
void led_control()
{
    uint8_t brightness_red = 200u;
    uint8_t brightness_green = 200u;

    /* LED1 Control: Check the status of Active mode sensors and control LED1 */
    if(Cy_CapSense_IsAnyWidgetActive(&cy_capsense_context))
    {
        /* LED1 Turns on (RED color) when there is touch detected */
        led_context.led_num[LED1].color_red = brightness_red;
        led_context.led_num[LED1].color_green = 0u;
        led_context.led_num[LED1].color_blue = 0u;
    }
    else
    {
        /* LED1 Turns OFF when there is no touch detected */
        led_context.led_num[LED1].color_red = 0u;
        led_context.led_num[LED1].color_green = 0u;
        led_context.led_num[LED1].color_blue = 0u;
    }

    /* LED2 Control: Based on application power mode */
    switch(capsense_state){
        case ACTIVE_MODE:

            /* LED2 Turns on (RED color) while in ACTIVE mode */
            led_context.led_num[LED2].color_red = brightness_red;
            led_context.led_num[LED2].color_green = 0u;
            led_context.led_num[LED2].color_blue = 0u;

            break;

        case ALR_MODE:

            /* LED2 Turns on (YELLOW color) while in ALR mode */
            led_context.led_num[LED2].color_red = brightness_red >> 1u;
            led_context.led_num[LED2].color_green = brightness_green >> 1u;
            led_context.led_num[LED2].color_blue = 0u;

            break;

        case WOT_MODE:

            /* LED2 Turns on (GREEN color) while in WOT mode */
            led_context.led_num[LED2].color_red = 0u;
            led_context.led_num[LED2].color_green = brightness_green;
            led_context.led_num[LED2].color_blue = 0u;

            break;

        default:
            /* Unknown power mode state. Unexpected situation. */
            CY_ASSERT(CY_ASSERT_FAILED);
            break;
    }

    /* LED3 is not used for this project */
    led_context.led_num[LED3].color_red = 0u;
    led_context.led_num[LED3].color_green = 0u;
    led_context.led_num[LED3].color_blue = 0u;

    serial_led_control(&led_context);
}
#endif

/*******************************************************************************
* Function Name: register_callback
********************************************************************************
*
* Summary:
*  Register Deep Sleep callbacks for EzI2C, SPI components
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void register_callback(void)
{
    #if ENABLE_EZI2C
    /* Register EzI2C Deep Sleep callback */
    Cy_SysPm_RegisterCallback(&ezi2cCallback);
    #endif

    #if ENABLE_SPI_SERIAL_LED
    /* Register SPI Deep Sleep callback */
    Cy_SysPm_RegisterCallback(&spiCallback);
    #endif

    /* Register Deep Sleep callback */
    Cy_SysPm_RegisterCallback(&deepSleepCb);
}

/*******************************************************************************
* Function Name: deep_sleep_callback
********************************************************************************
*
* Summary:
* Deep Sleep callback implementation. Waits for the completion of SPI transaction.
* And change the SPI GPIOs to highZ while transition to deep-sleep and vice-versa
*
* Parameters:
*  callbackParams: The pointer to the callback parameters structure cy_stc_syspm_callback_params_t.
*  mode: Callback mode, see cy_en_syspm_callback_mode_t
*
* Return:
*  Entered status, see cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t deep_sleep_callback(
        cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode)
{
    cy_en_syspm_status_t ret_val = CY_SYSPM_FAIL;

    switch (mode)
    {
        case CY_SYSPM_CHECK_READY:

            ret_val = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_CHECK_FAIL:

            ret_val = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_BEFORE_TRANSITION:

            #if ENABLE_SPI_SERIAL_LED
            /* SPI pins drive mode to Analog HighZ */
            Cy_GPIO_SetDrivemode(CYBSP_SPI_MOSI_PORT, CYBSP_SPI_MOSI_PIN, CY_GPIO_DM_ANALOG);
            #endif

            ret_val = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_AFTER_TRANSITION:

            #if ENABLE_SPI_SERIAL_LED
            /* SPI pins drive mode to Strong */
            Cy_GPIO_SetDrivemode(CYBSP_SPI_MOSI_PORT, CYBSP_SPI_MOSI_PIN, CY_GPIO_DM_STRONG_IN_OFF);
            #endif

            ret_val = CY_SYSPM_SUCCESS;
            break;

        default:
            /* Don't do anything in the other modes */
            ret_val = CY_SYSPM_SUCCESS;
            break;
    }
    return ret_val;
}

/* [] END OF FILE */
