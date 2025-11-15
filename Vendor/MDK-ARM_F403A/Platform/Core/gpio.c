
#include "gpio.h"

const PinInfo_TypeDef PIN_MAP[PIN_MAX] =
{
  /* TODO: ADC引脚配置, 目前只有ADC1 */
  
  /*GPIO_Type* GPIOx;  //对应GPIOx地址
  TIM_Type* TIMx;      //对应TIMx地址
  ADC_Type* ADCx;      //对应ADCx地址

  uint16_t GPIO_Pin_x;    //对应GPIO_Pin位
  uint8_t TimerChannel;   //对应定时器通道
  uint8_t ADC_CHANNEL;*/  //对应ADC通道
  /* PA */
  {GPIOA, TIM2,  ADC1, GPIO_PINS_0,  1, ADC_CHANNEL_0  }, /* PA0 / TIM2_CH1 / ADC1_IN0 */
  {GPIOA, TIM2,  ADC1, GPIO_PINS_1,  2, ADC_CHANNEL_1  }, /* PA1 / TIM2_CH2 / ADC1_IN1 */
  {GPIOA, TIM2,  ADC1, GPIO_PINS_2,  3, ADC_CHANNEL_2  }, /* PA2 / TIM2_CH3 / ADC1_IN2 */
  {GPIOA, TIM2,  ADC1, GPIO_PINS_3,  4, ADC_CHANNEL_3  }, /* PA3 / TIM2_CH4 / ADC1_IN3 */
  {GPIOA, NULL,  ADC1, GPIO_PINS_4,  0, ADC_CHANNEL_4  }, /* PA4 / ADC1_IN4 */
  {GPIOA, NULL,  ADC1, GPIO_PINS_5,  0, ADC_CHANNEL_5  }, /* PA5 / ADC1_IN5 */
  {GPIOA, TIM3,  ADC1, GPIO_PINS_6,  1, ADC_CHANNEL_6  }, /* PA6 / TIM3_CH1 / ADC1_IN6 */
  {GPIOA, TIM3,  ADC1, GPIO_PINS_7,  2, ADC_CHANNEL_7  }, /* PA7 / TIM3_CH2 / ADC1_IN7 */
  {GPIOA, TIM1,  NULL, GPIO_PINS_8,  1, ADC_CHANNEL_X  }, /* PA8 / TIM1_CH1 */
  {GPIOA, TIM1,  NULL, GPIO_PINS_9,  2, ADC_CHANNEL_X  }, /* PA9 / TIM1_CH2 */
  {GPIOA, TIM1,  NULL, GPIO_PINS_10, 3, ADC_CHANNEL_X  }, /* PA10 / TIM1_CH3 */
  {GPIOA, TIM1,  NULL, GPIO_PINS_11, 4, ADC_CHANNEL_X  }, /* PA11 / TIM1_CH4 */
  {GPIOA, NULL,  NULL, GPIO_PINS_12, 0, ADC_CHANNEL_X  }, /* PA12 */
  {GPIOA, NULL,  NULL, GPIO_PINS_13, 0, ADC_CHANNEL_X  }, /* PA13 */
  {GPIOA, NULL,  NULL, GPIO_PINS_14, 0, ADC_CHANNEL_X  }, /* PA14 */
  {GPIOA, NULL,  NULL, GPIO_PINS_15, 0, ADC_CHANNEL_X  }, /* PA15 */

  /* PB */
  {GPIOB, TIM3,  ADC1, GPIO_PINS_0,  3, ADC_CHANNEL_8  }, /* PB0 / TIM3_CH3 / ADC1_IN8 */
  {GPIOB, TIM3,  ADC1, GPIO_PINS_1,  4, ADC_CHANNEL_9  }, /* PB1 / TIM3_CH4 / ADC1_IN9 */
  {GPIOB, NULL,  NULL, GPIO_PINS_2,  0, ADC_CHANNEL_X  }, /* PB2 */
  {GPIOB, NULL,  NULL, GPIO_PINS_3,  0, ADC_CHANNEL_X  }, /* PB3 */
  {GPIOB, NULL,  NULL, GPIO_PINS_4,  0, ADC_CHANNEL_X  }, /* PB4 */
  {GPIOB, NULL,  NULL, GPIO_PINS_5,  0, ADC_CHANNEL_X  }, /* PB5 */
  {GPIOB, TIM4,  NULL, GPIO_PINS_6,  1, ADC_CHANNEL_X  }, /* PB6 / TIM4_CH1 */
  {GPIOB, TIM4,  NULL, GPIO_PINS_7,  2, ADC_CHANNEL_X  }, /* PB7 / TIM4_CH2 */
  {GPIOB, TIM4,  NULL, GPIO_PINS_8,  3, ADC_CHANNEL_X  }, /* PB8 / TIM4_CH3 */
  {GPIOB, TIM4,  NULL, GPIO_PINS_9,  4, ADC_CHANNEL_X  }, /* PB9 / TIM4_CH4 */
  {GPIOB, NULL,  NULL, GPIO_PINS_10, 0, ADC_CHANNEL_X  }, /* PB10 */
  {GPIOB, NULL,  NULL, GPIO_PINS_11, 0, ADC_CHANNEL_X  }, /* PB11 */
  {GPIOB, TIM1,  NULL, GPIO_PINS_12, 0, ADC_CHANNEL_X  }, /* PB12 */
  {GPIOB, TIM1,  NULL, GPIO_PINS_13, 1, ADC_CHANNEL_X  }, /* PB13 */
  {GPIOB, TIM1,  NULL, GPIO_PINS_14, 2, ADC_CHANNEL_X  }, /* PB14 */
  {GPIOB, TIM1,  NULL, GPIO_PINS_15, 3, ADC_CHANNEL_X  }, /* PB15 */

  /* PC */
  {GPIOC, NULL,  ADC1, GPIO_PINS_0,  0, ADC_CHANNEL_10 }, /* PC0 / ADC1_IN10 */
  {GPIOC, NULL,  ADC1, GPIO_PINS_1,  0, ADC_CHANNEL_11 }, /* PC1 / ADC1_IN11 */
  {GPIOC, NULL,  ADC1, GPIO_PINS_2,  0, ADC_CHANNEL_12 }, /* PC2 / ADC1_IN12 */
  {GPIOC, NULL,  ADC1, GPIO_PINS_3,  0, ADC_CHANNEL_13 }, /* PC3 / ADC1_IN13 */
  {GPIOC, NULL,  ADC1, GPIO_PINS_4,  0, ADC_CHANNEL_14 }, /* PC4 / ADC1_IN14 */
  {GPIOC, NULL,  ADC1, GPIO_PINS_5,  0, ADC_CHANNEL_15 }, /* PC5 / ADC1_IN15 */
  {GPIOC, TMR8,  NULL, GPIO_PINS_6,  1, ADC_CHANNEL_X  }, /* PC6 / TMR8_CH1 */
  {GPIOC, TMR8,  NULL, GPIO_PINS_7,  2, ADC_CHANNEL_X  }, /* PC7 / TMR8_CH2 */
  {GPIOC, TMR8,  NULL, GPIO_PINS_8,  3, ADC_CHANNEL_X  }, /* PC8 / TMR8_CH3 */
  {GPIOC, TMR8,  NULL, GPIO_PINS_9,  4, ADC_CHANNEL_X  }, /* PC9 / TMR8_CH4 */
  {GPIOC, NULL,  NULL, GPIO_PINS_10, 0, ADC_CHANNEL_X  }, /* PC10 */
  {GPIOC, NULL,  NULL, GPIO_PINS_11, 0, ADC_CHANNEL_X  }, /* PC11 */
  {GPIOC, NULL,  NULL, GPIO_PINS_12, 0, ADC_CHANNEL_X  }, /* PC12 */
  {GPIOC, NULL,  NULL, GPIO_PINS_13, 0, ADC_CHANNEL_X  }, /* PC13 */
  {GPIOC, NULL,  NULL, GPIO_PINS_14, 0, ADC_CHANNEL_X  }, /* PC14 */
  {GPIOC, NULL,  NULL, GPIO_PINS_15, 0, ADC_CHANNEL_X  }, /* PC15 */

  /* PD */
  {GPIOD, NULL,  NULL, GPIO_PINS_0,  0, ADC_CHANNEL_X  }, /* PD0 */
  {GPIOD, NULL,  NULL, GPIO_PINS_1,  0, ADC_CHANNEL_X  }, /* PD1 */
  {GPIOD, NULL,  NULL, GPIO_PINS_2,  0, ADC_CHANNEL_X  }, /* PD2 */
  {GPIOD, NULL,  NULL, GPIO_PINS_3,  0, ADC_CHANNEL_X  }, /* PD3 */
  {GPIOD, NULL,  NULL, GPIO_PINS_4,  0, ADC_CHANNEL_X  }, /* PD4 */
  {GPIOD, NULL,  NULL, GPIO_PINS_5,  0, ADC_CHANNEL_X  }, /* PD5 */
  {GPIOD, NULL,  NULL, GPIO_PINS_6,  0, ADC_CHANNEL_X  }, /* PD6 */
  {GPIOD, NULL,  NULL, GPIO_PINS_7,  0, ADC_CHANNEL_X  }, /* PD7 */
  {GPIOD, NULL,  NULL, GPIO_PINS_8,  0, ADC_CHANNEL_X  }, /* PD8 */
  {GPIOD, NULL,  NULL, GPIO_PINS_9,  0, ADC_CHANNEL_X  }, /* PD9 */
  {GPIOD, NULL,  NULL, GPIO_PINS_10, 0, ADC_CHANNEL_X  }, /* PD10 */
  {GPIOD, NULL,  NULL, GPIO_PINS_11, 0, ADC_CHANNEL_X  }, /* PD11 */
  {GPIOD, TIM4,  NULL, GPIO_PINS_12, 1, ADC_CHANNEL_X  }, /* PD12 / TIM4_CH1 */
  {GPIOD, TIM4,  NULL, GPIO_PINS_13, 2, ADC_CHANNEL_X  }, /* PD13 / TIM4_CH2 */
  {GPIOD, TIM4,  NULL, GPIO_PINS_14, 3, ADC_CHANNEL_X  }, /* PD14 / TIM4_CH3 */
  {GPIOD, TIM4,  NULL, GPIO_PINS_15, 4, ADC_CHANNEL_X  }, /* PD15 / TIM4_CH4 */

  /* PE */
  {GPIOE, NULL,  NULL, GPIO_PINS_0,  0, ADC_CHANNEL_X  }, /* PE0 */
  {GPIOE, NULL,  NULL, GPIO_PINS_1,  0, ADC_CHANNEL_X  }, /* PE1 */
  {GPIOE, NULL,  NULL, GPIO_PINS_2,  0, ADC_CHANNEL_X  }, /* PE2 */
  {GPIOE, NULL,  NULL, GPIO_PINS_3,  0, ADC_CHANNEL_X  }, /* PE3 */
  {GPIOE, NULL,  NULL, GPIO_PINS_4,  0, ADC_CHANNEL_X  }, /* PE4 */
  {GPIOE, NULL,  NULL, GPIO_PINS_5,  0, ADC_CHANNEL_X  }, /* PE5 */
  {GPIOE, NULL,  NULL, GPIO_PINS_6,  0, ADC_CHANNEL_X  }, /* PE6 */
  {GPIOE, NULL,  NULL, GPIO_PINS_7,  0, ADC_CHANNEL_X  }, /* PE7 */
  {GPIOE, NULL,  NULL, GPIO_PINS_8,  0, ADC_CHANNEL_X  }, /* PE8 */
  {GPIOE, TIM1,  NULL, GPIO_PINS_9,  1, ADC_CHANNEL_X  }, /* PE9 / TIM1_CH1 */
  {GPIOE, NULL,  NULL, GPIO_PINS_10, 0, ADC_CHANNEL_X  }, /* PE10 */
  {GPIOE, TIM1,  NULL, GPIO_PINS_11, 2, ADC_CHANNEL_X  }, /* PE11 / TIM1_CH2 */
  {GPIOE, NULL,  NULL, GPIO_PINS_12, 0, ADC_CHANNEL_X  }, /* PE12 */
  {GPIOE, TIM1,  NULL, GPIO_PINS_13, 3, ADC_CHANNEL_X  }, /* PE13 / TIM1_CH3 */
  {GPIOE, TIM1,  NULL, GPIO_PINS_14, 4, ADC_CHANNEL_X  }, /* PE14 / TIM1_CH4 */
  {GPIOE, NULL,  NULL, GPIO_PINS_15, 0, ADC_CHANNEL_X  }, /* PE15 */
};


