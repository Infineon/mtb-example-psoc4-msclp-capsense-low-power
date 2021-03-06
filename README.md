# PSoC&trade; 4: MSCLP CAPSENSE&trade; low power

This code example demonstrates an implementation of a low-power application including recommended power states and transitions, tuning parameter adjustments, and the method of tuning. This example uses a single self-capacitance-based button in multi sense CAPSENSE&trade; low power (MSCLP - 5th-generation low-power CAPSENSE&trade;) to demonstrate different considerations to implement a low-power design.

This document also explains how to manually tune the self-capacitance-based low-power widget for optimum performance w.r.t parameters such as power consumption and response time using the CSD-RM sensing technique and CAPSENSE&trade; Tuner.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-psoc4-msclp-capsense-low-power)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzUxMTEiLCJTcGVjIE51bWJlciI6IjAwMi0zNTExMSIsIkRvYyBUaXRsZSI6IlBTb0MmdHJhZGU7IDQ6IE1TQ0xQIENBUFNFTlNFJnRyYWRlOyBsb3cgcG93ZXIiLCJyaWQiOiJqb2JpIiwiRG9jIHZlcnNpb24iOiIxLjAuMSIsIkRvYyBMYW5ndWFnZSI6IkVuZ2xpc2giLCJEb2MgRGl2aXNpb24iOiJNQ0QiLCJEb2MgQlUiOiJJQ1ciLCJEb2MgRmFtaWx5IjoiUFNPQyJ9)

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v3.0 or later

  **Note:** This code example version requires ModusToolbox&trade; software version 3.0 and is not backward compatible with v2.4 or older versions.

- Board support package (BSP) minimum required version: 0.5.0
- Programming language: C
- Associated parts: [PSoC&trade; 4000T](www.infineon.com/002-33949)


## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg embedded compiler v10.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`
- Arm&reg; compiler v6.13 (`ARM`)
- IAR C/C++ compiler v8.42.2 (`IAR`)


## Supported kits (make variable 'TARGET')

