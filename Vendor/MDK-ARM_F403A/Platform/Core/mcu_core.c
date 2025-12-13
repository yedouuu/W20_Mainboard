
#include "mcu_core.h"

void Core_Init(void)
{
  // 初始化MCU核心相关设置
  /* 禁用 JTAG，保留 SWD 调试接口 */
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  gpio_pin_remap_config(SWJTAG_MUX_010, TRUE);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  system_clock_config();
  SysTick_Init();
  Uart_Print_Init(115200);
}

/**
 * @brief 快速设置引脚模式
 * 
 * @param pin 
 * @param mode 
 * @return uint8_t 
 */
uint8_t pinMode(Pin_TypeDef pin, PinMode_TypeDef mode)
{
  if(!IS_PIN(pin))
  {
    return 0;
  }

  if (mode == INPUT_ANALOG_DMA)
  {
    if (!IS_ADC_PIN(pin))
    {
      return 0;
    }
    pinMode(pin, INPUT_ANALOG);
    ADC_DMA_Register(PIN_MAP[pin].ADC_Channel);
  }
  else if (mode == PWM)
  {
    PWM_Init(pin, PWM_RESOLUTION_DEFAULT, PWM_FREQUENCY_DEFAULT);
  }
  else 
  {
    GPIOx_Init(
        PIN_MAP[pin].GPIOx,
        PIN_MAP[pin].GPIO_Pin_x,
        mode,
        GPIO_DRIVE_DEFAULT
    );
  }
  return 0;
}
