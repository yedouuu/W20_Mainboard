
#ifndef __MCU_TYPE_H__
#define __MCU_TYPE_H__

#include "at32f403a_407.h"
#include "at32f403a_407_conf.h"
#include "at32f403a_407_clock.h"

#define __AT32__
#define __AT32F4__

#define F_CPU                       SystemCoreClock
#define CYCLES_PER_MICROSECOND      (F_CPU / 1000000U)

typedef gpio_type                   GPIO_TypeDef;
typedef spi_type                    SPI_TypeDef;
typedef tmr_type                    TIM_TypeDef;

#define GPIO_Pin_0                  GPIO_PINS_0
#define GPIO_Pin_1                  GPIO_PINS_1
#define GPIO_Pin_2                  GPIO_PINS_2
#define GPIO_Pin_3                  GPIO_PINS_3
#define GPIO_Pin_4                  GPIO_PINS_4
#define GPIO_Pin_5                  GPIO_PINS_5
#define GPIO_Pin_6                  GPIO_PINS_6
#define GPIO_Pin_7                  GPIO_PINS_7
#define GPIO_Pin_8                  GPIO_PINS_8
#define GPIO_Pin_9                  GPIO_PINS_9
#define GPIO_Pin_10                 GPIO_PINS_10
#define GPIO_Pin_11                 GPIO_PINS_11
#define GPIO_Pin_12                 GPIO_PINS_12
#define GPIO_Pin_13                 GPIO_PINS_13
#define GPIO_Pin_14                 GPIO_PINS_14
#define GPIO_Pin_15                 GPIO_PINS_15
#define GPIO_Pin_All                GPIO_PINS_ALL

#define TIM1                        TMR1
#define TIM2                        TMR2
#define TIM3                        TMR3
#define TIM4                        TMR4
#define TIM5                        TMR5
#define TIM6                        TMR6
#define TIM7                        TMR7
#define TIM8                        TMR8
#define TIM9                        TMR9
#define TIM10                       TMR10
#define TIM11                       TMR11
#define TIM12                       TMR12
#define TIM13                       TMR13
#define TIM14                       TMR14

#define ADC_CHANNEL_0               ADC_CHANNEL_0
#define ADC_CHANNEL_1               ADC_CHANNEL_1
#define ADC_CHANNEL_2               ADC_CHANNEL_2
#define ADC_CHANNEL_3               ADC_CHANNEL_3
#define ADC_CHANNEL_4               ADC_CHANNEL_4
#define ADC_CHANNEL_5               ADC_CHANNEL_5
#define ADC_CHANNEL_6               ADC_CHANNEL_6
#define ADC_CHANNEL_7               ADC_CHANNEL_7
#define ADC_CHANNEL_8               ADC_CHANNEL_8
#define ADC_CHANNEL_9               ADC_CHANNEL_9
#define ADC_CHANNEL_10              ADC_CHANNEL_10
#define ADC_CHANNEL_11              ADC_CHANNEL_11
#define ADC_CHANNEL_12              ADC_CHANNEL_12
#define ADC_CHANNEL_13              ADC_CHANNEL_13
#define ADC_CHANNEL_14              ADC_CHANNEL_14
#define ADC_CHANNEL_15              ADC_CHANNEL_15
#define ADC_CHANNEL_16              ADC_CHANNEL_16
#define ADC_CHANNEL_17              ADC_CHANNEL_17
#define ADC_CHANNEL_X               ((uint8_t)0xFF)

#define TIM_SELECT_CHANNEL_1     TMR_SELECT_CHANNEL_1
#define TIM_SELECT_CHANNEL_1C    TMR_SELECT_CHANNEL_1C
#define TIM_SELECT_CHANNEL_2     TMR_SELECT_CHANNEL_2
#define TIM_SELECT_CHANNEL_2C    TMR_SELECT_CHANNEL_2C
#define TIM_SELECT_CHANNEL_3     TMR_SELECT_CHANNEL_3
#define TIM_SELECT_CHANNEL_3C    TMR_SELECT_CHANNEL_3C
#define TIM_SELECT_CHANNEL_4     TMR_SELECT_CHANNEL_4

/*=========================
   MCU configuration
 *=========================*/

/* System tick */
#define SYSTICK_TICK_FREQ                   1000 // Hz
#define SYSTICK_PRIORITY                    0

/* GPIO */
#define GPIO_DRIVE_DEFAULT                  GPIO_DRIVE_STRENGTH_STRONGER

/* External Interrupt */
#define EXTI_PREPRIORITY_DEFAULT            14
#define EXTI_SUBPRIORITY_DEFAULT            0

/* Timer Interrupt */
#define TIMER_PREEMPTIONPRIORITY_DEFAULT    3
#define TIMER_SUBPRIORITY_DEFAULT           0

/* PWM */
#define PWM_RESOLUTION_DEFAULT              1000
#define PWM_FREQUENCY_DEFAULT               10000

#endif
