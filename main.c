/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC 4: MSCLP CAPSENSE Low Power
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

/* Scan time in ms ~=0.1ms */
#define ACTIVE_MODE_FRAME_SCAN_TIME      (0u)

/* Scan time in ms ~=0.1ms */
#define ALR_MODE_FRAME_SCAN_TIME         (0u)

/* Measured ILO frequency */
#define MEASURED_ILO                     (40u)

/*******************************************************************************
* Macros
********************************************************************************/
#define CAPSENSE_MSC0_INTR_PRIORITY      (3u)
#define CY_ASSERT_FAILED                 (0u)

/* Setting SWD_DEBUG_ENABLE to 0 enabled EZI2C */
#define ENABLE_EZI2C                     (!SWD_DEBUG_ENABLE & ENABLE_TUNER)

#if ENABLE_EZI2C
/* EZI2C interrupt priority must be higher than CAPSENSE interrupt. */
#define EZI2C_INTR_PRIORITY              (2u)
#endif

#define TIME_IN_MS                       (1000u)

/* Expected ILO frequency */
#define EXPECTED_ILO                     (40u)

/* 128Hz Refresh rate in Active mode */
#define ACTIVE_MODE_REFRESH_RATE_CORRECTED    (ACTIVE_MODE_REFRESH_RATE * EXPECTED_ILO / MEASURED_ILO)
/* 32Hz Refresh rate in Active-Low Refresh rate(ALR) mode */
#define ALR_MODE_REFRESH_RATE_CORRECTED  (ALR_MODE_REFRESH_RATE * EXPECTED_ILO / MEASURED_ILO )

#if ((TIME_IN_MS / ACTIVE_MODE_REFRESH_RATE) > ACTIVE_MODE_FRAME_SCAN_TIME)
    #define ACTIVE_MODE_TIMER            (TIME_IN_MS / ACTIVE_MODE_REFRESH_RATE_CORRECTED - ACTIVE_MODE_FRAME_SCAN_TIME)
#elif
    #define ACTIVE_MODE_TIMER            (0u)
#endif

#if ((TIME_IN_MS / ALR_MODE_REFRESH_RATE) > ALR_MODE_FRAME_SCAN_TIME)
    #define ALR_MODE_TIMER               (TIME_IN_MS / ALR_MODE_REFRESH_RATE_CORRECTED - ALR_MODE_FRAME_SCAN_TIME)
#elif
    #define ALR_MODE_TIMER               (0u)
#endif

#define ACTIVE_MODE_TIMEOUT              (ACTIVE_MODE_REFRESH_RATE_CORRECTED * ACTIVE_MODE_TIMEOUT_SEC)

#define ALR_MODE_TIMEOUT                 (ALR_MODE_REFRESH_RATE_CORRECTED * ALR_MODE_TIMEOUT_SEC)


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
static void capsense_msc0_isr(void);

