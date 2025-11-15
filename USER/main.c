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
#include "HAL.h"
#include "mcu_core.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "Logger.h"

#define DELAY                            1000

void tim8_irq_callback(void)
{
  log_d("TIM8 interrupt triggered.");
  BSP_LED_Toggle(LED5);
}

void tim6_irq_callback(void)
{
  log_d("TIM6 interrupt triggered.");
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
  // BSP_KEY_Init();
  App_Init();

  Timer_SetInterrupt(TIM8, 1000000, tim8_irq_callback); // 1ms
  Timer_SetInterrupt(TIM6, 1000000, tim6_irq_callback); // 1ms

  Timer_SetEnable(TIM8, TRUE);

  log_i("System initialized.");

  while(1)
  {
    // BSP_LED_Toggle(LED2);
    // log_d("Toggle LED %d", LED2);
    // HAL_DelayMs(g_speed * DELAY);

    // BSP_LED_Toggle(LED3);
    // log_d("Toggle LED %d", LED3);
    // HAL_DelayMs(g_speed * DELAY);

    // BSP_LED_Toggle(LED4);
    // log_d("Toggle LED %d", LED4);
    // HAL_DelayMs(g_speed * DELAY);
  }
}

/**
  * @}
  */

/**
  * @}
  */
