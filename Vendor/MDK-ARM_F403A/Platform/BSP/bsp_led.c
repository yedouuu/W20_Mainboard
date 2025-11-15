
#include "bsp_led.h"
#include "gpio.h"
#include "pwm.h"


uint16_t led_gpio_pin[LED_NUM] = { LED2_PIN, LED3_PIN, LED4_PIN, DEBUG_PIN };

void BSP_LED_Init(void)
{
  // gpio_init_type gpio_init_struct;
  
  // PWM_Init(PD13, PWM_RESOLUTION_DEFAULT, PWM_FREQUENCY_DEFAULT);
  // PWM_Init(PD14, PWM_RESOLUTION_DEFAULT, PWM_FREQUENCY_DEFAULT);
  // PWM_Init(PD15, PWM_RESOLUTION_DEFAULT, PWM_FREQUENCY_DEFAULT);
  
  // PWM_Write(PD13, 1000);
  // GPIOx_Init(PIN_MAP[DEBUG_PIN].GPIOx, PIN_MAP[DEBUG_PIN].GPIO_Pin_x, OUTPUT, GPIO_DRIVE_DEFAULT);
  
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
    gpio_bits_write(pinInfo.GPIOx, pinInfo.GPIO_Pin_x, (status == RESET) ? SET : RESET);
  }
}