#if ENABLE_EZI2C
static void ezi2c_isr(void);
static void initialize_capsense_tuner(void);
static void tuner_wrapper(void);
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
*  - initialize CAPSENSE
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
    #elif !SWD_DEBUG_ENABLE
    /* EZI2C pins drive mode to Analog HighZ */
    Cy_GPIO_SetDrivemode(CYBSP_I2C_SDA_PORT, CYBSP_I2C_SDA_PIN, CY_GPIO_DM_ANALOG);
    Cy_GPIO_SetDrivemode(CYBSP_I2C_SCL_PORT, CYBSP_I2C_SCL_PIN, CY_GPIO_DM_ANALOG);
    #endif

    /* Initialize MSC CAPSENSE */
    initialize_capsense();

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
    capsense_state_timeout = 0u;

    /* Configure the MSCLP wake up timer as per the ACTIVE mode refresh rate */
    Cy_CapSense_ConfigureMsclpTimer(ACTIVE_MODE_TIMER, &cy_capsense_context);

    for (;;)
    {
        switch(capsense_state)
        {
            case ACTIVE_MODE:

                Cy_CapSense_ScanAllSlots(&cy_capsense_context);

                while (Cy_CapSense_IsBusy(&cy_capsense_context))
                {
                    Cy_SysPm_CpuEnterDeepSleep ();
                }

                Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);

                #if ENABLE_SPI_SERIAL_LED
                /* Serial LED control for showing the CAPSENSE touch status and power mode */
                led_control();
                #endif

                #if ENABLE_EZI2C
                /* Establishes synchronized communication with the CAPSENSE Tuner tool */
                tuner_wrapper();
                #endif

                /* Scan, process and check the status of the all Active mode sensors */
                if(Cy_CapSense_IsAnyWidgetActive(&cy_capsense_context))
                {
                    capsense_state_timeout = 0u;
                }
                else
                {
                    capsense_state_timeout++;

                    if(ACTIVE_MODE_TIMEOUT < capsense_state_timeout)
                    {
                        capsense_state = ALR_MODE;
                        capsense_state_timeout = 0;

                        /* Configure the MSCLP wake up timer as per the ALR mode refresh rate */
                        Cy_CapSense_ConfigureMsclpTimer(ALR_MODE_TIMER, &cy_capsense_context);
                    }
                }

                break;
                /* End of ACTIVE_MODE */

            /* Active Low Refresh-rate Mode */
            case ALR_MODE :

                Cy_CapSense_ScanAllSlots(&cy_capsense_context);

                while (Cy_CapSense_IsBusy(&cy_capsense_context))
                {
                    Cy_SysPm_CpuEnterDeepSleep ();
                }

                Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);

                #if ENABLE_SPI_SERIAL_LED
                /* Serial LED control for showing the CAPSENSE touch status and power mode */
                led_control();
                #endif

                #if ENABLE_EZI2C
                /* Establishes synchronized communication with the CAPSENSE Tuner tool */
                tuner_wrapper();
                #endif

                /* Scan, process and check the status of the all Active mode sensors */
                if(Cy_CapSense_IsAnyWidgetActive(&cy_capsense_context))
                {
                    capsense_state = ACTIVE_MODE;
                    capsense_state_timeout = 0u;

                    /* Configure the MSCLP wake up timer as per the ACTIVE mode refresh rate */
                    Cy_CapSense_ConfigureMsclpTimer(ACTIVE_MODE_TIMER, &cy_capsense_context);
                }
                else
                {
                    capsense_state_timeout++;

                    if(ALR_MODE_TIMEOUT < capsense_state_timeout)
                    {
                        capsense_state = WOT_MODE;
                        capsense_state_timeout = 0;

                        /* Disabling CIC2 filter for WOT mode sensors */
                        cy_capsense_context.ptrCommonConfig->ptrChConfig->ptrHwBase->FILTER_CTL &=
                                    (~(1uL << MSCLP_FILTER_CTL_FILTER_MODE_Pos));
                    }
                }

                break;
                /* End of Active-Low Refresh Rate(ALR) mode */

            /* Wake On Touch Mode */
            case WOT_MODE :

                Cy_CapSense_ScanAllLpSlots(&cy_capsense_context);

                #if ENABLE_SPI_SERIAL_LED
                /* Serial LED control for showing the CAPSENSE touch status and power mode */
                led_control();
                #endif

                while (Cy_CapSense_IsBusy(&cy_capsense_context))
                {
                    Cy_SysPm_CpuEnterDeepSleep ();
                }

                #if ENABLE_EZI2C
                /* Establishes synchronized communication with the CAPSENSE Tuner tool */
                tuner_wrapper();
                #endif

                if (Cy_CapSense_IsAnyLpWidgetActive(&cy_capsense_context))
                {
                    capsense_state = ACTIVE_MODE;
                    capsense_state_timeout = 0;

                    /* Configure the MSCLP wake up timer as per the ACTIVE mode refresh rate */
                    Cy_CapSense_ConfigureMsclpTimer(ACTIVE_MODE_TIMER, &cy_capsense_context);

                    /* Enabling CIC2 filters for ACTIVE mode sensors */
                    cy_capsense_context.ptrCommonConfig->ptrChConfig->ptrHwBase->FILTER_CTL |=
                                (1uL << MSCLP_FILTER_CTL_FILTER_MODE_Pos);
                }
                else
                {
                    capsense_state_timeout = 0;
                    capsense_state = ALR_MODE;

                    /* Configure the MSCLP wake up timer as per the ALR mode refresh rate */
                    Cy_CapSense_ConfigureMsclpTimer(ALR_MODE_TIMER, &cy_capsense_context);

                    /* Enabling CIC2 filters for ALR mode sensors */
                    cy_capsense_context.ptrCommonConfig->ptrChConfig->ptrHwBase->FILTER_CTL |=
                                (1uL << MSCLP_FILTER_CTL_FILTER_MODE_Pos);
                }

                break;
                /* End of "WAKE_ON_TOUCH_MODE" */

            default:
                /**  Unknown power mode state. Unexpected situation.  **/
                CY_ASSERT(CY_ASSERT_FAILED);
                break;

        }
    }
}

