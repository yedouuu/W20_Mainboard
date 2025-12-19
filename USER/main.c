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
#include "drv_wrapper.h"
#include "mcu_core.h"
#include "Logger.h"
#include "device_manager.h"
#include "Services/pocket_detect.h"

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demos.h"


#define DELAY                            500


extern __IO uint8_t g_ns2009_irq_flag;
extern void NS2009_TickHandler(void);
extern void DRV_TimerIntervalCore(void);

void tim8_irq_callback(void)
{
  // log_d("TIM8 interrupt triggered.");
  BSP_LED_Toggle(LED1);
  //ns2009_irq_flag = 1;
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

void lv_log_adapater(lv_log_level_t level, const char * buf)
{
  switch(level)
  {
    case LV_LOG_LEVEL_ERROR:
      log_e("LVGL", "%s", buf);
      break;
    case LV_LOG_LEVEL_WARN:
      log_w("LVGL", "%s", buf);
      break;
    case LV_LOG_LEVEL_INFO:
      log_i("LVGL", "%s", buf);
      break;
    case LV_LOG_LEVEL_TRACE:
      log_d("LVGL", "%s", buf);
      break;
    default:
      log_v("LVGL", "%s", buf);
      break;
  }
}


void lv_example_get_started_1(void)
{
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void lv_task_handler_adapter(void)
{
  lv_task_handler(); /* let the GUI do its work */
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
  BSP_KEY_Init();
  loggerInit(LOG_LEVEL_DEBUG);
  DM_DeviceInitALL();
  App_Init();
  ADC_DMA_Init();

  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();
  lv_demo_widgets();
  
  log_i("System initialized.");
  

  //BSP_LCD_Clear(&lcd_main_res, RED);
  // Device_t* lcd_main = DM_DeviceFind("LCD_MAIN");
  // DRV_LCD_Clear(lcd_main, BLUE);

  // DRV_LCD_SetBlock(lcd_main, 0, 0, 479, 0);
  // DRV_LCD_WriteBlock(lcd_main, GREEN, 480);

  // DRV_LCD_SetBlock(lcd_main, 0, 5, 479, 5);
  // DRV_LCD_WriteBlock(lcd_main, RED, 480);

  // DRV_LCD_SetBlock(lcd_main, 0, 10, 10, 10);
  // DRV_LCD_WriteBlock(lcd_main, RED, 10);

  // DRV_LCD_SetBlock(lcd_main, 10, 10, 10, 10);
  // DRV_LCD_WriteBlock(lcd_main, GREEN, 1);

  // DRV_LCD_SetBlock(lcd_main, 240, 136, 240, 136);
  // DRV_LCD_WriteBlock(lcd_main, GREEN, 1);


  // for ( uint32_t i=100; i<200; i++ ) {
  //   for ( uint32_t j=0; j<480; j++ ) {
  //     BSP_LCD_Set_Block(lcd_main->res, j, i, j+1, i+1);
  //     DRV_LCD_WritePoint(lcd_main, GREEN);

  //     // DRV_LCD_DrawPoint(lcd_main, j, i, GREEN);
  //   }
  // }
  
  // for ( uint32_t i=200; i<272; i++ ) {
  //   for ( uint32_t j=0; j<480; j++ ) {
  //     DRV_LCD_DrawPoint(lcd_main, j, i, BLUE);
  //   }
  // }

  // DRV_LCD_SetBlock(lcd_main, 0, 0, 479, 99);
  // DRV_LCD_WriteBlock(lcd_main, RED, 480*100);

  // DRV_LCD_SetBlock(lcd_main, 0, 100, 479, 199);
  // DRV_LCD_WriteBlock(lcd_main, YELLOW, 480*100);

  // DRV_LCD_SetBlock(lcd_main, 0, 200, 479, 271);
  // DRV_LCD_WriteBlock(lcd_main, CYAN, 480*72);

  // 按钮
  // lv_obj_t *myBtn = lv_btn_create(lv_scr_act());                               // 创建按钮; 父对象：当前活动屏幕
  // lv_obj_set_pos(myBtn, 10, 10);                                               // 设置坐标
  // lv_obj_set_size(myBtn, 120, 50);                                             // 设置大小
  
  // // 按钮上的文本
  // lv_obj_t *label_btn = lv_label_create(myBtn);                                // 创建文本标签，父对象：上面的btn按钮
  // lv_obj_align(label_btn, LV_ALIGN_CENTER, 0, 0);                              // 对齐于：父对象
  // lv_label_set_text(label_btn, "Test");                                        // 设置标签的文本

  // // 独立的标签
  // lv_obj_t *myLabel = lv_label_create(lv_scr_act());                           // 创建文本标签; 父对象：当前活动屏幕
  // lv_label_set_text(myLabel, "Hello world!");                                  // 设置标签的文本
  // lv_obj_align(myLabel, LV_ALIGN_CENTER, 0, 0);                                // 对齐于：父对象
  // lv_obj_align_to(myBtn, myLabel, LV_ALIGN_OUT_TOP_MID, 0, -20);               // 对齐于：某对象

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

  Device_t* touch_main = DM_DeviceFind("TOUCH_MAIN");

  // Pocket_Detect_Init();
  // ADCx_Start(ADC1);
	// 按钮

  // lv_example_get_started_1();

  DRV_SetInterval(lv_task_handler_adapter, 5, TIMER_INTERVAL_REPEAT);
  DRV_SetInterval(NS2009_TickHandler, 1, TIMER_INTERVAL_REPEAT);

  while(1)
  {  
    /* 5ms调用一次 */
    // DRV_DelayMs(1);
    // static uint32_t cnt = 0;
    // if ( cnt++ >= 5 ) {
		// 	cnt = 0;
    //   // lv_timer_handler(); /* let the GUI do its work */
    //   lv_task_handler();
    // }

    // extern __IO uint8_t g_ns2009_irq_flag;
    // if (g_ns2009_irq_flag) {
    //   g_ns2009_irq_flag = 0;

    //   DRV_Touch_Point_t point;
    //   DRV_Touch_Read(touch_main, &point);
    //   log_d("Touch Read (IRQ): X=%d, Y=%d, Pressed=%d", point.x, point.y, point.pressed);
    // }

    if ( dma_trans_complete_flag > 0 ) {
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

    /* TEST #2 PWM */
    // uint32_t duty = 0;
    // for(duty = 0; duty <= 1000; duty += 50)
    // {
    //   PWM_Write(PD13, duty);
    //   PWM_Write(PD14, duty);
    //   PWM_Write(PD15, duty);
    //   HAL_DelayMs(20);
    // }

    // for(duty = 1000; duty > 0; duty -= 50)
    // {
    //   PWM_Write(PD13, duty);
    //   PWM_Write(PD14, duty);
    //   PWM_Write(PD15, duty);
    //   HAL_DelayMs(20);
    // }

    // DRV_DelayMs(DELAY);
  }
}

