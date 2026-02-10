/*
 * MIT License
 * Copyright (c) 2025 Yedouuu | UNION
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
#ifndef __DRV_ENCODER_WRAPPER_H__
#define __DRV_ENCODER_WRAPPER_H__

/* Includes -----------------------------------------------------------------*/
#include "Common/common.h"
#include "DeviceManager/device_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types -----------------------------------------------------------*/

typedef enum
{
  DRV_Encoder_NONE,
  DRV_Encoder_RAISE,
  DRV_Encoder_FALL,
} DRV_Encoder_State_e;


typedef struct __DRV_Encoder_Ops_t
{
  Device_Ops_t   base;
  const uint32_t magic;
  void (*BSP_Init)(void);
  void (*BSP_Deinit)(void);
  void (*Enable)(void);
  void (*Disable)(void);
} DRV_Encoder_Ops_t;

typedef struct __DRV_Encoder_Priv_t
{
  DRV_Encoder_State_e state;    /* Encoder当前状态, #Ref DRV_Encoder_State_e */
  uint16_t            enc_tick; /* 编码器原始数据 */
} DRV_Encoder_Priv_t;

/* Exported functions prototypes ---------------------------------------------*/
Status_t DRV_Encoder_Init(Device_t *encoder_dev);
Status_t DRV_Encoder_DeInit(Device_t *encoder_dev);
Status_t DRV_Encoder_Enable(Device_t *encoder_dev);
Status_t DRV_Encoder_Disable(Device_t *encoder_dev);
Status_t DRV_Encoder_GetState(Device_t            *encoder_dev,
                              DRV_Encoder_State_e *status);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_ENCODER_WRAPPER_H__ */