/*******************************************************************************
* Function Name: initialize_capsense
********************************************************************************
* Summary:
*  This function initializes the CAPSENSE and configures the CAPSENSE
*  interrupt.
*
*******************************************************************************/
static void initialize_capsense(void)
{
    cy_capsense_status_t status = CY_CAPSENSE_STATUS_SUCCESS;

    /* CapSense interrupt configuration MSCLP 0 */
    const cy_stc_sysint_t capsense_msc0_interrupt_config =
    {
        .intrSrc = CY_MSCLP0_LP_IRQ,
        .intrPriority = CAPSENSE_MSC0_INTR_PRIORITY,
    };

    /* Capture the MSC HW block and initialize it to the default state. */
    status = Cy_CapSense_Init(&cy_capsense_context);

    if (CY_CAPSENSE_STATUS_SUCCESS == status)
    {
        /* Initialize CAPSENSE interrupt for MSC 0 */
        Cy_SysInt_Init(&capsense_msc0_interrupt_config, capsense_msc0_isr);
        NVIC_ClearPendingIRQ(capsense_msc0_interrupt_config.intrSrc);
        NVIC_EnableIRQ(capsense_msc0_interrupt_config.intrSrc);

        /* Setting calibration percentage to 70 */
        Cy_CapSense_SetCalibrationTarget(70u, CY_CAPSENSE_CSD_GROUP, &cy_capsense_context);

        status = Cy_CapSense_Enable(&cy_capsense_context);

        /* CIC2 filter is not applied for Low power widgets. Refer errata in
         * PSoC 4000T Datasheet for more details.
         * Disabling CIC2 filter to auto-calibrate low power sensors */
        cy_capsense_context.ptrCommonConfig->ptrChConfig->ptrHwBase->FILTER_CTL &=
                    ~(1uL << MSCLP_FILTER_CTL_FILTER_MODE_Pos);
        /* Set Maximum rawcount for low power widget calibration with CIC1 filter */
        CY_CAPSENSE_LOWPOWER0_MAX_RAW_COUNT_VALUE =
                    CY_CAPSENSE_LOWPOWER0_SNS_CLK_VALUE * CY_CAPSENSE_LOWPOWER0_NUM_SUBCONVERSIONS_VALUE;

        /* Calibrate all the low power widgets */
        Cy_CapSense_CalibrateAllLpSlots(&cy_capsense_context);

        /* Sets CIC2 mode for active widgets  only */
        if (WOT_MODE != capsense_state)
        {
            cy_capsense_context.ptrCommonConfig->ptrChConfig->ptrHwBase->FILTER_CTL |=
                        (1uL << MSCLP_FILTER_CTL_FILTER_MODE_Pos);
        }
    }

    if(status != CY_CAPSENSE_STATUS_SUCCESS)
    {
        /* This status could fail before tuning the sensors correctly.
         * Ensure that this function passes after the CAPSENSE sensors are tuned
         * as per procedure give in the Readme.md file */
    }
}

