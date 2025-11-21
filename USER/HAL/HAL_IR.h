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
#ifndef __HAL_IR_H__
#define __HAL_IR_H__


/* Includes -----------------------------------------------------------------*/
#include "Common/common.h"
#include "DeviceManager.h"

typedef enum
{
  HAL_IR_HOPPER        = 0,
  HAL_IR_STACKER       = 1,
  HAL_IRR              = 2,
  HAL_IRL              = 3,
  HAL_IR_MAX
} HAL_IR_Type_e;


typedef enum
{
  HAL_IR_HOLD       = 0,
  HAL_IR_FREE       = 1,
  HAL_IR_DEBOUNCE   = 2,
} HAL_IR_Status_e;


typedef enum
{
  HAL_IR_PWM_DUTY_0   = 0,     /* 0% */
  HAL_IR_PWM_DUTY_25  = 1,     /* 25% */
  HAL_IR_PWM_DUTY_50  = 2,     /* 50% */
  HAL_IR_PWM_DUTY_75  = 3,     /* 75% */
  HAL_IR_PWM_DUTY_100 = 4,     /* 100% */
  HAL_IR_PWM_DUTY_MAX = 0xFF,
} HAL_IR_PWM_Duty_e;


typedef struct _HAL_IR_Ops_t
{
  Device_Ops_t   base;
  void         (*BSP_Init)(const void* resource);
  void         (*Enable)(const void* resource);
  void         (*Disable)(const void* resource);
  void         (*SetPWM)(const void* resource, uint16_t duty);
  void         (*GetRawData)(const void* resource, uint16_t* raw);
} HAL_IR_Ops_t;


typedef struct _HAL_IR_Priv_t
{
  const HAL_IR_Type_e   type;
        uint16_t        pwm_duty;
        HAL_IR_Status_e status;
        uint16_t        last_data;
} HAL_IR_Priv_t;


/* Exported functions prototypes ---------------------------------------------*/
Status_t HAL_IR_Init(Device_t* ir_dev);
Status_t HAL_IR_DeInit(Device_t* ir_dev);
Status_t HAL_IR_Enable(Device_t* ir_dev);
Status_t HAL_IR_Disable(Device_t* ir_dev);
Status_t HAL_IR_GetRawData(Device_t* ir_dev, uint8_t* buf, uint32_t* len);
Status_t HAL_IR_SetPWM(Device_t* ir_dev, HAL_IR_PWM_Duty_e duty);


#endif /* __HAL_IR_H__ */