- [PSoC&trade; 4000T CAPSENSE&trade; evaluation kit](https://www.infineon.com/CY8CKIT-040T) (`CY8CKIT-040T`) - Default `TARGET`

## Hardware setup

This example uses the board's default configuration. See the [Kit user guide](www.infineon.com/002-34472) to ensure that the board is configured correctly.

## Software setup

This example requires no additional software or tools.

## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox&trade; Application**). This launches the [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. (Optional) Change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the Project Creator as both a GUI tool and the command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

This tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

The following example will clone the "[PSoC&trade; 4: MSCLP CAPSENSE&trade; low power](https://github.com/Infineon/mtb-example-psoc4-msclp-capsense-low-power)" application with the desired name "CAPSENSE_Low_Power" configured for the *CY8CKIT-040T* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CKIT-040T --app-id mtb-example-psoc4-msclp-capsense-low-power --user-app-name CAPSENSE_Low_Power --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP, and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br>

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

The project has the necessary settings by default, so you can go to [Operation](#operation) to test the example. If you want to understand the tuning process and follow the stages for this kit or your own board, go to [Tuning procedure](#tuning-procedure) and then test it using [Operation](#operation).

## Operation

1. Connect the USB cable between the [CY8CKIT-040T kit](https://www.infineon.com/CY8CKIT-040T) and the PC as follows:

   **Figure 1. Connecting the CY8CKIT-040T kit with the PC**

   <img src="images/psoc4000t_kit_connected.png" alt="Figure 1" width="350"/>

2. The SWD, User LED, and I2C modes are multiplexed to the same pins (GPIO pins P3[2] and P3[3]) in CY8CKIT-040T. The interface can be switched between these using FW-loader commands. Download and unzip the *Batch_files_for_mux_selection* zipped file from the Beta package to the *bin* folder inside the *fw-loader* folder in the ModusToolbox&trade; installation directory.

   Default location: */ModusToolbox/tools_3.0/fw-loader/bin*

   Select the SWD mode by running the *Select_SWD* batch file to program the CY8CKIT-040T kit. For detailed information, see Section 2.3.1 in [CY8CKIT-040T kit user guide](www.infineon.com/002-34472).

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade;</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
      </details>

   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. You can specify a target and toolchain manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CY8CKIT-040T TOOLCHAIN=GCC_ARM
      ```
   </details>

4. After programming, the application starts automatically.

5. To test the application, see the LED2 state change with respect to different low-power states based on the user interaction. Place your finger over the CAPSENSE&trade; button and notice that LED1 turns ON when touched, and turns OFF when the finger is lifted.

   **Table 1. LED2 state for different application power modes**

   | Power mode state  | LED2  |
   |:---------------------| :-----|
   | Active  | Red |
   | Active Low Refresh rate <br>(ALR)  | Yellow |
   | Wake on Touch <br>(WoT) | Green  |

6. Verify that the application is transitioning to different power modes based on user input conditions as follows:

   **Figure 2. Low-power mode state machine**

   <img src="images/capsense_lp_firmware_state_machine.png" alt="Figure 2" width="400"/>

### Monitor data using CAPSENSE&trade; Tuner

1. Select the I2C mode by running the *Select_I2C* batch file to enable the I2C communication between onboard KitProg and CAPSENSE&trade; Tuner.

2. Open CAPSENSE&trade; Tuner from the 'Tools' section in the IDE Quick Panel.

   You can also run the CAPSENSE&trade; Tuner application standalone from *{ModusToolbox&trade; install directory}/ModusToolbox/tools_{version}/capsense-configurator/capsense-tuner*. In this case, after opening the application, select **File** > **Open** and open the *design.cycapsense* file of the respective application, which is present in the *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/COMPONENT_BSP_DESIGN_MODUS/* folder.

   See the [ModusToolbox&trade; user guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *ModusToolbox install directory}/docs_{version}/mtb_user_guide.pdf*)for options to open the CAPSENSE&trade; tuner application using the CLI.

3. Ensure that the kit is in CMSIS-DAP Bulk mode (KitProg3 Status LED is ON and not blinking). See [Firmware-loader](https://github.com/Infineon/Firmware-loader) to learn on how to update the firmware and switch modes in KitProg3.

4. In the tuner application, click on the **Tuner communication setup** icon or select **Tools** > **Tuner communication setup**. In the window that appears, select the I2C checkbox under KitProg3 and configure as follows:

   - I2C address: 8
   - Sub-address: 2 bytes
   - Speed (kHz): 400

   These are the same values set in the EZI2C resource.

   **Figure 3. Tuner communication setup parameters**

   <img src="images/tuner-comm-setup.png" alt="Figure 3" width="550"/>

5. Click **Connect** or select **Communication** > **Connect** to establish a connection.

   **Figure 4. Establish connection**

   <img src="images/tuner-connect.png" alt="Figure 4" width="300" />

6. Click **Start** or select **Communication** > **Start** to start data streaming from the device.

   **Figure 5. Start tuner communication**

   <img src="images/tuner-start.png" alt="Figure 5" width="300" />

   The *Widget/Sensor Parameters* tab is updated with the parameters configured in the *CAPSENSE&trade; Configurator* window. The tuner displays the data from the sensor in the **Widget View** and **Graph View** tabs.

7. Set the **Read mode** to Synchronized mode. Navigate to the **Widget view** tab and notice that the **Button0** widget is highlighted in blue when you touch it.

   **Figure 6. Widget view of the CAPSENSE&trade; Tuner**

   <img src="images/tuner-widget-view.png" alt="Figure 6" width="750"/>

8. Go to the **Graph View** tab to view the raw count, baseline, difference count and status for each sensor. Observe that the low-power widget sensor (**LowPower0_Sns0**) raw count is plotted once the device completes the full frame scan (or detects a touch) in **WOT** mode and moved to **Active/ALR** mode.

   **Figure 7. Graph view of the CAPSENSE&trade; Tuner**

   <img src="images/tuner-graph-view-intro.png" alt="Figure 7" width="750"/>

9. Observe the **Widget/Sensor parameters** section in the CAPSENSE&trade; Tuner window as shown in Figure 7.

10. Switch to the **SNR Measurement** tab for measuring the SNR and verify that the SNR is above 5:1 and Signal count is above 50; select **Button0** and **Button0_Sns0** sensor, and then click **Acquire noise** as follows:

    **Figure 8. CAPSENSE&trade; Tuner - SNR measurement: acquire noise**

    <img src="images/tuner-acquire-noise.png" alt="Figure 8" width="750"/>

      **Note:** Because the scan refresh rate is lower in **ALR** mode, it will take more time to acquire noise. Touch the CAPSENSE&trade; button before clicking **Acquire noise** to transition the device to **ACTIVE** mode to get signal faster.

11. Once the noise is acquired, place the metal finger (6 mm is used in this example) at a position on the button and then click **Acquire signal**. Ensure that the metal finger remains on the button as long as the signal acquisition is in progress. Observe the SNR is above 5:1 and Signal count is above 50.

    The calculated SNR on this button is displayed, as Figure 9 shows. Based on your end system design, test the signal with a finger that matches the size of your normal use case. Typically, finger size targets are 4 mm to 8 mm. Consider testing with smaller sizes that should be rejected by the system to ensure that they do not reach the finger threshold.

    **Figure 9. CAPSENSE&trade; Tuner - SNR measurement: acquire signal**

    <img src="images/tuner-acquire-signal.png" alt="Figure 9" width="750"/>

12. To measure the SNR of the low-power sensor (**LowPower0_Sns0**), set the **Finger threshold** to max (65535) in **Widget/Sensor Parameters** as shown in Figure 10 for both **Button0** and **LowPower0** widgets. This is required as the application will stop scanning the low-power sensor when there is a touch detected and transition to Active mode.

      **Figure 10. CAPSENSE&trade; update finger threshold**

      <img src="images/tuner-threshold-update.png" alt="Figure 10" width="750"/>

13. Repeat steps 10 and 11 to observe the SNR and signal as Figure 11 shows.

       **Figure 11. CAPSENSE&trade; Tuner - SNR measurement: low-power widget**

       <img src="images/tuner-lowpower-snr.png" alt="Figure 11" width="750"/>


### Measure current at different power modes

1. Disable the serial LED and tuner functions to measure the current used for CAPSENSE&trade; sensing in each power mode.

   Disable the `ENABLE_SPI_SERIAL_LED` and `ENABLE_TUNER` macros in *main.c* as follows:

    ```
      #define ENABLE_SPI_SERIAL_LED            (0u)

      #define ENABLE_TUNER                     (0u)
    ```

2. Run the *Select_No_Connection* batch file, to completely disconnect the MCU from the onboard KitProg.

3. Connect an ammeter to evaluate the low-power feature of the device as follows:

   **Figure 12. Current measurement setup**

   <img src="images/psoc-4000t-kit-ammeter-setup.png" alt="Figure 12" width="500"/>

4. After reset, the application transitions to low-power states if there is no user activity, i.e., no button touch detection, to reduce the power consumption as follows:

   **Figure 13. Power mode transition - no user activity**

   <img src="images/power-mode-transition-no-touch.png" alt="Figure 13" width="900"/>

5. If there is touch detection while in low-power states, the application transitions to active mode with the highest refresh rate as follows:

   **Figure 14. Power mode transition - touch detection**

   <img src="images/power-mode-transition-wot-touch.png" alt="Figure 14" width="900"/>

   **Table 2. Measured current for different modes**

   | Power mode  | LED2 State |Refresh rate(Hz) | Current consumption (uA)  |
   |:---------------------| :-----|:-----|:-----|
   | Active  | RED |128 |149 |
   | Active Low Refresh rate <br>(ALR)  | YELLOW |32 |36.5 |
   | Wake on Touch <br>(WoT) | GREEN  |16 |4.3 |

 6. The ILO clock is used to control the refresh rate in this project; this clock is expected to have up to 50% variation across different PSoC&trade; 4000T devices. Measure the actual ILO clock frequency and update the `MEASURED_ILO` macro in *main.c* as follows to get the expected current results:

    ```
      #define MEASURED_ILO                     (40u)
    ```
<br>

## Tuning procedure

### Create custom BSP for your board

1. Create a custom BSP for your board having any device, by following the steps given in [KBA231373](https://community.cypress.com/t5/Knowledge-Base-Articles/ModusToolbox-2-2-and-later-Make-a-Custom-BSP-KBA231373/ta-p/251107). In this code example, it was created for the device "CY8C4046LQI-T452".

2. Open the *design.modus* file from *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/COMPONENT_BSP_DESIGN_MODUS/* folder obtained in the previous step and enable CAPSENSE&trade; to get *design.cycapsense* file. CAPSENSE&trade; configuration can then be started from scratch as explained below.

### Tuning flow

The following steps explain the tuning procedure for low-power widgets; for tuning regular widgets, see [PSoC&trade; 4: MSCLP self-capacitance button tuning](https://github.com/Infineon/mtb-example-psoc4-msclp-self-capacitance-button).

**Note:** See the section "Selecting CAPSENSE&trade; hardware parameters" in the [PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide](https://www.cypress.com/AN85951) to learn about the considerations for selecting each parameter values. In addition, see the "Low-Power widget parameters" section in the [Achieving lowest power capacitive sensing with PSoC&trade; 4000T](https://www.infineon.com/002-34231) to learn about the considerations for parameter values specific to low-power widgets.


**Figure 15. Low-power widget tuning flow**

<img src="images/tuning-flowchart.png" alt="Figure 15" width="600" />

Do the following to tune the button widget:

- [Stage 1: Set the initial hardware parameters](#stage-1-set-the-initial-hardware-parameters)

- [Stage 2: Measure sensor parasitic capacitance (Cp)](#stage-2-measure-sensor-parasitic-capacitance-cp)

- [Stage 3: Calculate and set the sense clock frequency and Init sub conversions](#stage-3-calculate-and-set-the-sense-clock-frequency-and-init-sub-conversions)

- [Stage 4: Fine tune for required SNR, power, and refresh rate](#stage-4-fine-tune-for-required-snr-power-and-refresh-rate)

- [Stage 5: Tune threshold parameters](#stage-5-tune-threshold-parameters)


### Stage 1: Set the initial hardware parameters
-------------

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Launch the Device Configurator tool.

   You can launch the Device Configurator in Eclipse IDE for ModusToolbox&trade; from the Tools section in the IDE Quick Panel or in stand-alone mode from *{ModusToolbox&trade; install directory}/ModusToolbox/tools_{version}/device-configurator/device-configurator*. In this case, after opening the application, select **File** > **Open** and open the *design.modus* file of the respective application, which is present in the *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/COMPONENT_BSP_DESIGN_MODUS* folder.

3. Enable CAPSENSE&trade; channel in Device Configurator as follows:

   **Figure 16. Enable CAPSENSE&trade; in Device Configurator**

   <img src="images/device-configurator.png" alt="Figure 16"/>

   Save the changes and close the window.

4. Launch the CAPSENSE&trade; Configurator tool.

   You can launch the CAPSENSE&trade; Configurator tool in Eclipse IDE for ModusToolbox&trade; from the "CAPSENSE&trade;" peripheral setting in the Device Configurator or directly from the Tools section in the IDE Quick Panel.

   You can also launch it in stand-alone mode from *{ModusToolbox&trade; install directory}/ModusToolbox/tools_{version}/capsense-configurator/capsense-configurator*. In this case, after opening the application, select **File** > **Open** and open the *design.cycapsense* file of the respective application, which is present in the *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/COMPONENT_BSP_DESIGN_MODUS* folder.

   See the [ModusToolbox&trade; CAPSENSE&trade; Configurator tool guide](https://www.cypress.com/ModusToolboxCapSenseConfig) for step-by-step instructions on how to configure and launch CAPSENSE&trade; in ModusToolbox&trade;.

5. In the **Basic** tab, configure a Button widget 'Button0' and a low-power widget 'LowPower0' as a CSD RM (self-cap) and set the CSD tuning mode as *Manual tuning*.

   **Figure 17. CAPSENSE&trade; Configurator - Basic tab**

   <img src="images/basic-csd-settings.png" alt="Figure 17"/>

6. Do the following in the **General** tab under the **Advanced** tab:

   1. Select **CAPSENSE&trade; IMO Clock frequency** as **48 MHz**.

   2. Set the **Modulator clock divider** to **1** to obtain the maximum available modulator clock frequency as recommended in the [CAPSENSE&trade; design guide](https://www.cypress.com/AN85951).

   3. Set the **Number of init sub-conversions** based on the hint shown when you hover over the edit box. Retain the default value (will be set in [Stage 3: Calculate and set the sense clock frequency and Init sub conversions](#stage-3-calculate-and-set-the-sense-clock-frequency-and-init-sub-conversions))

   4. Use **Wake-on-Touch settings** to set the refresh rate and frame timeout while in lowest power mode (Wake-on-Touch mode).

   5. Set **Wake-on-Touch scan interval (ms)** based on the required low-power state scan refresh rate. For example, to get a 16-Hz refresh rate, set the value to **63**.

   6. Set the **Number of frames in Wake-on-Touch** as the maximum number of frames to be scanned in WoT mode if there is no touch detected. This determines the maximum time the device will be kept in the lowest-power mode if there is no user activity. The maximum time can be calculated by multiplying this parameter with the **Wake-on-Touch scan interval (ms)** value.

      For example, to get 10 seconds as maximum time in WoT mode, set **Number of frames in Wake-on-Touch** to **160** for the  scan interval set as 63 ms.

      **Note:** For tuning low-power widgets, **Number of frames in Wake-on-Touch** must be less than the  **Maximum number of raw counts in SRAM** based on the number of sensors in WoT mode as follows:

      **Table 3. Maximum number of Raw counts in SRAM**

      | Number of low <br> power widgets  | Maximum number of <br> raw counts in SRAM  |
         |:---------------------| :-----|
         | 1  | 245 |
         | 2  | 117 |
         | 3  | 74 |
         | 4  | 53 |
         | 5  | 40 |
         | 6  | 31 |
         | 7  | 25 |
         | 8  | 21 |

   7. Retain the default settings for all regular and low-power widget filters. You can enable or update the filters later depending on the signal-to-noise ratio (SNR) requirements in [Stage 4: Fine tune for required SNR, power, and refresh rate](#stage-4-fine-tune-for-required-snr-power-and-refresh-rate).

      Filters are used to reduce the peak-to-peak noise. Using filters will result in a higher scan time.

      **Figure 18. CAPSENSE&trade; Configurator - General settings**

      <img src="images/advanced-general-settings.png" alt="Figure 18"/>

      **Note:** Each tab has a **Restore Defaults** button to restore the parameters of that tab to their default values.

7. Go to the **CSD settings** tab and make the following changes:

   1. Set **Inactive sensor connection** as **Shield**.
   2. Set **Shield mode** as **Active**.
   3. Set **Total shield count** as **1**.
   4. Select **Enable CDAC auto-calibration** and **Enable compensation CDAC**.

      This helps in achieving the required CDAC calibration levels (85% of maximum count) for all sensors in the widget while maintaining the same sensitivity across the sensor elements.

   5. Select **Enable CDAC dither**.

      This helps in removing flat-spots, by adding white noise that moves the conversion point around the flat-spots region.

      **Figure 19. CAPSENSE&trade; Configurator - Advanced CSD settings**

      <img src="images/advanced-csd-settings.png" alt="Figure 19"/>

8. Go to the **Widget details** tab. Select **LowPower0** from the left pane, and then set the following:

   - **Sense clock divider:** Retain the default value (will be set in [Stage 3: Calculate and set the sense clock frequency and Init sub conversions](#stage-3-calculate-and-set-the-sense-clock-frequency-and-init-sub-conversions))

   - **Clock source:** Direct

      **Note:** Spread spectrum clock (SSC) or PRS clock can be used as a clock source to deal with EMI/EMC issues.

   - **Number of sub-conversions: 60**

     60 is a good starting point to ensure a fast scan time and sufficient signal. This value will be adjusted as required in [Stage 4: Fine tune for required SNR, power, and refresh rate](#stage-4-fine-tune-for-required-snr-power-and-refresh-rate).

   - **Finger Threshold:** 65535

      Finger threshold is set to maximum to avoid waking up of device from WoT mode due to touch detection; this is required to find the signal and SNR. Change the Finger threshold of **Button0** as well to **65535**.

   - **Noise Threshold:** 10

   - **Negative Noise Threshold:** 10

   - **Low baseline reset:** 10

   - **ON debounce:** 1

     These values reduce the influence of the baseline on the sensor signal, which helps to get the true difference count. Retain the default values for all other threshold parameters; these parameters are set in [Stage 5: Tune threshold parameters](#stage-5-tune-threshold-parameters).

      **Figure 20. CAPSENSE&trade; Configurator - Widget details tab under the Advanced tab**

     <img src="images/advanced-widget-settings.png" alt="Figure 20" />

9. Go to the **Scan Configuration** tab to select the pins and the scan slots. Do the following:

   1. Configure pins for the electrodes using the drop-down menu.

   2. Configure the scan slots using **Auto-assign slots** option. The other option is to allot each sensor a scan slot based on the entered slot number.

   3. Check the notice list for warning or errors.

      **Figure 21. Scan Configuration tab**

      <img src="images/scan-configuration.png" alt="Figure 21"/>

10. Click **Save** to apply the settings.

### Stage 2: Measure sensor parasitic capacitance (Cp)
------------------

#### **Use an LCR meter to determine the Cp of the sensor**

Measure the Cp of the sensor electrode of the button using an LCR meter. The Cp should be measured between the sensor electrode (sensor pin) and the device ground.

   **Table 4. Cp values obtained for CAPSENSE&trade; button in CY8CKIT-040T kit**

   | Kit     | Parasitic capacitance (Cp) in pF
   |:---------------------|:----------------------------------------|
   | CY8CKIT-040T (Pin P4.1) with shield |          4 pF  |
   | CY8CKIT-040T (Pin P4.1) without shield  |      13 pF |
   | CY8CKIT-040T (Pin P3.0)  shield  |      38 pF |


### Stage 3: Calculate and set the sense clock frequency
------------------

1. Calculate the maximum sense clock frequencies using **Equation 1** for the sensor and shield pin.

   **Equation 1. Maximum sense clock frequency**

   ![Maximum sense clock frequency](images/fsw-equation.png)

   where,
   - Cp is the parasitic capacitances of the electrodes. This value is obtained from [Stage 2: Measure sensor parasitic capacitance (Cp)](#stage-2-measure-sensor-parasitic-capacitance-cp).

   - R<sub>SeriesTotal</sub> is the total series resistance, which includes the 525-ohm internal resistance for the sensor pin (or 180-ohm internal resistance for the Active Shield pin), the external series resistance (in CY8CKIT-040T, it is 560 ohm), and the trace resistance. Include the trace resistance if high-resistive material such as ITO or conductive ink is used.

2. Use **Equation 2** to obtain the **sense clock divider** value. It is obtained by dividing the **Modulator clock frequency** (48 MHz) by the calculated **Maximum sense clock frequency (kHz)**, and choosing the nearest ceiling sense clock divider option in the CAPSENSE&trade; Configurator. Use the largest of the sensor or shield **Sense clock divider**.

   **Equation 2. Sense clock divider**

   ![Sense clock divider](images/sense-clock-divider-equation.png)


   **Table 5. Sense clock frequency settings for CY8CKIT-040T Kit from Cp**

   |Development kit | Cp of the electrode (pF)| R<sub>SeriesTotal</sub> (ohm) |  Maximum sense clock frequency (MHz)| Sense clock divider |
   | --- | --- | --- | --- | --- |
   |CY8CKIT-040T (Pin P4.1) Sensor pin| 4 | 1085 | 11.52| 4.2 |
   |CY8CKIT-040T (Pin P3.0) Shield pin| 39 | 740 | 1.73 | 27.75 |


   **Notes:**

   - If you are explicitly using the PRS or SSCx clock source to lower the electromagnetic interference, ensure that you select the sense clock frequency that meets the conditions mentioned in the [ModusToolbox&trade; CAPSENSE&trade; Configurator guide](https://www.cypress.com/file/492896/download) in addition to the above conditions. PRS and SSCx techniques spread the frequency across a range.

   - Actual Sense clock frequency value is chosen such that the divider is divisible by 4 in order to have all the four scan phases for equal duration.

   **Table 6. Sense clock frequency settings for CY8CKIT-040T kit**

   |Development kit | Sense clock divider |
   | --- | ---  |
   |CY8CKIT-040T (Pin P4.1)|  28 |


3. Program the board.

4. Verify the auto-calibration.

   Capture the raw counts of each sensor (as shown in Figure 22) and verify that they are approximately (+/- 5%) equal to 70% of the MaxCount. See [AN234231 ??? Achieving lowest-power capacitive sensing with PSoC&trade; 4000T](https://www.cypress.com/AN234231) for the MaxCount equation.

   **Note:** The signal saturates at 85% calibration because of its thin overlay (1 mm). Therefore, the auto-calibration percentage is changed to 70% via the `Cy_CapSense_SetCalibrationTarget()` API in the *main.c* file to avoid signal saturation.

      **Figure 22. Verifying the raw count calibration level**

      <img src="images/tuner-graph-view.png" alt="Figure 22"/>


### Stage 4: Fine-tune for required SNR, power, and refresh rate
-----------------------

1. Measure the SNR as mentioned in the [Operation](#operation) section, and perform sensor tuning until the minimum SNR of 5:1 and a signal count of at least 50 are achieved.

2. Update the number of sub-conversions.

   Do the following to update the number of sub-conversions:

   1. Update the number of sub-conversions (N<sub>sub</sub>) directly in the **Widget/Sensor parameters** tab of the CAPSENSE&trade; Tuner.

   2. CY8CKIT-040T has a built-in CIC2 filter which increases the resolution for the same scan time. See [AN234231 - Achieving lowest-power capacitive sensing with PSoC&trade; 4000T](https://www.cypress.com/AN234231) for detailed information on the CIC2 filter.

   3. The current consumption is directly proportional to the number of sub-conversion; therefore, decrease the number of sub-conversions to achieve a lower current consumption.

      **Note:** Number of sub-conversion should be greater than or equal to 4.

   4. Calculate the decimation rate using **Equation 3**. The resolution increases with an increase in the decimation rate; therefore, set the maximum decimation rate achieved.

      **Equation 3. Decimation rate**

      ![Equation 3](images/decimation-equation.png)

   5. Load the parameters to the device and measure SNR as mentioned in the Steps 10 and 11 in the [Operation](#operation) section. Perform tuning until the minimum SNR of 5:1 and at least a signal count at least greater than 50 are achieved.

   6. Skip this step if the following conditions are met:

      - Measured SNR from the previous stage is greater than 5:1.

      - Signal count is greater than 50.

      - Each power mode has the least current/power consumption.

   If your system is very noisy (counts >20), do the following to enable filters:

   1. Open **CAPSENSE&trade; Configurator** from ModusToolbox&trade; Quick Panel and select the appropriate filter:

      **Figure 23. Filter settings in CAPSENSE&trade; Configurator**

      <img src="images/advanced-filter-settings.png" alt="Figure 23"/>

      **Note:**
      - Add the filter based on the type of noise in your measurements. See [ModusToolbox&trade; CAPSENSE&trade; configurator guide](https://www.cypress.com/file/492896/download) for details.
      - The current example has an in-built CIC2 filter, therefore it is recommended to enable IIR filter.

   2. Reprogram the device to update the filter settings.


### Stage 5: Tune threshold parameters
--------------------

The software threshold is set for each widget based on the diff counts. Do the following in CAPSENSE&trade; Tuner to set up the threshold for a widget:

1. Switch to the **Graph View** tab and select **Button0**.

2. Touch the sensor and monitor the touch signal in the **Sensor signal** graph as Figure 24 shows. Place the metal finger at all possible positions and use the least signal for setting the software threshold. In addition, ensure that the metal finger is grounded.

   **Figure 24. Sensor signal when the sensor is touched**

   <img src="images/tuner-diff-signal.png" alt="Figure 24"/>

3. For the **LowPower0_Sns0** low power sensor, switch to the **SNR Measurement** tab and follow steps 10 and 11 in the [Monitor data using CAPSENSE&trade; Tuner](#monitor-data-using-capsensetrade-tuner) section.

4. When the signal is measured, set the thresholds according to the following recommendations:

   - Finger threshold = 80 percent of the signal

   - Noise threshold = 40 percent of the signal

   - Negative noise threshold = 40 percent of the signal

   - Debounce = 1

5. Set the threshold parameters in the **Widget/Sensor parameters** section of the CAPSENSE&trade; Tuner:

   **Figure 25. Widget threshold parameters**

   <img src="images/tuner-threshold-settings.png" alt="Figure 25"/>

6. Apply the settings to the device and to the project by clicking **To device** and then **To Project** as Figure 26 shows, and close the tuner. The change is updated in the *design.cycapsense* file and reflected in the CAPSENSE&trade; Configurator.

   **Figure 26. Apply to project setting**

   <img src="images/tuner-apply-settings.png" alt="Figure 26"/>

7. Consider testing with metal fingers with smaller sizes that should be rejected by the system to ensure that they do not reach the finger threshold.

   If your sensor is tuned correctly, you will observe the touch status go from 0 to 1 in the **Status** sub-window of the **Graph View** window as Figure 27 shows. The successful tuning of the button is also indicated by the LED1 in the kit; the LED1 is turned ON when the finger touches the button and turned OFF when the finger is removed.

   **Figure 27. Sensor status in CAPSENSE&trade; Tuner**

   <img src="images/tuner-status.png" alt="Figure 27"/>

8. Close the CAPSENSE&trade; Tuner and launch **CAPSENSE&trade; Configurator**. You should now see all the changes that you made in the CAPSENSE&trade; Tuner reflected in the **CAPSENSE&trade; Configurator**.

   **Table 7. Software tuning parameters obtained based on sense for CY8CKIT-040T**

   |Parameter|	 Button0|	 LowPower0|
   |:--------|:------|:------|
   |Signal	| 409 |63 |
   |Finger threshold 	| 327 |50 |
   |Noise threshold |164|25|
   |Negative noise threshold	|164 |25 |
   |Low baseline reset	| 30 |30 |
   |Hysteresis	| 41 |NA |
   |ON debounce	| 3|1|


## Debugging

You can debug this project to step through the code. In the IDE, use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For more details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.cypress.com/MTBEclipseIDEUserGuide).

**Note:** The debug port is disabled by default for CY8CKIT-040T, because it uses the pins P3[2] (SWDIO) and P3[3] (SWDCK) for I2C SDA and SCL. If debug is required, do the following:
   1. Enable the **Debug Mode** under the **Systems** tab in the **Device Configurator** and change the **Debug Mode** setting to **SWD**.
   2. Disable I2C in the project by disabling the **Serial Communication Block (SCB)** resource with **EZI2C** personality in **Peripherals Tab** in the **Device Configurator**.
   3. Enable the **SWD_DEBUG_ENABLE** macro present in the *main.c* file as follows: *#define SWD_DEBUG_ENABLE (1u)*. See **Step 4** in **Section 2.3** of the Kit guide for more details.

## Design and implementation

The project contains one button configured as a regular and low-power widget in CSD-RM sensing mode. See the [Tuning procedure](#tuning-procedure) section for step-by-step instructions to configure other settings of the **CAPSENSE&trade; Configurator**.

There are two LEDs used in this project. LED1 shows the button touch status: it turns ON when touched and turns OFF when the finger is lifted. LED2 shows the current low-power state of the application. See the following table for the LED2 response based on the low-power state.

**Table 8. LED2 state for different application power mode**

   | Power mode state  | LED2  |
   |:---------------------| :-----|
   | Active  | Red |
   | Active Low Refresh rate <br>(ALR)  | Yellow |
   | Wake on Touch <br>(WoT) | Green  |

There are three power states defined for this project:

- **ACTIVE** mode

- Active Low Refresh rate (**ALR**) mode

- Wake On Touch (**WoT**) mode

After reset, the device is in **ACTIVE** mode, and scans the regular CAPSENSE&trade; widgets with a high refresh rate (**128 Hz**). If user activity is detected in any other mode, the device is transferred to **ACTIVE** mode to provide the best user interface experience. This mode has the highest power consumption; thus, your design should reduce the time spent in **ACTIVE** mode as much as possible.

If there is no user activity for a certain time (`ACTIVE_MODE_TIMEOUT_SEC` = 10 seconds), the application transitions to **ALR** mode. Here, the refresh rate is reduced to **32 Hz**; this mode acts as an intermediate state before moving to the lowest-power mode (**WoT** mode). This mode can also be used for periodically updating baselines of sensors while there is no user activity for a long time.

Further non-activity for a certain time (`ALR_MODE_TIMEOUT_SEC` = 5 seconds) transitions the application to the lowest-power mode - **Wake-on-Touch** mode, which scans the low-power widget at a low refresh rate (**16 Hz**) and processes the results without CPU intervention.

**Note:** An internal low-power timer (MSCLP timer) is available in CAPSENSE&trade; MSCLP hardware to set refresh rate for each power mode as follows:
- For **ACTIVE** and **ALR** modes: Use the `Cy_CapSense_ConfigureMsclpTimer()` function.
- For **WoT** mode: Use the **Wake-on-Touch scan interval** in CAPSENSE&trade; Configurator.

Different power modes and transition conditions for a typical use case are as follows:

   **Figure 28. State machine showing different power states of the device**

   <img src="images/capsense_lp_firmware_state_machine.png" alt="Figure 28" width="500"/>


The project uses the [CAPSENSE&trade; middleware](https://github.com/Infineon/capsense) (see ModusToolbox&trade; user guide for more details on selecting a middleware). See [AN85951 ??? PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide](https://www.cypress.com/documentation/application-notes/an85951-psoc-4-and-psoc-6-mcu-capsense-design-guide) for more details on CAPSENSE&trade; features and usage.

The [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) provides a GUI-based tuner application for debugging and tuning the CAPSENSE&trade; system. The CAPSENSE&trade; Tuner application works with EZI2C and UART communication interfaces. This project has an SCB block configured in EZI2C mode to establish communication with the onboard KitProg, which in turn enables reading the CAPSENSE&trade; raw data by the CAPSENSE&trade; Tuner. See [EZI2C Peripheral settings](#resources-and-settings).

The CAPSENSE&trade; data structure that contains the CAPSENSE&trade; raw data is exposed to the CAPSENSE&trade; Tuner by setting up the I2C communication data buffer with the CAPSENSE&trade; data structure. This enables the tuner to access the CAPSENSE&trade; raw data for tuning and debugging CAPSENSE&trade;.

The successful tuning of the button is indicated by an LED in the pioneer kit: the LED is turned ON when the finger touches the button and turned OFF when the finger is removed from the button.


### Set up the VDDA supply voltage and debug mode in Device Configurator

1. Open Device Configurator from the Quick Panel.

2. Go to the **System** tab. Select the **Power** resource, and set the VDDA value under **Operating conditions** as follows:

   **Figure 29. Setting the VDDA supply in system tab of Device Configurator**

   <img src="images/vdda-settings.png" alt="Figure 29"/>
3. By default, SWD pins are active in all device power modes. Disable debug mode to disable SWD pins and thereby reduce the power consumption as follows:

   **Figure 30. Disable Debug mode in the System tab of Device Configurator**

   <img src="images/disable-swd.png" alt="Figure 30"/>



### Resources and settings

**Figure 31. EZI2C settings**

<img src="images/ezi2c-config.png" alt="Figure 31" width="800"/>

**Figure 32. SPI settings**

<img src="images/spi-config.png" alt="Figure 32" width="800"/>

**Table 9. Application resources**

| Resource  |  Alias/Object     |    Purpose     |
| :------- | :------------    | :------------ |
| SCB (I2C) (PDL) | CYBSP_EZI2C          | EZI2C slave driver to communicate with CAPSENSE&trade; Tuner |
| SCB (SPI) (PDL) | CYBSP_MASTER_SPI          | SPI master driver to control serial LEDs |
| CAPSENSE&trade; | CYBSP_MSC | CAPSENSE&trade; driver to interact with the MSC hardware and interface the CAPSENSE&trade; sensors |
| Digital pin | CYBSP_SERIAL_LED | To show the button operation and power mode states|

</details>

<br>

### Firmware flow

**Figure 33. Firmware flowchart**

<img src="images/firmware-flowchart.png" alt="Figure 33" width="800"/>

<br>

## Related resources

Resources  | Links
-----------|----------------------------------
Application notes  | [AN79953](https://www.cypress.com/AN79953) ??? Getting started with PSoC&trade; 4 <br>  [AN85951](https://www.cypress.com/AN85951) ??? PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide <br>  [AN234231](https://www.cypress.com/AN234231) ??? Achieving lowest-power capacitive sensing with PSoC&trade; 4000T
Code examples  | [Using ModusToolbox&trade; software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation |  [PSoC&trade; 4 datasheets](https://www.cypress.com/search/all/PSOC%204%20datasheets?sort_by=search_api_relevance&f%5B0%5D=meta_type%3Atechnical_documents) <br>[PSoC&trade; 4 technical reference manuals](https://www.cypress.com/search/all/PSoC%204%20Technical%20Reference%20Manual?sort_by=search_api_relevance&f%5B0%5D=meta_type%3Atechnical_documents)
Development kits | Select your kits from the [Evaluation Board Finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board) page.
Libraries on GitHub  | [mtb-pdl-cat2](https://github.com/Infineon/mtb-pdl-cat2) ???  PSoC&trade; 4 Peripheral Driver Library (PDL) <br>  [mtb-hal-cat2](https://github.com/Infineon/mtb-hal-cat2)  ??? Hardware Abstraction Layer (HAL) library
Middleware on GitHub  | [capsense](https://github.com/Infineon/capsense) ??? CAPSENSE&trade; library and documents
Tools | [Eclipse IDE for ModusToolbox&trade; software](https://www.cypress.com/modustoolbox) ???  ModusToolbox&trade; software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC&trade; Wi-Fi and Bluetooth?? connectivity devices.
<br>

## Other resources

Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

## Document history


Document title: *CE235111* ??? *PSoC&trade; 4: MSCLP CAPSENSE&trade; low power*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example
 1.0.1   | Minor readme update



---------------------------------------------------------

?? Cypress Semiconductor Corporation, 2022. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress???s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress???s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolbox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit cypress.com. Other names and brands may be claimed as property of their respective owners.
