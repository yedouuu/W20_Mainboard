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
#include "bsp_encoder.h"
#include "mcu_core.h"
#include "Common/common.h"


void BSP_Encoder_IRQ_Handler(void)
{
  /* 读取引脚状态 */
  static uint8_t last_state = 0;
  uint8_t        curr_state = digitalRead_FAST(ENCODER_READ_PIN);

  // log_d("Encoder IRQ: last_state=%d, curr_state=%d", last_state, curr_state);
  /* 检测下降沿 */
  if (last_state == 1 && curr_state == 0)
  {
    // log_d("Encoder [falling] edge detected.");
    /* TODO: 在这里处理编码器下降沿事件 */
    digitalWrite_LOW(ENCODER_TO_FPGA_PIN);
  }
  else
  {
    // log_d("Encoder [rising] edge detected.");
    digitalWrite_HIGH(ENCODER_TO_FPGA_PIN);
  }
  last_state = curr_state;
}

void BSP_Encoder_Init(void)
{
  pinMode(ENCODER_CTRL_PIN, OUTPUT);
  pinMode(ENCODER_TO_FPGA_PIN, OUTPUT);
  pinMode(ENCODER_READ_PIN, INPUT_PULLUP);

  EXTIx_Init(ENCODER_READ_PIN, BSP_Encoder_IRQ_Handler, CHANGE, 3, 0);
}

void BSP_Encoder_DeInit(void)
{
  EXTIx_DeInit(ENCODER_READ_PIN);
}

void BSP_Encoder_Enable(void)
{
  digitalWrite_HIGH(ENCODER_CTRL_PIN);
}

void BSP_Encoder_Disable(void)
{
  digitalWrite_LOW(ENCODER_CTRL_PIN);
}
