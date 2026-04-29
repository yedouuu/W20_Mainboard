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
#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"


/*=========================
   Hardware Configuration
 *=========================*/

/* Button */
#define USER_BUTTON_PIN                  GPIO_PINS_0
#define USER_BUTTON_PORT                 GPIOA
#define USER_BUTTON_CRM_CLK              CRM_GPIOA_PERIPH_CLOCK

#define EXTI_LINE_USER_BUTTON            EXINT_LINE_0
#define EXTI_POLARITY_USER_BUTTON        EXINT_TRIGGER_RISING_EDGE
#define EXTI_CRM_CLK_USER_BUTTON         CRM_IOMUX_PERIPH_CLOCK

/* LED */
#define LED_NUM                          2

#define LED1_PIN                         PE4
#define DEBUG_PIN                        PE3

/* IR */
#define IR_HOP_RX_PIN                    PA5
#define IR_STA_RX_PIN                    PA6
#define IR_HOP_STA_CTRL_PIN              PD6

#define IRL_PWM_PIN                      PC6  /* Remap: TIM3 C1 */
#define IRR_PWM_PIN                      PC7  /* Remap: TIM3 C2 */
#define IRL_RX_PIN                       PA7
#define IRR_RX_PIN                       PB0
#define IR_CHANNEL_CTRL_PIN              PD12

/* UV */
#define UV_LEFT_PIN                      PC4  /* ADC1_IN14 */
#define UV_RIGHT_PIN                     PC5  /* ADC1_IN15 */

/* MG */
#define MGS_LEFT_1_PIN                   PA4  /* ADC1_IN4 */
#define MGS_LEFT_2_PIN                   PC0  /* ADC1_IN10 */
#define MGS_LEFT_3_PIN                   PC2  /* ADC1_IN12 */
#define MGS_RIGHT_1_PIN                  PA3  /* ADC1_IN3 */
#define MGS_RIGHT_2_PIN                  PC1  /* ADC1_IN11 */
#define MGS_RIGHT_3_PIN                  PC3  /* ADC1_IN13 */
#define MGB_BM_PIN                       PA1  /* ADC1_IN1 */
#define MGB_HD_PIN                       PA2  /* ADC1_IN2 */

/* Screen */
#define SCREEN_XMC_D0_PIN                PD14
#define SCREEN_XMC_D1_PIN                PD15
#define SCREEN_XMC_D2_PIN                PD0
#define SCREEN_XMC_D3_PIN                PD1
#define SCREEN_XMC_D4_PIN                PE7
#define SCREEN_XMC_D5_PIN                PE8
#define SCREEN_XMC_D6_PIN                PE9
#define SCREEN_XMC_D7_PIN                PE10

#define SCREEN_RD_PIN                    PD4
#define SCREEN_WR_PIN                    PD5
#define SCREEN_CS_PIN                    PD7
#define SCREEN_DC_PIN                    PD11
#define SCREEN_RESET_PIN                 PE0
#define SCREEN_BLK_PIN                   PA15    /* Remap: TIM2 */

#define SCREEN_XMC_BANK                  XMC_BANK1_NOR_SRAM1
#define SCREEN_MAIN_HOR_RES              (480)
#define SCREEN_MAIN_VER_RES              (272)
#define SCREEN_MAIN_TOUCH_IC             "NS2009"

#define TOUCH_NS2009_I2C_SCL_PIN         PB8
#define TOUCH_NS2009_I2C_SDA_PIN         PB9
#define TOUCH_NS2009_INT_PIN             PE5
#define TOUCH_NS2009_A0_PIN              PE6       /* A0 */
#define TOUCH_NS2009_ADC_RESO            (4095.0f) /* 12位ADC分辨率 */
#define TOUCH_NS2009_PRESSURE_MIN        (0.3f)    /* 最小压力 */
#define TOUCH_NS2009_PRESSURE_MAX        (2.0f)    /* 最大压力 */

/* Keyboard */
#define KEY1_IN_PIN                      PE11
#define KEY2_IN_PIN                      PE12
#define KEY3_IN_PIN                      PE13
#define KEY4_IN_PIN                      PE14
#define KEY5_OUT_PIN                     PE15
#define KEY6_OUT_PIN                     PB12
#define KEY7_OUT_PIN                     PB13

#define TM1638_SPI_STB_PIN               PE11
#define TM1638_SPI_CLK_PIN               PE12
#define TM1638_SPI_DIO_PIN               PE13

/* SPI Flash */
#define SFLASH_CS_PIN                    PA8     /* SPIM_CS */
#define SFLASH_CLK_PIN                   PB1     /* SPIM_SCK */
#define SFLASH_HOLD_PIN                  PB6     /* SPIM_IO3 */
#define SFLASH_WP_PIN                    PB7     /* SPIM_IO2 */
#define SFLASH_MOSI_PIN                  PB10    /* Remap: SPIM_IO0 */
#define SFLASH_MISO_PIN                  PB11    /* Remap: SPIM_IO1 */

/* Motor */
#define MOTOR_MAIN_DRV_IC               "AT8236"
#define MOTOR_MAIN_IN1_PWM_PIN           PB14    /* TIM12 C1 */
#define MOTOR_MAIN_IN2_PIN               PD3
#define MOTOR_STACKER_EN_PWM_PIN         PB15    /* TIM12 C2 */

/* Encoder */
#define ENCODER_CTRL_PIN                 PD13     /* If use PWM then Remap: TIM4 C2 */
#define ENCODER_READ_PIN                 PA0      /* 采样引脚 ADC1_IN0 */
#define ENCODER_TO_FPGA_PIN              PE2      /* 输出码盘信号给FPGA的引脚 */
#define ENCODER_RESOLUTION               (0.5f)   /* 码盘间隔宽度mm */

/* Sensors */
// #define CONFIG_SENSOR_ENABLE        1

// #if CONFIG_SENSOR_ENABLE
// #  define CONFIG_SENSOR_IMU_ENABLE  1
// #  define CONFIG_SENSOR_MAG_ENABLE  1
// #endif

// #define NULL_PIN                    PD0

/* Screen */
// #define CONFIG_SCREEN_CS_PIN        PB0
// #define CONFIG_SCREEN_DC_PIN        PA4
// #define CONFIG_SCREEN_RST_PIN       PA6
// #define CONFIG_SCREEN_SCK_PIN       PA5
// #define CONFIG_SCREEN_MOSI_PIN      PA7
// #define CONFIG_SCREEN_BLK_PIN       PB1  // TIM3
// #define CONFIG_SCREEN_SPI           SPI

// #define CONFIG_SCREEN_HOR_RES       240
// #define CONFIG_SCREEN_VER_RES       240

/* HAL Interrupt Update Timer */
// #define CONFIG_HAL_UPDATE_TIM       TIM4

/* Show Stack & Heap Info */
// #define CONFIG_SHOW_STACK_INFO      0
// #define CONFIG_SHOW_HEAP_INFO       0

/* Use Watch Dog */
// #define CONFIG_WATCH_DOG_ENABLE     1
// #if CONFIG_WATCH_DOG_ENABLE
// #  define CONFIG_WATCH_DOG_TIMEOUT (10 * 1000) // [ms]
// #endif

#endif
