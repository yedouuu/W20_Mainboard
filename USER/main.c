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
#include "HAL.h"
#include "mcu_core.h"
#include "Logger.h"
#include "DeviceManager.h"

#define DELAY                            500

void tim8_irq_callback(void)
{
  // log_d("TIM8 interrupt triggered.");
}

void tim6_irq_callback(void)
{
  // log_d("TIM6 interrupt triggered.");
  BSP_LED_Toggle(DEBUG_PIN);
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
  loggerInit(LOG_LEVEL_DEBUG);
  DM_DeviceInitALL();
  App_Init();
  
  log_i("System initialized.");
  

  Timer_SetInterrupt(TIM8, 1000000, tim8_irq_callback); // 1000ms
  Timer_SetInterrupt(TIM6, 10000, tim6_irq_callback); // 100ms

  Timer_SetEnable(TIM8, TRUE);
  Timer_SetEnable(TIM6, TRUE);

  log_i("PWM Init Channel %d", PWM_Init(PD13, 1000, 240000));
  log_i("PWM Init Channel %d", PWM_Init(PD14, 1000, 240000));
  log_i("PWM Init Channel %d", PWM_Init(PD15, 1000, 240000));
  
  PWM_Write(PD13, 500);
  PWM_Write(PD14, 500);
  PWM_Write(PD15, 500);

  ADC_DMA_Init();
  ADCx_Start(ADC1);

  Device_t *ir_hop = DM_DeviceFind("IR_HOPPER");
  Device_t *ir_sta = DM_DeviceFind("IR_STACKER");

  if ( ir_hop == NULL || ir_sta == NULL) {
    log_e("IR devices not found!");
    while(1) {
      BSP_LED_Toggle(LED_GREEN);
      HAL_DelayMs(100);
    }
  }


  HAL_IR_Enable(ir_hop);
  HAL_IR_Enable(ir_sta);

  // BSP_IR_Enable(IR_HOPPER);
  // BSP_IR_Enable(IR_STACKER);

  while(1)
  {
    if ( dma_trans_complete_flag > 0 ) {
      /* 12位ADC转换为3.3V电压 */
      uint8_t buf1[2];
      uint8_t buf2[2];
      HAL_IR_GetRawData(ir_hop, buf1, sizeof(buf1));
      uint16_t raw_hop = *(uint16_t *)buf1;
      HAL_IR_GetRawData(ir_sta, buf2, sizeof(buf2));
      uint16_t raw_sta = *(uint16_t *)buf2;

      float voltage_hop = (float)raw_hop * 3.3f / 4095.0f;
      float voltage_sta = (float)raw_sta * 3.3f / 4095.0f;
      log_d("IR Hopper Voltage: %fV", voltage_hop);
      log_d("IR Stacker Voltage: %fV", voltage_sta);

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

    BSP_LED_Toggle(LED_GREEN);
    HAL_DelayMs(DELAY);

  }
}

