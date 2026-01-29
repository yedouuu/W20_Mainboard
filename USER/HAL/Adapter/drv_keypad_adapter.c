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

static DRV_Keypad_Ops_t keypad_tm1638_ops = {
    .base =
        {
            .init   = DRV_Keypad_Init,
            .deinit = DRV_Keypad_DeInit,
            .open   = NULL,
            .close  = NULL,
            .read   = NULL,
            .write  = NULL,
            .ioctl  = NULL,
        },
    .magic     = DEVICE_MAGIC_KEYPAD,
    .BSP_Init  = BSP_TM1638_Init,
    .GetBitmap = BSP_TM1638_ReadKey,
};

static DRV_Keypad_Priv_t keypad_tm1638_priv = {
    .type             = DRV_KEYPAD_TM1638,
    .key_states       = NULL,
    .key_code         = NULL,
    .key_num          = 10,
    .press_start_time = NULL,
    .debounce_ms      = 50,
    .long_press_ms    = 1000,
};

Device_t keypad_tm1638 = {
    .name      = "KEYPAD",
    .dev_class = DEV_CLASS_INPUT,
    .ops       = (void *)&keypad_tm1638_ops,
    .priv      = (void *)&keypad_tm1638_priv,
    .res       = NULL,
};

DEVICE_EXPORT(keypad_tm1638);
