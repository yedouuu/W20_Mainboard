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
#ifndef __DRV_KEYPAD_WRAPPER_H__
#define __DRV_KEYPAD_WRAPPER_H__

/* Includes ----------------------------------------------------------*/
#include "Common/common.h"
#include "DeviceManager/device_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KEYPAD_MAX_KEY_NUM 32

typedef enum
{
  DRV_KEYPAD_TM1638 = 0,
  DRV_KEYPAD_GPIO   = 1,
  DRV_KEYPAD_MAX
} DRV_Keypad_Type_e;

typedef enum
{
  DRV_KEYPAD_RELEASED     = 0,
  DRV_KEYPAD_PRESSED      = 1,
  DRV_KEYPAD_LONG_PRESSED = 2,
} DRV_Keypad_State_e;

typedef enum
{
  KEY_PHY_UNDEFINE = 0x00,
  KEY_PHY_START    = 1,
  KEY_PHY_CLEAR    = 2,
  KEY_PHY_MODE     = 3,
  KEY_PHY_UP       = 4,
  KEY_PHY_DOWN     = 5,
  KEY_PHY_LEFT     = 6,
  KEY_PHY_RIGHT    = 7,
  KEY_PHY_LIST     = 8,
  KEY_PHY_AUTO     = 9,
  KEY_PHY_BATCH    = 10,
  KEY_PHY_PRINT    = 11,
  KEY_PHY_ESC      = 12,
  KEY_PHY_SET      = 13,
  KEY_PHY_ADD      = 14,
  KEY_PHY_MAX,
  KEY_PHY_INVALID  = 0xFF,
} DRV_Keypad_keycode_e;

typedef struct __DRV_Keypad_Ops_t
{
  Device_Ops_t   base;
  const uint32_t magic;
  void (*BSP_Init)(void);
  void (*GetBitmap)(uint32_t *bitmap);

} DRV_Keypad_Ops_t;

typedef struct __DRV_Keypad_Priv_t
{
  const DRV_Keypad_Type_e type;
  DRV_Keypad_State_e      key_states[KEYPAD_MAX_KEY_NUM]; // 按键状态
  DRV_Keypad_keycode_e   *key_phy_map;                    // 物理按键映射表
  uint32_t                press_start_time[KEYPAD_MAX_KEY_NUM]; // 长按计时
  uint8_t                 key_num;                              // 按键数量
  uint32_t                debounce_ms;
  uint32_t                long_press_ms;
} DRV_Keypad_Priv_t;

/* Exported functions prototypes ---------------------------------------------*/
Status_t DRV_Keypad_Init(Device_t *keypad_dev);
Status_t DRV_Keypad_DeInit(Device_t *keypad_dev);
Status_t DRV_Keypad_GetBitmap(Device_t *keypad_dev, uint32_t *bitmap);
Status_t DRV_Keypad_ReadKey(Device_t *keypad_dev,
                            uint8_t  *key_idx,
                            uint8_t  *key_cnt);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_KEYPAD_WRAPPER_H__ */
