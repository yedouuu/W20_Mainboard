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

/**
 * @brief 读取按键状态并更新私有数据结构
 * @note  根据按键位图更新每个按键的状态（按下、释放、长按）
 * @attention 需要定期调用以保持按键状态的准确性
 *
 * @param keypad_dev 按键设备指针
 * @param key_idx 输出参数，返回当前检测到的按键索引
 * @param key_cnt 输出参数，返回当前检测到的按键数量
 * @return Status_t 操作状态
 */
Status_t DRV_Keypad_ReadKey(Device_t *keypad_dev,
                            uint8_t  *key_idx,
                            uint8_t  *key_cnt)
{
  DRV_Keypad_Priv_t *priv = (DRV_Keypad_Priv_t *)keypad_dev->priv;
  if (priv == NULL)
  {
    log_e("Device %s has no private data!", keypad_dev->name);
    return kStatus_InvalidArgument;
  }
  if ( key_idx == NULL ) {
    log_e("key_idx is NULL!");
    return kStatus_InvalidArgument;
  }

  uint8_t __key_cnt = 0;

  /* 重新获取, 确保返回的状态为最新状态 */
  uint32_t bitmap = 0;
  Status_t ret    = DRV_Keypad_GetBitmap(keypad_dev, &bitmap);
  if (ret != kStatus_Success)
  {
    log_e("fail to get bitmap with status: %d", ret);
    return ret;
  }

  /* 扫描bitmap, 更新key_states中的状态 */
  for (uint8_t i = 0; i < KEYPAD_MAX_KEY_NUM; i++)
  {
    /* TODO: 多按键触发, 当前版本只支持单按键 */
    if ( __key_cnt > 0 ) {
      break;
    }

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
        __key_cnt++;
      }
      else if (priv->key_states[i] == DRV_KEYPAD_PRESSED)
      {
        // Check for long press
        if ((DRV_GetTickMs() - priv->press_start_time[i]) >=
            priv->long_press_ms)
        {
          *key_idx = i;
          __key_cnt++;
          priv->key_states[i] = DRV_KEYPAD_LONG_PRESSED;
        }
      }
      else if (priv->key_states[i] == DRV_KEYPAD_LONG_PRESSED)
      {
        // Already in long press state, do nothing
        *key_idx = i;
        __key_cnt++;
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

  if ( key_cnt != NULL ) {
    *key_cnt = __key_cnt;
  }

  return kStatus_Success;
}
