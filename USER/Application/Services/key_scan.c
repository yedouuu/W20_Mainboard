
#include "key_scan.h"
#include "DeviceManager/device_manager.h"
#include "HAL/drv_keypad_wrapper.h"
#include "HAL/drv_tick_wrapper.h"

static Device_t *keypad_dev   = NULL;

/* Private Prototype ------------------------------------------------------- */

Status_t __Keypad_ReadKey(void);

Status_t Key_Scan_Init(void)
{
  keypad_dev = DM_DeviceFind("KEYPAD_001");
  if (keypad_dev == NULL)
  {
    log_e("KEYPAD_001 device not found!");
    return kStatus_NotFound;
  }

  return kStatus_Success;
}

void Key_ScanTask(void)
{
  DRV_Keypad_Priv_t *priv = (DRV_Keypad_Priv_t *)keypad_dev->priv;
  if (priv == NULL)
  {
    log_e("Device %s has no private data!", keypad_dev->name);
    return;
  }

  Status_t status = __Keypad_ReadKey();
  if (status != kStatus_Success)
  {
    log_e("__Keypad_ReadKey failed with status: %d", status);
    return;
  }
}

/**
 * @brief 读取按键状态并更新私有数据结构
 * @return Status_t 操作状态
 */
Status_t __Keypad_ReadKey()
{
  DRV_Keypad_Priv_t *priv = (DRV_Keypad_Priv_t *)keypad_dev->priv;
  if (priv == NULL)
  {
    log_e("Device %s has no private data!", keypad_dev->name);
    return kStatus_InvalidArgument;
  }

  uint8_t __key_cnt = 0;
  uint8_t __key_idx = 0;

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
    // if (__key_cnt > 0) { break; }

    uint32_t key_mask = (1U << i);
    if (bitmap & key_mask)
    {
      // Key is pressed
      if (priv->key_states[i] == DRV_KEYPAD_RELEASED)
      {
        __key_idx                 = i;
        priv->key_states[i]       = DRV_KEYPAD_PRESSED;
        priv->press_start_time[i] = DRV_GetTickMs();
        __key_cnt++;
        // TODO: 发送按键按下事件
        log_d("Key %d is pressed", i);
      }
      else if (priv->key_states[i] == DRV_KEYPAD_PRESSED)
      {
        // Check for long press
        if ((DRV_GetTickMs() - priv->press_start_time[i]) >=
            priv->long_press_ms)
        {
          __key_idx = i;
          __key_cnt++;
          priv->key_states[i] = DRV_KEYPAD_LONG_PRESSED;
          // TODO: 发送按键长按事件
          log_d("Key %d is long pressed", i);
        }
      }
      else if (priv->key_states[i] == DRV_KEYPAD_LONG_PRESSED)
      {
        __key_idx = i;
        __key_cnt++;
      }
    }
    else
    {
      // Key is released
      if (priv->key_states[i] != DRV_KEYPAD_RELEASED)
      {
        log_d("Key %d is released", i);
        // TODO: 发送按键释放事件
      }
      priv->key_states[i]       = DRV_KEYPAD_RELEASED;
      priv->press_start_time[i] = 0;
    }
  }

  return kStatus_Success;
}