/**
  * @brief  GPIO初始化
  * @param  GPIOx: GPIO地址
  * @param  GPIO_Pin_x: GPIO对应位
  * @param  GPIO_Mode_x: GPIO模式
  * @param  GPIO_Drive_x: GPIO速度
  * @retval 无
  */
void GPIOx_Init(
    GPIO_TypeDef* GPIOx,
    uint16_t GPIO_Pin_x,
    PinMode_TypeDef Mode,
    gpio_drive_type GPIO_Drive_x
)
{
  gpio_init_type gpio_init_struct;
  crm_periph_clock_type CRM_GPIOx_PERIPH_CLOCK;

  if(GPIOx == GPIOA)     CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOA_PERIPH_CLOCK;
  else if(GPIOx == GPIOB)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOB_PERIPH_CLOCK;
  else if(GPIOx == GPIOC)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOC_PERIPH_CLOCK;
  else if(GPIOx == GPIOD)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOD_PERIPH_CLOCK;
#ifdef GPIOE
  else if(GPIOx == GPIOE)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOE_PERIPH_CLOCK;
#endif
#ifdef GPIOF
  else if(GPIOx == GPIOF)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOF_PERIPH_CLOCK;
#endif
#ifdef GPIOG
  else if(GPIOx == GPIOG)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOG_PERIPH_CLOCK;
#endif
#ifdef GPIOH
  else if(GPIOx == GPIOH)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOH_PERIPH_CLOCK;
#endif
  else return;

  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins = GPIO_Pin_x;
  gpio_init_struct.gpio_drive_strength = GPIO_Drive_x;

  if(Mode == INPUT)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
      gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  }
  else if(Mode == INPUT_PULLUP)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
      gpio_init_struct.gpio_pull = GPIO_PULL_UP;
  }
  else if(Mode == INPUT_PULLDOWN)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
      gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  }
  else if(Mode == INPUT_ANALOG)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
      gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  }
  else if(Mode == OUTPUT)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
      gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  }
  else if(Mode == OUTPUT_OPEN_DRAIN)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
      gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
  }
  else if(Mode == OUTPUT_AF_PP)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
      gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  }
  else if(Mode == OUTPUT_AF_OD)
  {
      gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
      gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
      gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
  }
  else
  {
      return;
  }

  crm_periph_clock_enable(CRM_GPIOx_PERIPH_CLOCK, TRUE);
  gpio_init(GPIOx, &gpio_init_struct);
}

