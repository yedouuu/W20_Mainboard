# AGENT.md

This file provides guidance to AI AGENT when working with code in this repository.

## Project Overview

Embedded firmware for a vertical bill counter (立式点钞机) running on **Artery AT32F403AVGT7** (ARM Cortex-M4F, 240MHz, 1MB Flash / 224KB RAM). Uses **FreeRTOS V10.2.1** for multitasking and **LVGL** for the 480×272 TFT LCD UI (8080 interface via XMC). The core function is communicating with a "ZM GL20" image recognition board over UART to count and validate currency, while driving a stepper motor and processing multiple sensor inputs (UV, magnetic, transmissive IR, reflective IR).

## Build & Development

- **Primary IDE**: Keil MDK — project file at `Vendor/MDK-ARM_F403A/MDK-ARM_F403A.uvprojx`
- **Alternative**: EIDE (VS Code extension) with artifacts in `Vendor/EIDE/build/W20_Mainboard/`
- **PC Simulator** (for UI development): VS Code workspace at `Vendor/Simulator/lv_port_pc_vscode/simulator.code-workspace` or Visual Studio solution at `Vendor/Simulator/lv_port_pc_visual_studio/LVGL.sln`
- **Debugging**: On-chip via Keil MDK with J-Link or similar probe
- **Tests**: Unity framework (`USER/Tests/`), run on-target via Keil debugger — not automated. Test entry point is `TEST_main()` in `unit_test.c`, called from `main.cpp` (currently commented out)

## Architecture

The codebase follows a strict layered architecture:

```
USER/Application/          ← App logic, Pages (MVP), Services, System
USER/HAL/                  ← Hardware Abstraction (type-safe wrappers + Adapter layer)
Framework/DeviceManager/   ← Auto-registration via linker section (.device_table)
Framework/PageManager/     ← C++ LVGL page stack with lifecycle callbacks
Vendor/.../Platform/BSP/   ← Low-level hardware access (GPIO, registers)
Vendor/.../Platform/Core/  ← MCU core (clock init, SysTick, GPIO/timer/UART abstractions)
Vendor/.../Firmware/        ← Artery AT32F403A std peripheral driver + CMSIS
```

### Device Registration (linker-section magic)

`DEVICE_EXPORT(dev)` in Adapter files places a `Device_t*` into the `.device_table` linker section. `DM_DeviceInitALL()` iterates the section at runtime to initialize all devices automatically — no manual registration in `main.cpp` needed. Each `Device_t` has a generic `Device_Ops_t` vtable (init/deinit/open/close/read/write/ioctl) plus driver-specific ops via a castable `ops` pointer.

**Usage rule**: System-level code uses `DeviceManager` interfaces (`DM_DeviceFind`, `DM_DeviceInit`). Business logic uses specific HAL interfaces (`DRV_Motor_Run`, `DRV_IR_GetRawData`) for type safety and readability.

### HAL Bridge Pattern

Adding a new peripheral follows this flow:
1. Implement hardware logic in **BSP** (`Vendor/.../Platform/BSP/bsp_xxx.c`)
2. Define type-safe interfaces in **HAL** (`USER/HAL/drv_xxx_wrapper.c/.h`)
3. Create a `Device_t` in **Adapter** (`USER/HAL/Adapter/`), map HAL functions to `Device_Ops`, register with `DEVICE_EXPORT`

Even if application code uses HAL functions directly, populate `Device_Ops` fully to support generic debugging tools.

### PageManager (C++ LVGL navigation)

Pages follow MVP pattern under `USER/Application/Pages/` with Page(Presenter), PageModel, PageView. Each page registers via `Install("ClassName", "pathName")` in `AppPageFactory`. The `__Template/` directory provides a starting point for new pages.

### FreeRTOS Task Layout

- LVGL_Task — priority 3, stack 2048 words (UI handler)
- TEST_Task / TEST_Task2 — priority 2, stack 128 words (development/test)
- Monitor_Task — priority 2, stack 256 words (runtime stats)
- `vApplicationTickHook` calls `lv_tick_inc(1)` every 1ms tick

### Key Services

- `machine_ctrl.c` — FSM for bill counter operation (IDLE → STARTUP → RUNNING → STANDBY → STOP)
- `pocket_detect.c` — Sensor detection (hopper, stacker, IR left/right)
- `key_scan.c` — Keyboard scanning via TM1638
- `sample.c` — ADC sampling for UV/magnetic sensors

## Code Conventions

- **Naming**: `Caps_Snake_Case` for functions/variables, `ALL_CAPS` for macros
- **Header guards**: `__FILENAME_H__`
- **Types**: `<stdint.h>` fixed-width types (`uint32_t`, `int16_t`, etc.)
- **Error handling**: `Status_t` enum from `USER/Application/Common/errno.h`
- **Logging**: `log_e`, `log_w`, `log_i`, `log_d`, `log_v` macros via `Utils/Logger.h`
- **Comments**: Doxygen-style (`@brief`, `@param`, `@retval`)
- **Formatting**: 2-space indentation, no tabs, 80-char column limit (`.clang-format` at repo root)
- **Language**: C (C99/C11) for most code; C++ in PageManager framework and LVGL port. `main.cpp` is C++.
