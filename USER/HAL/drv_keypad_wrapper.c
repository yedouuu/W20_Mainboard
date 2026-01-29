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
#include "drv_keypad_wrapper.h"
#include "drv_tick_wrapper.h"

/**
 * @brief  内部辅助函数：安全获取并校验 Ops 指针
 * @note   集中处理类型检查，防止非法强转
 * @param  dev: 通用设备指针
 * @retval 有效的 DRV_Keypad_Ops_t 指针，如果校验失败返回 NULL
 */
static DRV_Keypad_Ops_t *__DRV_Keypad_GetOps(Device_t *dev)
{
  if (dev == NULL)
  {
    log_e("Device is NULL!");
    return NULL;
  }

  if (dev->dev_class != DEV_CLASS_INPUT)
  {
    log_e("Type Mismatch: Device %s is not an Input Device!", dev->name);
    return NULL;
  }

  if (dev->ops == NULL)
  {
    log_e("Device %s has no ops!", dev->name);
    return NULL;
  }

  DRV_Keypad_Ops_t *ops = (DRV_Keypad_Ops_t *)dev->ops;
  if (ops->magic != DEVICE_MAGIC_KEYPAD)
  {
    log_e("Magic Mismatch: Device %s is not a Keypad Device!", dev->name);
    return NULL;
  }

  return ops;
}

Status_t DRV_Keypad_Init(Device_t *keypad_dev)
{
  DRV_Keypad_Ops_t *ops = __DRV_Keypad_GetOps(keypad_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->BSP_Init();
  return kStatus_Success;
}

Status_t DRV_Keypad_DeInit(Device_t *keypad_dev)
{
  /* Currently no de-initialization needed */
  return kStatus_Success;
}

Status_t DRV_Keypad_GetBitmap(Device_t *keypad_dev, uint32_t *bitmap)
{
  DRV_Keypad_Ops_t *ops = __DRV_Keypad_GetOps(keypad_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->GetBitmap(bitmap);
  return kStatus_Success;
}


Status_t DRV_Keypad_ReadKey(Device_t *keypad_dev,
                            uint32_t  bitmap,
                            uint8_t  *key_idx)
{
  DRV_Keypad_Priv_t *priv = (DRV_Keypad_Priv_t *)keypad_dev->priv;
  if (priv == NULL)
  {
    log_e("Device %s has no private data!", keypad_dev->name);
    return kStatus_InvalidArgument;
  }

  for (uint8_t i = 0; i < KEYPAD_MAX_KEY_NUM; i++)
  {
    uint32_t key_mask = (1U << i);
    if (bitmap & key_mask)
    {
      // Key is pressed
      log_d("Key %d is pressed", i);
      if (priv->key_states[i] == DRV_KEYPAD_RELEASED)
      {
        *key_idx                  = i;
        priv->key_states[i]       = DRV_KEYPAD_PRESSED;
        priv->press_start_time[i] = DRV_GetTickMs();
      }
      else if (priv->key_states[i] == DRV_KEYPAD_PRESSED)
      {
        // Check for long press
        if ((DRV_GetTickMs() - priv->press_start_time[i]) >=
            priv->long_press_ms)
        {
          *key_idx            = i;
          priv->key_states[i] = DRV_KEYPAD_LONG_PRESSED;
        }
      }
      else if (priv->key_states[i] == DRV_KEYPAD_LONG_PRESSED)
      {
        // Already in long press state, do nothing
        *key_idx = i;
      }
    }
    else
    {
      // Key is released
      if (priv->key_states[i] != DRV_KEYPAD_RELEASED)
      {
        log_d("Key %d is released", i);
      }
      priv->key_states[i]       = DRV_KEYPAD_RELEASED;
      priv->press_start_time[i] = 0;
    }
  }

  return kStatus_Success;
}
