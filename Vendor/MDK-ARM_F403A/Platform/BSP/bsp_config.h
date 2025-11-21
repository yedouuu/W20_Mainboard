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

#define LED_GREEN_PIN                    PE4
#define DEBUG_PIN                        PE3

/* IR */
#define IR_HOP_RX_PIN                    PA5
#define IR_STA_RX_PIN                    PA6
#define IR_HOP_STA_CTRL_PIN              PD6

#define IRL_PIN                          PC6
#define IRR_PIN                          PC7
#define IRL_RX_PIN                       PA7
#define IRR_RX_PIN                       PB0
#define IR_CHANNEL_CTRL_PIN              PD12

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
