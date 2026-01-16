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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __BSP_IR_H__
#define __BSP_IR_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"
#include "bsp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  IR_HOPPER  = 0,
  IR_STACKER = 1,
  IRR        = 2,
  IRL        = 3,
  IR_MAX
} IR_type_e;

typedef enum
{
  BSP_IR_DUTY_CYCLE_0   = 0, /* 0% */
  BSP_IR_DUTY_CYCLE_25  = 1, /* 25% */
  BSP_IR_DUTY_CYCLE_50  = 2, /* 50% */
  BSP_IR_DUTY_CYCLE_75  = 3, /* 75% */
  BSP_IR_DUTY_CYCLE_100 = 4, /* 100% */
  BSP_IR_DUTY_CYCLE_MAX = 0xFF,
} IR_Duty_Cycle_e;

typedef enum
{
  IR_HOLD     = 0,
  IR_FREE     = 1,
  IR_DEBOUNCE = 2,
} IR_status_e;

typedef struct _IR_Resource_t IR_Resource_t;

extern const IR_Resource_t ir_hopper_res;
extern const IR_Resource_t ir_stacker_res;
extern const IR_Resource_t ir_right_res;
extern const IR_Resource_t ir_left_res;

/* Exported functions prototypes ---------------------------------------------*/
void BSP_IR_Init(const void *ir);
void BSP_IR_Enable(const void *ir);
void BSP_IR_Disable(const void *ir);
void BSP_IR_SetPWM(const void *ir, uint16_t duty);
void BSP_IR_GetRawData(const void *ir, uint16_t *raw);

#ifdef __cplusplus
}
#endif

#endif
