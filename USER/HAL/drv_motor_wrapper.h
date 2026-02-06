/*
 * MIT License
 * Copyright (c) 2026 Yedouuu | UNION
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
#ifndef __DRV_MOTOR_WRAPPER_H__
#define __DRV_MOTOR_WRAPPER_H__

/* Includes -----------------------------------------------------------------*/
#include "Common/common.h"
#include "DeviceManager/device_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  DRV_MOTOR_MAIN    = 0,
  DRV_MOTOR_STACKER = 1,
  DRV_MOTOR_REJECT  = 2,
  DRV_MOTOR_HOPPER  = 3,
  DRV_MOTOR_MAX
} DRV_Motor_Type_e;

typedef enum
{
  DRV_MOTOR_STOP     = 0,
  DRV_MOTOR_FORWARD  = 1,
  DRV_MOTOR_BACKWARD = 2,
  DRV_MOTOR_BRAKE    = 3,
} DRV_Motor_State_e;

typedef struct __DRV_Motor_Ops_t
{
  Device_Ops_t base;
  uint32_t     magic; // 设备魔数，用于类型校验
  Status_t (*Operate)(const void *res, DRV_Motor_State_e operate);
  void (*BSP_Init)(const void *res);
  void (*SetPWM)(const void *res, uint8_t pwm);
} DRV_Motor_Ops_t;

typedef struct __DRV_Motor_Priv_t
{
  const DRV_Motor_Type_e type;
  DRV_Motor_State_e      state;    /* 电机状态, #Ref DRV_Motor_State_e */
  uint8_t                pwm_duty; /* PWM占空比 */
} DRV_Motor_Priv_t;

/* Exported functions prototypes ---------------------------------------------*/
Status_t DRV_Motor_Init(Device_t *motor_dev);
Status_t DRV_Motor_Operate(Device_t *motor_dev, DRV_Motor_State_e operate);
Status_t DRV_Motor_SetPWM(Device_t *motor_dev, uint8_t pwm);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_MOTOR_WRAPPER_H__ */
