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
#include "Common/common.h"
#include "DeviceManager/device_manager.h"
#include "drv_keypad_wrapper.h"
#include "bsp_tm1638.h"

static Status_t __DRV_Keypad_Read_Adapter(Device_t *dev,
                                          uint8_t  *buf,
                                          uint32_t  len)
{
  if (len < sizeof(uint32_t))
  {
    log_e("buffer too small needs at least %d bytes", sizeof(uint32_t));
    return kStatus_InvalidArgument;
  }
  uint32_t key_bitmap = 0;
  Status_t ret        = DRV_Keypad_GetBitmap(dev, &key_bitmap);
  if (ret == kStatus_Success) { memcpy(buf, &key_bitmap, 4); }
  return ret;
}

static DRV_Keypad_Ops_t keypad_tm1638_ops = {
    .base =
        {
            .init   = DRV_Keypad_Init,
            .deinit = DRV_Keypad_DeInit,
            .open   = NULL,
            .close  = NULL,
            .read   = __DRV_Keypad_Read_Adapter,
            .write  = NULL,
            .ioctl  = NULL,
        },
    .magic     = DEVICE_MAGIC_KEYPAD,
    .BSP_Init  = BSP_TM1638_Init,
    .GetBitmap = BSP_TM1638_ReadKey,
};

static uint8_t tm1638_key_map[KEYPAD_MAX_KEY_NUM] = {
    KEY_PHY_UNDEFINE, KEY_PHY_CLEAR,    KEY_PHY_MODE,     KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_LIST,     KEY_PHY_AUTO,     KEY_PHY_INVALID,

    KEY_PHY_UNDEFINE, KEY_PHY_PRINT,    KEY_PHY_BATCH,    KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_START,    KEY_PHY_ESC,      KEY_PHY_INVALID,

    KEY_PHY_UNDEFINE, KEY_PHY_ADD,      KEY_PHY_SET,      KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_INVALID,

    KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_INVALID,
    KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_UNDEFINE, KEY_PHY_INVALID};

static DRV_Keypad_Priv_t keypad_tm1638_priv = {
    .type             = DRV_KEYPAD_TM1638,
    .key_states       = {DRV_KEYPAD_RELEASED},
    .key_phy_map      = tm1638_key_map,
    .key_num          = 10,
    .press_start_time = {0},
    .debounce_ms      = 50,
    .long_press_ms    = 1000,
};

Device_t keypad_tm1638_001 = {
    .name      = "KEYPAD_001",
    .dev_class = DEV_CLASS_INPUT,
    .ops       = (void *)&keypad_tm1638_ops,
    .priv      = (void *)&keypad_tm1638_priv,
    .res       = NULL,
};

DEVICE_EXPORT(keypad_tm1638_001);
