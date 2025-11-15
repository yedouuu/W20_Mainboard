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
#include "pwm.h"

#define DELAY                            1000

void tim8_irq_callback(void)
{
  log_d("TIM8 interrupt triggered.");
}

void tim6_irq_callback(void)
{
  // log_d("TIM6 interrupt triggered.");
  BSP_LED_Toggle(LED5);
}

void pwm_tmr4_pd12_15_init(void)
{
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    gpio_pin_remap_config(TMR4_MUX, TRUE);

    crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);

    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = GPIO_PINS_12 | GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init(GPIOD, &gpio_init_struct);

    tmr_base_init(TMR4, 2400 - 1, 1000 - 1);
    tmr_output_config_type oc;
    tmr_output_default_para_init(&oc);
    oc.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    oc.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;

    tmr_output_channel_config(TMR4, TMR_SELECT_CHANNEL_1, &oc);
    tmr_channel_value_set(TMR4, TMR_SELECT_CHANNEL_1, 100);

    tmr_counter_enable(TMR4, TRUE);
}


/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  Core_Init();
  // BSP_LED_Init();
  // BSP_KEY_Init();
  App_Init();

  log_i("System initialized.");

  Timer_SetInterrupt(TIM8, 1000000, tim8_irq_callback); // 1ms
  Timer_SetInterrupt(TIM6, 10000, tim6_irq_callback); // 1ms

  Timer_SetEnable(TIM8, TRUE);
  Timer_SetEnable(TIM6, TRUE);


  // pwm_tmr4_pd12_15_init();

  // gpio_pin_remap_config(TMR4_MUX, TRUE);
  // crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  // log_i("PWM Init Channel %d", PWM_Init(PD13, 1000, 2400));
  // log_i("PWM Init Channel %d", PWM_Init(PD14, 1000, 2400));
  // log_i("PWM Init Channel %d", PWM_Init(PD15, 1000, 2400));
  // PWM_Init(PD14, 1000, 2400);
  // PWM_Init(PD15, 1000, 2400);

  // PWM_Init(PB6, 1000, 2400);
  // PWM_Init(PB7, 1000, 2400);
  
  PWM_Write(PD13, 500);
  PWM_Write(PD14, 500);
  PWM_Write(PD15, 500);
	
	// PWM_Init(PB6, 500, 2400);
  // PWM_Init(PB7, 500, 2400);

  while(1)
  {
    uint32_t duty = 0;

    for(duty = 0; duty <= 1000; duty += 50)
    {
      PWM_Write(PB13, duty);
      HAL_DelayMs(20);
    }

    for(duty = 1000; duty > 0; duty -= 50)
    {
      PWM_Write(PB13, duty);
      HAL_DelayMs(20);
    }
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
