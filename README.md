# PSoC&trade; 4: MSCLP CAPSENSE&trade; low power

This code example demonstrates an implementation of a low-power application including recommended power states and transitions, tuning parameter adjustments, and the tuning method. This example uses a single self-capacitance-based button in multi-sense CAPSENSE&trade; low-power (MSCLP - 5th-generation low-power CAPSENSE&trade;) to demonstrate different considerations to implement a low-power design.

This document also explains how to manually tune the self-capacitance-based low-power widget for optimum performance according to parameters such as power consumption and response time using the CSD-RM sensing technique and the CAPSENSE&trade; Tuner.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-psoc4-msclp-capsense-low-power)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzUxMTEiLCJTcGVjIE51bWJlciI6IjAwMi0zNTExMSIsIkRvYyBUaXRsZSI6IlBTb0MmdHJhZGU7IDQ6IE1TQ0xQIENBUFNFTlNFJnRyYWRlOyBsb3cgcG93ZXIiLCJyaWQiOiJuYWxsdXNhbXkiLCJEb2MgdmVyc2lvbiI6IjIuMC4wIiwiRG9jIExhbmd1YWdlIjoiRW5nbGlzaCIsIkRvYyBEaXZpc2lvbiI6Ik1DRCIsIkRvYyBCVSI6IklDVyIsIkRvYyBGYW1pbHkiOiJQU09DIn0=)

## Requirements

- [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) v3.2 or later

 > **Note:** This code example requires ModusToolbox&trade; v3.2 and is not backward compatible with v3.1 or older.

