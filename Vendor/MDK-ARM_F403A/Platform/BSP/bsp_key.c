/**
 * @file bsp_key.c
 * @author your name (you@domain.com)
 * @brief 开发板测试例程：按键扫描
 * @version 0.1
 * @date 2026-05-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <stdio.h>

#include "Core/systick.h"
#include "Core/exti.h"
#include "Core/gpio.h"

#include "bsp_key.h"

#include "Logger.h"

uint8_t g_speed = 1;

/**
 * @brief  button handler function
 * @param  none
 * @retval none
 */
void button_isr(void)
{
  /* clear interrupt pending bit */
  // exint_flag_clear(EXINT_LINE_0);

  /* check input pin state */
  if (SET == gpio_input_data_bit_read(USER_BUTTON_PORT, USER_BUTTON_PIN))
  {
    if (g_speed == 1)
      g_speed = 4;
    else
      g_speed = 1;
    log_i("Speed changed to %d", g_speed);
    printf("USER BUTTON PRESS!\r\n");
  }
}

void BSP_KEY_Init(void)
{
  attachInterrupt(PA0, button_isr, RISING);
}
