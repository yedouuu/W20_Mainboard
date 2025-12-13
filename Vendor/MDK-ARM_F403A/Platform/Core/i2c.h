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
#ifndef __I2C_H__
#define __I2C_H__

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"
#include "gpio.h"
#include "Common/errno.h"
#include "Common/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported defines ----------------------------------------------------------*/
#define I2C_TIMEOUT_DEFAULT_MS      1000  // Default timeout in milliseconds
#define I2C_TIMEOUT_MAX             0xFFFFFFFF
#define I2C_MAX_INSTANCES           3     // AT32F403A has I2C1, I2C2, I2C3

/* I2C Speed definitions */
#define I2C_SPEED_STANDARD          100000  // 100 kHz
#define I2C_SPEED_FAST              400000  // 400 kHz

/* Exported types ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

Status_t I2Cx_Init(I2C_TypeDef *I2Cx, uint32_t baudRate);
Status_t I2Cx_Master_Read(I2C_TypeDef *I2Cx, uint8_t slaveAddr, void *buf, uint32_t length);
Status_t I2Cx_Master_Write(I2C_TypeDef *I2Cx, uint8_t slaveAddr, void *buf, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
