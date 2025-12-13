/*
 * MIT License
 * Copyright (c) 2025 Yedouuu | UNION
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "bsp_led.h"
#include "gpio.h"


uint16_t led_gpio_pin[LED_NUM] = { LED1_PIN, DEBUG_PIN };

void BSP_LED_Init(void)
{
  for (uint8_t i = 0; i < LED_NUM; i++)
  {
    PinInfo_TypeDef pinInfo = PIN_MAP[led_gpio_pin[i]];
    GPIOx_Init(pinInfo.GPIOx, pinInfo.GPIO_Pin_x, OUTPUT, GPIO_DRIVE_DEFAULT);
  }
}

void BSP_LED_On(uint8_t led)
{
  if (led < LED_NUM)
  {
    PinInfo_TypeDef pinInfo = PIN_MAP[led_gpio_pin[led]];
    gpio_bits_reset(pinInfo.GPIOx, pinInfo.GPIO_Pin_x);
  }
}

void BSP_LED_Off(uint8_t led)
{
  if (led < LED_NUM)
  {
    PinInfo_TypeDef pinInfo = PIN_MAP[led_gpio_pin[led]];
    gpio_bits_set(pinInfo.GPIOx, pinInfo.GPIO_Pin_x);
  }
}

void BSP_LED_Toggle(uint8_t led)
{
  if (led < LED_NUM)
  {
    PinInfo_TypeDef pinInfo = PIN_MAP[led_gpio_pin[led]];
    uint8_t status = gpio_input_data_bit_read(pinInfo.GPIOx, pinInfo.GPIO_Pin_x);
    gpio_bits_write(pinInfo.GPIOx, pinInfo.GPIO_Pin_x, (status == FALSE) ? TRUE : FALSE);
  }
}
