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

#include "bsp_mg.h"
#include "mcu_core.h"

struct _MG_Resource_t
{
  uint8_t read_pin; /* ADC channel */
};

const MG_Resource_t mgs_left_1_res = {
    .read_pin = MGS_LEFT_1_PIN,
};

const MG_Resource_t mgs_left_2_res = {
    .read_pin = MGS_LEFT_2_PIN,
};

const MG_Resource_t mgs_left_3_res = {
    .read_pin = MGS_LEFT_3_PIN,
};

const MG_Resource_t mgs_right_1_res = {
    .read_pin = MGS_RIGHT_1_PIN,
};

const MG_Resource_t mgs_right_2_res = {
    .read_pin = MGS_RIGHT_2_PIN,
};

const MG_Resource_t mgs_right_3_res = {
    .read_pin = MGS_RIGHT_3_PIN,
};

const MG_Resource_t mgb_bm_res = {
    .read_pin = MGB_BM_PIN,
};

const MG_Resource_t mgb_hd_res = {
    .read_pin = MGB_HD_PIN,
};

void BSP_MG_Init(const void *mg)
{
  MG_Resource_t *mg_res = (MG_Resource_t *)mg;
  pinMode((Pin_TypeDef)mg_res->read_pin, INPUT_ANALOG_DMA);
}

void BSP_MG_GetRawData(const void *mg, uint16_t *raw)
{
  MG_Resource_t  *mg_res  = (MG_Resource_t *)mg;
  PinInfo_TypeDef pinInfo = PIN_MAP[mg_res->read_pin];
  *raw                    = ADC_DMA_GetValue(pinInfo.ADC_Channel);
}
