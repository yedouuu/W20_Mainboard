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

#include "bsp_uv.h"
#include "mcu_core.h"

struct _UV_Resource_t
{
  uint8_t read_pin; /* ADC channel */
};

const UV_Resource_t uv_left_res = {
    .read_pin = UV_LEFT_PIN,
};

const UV_Resource_t uv_right_res = {
    .read_pin = UV_RIGHT_PIN,
};

void BSP_UV_Init(const void *uv)
{
  UV_Resource_t *uv_res = (UV_Resource_t *)uv;
  pinMode((Pin_TypeDef)uv_res->read_pin, INPUT_ANALOG_DMA);
}

void BSP_UV_GetRawData(const void *uv, uint16_t *raw)
{
  UV_Resource_t  *uv_res  = (UV_Resource_t *)uv;
  PinInfo_TypeDef pinInfo = PIN_MAP[uv_res->read_pin];
  *raw                    = ADC_DMA_GetValue(pinInfo.ADC_Channel);
}
