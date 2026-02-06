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
#include "drv_motor_wrapper.h"

/**
 * @brief  内部辅助函数：安全获取并校验 Ops 指针
 * @note   集中处理类型检查，防止非法强转
 * @param  dev: 通用设备指针
 * @retval 有效的 DRV_Motor_Ops_t 指针，如果校验失败返回 NULL
 */
static DRV_Motor_Ops_t *__DRV_Motor_GetOps(Device_t *dev)
{
  if (dev == NULL)
  {
    log_e("Device is NULL!");
    return NULL;
  }

  if (dev->dev_class != DEV_CLASS_MOTOR)
  {
    log_e("Type Mismatch: Device %s is not a Motor!", dev->name);
    return NULL;
  }

  if (dev->ops == NULL)
  {
    log_e("Device %s has no ops!", dev->name);
    return NULL;
  }

  DRV_Motor_Ops_t *ops = (DRV_Motor_Ops_t *)dev->ops;
  if (ops->magic != DEVICE_MAGIC_MOTOR)
  {
    log_e("Magic Mismatch: Device %s is not a Motor Device!", dev->name);
    return NULL;
  }

  return ops;
}

Status_t DRV_Motor_Init(Device_t *motor_dev)
{
  DRV_Motor_Ops_t *ops = __DRV_Motor_GetOps(motor_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->BSP_Init(motor_dev->res);
  return kStatus_Success;
}

Status_t DRV_Motor_Operate(Device_t *motor_dev, DRV_Motor_State_e state)
{
  DRV_Motor_Ops_t *ops = __DRV_Motor_GetOps(motor_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  DRV_Motor_Priv_t *priv = (DRV_Motor_Priv_t *)motor_dev->priv;
  priv->state            = state;

  ops->Operate(motor_dev->res, state);
  return kStatus_Success;
}

Status_t DRV_Motor_SetPWM(Device_t *motor_dev, uint8_t pwm)
{
  DRV_Motor_Ops_t *ops = __DRV_Motor_GetOps(motor_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  DRV_Motor_Priv_t *priv = (DRV_Motor_Priv_t *)motor_dev->priv;
  priv->pwm_duty         = pwm;

  ops->SetPWM(motor_dev->res, pwm);
  return kStatus_Success;
}