- Board support package (BSP) minimum required version: 3.2.0
- Programming language: C
- Associated parts: [PSoC&trade; 4000T](https://www.infineon.com/002-33949)


## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; Embedded Compiler v11.3.1 (`GCC_ARM`) – Default value of `TOOLCHAIN`
- Arm&reg; Compiler v6.16 (`ARM`)
- IAR C/C++ Compiler v9.30.1 (`IAR`)


## Supported kits (make variable 'TARGET')

- [PSoC&trade; 4000T CAPSENSE&trade; Evaluation Kit](https://www.infineon.com/CY8CKIT-040T) (`CY8CKIT-040T`) – Default value of `TARGET`

## Hardware setup

This example uses the board's default configuration. See the [kit user guide](https://www.infineon.com/002-34870) to ensure that the board is configured correctly to use VDDA at 1.8 V.

## Software setup

See the [ModusToolbox&trade; tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.
This example requires no additional software or tools.

## Using the code example

### Create the project

The ModusToolbox&trade; tools package provides the Project Creator as both a GUI tool and a command line tool.

<details><summary><b>Use Project Creator GUI</b></summary>

1. Open the Project Creator GUI tool.

   There are several ways to do this, including launching it from the dashboard or from inside the Eclipse IDE. For more details, see the [Project Creator user guide](https://www.infineon.com/ModusToolboxProjectCreator) (locally available at *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/docs/project-creator.pdf*).

2. On the **Choose Board Support Package (BSP)** page, select a kit supported by this code example. See [Supported kits](#supported-kits-make-variable-target).

   > **Note:** To use this code example for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. On the **Select Application** page:

   a. Select the **Applications(s) Root Path** and the **Target IDE**.

   > **Note:** Depending on how you open the Project Creator tool, these fields may be pre-selected for you.

   b.	Select this code example from the list by enabling its check box.

   > **Note:** You can narrow the list of displayed examples by typing in the filter box.

   c. (Optional) Change the suggested **New Application Name** and **New BSP Name**.

   d. Click **Create** to complete the application creation process.

</details>

<details><summary><b>Use Project Creator CLI</b></summary>

The 'project-creator-cli' tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the 'project-creator-cli' tool. On Windows, use the command-line 'modus-shell' program provided in the ModusToolbox&trade; installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing "modus-shell" in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.


The following example clones the "[PSoC&trade; 4: MSCLP CAPSENSE&trade; low power](https://github.com/Infineon/mtb-example-psoc4-msclp-capsense-low-power)" application with the desired name "CAPSENSE_Low_Power" configured for the *CY8CKIT-040T* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CKIT-040T --app-id mtb-example-psoc4-msclp-capsense-low-power --user-app-name CAPSENSE_Low_Power --target-dir "C:/mtb_projects"
   ```

The 'project-creator-cli' tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the <id> field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the <id> field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

> **Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at {ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf).

</details>



### Open the project

After the project has been created, you can open it in your preferred development environment.


<details><summary><b>Eclipse IDE</b></summary>

If you opened the Project Creator tool from the included Eclipse IDE, the project will open in Eclipse automatically.

For more details, see the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>


<details><summary><b>Visual Studio (VS) Code</b></summary>

Launch VS Code manually, and then open the generated *{project-name}.code-workspace* file located in the project directory.

For more details, see the [Visual Studio Code for ModusToolbox&trade; user guide](https://www.infineon.com/MTBVSCodeUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_vscode_user_guide.pdf*).

</details>


<details><summary><b>Keil µVision</b></summary>

Double-click the generated *{project-name}.cprj* file to launch the Keil µVision IDE.

For more details, see the [Keil µVision for ModusToolbox&trade; user guide](https://www.infineon.com/MTBuVisionUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_uvision_user_guide.pdf*).

</details>


<details><summary><b>IAR Embedded Workbench</b></summary>

Open IAR Embedded Workbench manually, and create a new project. Then select the generated *{project-name}.ipcf* file located in the project directory.

For more details, see the [IAR Embedded Workbench for ModusToolbox&trade; user guide](https://www.infineon.com/MTBIARUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_iar_user_guide.pdf*).

</details>


<details><summary><b>Command line</b></summary>

If you prefer to use the CLI, open the appropriate terminal, and navigate to the project directory. On Windows, use the command-line 'modus-shell' program; on Linux and macOS, you can use any terminal application. From there, you can run various `make` commands.

For more details, see the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Connect the USB cable between the [CY8CKIT-040T kit](https://www.infineon.com/CY8CKIT-040T) and the PC as shown in **Figure 1**.

   ##### **Figure 1. Connecting the CY8CKIT-040T kit with the PC**

   <img src="images/psoc4000t_kit_connected.png" alt="Figure 1" width="350"/>

2. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE.
   </details>
   
   
   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```
   </details>

3. After programming, the application starts automatically.

   > **Note:** After programming, you see the following error message if debug mode is disabled. This can be ignored or enabling the debug mode will solve this error.

   ``` c
   "Error: Error connecting Dp: Cannot read IDR"
   ```

4. To test the application, observe the LED2 state change depending on the different low-power states based on user interaction. Place your finger over the CAPSENSE&trade; button and observe LED1 turn ON when touched, and turn OFF when you lift your finger.

   **Table 1. LED2 state for different application power modes**

    Power mode state  | LED2 
   :---------------------| :-----
    Active  | Red
    Active Low Refresh rate <br>(ALR)  | Yellow
    Wake on Touch <br>(WoT) | Green

<br>

5. Verify that the application is transitioning to different power modes based on the following user input conditions:

   - If there is no user activity for a certain time (ACTIVE_MODE_TIMEOUT_SEC = 10 seconds), the application transitions to ALR mode. Here, the refresh rate is reduced to 32 Hz.
   
   - Further non-activity for a certain time (ALR_MODE_TIMEOUT_SEC = 5 seconds) transitions the application to the lowest-power mode - WoT mode which scans the low-power widget at a low refresh rate (16 Hz).

   ##### **Figure 2. Low-power mode state machine**

   <img src="images/capsense_lp_firmware_state_machine.png" alt="Figure 2" width="500"/>

### Monitor data using CAPSENSE&trade; Tuner


1. Open CAPSENSE&trade; Tuner from the **Tools** section in the **IDE Quick Panel**.

   You can also run the CAPSENSE&trade; Tuner application in standalone mode from *{ModusToolbox&trade; install directory}/ModusToolbox/tools_{version}/capsense-configurator/capsense-tuner*. In this case, after opening the application, select **File** > **Open** and open the *design.cycapsense* file of the respective application, which is present in the *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/config* folder.

   See the [ModusToolbox&trade; user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*) for options to open the CAPSENSE&trade; Tuner application using the CLI.

2. Ensure that the status LED is ON and not blinking. This indicates that the onboard KitProg3 is in CMSIS-DAP bulk mode. See [Firmware-loader](https://github.com/Infineon/Firmware-loader) to learn how to update the firmware and switch modes in KitProg3.

3. In the Tuner application, click on the **Tuner Communication Setup** icon   or select **Tools** > **Tuner Communication setup**. 
   ##### **Figure 3. Tuner Communication Setup**
   <img src="images/tuner-comm-setup.png" alt="Figure 3"/> 

   In the window that appears, select I2C under KitProg3 and configure it as follows:

   - **I2C address:** 8
   - **Sub-address:** 2-Bytes
   - **Speed (kHz):** 400

   These are the same values set in the EZI2C resource.

   ##### **Figure 4. Tuner Communication Setup parameters**

   <img src="images/tuner-comm-settings.png" alt="Figure 4" width="550"/>

4. Click **Connect** or select **Communication** > **Connect** to establish a connection.

   ##### **Figure 5. Establish connection**

   <img src="images/tuner-connect.png" alt="Figure 5" width="300" />

5. Click **Start** or select **Communication** > **Start** to start data streaming from the device.

   ##### **Figure 6. Start tuner communication**

   <img src="images/tuner-start.png" alt="Figure 6" width="300" />

   The **Widget/Sensor Parameters** tab is updated with the parameters configured in the CAPSENSE&trade; Configurator window. The tuner displays the data from the sensor in the **Widget View** and **Graph View** tabs.

6. Set the **Read mode** to **Synchronized**. Navigate to the **Widget View** tab and observe that the **Button0** widget is highlighted in blue when you touch it.

   ##### **Figure 7. Widget view of the CAPSENSE&trade; Tuner**

   <img src="images/tuner-widget-view.png" alt="Figure 7" width="750"/>

7. Go to the **Graph View** tab to view the raw count, baseline, difference count, and status for each sensor. Observe that the low-power widget sensor (**LowPower0_Sns0**) raw count is plotted after the device completes a full-frame scan (or detects a touch) in **WOT** mode and moves to **Active/ALR** mode.

   ##### **Figure 8. Graph view of the CAPSENSE&trade; Tuner**

   <img src="images/tuner-graph-view-intro.png" alt="Figure 8" width="750"/>

8. Observe the **Widget/Sensor parameters** section in the CAPSENSE&trade; Tuner window as shown in **Figure 8**.

9. Switch to the **SNR Measurement** tab for measuring the SNR and verify that the SNR is above 5:1 and the signal count is above 50; select the **Button0** and **Button0_Sns0** sensor, and then click **Acquire Noise** as shown in **Figure 9**.

    ##### **Figure 9. CAPSENSE&trade; Tuner - SNR measurement: Acquire noise**

    <img src="images/tuner-acquire-noise.png" alt="Figure 9" width="750" />

      > **Note:** Because the scan refresh rate is lower in **ALR** and **WoT** mode, it takes more time to acquire noise. Touch the CAPSENSE&trade; button before clicking **Acquire Noise** to transition the device to **Active** mode to receive the signal faster.

10. After noise is acquired, place the finger at a position on the button and click **Acquire Signal**. Ensure that the finger remains on the button as long as the signal acquisition is in progress. Observe the SNR is above 5:1 and the signal count is above 50.

    The calculated SNR on this button is displayed, as shown in **Figure 10**. Based on your end system design, test the signal with a finger that matches the size of your normal use case. Consider testing with smaller sizes, which the system should reject, to ensure that they do not reach the finger threshold.

    ##### **Figure 10. CAPSENSE&trade; Tuner - SNR measurement: Acquire signal**

    <img src="images/tuner-acquire-signal.png" alt="Figure 10" width="750"/>

11. To measure the SNR of the low-power sensor (**LowPower0_Sns0**), set the **Finger threshold** to max (65535) in **Widget/Sensor Parameters** as shown in **Figure 11** for both **Button0** and **LowPower0** widgets. This is to stop the system from detecting a touch in low-power and ALR modes and to avoid state transitions between low-power or ALR and Active modes.
      
      Use the **Apply to device** option to set the modified parameters to the device instantaneously. But make the final configuration using the CAPSENSE&trade; Configurator. 

      ##### **Figure 11. CAPSENSE&trade; update finger threshold**
      
      <img src="images/tuner-threshold-update.png" alt="Figure 11" width="750"/>

      ##### **Figure 12. Apply changes to the device**

      <img src="images/tuner-apply-settings-device.png" alt="Figure 12" />

12. Repeat steps 10 and 11 to observe the SNR and signal count as shown in **Figure 12**.

      ##### **Figure 13. CAPSENSE&trade; Tuner - SNR measurement: low-power widget**

      <img src="images/tuner-lowpower-snr.png" alt="Figure 13" width="750"/>

<br>

> **Note :** A 6 mm metal finger is used in this example for tuning. 

## Operation at other voltages

[CY8CKIT-040T kit](https://www.infineon.com/CY8CKIT-040T) supports operating voltages of 1.8 V, 3.3 V, and 5 V. Use voltage selection switch available on top of the kit to set the preferred operating voltage and see the [Set up the VDDA supply voltage and debug mode in Device Configurator](#set-up-the-vdda-supply-voltage-and-debug-mode-in-device-configurator) section.

This application functionalities are optimally tuned for 1.8 V. However, you can observe the basic functionalities working across other voltages. 

It is recommended to tune the application with the preferred voltages for better performance.


## Measure current at different power modes

1. Disable the run time measurement, serial LED, and tuner macros to measure the current used for CAPSENSE&trade; sensing in each power mode in *main.c*.

   ```
      #define ENABLE_RUN_TIME_MEASUREMENT      (0u)
      
      #define ENABLE_SPI_SERIAL_LED            (0u)

      #define ENABLE_TUNER                     (0u)
    ```
2. Disable the self-test library from the CAPSENSE&trade; Configurator as follows:
   ##### **Figure 14. Disable self-test library**

   <img src="images/self-test-disable.png" alt="Figure 14" width="800"/>

3. Disable the debug mode if enabled. By default, it is disabled. To enable, see the [Set up the VDDA supply voltage and debug mode in Device Configurator](#set-up-the-vdda-supply-voltage-and-debug-mode-in-device-configurator) section. Enabling the debug mode keeps the SWD pins active in all device power modes and even during Deep Sleep, leading to increased power consumption.

4. Connect the kit to a power analyzer such as KEYSIGHT - N6705C using a current measure header to evaluate the low-power feature of the device as shown in **Figure 15**.

   ##### **Figure 15. Power analyzer connection**

   <img src="images/psoc-4000t-kit-ammeter-setup.png" alt="Figure 15" width="800"/>

5. Control the power analyzer device through the laptop using a software tool called "Keysight BenchVue Advanced Power Control and Analysis".

6. Select the current measurement option through the instrument control setup. Then, select and turn on the output channel, as shown in **Figure 16**:

   ##### **Figure 16. Current measurement setup**

   <img src="images/current_measurement_setup.png" alt="Figure 16" width="300"/>

7. Capture the data using the data log option from the tool. The average current consumption is measured using cursors on each power mode, as shown in **Figure 17**.

   ##### **Figure 17. Current measurement**

   <img src="images/power_measurement.png" alt="Figure 17" width="900"/>

8. After resetting, the application transitions to low-power states to reduce power consumption if it does not detect user activity, such as button touch detection.

   ##### **Figure 18. Power mode transition - no user activity**
   
   <img src="images/power-mode-transition-no-touch.png" alt="Figure 18" width="900"/>

9. If the application detects touch while in low-power states, it transitions to active mode with the highest refresh rate, as shown in **Figure 19**:

   ##### **Figure 19. Power mode transition - touch detection**

   <img src="images/power-mode-transition-wot-touch.png" alt="Figure 19" width="900"/>

   **Table 2. Measured current for different modes**

    Power mode  | LED2 State |Refresh rate(Hz) | Current consumption (µA)
   :---------------------| :-----|:-----:|:-----:
    Active  | RED |128 |70
    Active Low Refresh rate <br>(ALR)  | YELLOW |32 |19
    Wake on Touch <br>(WoT) | GREEN  |16 |3.6

<br>

> **Note :** The above WoT current was measured on a kit with a Deep Sleep current of 1.7 µA. If the kit has a Deep Sleep current of 2.5 µA (typical), the WoT current is expected to be ~4.4 µA.

<br>

## Tuning procedure

<details><summary><b>Create custom BSP for your board</b></summary>

1. Create a custom BSP for your board with any device by following the steps given in [ModusToolbox&trade; BSP Assistant user guide](https://www.infineon.com/ModusToolboxBSPAssistant). This code example is created for the CY8C4046LQI-T452 device.

2. Open the *design.modus* file from the *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/config/* folder obtained in the previous step and enable CAPSENSE&trade; to get the *design.cycapsense* file. CAPSENSE&trade; configuration can be started from scratch as follows:


</details>

<br>

> **Note:** See the section **Selecting CAPSENSE&trade; hardware parameters** in [AN85951 PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide](https://www.infineon.com/AN85951) to learn about the considerations for selecting each parameter value. In addition, see the **Low-Power widget parameters** section in [AN234231 Achieving lowest power capacitive sensing with PSoC&trade; 4000T](https://www.infineon.com/AN234231) to learn about the considerations for parameter values specific to low-power widgets.


##### **Figure 20. Low-power widget tuning flow**

<img src="images/tuning-flowchart.png" alt="Figure 20" width="600" />

Do the following to tune the button widget:

- [Stage 1: Set the initial hardware parameters](#stage-1-set-the-initial-hardware-parameters)

- [Stage 2: Set the sense clock frequency](#stage-2-set-the-sense-clock-frequency)

- [Stage 3: Fine tune for the required SNR, power, and refresh rate](#stage-3-fine-tune-for-required-snr-power-and-refresh-rate)

- [Stage 4: Tune the threshold parameters](#stage-4-tune-threshold-parameters)


### **Stage 1: Set the initial hardware parameters**
-------------

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Launch the Device Configurator tool.

   You can launch the Device Configurator in Eclipse IDE for ModusToolbox&trade; from the **Tools** section in the IDE **Quick Panel** or in standalone mode from *{ModusToolbox&trade; install directory}/ModusToolbox/tools_{version}/device-configurator/device-configurator*. In this case, after opening the application, select **File** > **Open** and open the *design.modus* file of the respective application located in the *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/config* folder.

3. Enable CAPSENSE&trade; channel in Device Configurator as shown in **Figure 21**:

   ##### **Figure 21. Enable CAPSENSE&trade; in Device Configurator**

   <img src="images/device-configurator.png" alt="Figure 21"/>

   Save the changes and close the window.

4. Launch the CAPSENSE&trade; Configurator tool.

   You can launch the CAPSENSE&trade; Configurator tool in Eclipse IDE for ModusToolbox&trade; from the "CAPSENSE&trade;" peripheral setting in the Device Configurator or directly from the **Tools** section in the **IDE Quick Panel**.

   You can also launch it in standalone mode through *{ModusToolbox&trade; install directory}/ModusToolbox/tools_{version}/capsense-configurator/capsense-configurator*. In this case, after opening the application, select **File** > **Open** and open the *design.cycapsense* file of the respective application present in the *{Application root directory}/bsps/TARGET_APP_\<BSP-NAME>/config* folder.

   See the [ModusToolbox&trade; CAPSENSE&trade; Configurator user guide](https://www.infineon.com/ModusToolboxCapSenseConfig) for step-by-step instructions on how to configure and launch CAPSENSE&trade; in ModusToolbox&trade;.

5. In the **Basic** tab, configure a Button widget **Button0** and a low-power widget **LowPower0** as a CSD RM (self-cap) and set the CSD tuning mode as **Manual tuning**.

   ##### **Figure 22. CAPSENSE&trade; Configurator - Basic tab**

   <img src="images/basic-csd-settings.png" alt="Figure 22"/>

6. Do the following in the **General** tab under the **Advanced** tab:
   <ol type="A">
   <li>

   Select **CAPSENSE&trade; IMO Clock frequency** as **46** MHz. 
   </li>

   <li>

   Set the **Modulator clock divider** to **1** to obtain the maximum available modulator clock frequency.
   </li>

   <li>

   Set the **Number of init sub-conversions** based on the hint you see when you hover over the edit box. Retain the default value.
   </li>

   <li>

   Use **Wake-on-Touch settings** to set the refresh rate and frame timeout while in the lowest power mode (Wake-on-Touch mode). Set **Wake-on-Touch scan interval (µs)** based on the required low-power state scan refresh rate. For example, to get a 16 Hz refresh rate, set the value to 62500. 
   </li>


   <li>

   Set the **Number of frames in Wake-on-Touch** as the maximum number of frames to be scanned in WoT mode if no touch is detected. This determines the maximum amount of time the device is kept in the lowest-power mode if there is no user activity. Calculate the maximum time by multiplying this parameter with the **Wake-on-Touch scan interval (µs)** value.

   For example, to get 10 seconds as the maximum time in WoT mode, set the **Number of frames in Wake-on-Touch** to 160 for a scan interval of 62500 µs.

   > **Note:** For tuning low-power widgets, the **Number of frames in Wake-on-Touch** must be less than the  **Maximum number of raw counts in SRAM** based on the number of sensors in WoT mode as follows:

   **Table 3. Maximum number of raw counts in SRAM**

   Number of low power widgets  | Maximum number of raw counts in SRAM
      :---------------------| :-----
      1  | 245
      2  | 117
      3  | 74
      4  | 53
      5  | 40
      6  | 31
      7  | 25
      8  | 21

      <br>

   ##### **Figure 23. CAPSENSE&trade; Configurator - General settings**

   <img src="images/advanced-general-settings.png" alt="Figure 23"/>
   
   </li>
   <li>

   Retain the default settings for all regular and low-power widget filters. You can enable or update the filters later depending on the signal-to-noise ratio (SNR) requirements in [Stage 3: Fine tune for required SNR, power, and refresh rate](#stage-3-fine-tune-for-required-snr-power-and-refresh-rate).

      Filters reduce the peak-to-peak noise, and using filters results in a higher scan time.
      
   </li>
   </ol>

   > **Note:** Each tab has a **Restore Defaults** button to restore the parameters of that tab to their default values.

7. Go to the **CSD Settings** tab and make the following changes:
   <ol type="A">
      <li>

      Set **Inactive sensor connection** as **Shield**.
      </li>

      <li>

      Set **Shield mode** as **Active**.
      </li>
      
      <li>

      Set **Total shield count** as **11**.

     To improve noise immunity, configure all unused sensor pins as shield.
      </li>
      <li>

      **Raw count calibration level(%)** helps achieve the required CDAC calibration levels (85% of maximum count by default) for all sensors in the widget while maintaining the same sensitivity across the sensor elements.
      You can reduce it if the application reaches the saturation level on a touch event.
    

      ##### **Figure 24. CAPSENSE&trade; Configurator - Advanced CSD settings**

      <img src="images/advanced-csd-settings.png" alt="Figure 24"/>
</li>
      </ol>

8. Go to the **Widget Details** tab. Select **LowPower0** from the left pane and set the following:

   - **Sense clock divider:** Retain the default value (to be set in [Stage 2: Set the sense clock frequency](#stage-2-set-the-sense-clock-frequency))

   - **Clock source:** Direct

      > **Note:** Use spread spectrum clock (SSC) or PRS clock as a clock source to deal with EMI/EMC issues. Ensure to tune based on your design recommendations. See [AN85951 – PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide](https://www.infineon.com/AN85951) for more details on the usage of different clock sources. 

   - **Number of sub-conversions:** 60

     60 is a good starting point to ensure a fast scan time and sufficient signal. This value is adjusted as required in [Stage 3: Fine tune for required SNR, power, and refresh rate](#stage-3-fine-tune-for-required-snr-power-and-refresh-rate).

   - **Finger threshold:** 65535

      Finger threshold is set to maximum to avoid waking up the device from the WoT mode because of touch detection; this is required to find the signal and SNR. 

   - **Noise threshold:** 10

   - **Negative noise threshold:** 10

   - **Low baseline reset:** 10

   - **ON debounce:** 1

     These threshold values reduce the influence of the baseline on the sensor signal, which helps to get the true difference count. These parameters are set in [Stage 4: Tune threshold parameters](#stage-4-tune-threshold-parameters).

      Next, select Button0 from the left pane and repeat the same configuration for that sensor as well.

      ##### **Figure 25. CAPSENSE&trade; Configurator - Widget details tab under the Advanced tab**

     <img src="images/advanced-widget-settings.png" alt="Figure 25" />

9. Go to the **Scan Configuration** tab to select the pins and the scan slots. Do the following:

      A. Configure pins for the electrodes using the drop-down menu.

      B. Configure the scan slots using the **Auto-assign slots** option. The other option is to allot each sensor a scan slot based on the entered slot number.

      C. Check the notice list for warnings or errors.

      ##### **Figure 26. Scan Configuration tab**

      <img src="images/scan-configuration.png" alt="Figure 26"/>

10. Click **Save** to apply the settings.


### **Stage 2: Set the sense clock frequency**
------------------
The sense clock is derived from the modulator clock using a clock-divider and is used to scan the sensor by driving the CAPSENSE&trade; switched capacitor circuits. Both the clock source and clock divider are configurable. The sense clock divider should be configured such that the pulse width of the sense clock is long enough to let the sensor capacitance charge and discharge completely. This is verified by observing the charging and discharging waveforms of the sensor using an oscilloscope and an active probe. The sensors should be probed close to the electrode, and not at the sense pins or the series resistor.

See [Figure 27](#figure-27-proper-charge-cycle-of-a-sensor) and [Figure 28](#figure-28-improper-charge-cycle-of-a-sensor) for waveforms observed on the shield. [Figure 27](#figure-27-proper-charge-cycle-of-a-sensor) shows proper charging when sense clock frequency is correctly tuned. The pulse width is at least 5 Tau, i.e., the voltage is reaching at least 99.3% of the required voltage at the end of each phase. [Figure 28](#figure-28-improper-charge-cycle-of-a-sensor) shows incomplete settling (charging/discharging).

##### **Figure 27. Proper charge cycle of a sensor**

<img src="images/sense_clock_valid.png" alt="Figure 27" width="600"/>

##### **Figure 28. Improper charge cycle of a sensor**

<img src="images/sense_clock_invalid.png" alt="Figure 28" width="600"/>

1. Program the board and launch CAPSENSE&trade; Tuner.

2. Observe the charging waveform of the sensor as described earlier. 

3. If the charging is incomplete, increase the sense clock divider using the CAPSENSE&trade; Tuner by selecting the sensor and editing the **Sense clock divider** parameter in the **Widget/Sensor Parameters** panel.

   > **Note:** The sense clock divider should be **divisible by 4**. This ensures that all four scan phases have equal durations. 

   After editing the value, click the **Apply to Device** button and observe the waveform again. Repeat this until you observe complete settling.  
   
4. Click the **Apply to Project** button so the configuration is saved to your project. 

   ##### **Figure 29. Sense clock divider setting**

   <img src="images/sense_clock_divider_setting.png" alt="Figure 29" />
   

5. Repeat this process for all sensors and the shield. Each sensor might require a different sense clock divider value to charge or discharge completely. But all sensors within the same scan slot need to have the same **Sense Clock source**, **Sense clock divider**, and **Number of sub-conversions**. Therefore, take the largest **Sense clock divider** in a given scan slot and apply it to all other sensors within that slot.


### **Stage 3: Fine-tune for required SNR, power, and refresh rate**
-----------------------

To ensure reliable operation, tune the sensor to have a minimum SNR of 5:1 and a minimum signal of 50. You can increase the sensitivity by increasing the **Number of sub-conversions** and decrease the noise by enabling filters.

   > **Note:** If sensor raw count saturates (equals **Max Raw count**) on touch, reduce **Raw count calibration level(%)** to avoid saturation.

Follow these steps to optimize these parameters:

1. Measure the SNR as mentioned in the [Operation](#operation) section.

2. If the SNR is less than 5:1, increase the **Number of sub-conversions**.  Edit the number of sub-conversions (N<sub>sub</sub>) directly in the **Widget/Sensor parameters** tab of the CAPSENSE&trade; Tuner.

     > **Note:** **Number of sub-conversions** should be greater than or equal to 8.

3. Load the parameters to the device and measure SNR as mentioned in steps 10 and 11 in the **Monitor data using CAPSENSE&trade; Tuner** section. 
   
      Repeat steps 1 to 3 until the following conditions are met:
      - Measured SNR from the previous stage is greater than 5:1
      - Signal count is greater than 50

4. If the system is very noisy (>40% of signal), enable filters.

   This example has the CIC2 filter enabled, which increases the resolution for the same scan time. See [AN234231 - Achieving lowest-power capacitive sensing with PSoC&trade; 4000T](https://www.infineon.com/AN234231) for detailed information on the CIC2 filter. Whenever you enable the CIC2 filter, it is recommended to enable the IIR filter for optimal noise reduction. Therefore, this example has the IIR filter enabled as well.
   <ol type="A">
   <li>

   Open CAPSENSE&trade; Configurator from ModusToolbox&trade; **Quick Panel** and select the appropriate filter:

   ##### **Figure 30. Filter settings in CAPSENSE&trade; Configurator**

   <img src="images/advanced-filter-settings.png" alt="Figure 30"/>
   </li>
   <li>

   Enable the filter based on the type of noise in your system. See [AN85951 – PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide](https://www.infineon.com/AN85951) for more details.
   </li>
   <li>

   Click **Save** and close CAPSENSE&trade; Configurator. Program the device to update the filter settings.
   </li>

   > **Note** : Increasing **Number of sub-conversions** and enabling filters increases the scan time, which in turn, reduces the sensor responsiveness and increases the power consumption. Therefore, the **Number of sub-conversions** and filter configuration must be optimized to achieve a balance between SNR, power and refresh rate. 




### **Stage 4: Tune threshold parameters**
--------------------

Various thresholds, relative to the signal, need to be set for each sensor. Do the following in CAPSENSE&trade; Tuner to set up the thresholds for a widget:

1. Switch to the **Graph View** tab and select **Button0**.

2. Touch the sensor and monitor the touch signal in the **Sensor signal** graph, as shown in **Figure 31**. 

   ##### **Figure 31. Sensor signal when the sensor is touched**

   <img src="images/tuner-diff-signal.png" alt="Figure 31"/>

3. Note the signal measured and set the thresholds according to the following recommendations:

   - **Finger threshold** = 80% of the signal

   - **Noise threshold** = 40% of the signal

   - **Negative noise threshold** = 40% of the signal

   - **Hysteresis** = 10% of the signal

   - **Debounce** = 3

4. Set the threshold parameters in the **Widget/Sensor parameters** section of the CAPSENSE&trade; Tuner:

   ##### **Figure 32. Widget threshold parameters**

   <img src="images/tuner-threshold-settings.png" alt="Figure 32"/>

5. For the **LowPower0_Sns0** low power sensor, first configure the finger threshold to 65535 and wait for the application to enter low-power mode. Because the Finger threshold is set to maximum, touching the low power button will not switch the application to active mode. Repeat steps 2 to 4 for the low power button.

6. Apply the settings to the device by clicking **Apply to Device**.

   ##### **Figure 33. Apply settings to device**

   <img src="images/tuner-apply-settings-device.png" alt="Figure 33"/>

   After applying the configuration, test the performance by touching the button. If your sensor is tuned correctly, you will see the touch status go from 0 to 1 in the **Status** panel of the **Graph View** tab as shown in **Figure 34**. The status of the button is also indicated by LED1 in the kit; LED1 turns ON when you touch the button and turns OFF when you remove your finger.

   ##### **Figure 34. Sensor status in CAPSENSE&trade; Tuner**

   <img src="images/tuner-status.png" alt="Figure 34"/>

7. Click **Apply to Project**, as shown in **Figure 35**. The change is updated in the *design.cycapsense* file. Close CAPSENSE&trade; Tuner and launch CAPSENSE&trade; Configurator. You can now check all the changes you have made in the CAPSENSE&trade; Tuner in the CAPSENSE&trade; Configurator.

   ##### **Figure 35. Apply settings to Project**

   <img src="images/tuner-apply-settings-project.png" alt="Figure 35"/>

   **Table 5. Sensor tuning parameters obtained for CY8CKIT-040T kit**

   Parameter|	 Button0|	 LowPower0
   :--------|:------|:------
   Signal	| 900 |900
   Finger threshold 	| 720 |720
   Noise threshold |360|360
   Negative noise threshold	|360 |360
   Low baseline reset	| 30 |30
   Hysteresis	| 90 |NA
   ON debounce	| 6|1

<br>

###  **Process time measurement**
--------------------


To set the optimum refresh rate of each power mode, measure the process time of the application.

Follow these steps to measure the process time of the blocks of application code while excluding the scan time:

1. Enable ENABLE_RUN_TIME_MEASUREMENT macro in *main.c* as follows:
      
      ```
      #define ENABLE_RUN_TIME_MEASUREMENT (1u)
      ```     
      This macro enables the system tick configuration and runtime measurement functionalities.

2.	Place the start_runtime_measurement() function call before your application code and the stop_runtime_measurement() function call after it. The stop_runtime_measurement() function returns the execution time in microseconds (µs). 
 
      ```   
         #if ENABLE_RUN_TIME_MEASUREMENT
            uint32_t run_time = 0;
            start_runtime_measurement();
         #endif

         /* User Application Code Start */
         .
         .
         .
         /* User Application Code Stop */

         #if ENABLE_RUN_TIME_MEASUREMENT
            run_time = stop_runtime_measurement();
         #endif
      ```

3.	Run the application in debug mode with breakpoints placed at the active_processing_time and alr_processing_time variables as follows,
      ```
         #if ENABLE_RUN_TIME_MEASUREMENT
            active_processing_time=stop_runtime_measurement();
         #endif
         and 
         #if ENABLE_RUN_TIME_MEASUREMENT
            alr_processing_time=stop_runtime_measurement();
         #endif
      ``` 
4. Read the variables by adding them into the **Expressions view** tab.
5. Update related macros with the earlier measured processing times in *main.c* as follows:
      ```
      #define ACTIVE_MODE_PROCESS_TIME     (xx)

      #define ALR_MODE_PROCESS_TIME        (xx)
      ```
### **Scan time measurement**
--------------------
Scan time is also required for calculating the refresh rate of the application power modes. The total scan time of all the widgets in this code example is 10 µs.

It can be calculated as follows:

The scan time includes the MSCLP initialization time, Cmod, and the total sub-conversions of the sensor. 

To control the Cmod initialization sequence, set the **Enable Coarse initialization bypass** configurator as follows:

Enable coarse initialization bypass | Behavior
:-------------|:---------
TRUE|Cmod initialization happens only once before scanning the sensors of the widget
FALSE| Cmod initialization happens before scanning each sensor of the widget

<br>

Use the following equations to measure the widget scan time based on coarse initialization bypass options selected: 

**Equation 2. Scan time calculation of a widget with coarse initialization bypass enabled**

<img src="images/scan_time_equation_bypass_enabled.png" width=500/>

**Equation 3. Scan time calculation of a widget with coarse initialization bypass disabled**

<img src="images/scan_time_equation_bypass_disabled.png" width=500/>


Where,

$n$ - Total number of sensors in the widget

$N_{init}$ - Number of init sub-conversions

$N_{sub}$ - Number of sub-conversions

$SnsClkDiv$ - Sense clock divider

$F_{mod}$ - Modulator clock frequency

$k$ - Measured Initialization time (MSCLP+Cmod).

This value of $k$ measured for this application is ~9 µs. It remains constant for all widgets. 

You can measure $k$ using the oscilloscope as shown in **Figure 36**.

 ##### **Figure 36. $k$ value measurement**

   <img src="images/scantime_wave.png" alt="Figure 36"/>

Update the following macros in *main.c* using the scan time calculated. The value remains the same for both the macros for this application.

```
#define ACTIVE_MODE_FRAME_SCAN_TIME     (xx)

#define ALR_MODE_FRAME_SCAN_TIME        (xx)
```


> **Note :** If the application has more than one widget, add the scan times of individual widgets calculated.


## Debugging


You can debug the example to step through the code.


<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).





<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.
</details>


By default, the debug option is disabled in the Device Configurator. To enable the debug option, see the [Set up VDDA and debug mode in Device Configurator](#set-up-the-vdda-supply-voltage-and-debug-mode-in-device-configurator) section. To achieve low power consumption, it is recommended to disable it. 


## Design and implementation

The project contains a button configured as a regular and low-power widget in CSD-RM sensing mode. See the [Tuning procedure](#tuning-procedure) section for step-by-step instructions to configure other settings of the CAPSENSE&trade; Configurator.

Two LEDs are used in this project: 
- LED1 shows the button touch status. It turns ON when you touch the button and turns OFF when you lift your finger. 
- LED2 shows the current low-power state of the application. 

The following table shows the LED2 response based on the low-power state.

**Table 6. LED2 state for different application power modes**

   Power mode state  | LED2 status/color
   :---------------------| :-----
   Active  | Red
   Active Low Refresh rate <br>(ALR)  | Yellow
   Wake on Touch <br>(WoT) | Green

   <br>

There are three power states defined for this project:

- Active mode

- Active Low Refresh rate (**ALR**) mode

- Wake-On-Touch (**WoT**) mode

After reset, the device is in Active mode, and scans the regular CAPSENSE&trade; widgets with a high refresh rate (**128 Hz**). If user activity is detected in any other mode, the device is transferred to the Active mode to provide the best user interface experience. Active mode has the highest power consumption. Therefore, ensure that your design reduces the time spent in Active mode as much as possible.

If there is no user activity for a certain time (`ACTIVE_MODE_TIMEOUT_SEC` = 10 seconds), the application transitions to the ALR mode. Here, the refresh rate is reduced to 32 Hz; this mode acts as an intermediate state before moving to the lowest-power mode (WoT mode). The WoT mode can also be used for periodically updating baselines of sensors while there is no user activity for a long time.

Further non-activity for a certain time (`ALR_MODE_TIMEOUT_SEC` = 5 seconds) transitions the application to the lowest-power (WoT) mode, which scans the low-power widget at a low refresh rate (16 Hz) and processes the results without CPU intervention.

> **Note:** An internal low-power timer (MSCLP timer) is available in CAPSENSE&trade; MSCLP hardware to set the refresh rate for each power mode as follows:
- ACTIVE and ALR modes: use the `Cy_CapSense_ConfigureMsclpTimer()` function
- WoT mode: use the Wake-on-Touch scan interval in CAPSENSE&trade; Configurator

Different power modes and transition conditions for a typical use case are as follows: 

   ##### **Figure 37. State machine showing different power states of the device**

   <img src="images/capsense_lp_firmware_state_machine.png" alt="Figure 37" width="500"/>


The project uses the [CAPSENSE&trade; middleware](https://github.com/Infineon/capsense) (see ModusToolbox&trade; user guide for more details on selecting a middleware). See [AN85951 – PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide](https://www.infineon.com/AN85951) for more details on CAPSENSE&trade; features and usage.

The [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) provides a GUI-based tuner application for debugging and tuning the CAPSENSE&trade; system. The CAPSENSE&trade; Tuner application works with EZI2C and UART communication interfaces. This project has an SCB block configured in EZI2C mode to establish communication with the onboard KitProg, which enables you to read the CAPSENSE&trade; raw data through the CAPSENSE&trade; Tuner. See [EZI2C Peripheral settings](#resources-and-settings).

The CAPSENSE&trade; data structure that contains the CAPSENSE&trade; raw data is exposed to the CAPSENSE&trade; Tuner by setting up the I2C communication data buffer with the CAPSENSE&trade; data structure. This enables the tuner to access the CAPSENSE&trade; raw data for tuning and debugging CAPSENSE&trade;.


### Set up the VDDA supply voltage and debug mode in Device Configurator

1. Open Device Configurator from the Quick Panel.

2. Go to the **System** tab. Select the **Power** resource and set the VDDA value under **Operating conditions** as shown in **Figure 38**.

   ##### **Figure 38. Setting the VDDA supply in System tab of Device Configurator**

   <img src="images/vdda-settings.png" alt="Figure 38"/>
<br>

3. By default, the debug mode is disabled for this application to reduce power consumption. Enable the debug mode to enable the SWD pins as as shown in **Figure 39*.

   ##### **Figure 39. Enable debug mode in the System tab of Device Configurator**

   <img src="images/enable_debug.png" alt="Figure 39"/>

<br>

## Resources and settings
<br>

1. The EZI2C and SPI configurations used for Tuner communications and RGB Led control respectively are shown in **Figure 40**.
   ##### **Figure 40. EZI2C settings**

   <img src="images/ezi2c-config.png" alt="Figure 40" width="800"/>

   ##### **Figure 41. SPI settings**

   <img src="images/spi-config.png" alt="Figure 41" width="800"/>
<br>

### **Table 7. Application resources**

 Resource  |  Alias/object     |    Purpose
 :------- | :------------    | :------------
 SCB (I2C) (PDL) | CYBSP_EZI2C          | EZI2C slave driver to communicate with CAPSENSE&trade; Tuner
 SCB (SPI) (PDL) | CYBSP_MASTER_SPI          | SPI master driver to control serial LEDs
 CAPSENSE&trade; | CYBSP_MSC | CAPSENSE&trade; driver to interact with the MSC hardware and interface the CAPSENSE&trade; sensors
 Digital pin | CYBSP_SERIAL_LED | To show the button operation and power mode states

</details>

<br>

## Firmware flow
<br>

##### **Figure 42. Firmware flowchart**

<img src="images/firmware-flowchart.png" alt="Figure 42" width="800"/>

<br>

## Related resources

Resources  | Links
-----------|----------------------------------
Application notes  | [AN79953](https://www.infineon.com/AN79953) – Getting started with PSoC&trade; 4 <br> [AN85951](https://www.infineon.com/AN85951) – PSoC&trade; 4 and PSoC&trade; 6 MCU CAPSENSE&trade; design guide <br>  [AN234231](https://www.infineon.com/AN234231) – Achieving lowest-power capacitive sensing with PSoC&trade; 4000T
Code examples  | [Using ModusToolbox&trade;](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation | [PSoC&trade; 4 datasheets](https://www.infineon.com/cms/en/search.html#!view=downloads&term=psoc4&doc_group=Data%20Sheet) <br>[PSoC&trade; 4 technical reference manuals](https://www.infineon.com/cms/en/search.html#!view=downloads&term=psoc4&doc_group=Additional%20Technical%20Information)
Development kits | Select your kits from the [Evaluation board finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board).
Libraries on GitHub  | [mtb-pdl-cat2](https://github.com/Infineon/mtb-pdl-cat2) – PSoC&trade; 4 Peripheral Driver Library (PDL) <br>  [mtb-hal-cat2](https://github.com/Infineon/mtb-hal-cat2) – Hardware Abstraction Layer (HAL) library
Middleware on GitHub  | [capsense](https://github.com/Infineon/capsense) – CAPSENSE&trade; library and documents
Tools  | [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSoC&trade; Industrial/IoT MCUs, AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices, XMC&trade; Industrial MCUs, and EZ-USB&trade;/EZ-PD&trade; wired connectivity controllers. ModusToolbox&trade; incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development.

<br>



## Other resources


Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.



## Document history


Document title: *CE235111* – *PSoC&trade; 4: MSCLP CAPSENSE&trade; low power*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example
 1.0.1   | Minor readme update
 2.0.0   | Major update to support ModusToolbox&trade; v3.0. This version is not backward compatible with previous versions of ModusToolbox&trade;
 3.0.0   | Major update to support ModusToolbox&trade; v3.1 and the BSP changes. This version is not backward compatible with previous versions of ModusToolbox&trade;
 3.0.1   | Minor configuration and readme update
 4.0.0   | Major update to support ModusToolbox™ v3.2 and CAPSENSE™ Middleware v5.0. This version is not backward compatible with previous versions of ModusToolbox™ software.

<br>



All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.


---------------------------------------------------------

© Cypress Semiconductor Corporation, 2022-2023. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, ModusToolbox, PSoC, CAPSENSE, EZ-USB, F-RAM, and TRAVEO are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.
