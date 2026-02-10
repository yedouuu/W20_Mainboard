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
#include "drv_encoder_wrapper.h"

/**
 * @brief  内部辅助函数：安全获取并校验 Ops 指针
 * @note   集中处理类型检查，防止非法强转
 * @param  dev: 通用设备指针
 * @retval 有效的 DRV_Encoder_Ops_t 指针，如果校验失败返回 NULL
 */
static DRV_Encoder_Ops_t *__DRV_Encoder_GetOps(Device_t *dev)
{
  if (dev == NULL)
  {
    log_e("Device is NULL!");
    return NULL;
  }

  if (dev->dev_class != DEV_CLASS_SENSOR)
  {
    log_e("Type Mismatch: Device %s is not a Sensor!", dev->name);
    return NULL;
  }

  if (dev->ops == NULL)
  {
    log_e("Device %s has no ops!", dev->name);
    return NULL;
  }

  DRV_Encoder_Ops_t *ops = (DRV_Encoder_Ops_t *)dev->ops;
  if (ops->magic != DEVICE_MAGIC_ENCODER)
  {
    log_e("Magic Mismatch: Device %s is not an Encoder Device!", dev->name);
    return NULL;
  }

  return ops;
}

Status_t DRV_Encoder_Init(Device_t *encoder_dev)
{
  DRV_Encoder_Ops_t *ops = __DRV_Encoder_GetOps(encoder_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  log_i("Init device: %s", encoder_dev->name);

  ops->BSP_Init();

  return kStatus_Success;
}

Status_t DRV_Encoder_DeInit(Device_t *encoder_dev)
{
  DRV_Encoder_Ops_t *ops = __DRV_Encoder_GetOps(encoder_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  DRV_Encoder_Priv_t *priv = (DRV_Encoder_Priv_t *)encoder_dev->priv;
  memset(priv, 0, sizeof(DRV_Encoder_Priv_t));

  ops->BSP_Deinit();
  return kStatus_Success;
}

Status_t DRV_Encoder_Enable(Device_t *encoder_dev)
{
  DRV_Encoder_Ops_t *ops = __DRV_Encoder_GetOps(encoder_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->Enable();
  return kStatus_Success;
}

Status_t DRV_Encoder_Disable(Device_t *encoder_dev)
{
  DRV_Encoder_Ops_t *ops = __DRV_Encoder_GetOps(encoder_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->Disable();
  return kStatus_Success;
}

Status_t DRV_Encoder_GetState(Device_t *encoder_dev, DRV_Encoder_State_e *state)
{
  DRV_Encoder_Priv_t *priv = (DRV_Encoder_Priv_t *)encoder_dev->priv;
  if (priv == NULL)
  {
    log_e("Encoder Device %s has no private data!", encoder_dev->name);
    return kStatus_InvalidArgument;
  }

  *state = priv->state;
  return kStatus_Success;
}
