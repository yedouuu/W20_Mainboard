/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */

#include "unity.h"

#include "Common/common.h"
#include "App.h"
#include "AppTasks.h"
#include "bsp_led.h"
#include "bsp_lcd.h"
#include "bsp_key.h"
#include "bsp_sflash.h"
#include "bsp_tm1638.h"
#include "bsp_motor.h"
#include "bsp_encoder.h"
#include "drv_wrapper.h"
#include "mcu_core.h"
#include "Logger.h"
#include "device_manager.h"
#include "Services/pocket_detect.h"
#include "Services/machine_ctrl.h"
#include "System/key_scan.h"
// #include "cm_backtrace.h"

#include "PageManager/PageManager.h"
#include "PageManager/PageBase.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demos.h"

#include "FreeRTOS.h"
#include "task.h"

#include "fault_test.h"
#include "unit_test.h"

#define DELAY 500

#ifdef __cplusplus
extern "C" {
#endif

extern void NS2009_TickHandler(void);

void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
  printf( "STACK OVERFLOW DETECTED: %s\n", pcTaskName );
}

void vApplicationTickHook()
{
  lv_tick_inc(1);
}

#ifdef __cplusplus
}
#endif


void tim8_irq_callback(void)
{
  // log_d("TIM8 interrupt triggered.");
  BSP_LED_Toggle(LED1);
  // Pocket_Detect_Update();
}

void tim6_irq_callback(void)
{
  // log_d("TIM6 interrupt triggered.");
  // BSP_LED_Toggle(LED_DEBUG);
}

/**
 * @brief  main function.
 * @param  none
 * @retval none
 */
int main(void)
{
  Core_Init();
  BSP_LED_Init();
  BSP_SFlash_Init();
  DRV_Init();
  loggerInit(LOG_LEVEL_DEBUG);
  Machine_Ctrl_Init();

  // cm_backtrace_init("W20_Mainboard_Firmware", "V1.0", "V1.0.0");
  DM_DeviceInitALL();

#if APP_TASK_ENABLE_LVGL
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
#endif

  Key_Scan_Init();

  App_Init();
  ADC_DMA_Init();

  // lv_demo_widgets();
  // lv_demo_benchmark();

  log_i("System initialized.");
  printf("System initialized.\r\n");

  Timer_SetInterrupt(TIM8, 500000, tim8_irq_callback); // 500ms
  Timer_SetEnable(TIM8, TRUE);

  Pocket_Detect_Init();
  ADCx_Start(ADC1);

  // DRV_SetInterval(lv_task_handler_adapter, 5, TIMER_INTERVAL_REPEAT);
  // DRV_SetInterval(NS2009_TickHandler, 1, TIMER_INTERVAL_REPEAT);
  // TEST_main();
  DRV_SetInterval(Key_ScanTask, 50, TIMER_INTERVAL_REPEAT);

  AppTasks_Init();

  vTaskStartScheduler();

  while (1)
  {
  }
}
