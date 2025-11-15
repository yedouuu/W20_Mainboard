# W20_Mainboard Project - AI Agent Instructions

This document provides essential guidance for AI coding agents working on the WL_W20_Mainboard embedded software project.

## 1. Project Overview & Architecture

This project is the main control software for a vertical bill counter, using an **AT32F403A MCU**. The core architecture is built on **FreeRTOS** for multitasking and **LVGL** for the user interface.

- **MCU**: Artery AT32F403A (`/Vendor/MDK-ARM_F403A`)
<!-- - **RTOS**: FreeRTOS (`/Libraries/FreeRTOS`) -->
- **GUI Library**: LVGL (`/Libraries/lvgl`)
- **Application Logic**: Resides in `/USER/`

The primary goal is to process data from a "ZM GL20" image recognition board via UART to count and validate currency. 
The system also drives a stepper motor for bill feeding, controls a TFT LCD display for user interface, and integrates multiple sensors including UV (ultraviolet) sensors for counterfeit detection, magnetic signal acquisition for magnetic ink detection, transmissive IR sensors for bill detection, and reflective IR sensors for positioning. External interfaces include a printer port for receipt printing and an external display interface for additional output.


### Key Directories:
- `USER/`: Main application-level code. This is where most new business logic should go.
    - `Application/`: High-level application tasks and state management.
    - `Middleware/`: Middleware integrations of Third-party libraries.
    - `HAL/`: Hardware Abstraction Layer for the AT32 MCU. Encapsulates direct hardware access by Using Bridge Deisgn Pattern and Object-Oriented principles.
    - `Resources/`: Fonts, images, and other UI resources.
    - `Utils/`: Utility functions and modules like logging.
    - `Common/`: Common definitions and shared components.
    - `Pages/`: LVGL screen definitions and UI logic.
- `Framework/`: Framework-level code.
    - `PageManager/`: Manages LVGL page transitions and lifecycle.
- `Vendor/`: Contains MCU-specific libraries, drivers, and the Keil MDK project files.
    - `MDK-ARM_F403A/`: Keil MDK project and AT32F403A vendor libraries.
    - `Simulator/`: Contains Visual Studio and VS Code simulator projects for off-target UI development and testing.
- `Libraries/`: Submodule reference for Third-party libraries like LVGL and FreeType, would be edited as needed, just a reference.


## 2. Development Workflow

The primary development and debugging environment is **Keil MDK**.

- **Project File**: `Vendor/MDK-ARM_F403A/MDK-ARM_F403A.uvprojx`
- **Build**: Use the Keil MDK IDE to build the project.
- **Debugging**: On-chip debugging is done via Keil MDK with a J-Link or similar probe.

For UI development, a simulator can be used:
- **VS Code Simulator**: `Simulator/lv_port_pc_vscode/simulator.code-workspace`
- **Visual Studio Simulator**: `Simulator/lv_port_pc_visual_studio/LVGL.sln`

When working on UI elements in `USER/Pages/`, it's often faster to test them in the simulator before deploying to the target hardware.

## 3. Code Conventions & Patterns

### Hardware Abstraction
- All direct hardware register manipulation should be within the `USER/HAL/` directory.
- The `HAL.h` file provides a clean interface for the application layer to access hardware features like LEDs, UART, etc.
- **Example**: To enable a peripheral clock, use the `crm_periph_clock_enable()` function from the vendor library, but wrap it in a higher-level HAL function if it's part of a larger initialization sequence. See `USER/HAL/HAL.c`.

<!-- ### RTOS Usage (FreeRTOS)
- Tasks are defined in `USER/Application/App.c`.
- Communication between tasks should prefer FreeRTOS primitives like queues and semaphores over global variables.
- Follow FreeRTOS naming conventions (e.g., `xTaskCreate`, `vTaskDelay`). -->

### UI Development (LVGL)
- UI screens are organized into files under `USER/Pages/`.
- Using MVP (Model-View-Presenter) pattern is encouraged for better separation of concerns.
- Each new screen should have this three files `MainModel.c(Model)`, `MainView.c(View)`, `Main.c(Presenter)`.
- Create a new C file for major UI component.
- Use LVGL's event-driven model for user interactions.
- Fonts and other resources are in `USER/Resources/`.

### Naming and Style
- The codebase primarily uses **This_Case** for functions and variables.
- Macros are in `ALL_CAPS_SNAKE_CASE`.
- Header guards follow the `__FILENAME_H__` pattern.
- Use `<stdint.h>` types like `uint32_t`, `int16_t` for integer variables to ensure portability and clarity.
- Indentation is 2 spaces. No tabs(IMPORTANT).
- Multi-line variable declarations should be aligned for better readability.
- Function comments should use Doxygen style for automatic documentation generation.


## 4. Key Integration Points

- **Image Board Communication**: The core of the application is communication with the ZM GL20 image board. This happens over UART. The protocol logic (sending commands, parsing responses) is a critical component. Any changes here must be carefully tested.

- **System Tick**: The core timing is based on SysTick. A custom implementation in `Vendor/MDK-ARM_F403A/Platform/Core/systick.c` provides `delay_ms()` and `delay_us()` for platform-level operations. For application layer code, use the unified HAL_Time interface for delay operations instead of calling these functions directly. This ensures proper abstraction and maintainability across different modules.

- **Clock Configuration**: System and peripheral clocks are configured via the **CRM** (Clock and Reset Management) module. See `at32f403a_407_crm.h` for peripheral clock definitions.