/*******************************************************************************
* Function Name: capsense_msc0_isr
********************************************************************************
* Summary:
*  Wrapper function for handling interrupts from CapSense MSC0 block.
*
*******************************************************************************/
static void capsense_msc0_isr(void)
{
    Cy_CapSense_InterruptHandler(CY_MSCLP0_HW, &cy_capsense_context);
}

#if ENABLE_EZI2C
/*******************************************************************************
* Function Name: initialize_capsense_tuner
********************************************************************************
* Summary:
*  EZI2C module to communicate with the CAPSENSE Tuner tool.
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

    /* Set the CAPSENSE data structure as the I2C buffer to be exposed to the
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

/*******************************************************************************
* Function Name: tuner_wrapper
********************************************************************************
* Summary:
*  Wrapper function for running tuner with low power widget recalibration as per
*  the errata mentioned in device datasheet.
*
*******************************************************************************/
void tuner_wrapper(void)
{
    if (CY_CAPSENSE_STATUS_RESTART_DONE == Cy_CapSense_RunTuner(&cy_capsense_context))
    {
        /* Disabling CIC2 filter to auto-calibrate low power sensors */
        cy_capsense_context.ptrCommonConfig->ptrChConfig->ptrHwBase->FILTER_CTL &=
                    ~(1uL << MSCLP_FILTER_CTL_FILTER_MODE_Pos);

        /* Set Maximum rawcount for low power widget calibration with CIC1 filter */
        CY_CAPSENSE_LOWPOWER0_MAX_RAW_COUNT_VALUE = (CY_CAPSENSE_LOWPOWER0_SNS_CLK_VALUE *
                    CY_CAPSENSE_LOWPOWER0_NUM_SUBCONVERSIONS_VALUE);

        /* Repeats auto-calibration for all the low power widgets */
        Cy_CapSense_CalibrateAllLpSlots(&cy_capsense_context);

        /* Sets CIC2 mode for active widgets only */
        if (WOT_MODE != capsense_state)
        {
            cy_capsense_context.ptrCommonConfig->ptrChConfig->ptrHwBase->FILTER_CTL |=
                        (1uL << MSCLP_FILTER_CTL_FILTER_MODE_Pos);
        }
    }
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
    volatile uint8_t brightness_red = 200u;
    volatile uint8_t brightness_green = 200u;

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
            led_context.led_num[LED2].color_red = brightness_red/2;
            led_context.led_num[LED2].color_green = brightness_green/2;
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

            #if ENABLE_EZI2C
            /* EzI2C pins drive mode to Analog HighZ */
            Cy_GPIO_SetDrivemode(CYBSP_I2C_SDA_PORT, CYBSP_I2C_SDA_PIN, CY_GPIO_DM_ANALOG);
            Cy_GPIO_SetDrivemode(CYBSP_I2C_SCL_PORT, CYBSP_I2C_SCL_PIN, CY_GPIO_DM_ANALOG);
            #endif

            #if ENABLE_SPI_SERIAL_LED
            /* SPI pins drive mode to Analog HighZ */
            Cy_GPIO_SetDrivemode(CYBSP_SPI_MOSI_PORT, CYBSP_SPI_MOSI_PIN, CY_GPIO_DM_ANALOG);
            #endif

            ret_val = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_AFTER_TRANSITION:

            #if ENABLE_EZI2C
            /* EzI2C pins drive mode to Analog HighZ */
            Cy_GPIO_SetDrivemode(CYBSP_I2C_SDA_PORT, CYBSP_I2C_SDA_PIN, CY_GPIO_DM_OD_DRIVESLOW);
            Cy_GPIO_SetDrivemode(CYBSP_I2C_SCL_PORT, CYBSP_I2C_SCL_PIN, CY_GPIO_DM_OD_DRIVESLOW);
            #endif

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
