
#include "key_scan.h"
#include "DeviceManager/device_manager.h"
#include "drv_keypad_wrapper.h"

void Key_ScanTask(void)
{
  Device_t *keypad = DM_DeviceFind("KEYPAD_001");

  DRV_Keypad_Priv_t *priv = (DRV_Keypad_Priv_t *)keypad->priv;
  if (priv == NULL)
  {
    log_e("Device %s has no private data!", keypad->name);
    return;
  }

  uint8_t  key_idx = 0;
  uint8_t  key_cnt = 0;
  Status_t status  = DRV_Keypad_ReadKey(keypad, &key_idx, &key_cnt);
  if (status != kStatus_Success)
  {
    log_e("DRV_Keypad_ReadKey failed with status: %d", status);
    return;
  }

  /* 有按键被按下 */
  if (key_cnt > 0)
  {
    log_d("key_idx= %d, key_cnt=%d, status: %d, Phy_Key Code: %d",
          key_idx,
          key_cnt,
          priv->key_states[key_idx],
          priv->key_phy_map[key_idx]);
  }
}
