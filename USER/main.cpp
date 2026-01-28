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
#include "bsp_led.h"
#include "bsp_lcd.h"
#include "bsp_key.h"
#include "bsp_sflash.h"
#include "bsp_tm1638.h"
#include "drv_wrapper.h"
#include "mcu_core.h"
#include "Logger.h"
#include "device_manager.h"
#include "Services/pocket_detect.h"
#include "cm_backtrace.h"


#include "PageManager/PageManager.h"
#include "PageManager/PageBase.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demos.h"

#include "fault_test.h"
#include "unit_test.h"

#define DELAY 500

extern __IO uint8_t g_ns2009_irq_flag;

#ifdef __cplusplus
extern "C" {
#endif

extern void NS2009_TickHandler(void);
extern void DRV_TimerIntervalCore(void);

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
  BSP_LED_Toggle(LED_DEBUG);
  lv_tick_inc(1);
  DRV_TimerIntervalCore();
}

__IO uint16_t dma_trans_complete_flag = 0;

/**
 * @brief  main function.
 * @param  none
 * @retval none
 */
int main(void)
{
  Core_Init();
  BSP_LED_Init();
  BSP_KEY_Init();
  BSP_SFlash_Init();
  BSP_TM1638_Init();
  DRV_Init();
  loggerInit(LOG_LEVEL_DEBUG);

  cm_backtrace_init("W20_Mainboard_Firmware", "V1.0", "V1.0.0");
  DM_DeviceInitALL();

  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  App_Init();
  ADC_DMA_Init();

  // lv_demo_widgets();
  // lv_demo_benchmark();

  log_i("System initialized.");
  printf("System initialized.\r\n");

  Timer_SetInterrupt(TIM8, 500000, tim8_irq_callback); // 500ms
  Timer_SetInterrupt(TIM6, 1000, tim6_irq_callback);   // 1ms

  Timer_SetEnable(TIM8, TRUE);
  Timer_SetEnable(TIM6, TRUE);

  log_i("PWM Init Channel %d", PWM_Init(PD13, 1000, 10000));
  log_i("PWM Init Channel %d", PWM_Init(PD14, 1000, 10000));
  log_i("PWM Init Channel %d", PWM_Init(PD15, 1000, 10000));

  PWM_Write(PD13, 500);
  PWM_Write(PD14, 500);
  PWM_Write(PD15, 500);

  // log_i("PWM Init Channel %d", PWM_Init(SCREEN_BLK_PIN, 1000, 10000));
  // PWM_Write(SCREEN_BLK_PIN, 500);

  Device_t *touch_main = DM_DeviceFind("TOUCH_MAIN");

  // Pocket_Detect_Init();
  // ADCx_Start(ADC1);

  // DRV_SetInterval(lv_task_handler_adapter, 5, TIMER_INTERVAL_REPEAT);
  // DRV_SetInterval(NS2009_TickHandler, 1, TIMER_INTERVAL_REPEAT);
  // TEST_main();
  DRV_SetInterval(BSP_TM1638_ReadKey, 500, TIMER_INTERVAL_REPEAT);


  while (1)
  {
    /* 5ms调用一次 */
    DRV_DelayMs(1);
    static uint32_t cnt = 0;
    if (cnt++ >= 5)
    {
      cnt = 0;
      lv_timer_handler(); /* let the GUI do its work */
    }

    // extern __IO uint8_t g_ns2009_irq_flag;
    // if (g_ns2009_irq_flag) {
    //   g_ns2009_irq_flag = 0;

    //   DRV_Touch_Point_t point;
    //   DRV_Touch_Read(touch_main, &point);
    //   log_d("Touch Read (IRQ): X=%d, Y=%d, Pressed=%d", point.x, point.y,
    //   point.pressed);
    // }

    if (dma_trans_complete_flag > 0)
    {
      // log_d("DMA TC Flag: %d", dma_trans_complete_flag);

      /* 12位ADC转换为3.3V电压 */
      // uint16_t raw_hop = 0;
      // uint16_t raw_sta = 0;
      // uint16_t raw_irr = 0;
      // uint16_t raw_irl = 0;
      // DRV_IR_GetRawData(ir_hop, &raw_hop);
      // DRV_IR_GetRawData(ir_sta, &raw_sta);
      // DRV_IR_GetRawData(irr, &raw_irr);
      // DRV_IR_GetRawData(irl, &raw_irl);

      // float voltage_hop = (float)raw_hop * 3.3f / 4095.0f;
      // float voltage_sta = (float)raw_sta * 3.3f / 4095.0f;
      // float voltage_irr = (float)raw_irr * 3.3f / 4095.0f;
      // float voltage_irl = (float)raw_irl * 3.3f / 4095.0f;
      // log_d("IR Hopper Voltage: %fV", voltage_hop);
      // log_d("IR Stacker Voltage: %fV", voltage_sta);
      // log_d("IR IRR Voltage: %fV", voltage_irr);
      // log_d("IR IRL Voltage: %fV", voltage_irl);

      dma_trans_complete_flag--;
    }

    // DRV_DelayMs(DELAY);
  }
}