/**
  * @brief  禁用JTAG引脚
  * @param  无
  * @retval 无
  */
void GPIO_JTAG_Disable(void)
{
  // RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
  // GPIO_PinsRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

/**
 * @brief  获取当前引脚对应的GPIOx编号
 * PA1 -> 1
 * PB3 -> 2
 * 
 * @param  Pin: 引脚编号
 * @retval 无
 */
uint8_t GPIO_GetPortNum(uint8_t Pin)
{
  uint8_t retval = 0xFF;
  uint8_t index;
  GPIO_TypeDef *GPIOx = PIN_MAP[Pin].GPIOx;

  GPIO_TypeDef *GPIO_Map[] =
      {
          GPIOA,
          GPIOB,
          GPIOC,
          GPIOD,
#ifdef GPIOE
          GPIOE,
#endif
#ifdef GPIOF
          GPIOF,
#endif
#ifdef GPIOG
          GPIOG
#endif
      };

  for (index = 0; index < sizeof(GPIO_Map) / sizeof(GPIO_Map[0]); index++)
  {
    if (GPIOx == GPIO_Map[index])
    {
      retval = index;
      break;
    }
  }

  return retval;
}

/**
  * @brief  获取当前引脚对应的 PinSource
  * PA1 --> 1
  * PB3 --> 3
  * 
  * @param  GPIO_Pin_x: GPIO对应位
  * @retval 无
  */
uint8_t GPIO_GetPinSource(uint16_t GPIO_Pin_x)
{
  uint8_t PinSource = 0;
  while(GPIO_Pin_x > 1)
  {
    GPIO_Pin_x >>= 1;
    PinSource++;
  }
  return PinSource;
}

/**
  * @brief  获取当前引脚对应的编号
  * PA1 --> 0x0001
  * PB3 --> 0x0008
  * 
  * @param  Pin: 引脚编号
  * @retval 无
  */
uint8_t GPIO_GetPinNum(uint8_t Pin)
{
  return GPIO_GetPinSource(PIN_MAP[Pin].GPIO_Pin_x);
}
