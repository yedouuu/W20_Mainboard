
#include "key_scan.h"
#include "DeviceManager/device_manager.h"
#include "drv_keypad_wrapper.h"

static uint8_t key_map[KEYPAD_MAX_KEY_NUM] = {
    KEY_PHY_UNDEFINE, KEY_PHY_CLEAR,    KEY_PHY_MODE,     KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_LIST,     KEY_PHY_AUTO,     KEY_PHY_INVALID,

    KEY_PHY_UNDEFINE, KEY_PHY_PRINT,    KEY_PHY_BATCH,    KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_START,    KEY_PHY_ESC,      KEY_PHY_INVALID,

    KEY_PHY_UNDEFINE, KEY_PHY_ADD,      KEY_PHY_SET,      KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_INVALID,

    KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_INVALID};

void Key_ScanTask(void)
{
  Device_t *keypad     = DM_DeviceFind("KEYPAD");
  uint32_t  key_bitmap = 0;

  DRV_Keypad_Priv_t *priv = (DRV_Keypad_Priv_t *)keypad->priv;
  if (priv == NULL)
  {
    log_e("Device %s has no private data!", keypad->name);
    return;
  }

  DRV_Keypad_GetBitmap(keypad, &key_bitmap);

  uint8_t  key_idx = 0;
  Status_t status  = DRV_Keypad_ReadKey(keypad, key_bitmap, &key_idx);
  if (status != kStatus_Success)
  {
    log_e("DRV_Keypad_ReadKey failed with status: %d", status);
    return;
  }

  log_d("Key Bitmap: 0x%08X, key_idx= %d, status: %d, Phy_Key Code: %d",
        key_bitmap,
        key_idx,
        priv->key_states[key_idx],
        key_map[key_idx]);
}
