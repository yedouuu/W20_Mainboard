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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "bsp_IR.h"
#include "mcu_core.h"
#include "Logger.h"

struct _IR_Resource_t{
  uint8_t    en_pin;           // 控制 IR 使能引脚
  uint8_t    read_pin;         // ADC channel 用于读值
  uint8_t    pwm_pin;          // 如果需要调制 PWM
  uint16_t   pwm_duty_default; // 当前 PWM 占空比
  uint16_t   pwm_resolution;   // PWM 分辨率
  uint16_t   pwm_frequency;    // PWM 频率
};

const IR_Resource_t ir_hopper_res = {
    .en_pin           = IR_HOP_STA_CTRL_PIN,
    .read_pin         = IR_HOP_RX_PIN,
    .pwm_pin          = PIN_MAX,
    .pwm_duty_default = 0,
    .pwm_resolution   = 0,
    .pwm_frequency    = 0,
};

const IR_Resource_t ir_stacker_res = {
    .en_pin           = IR_HOP_STA_CTRL_PIN,
    .read_pin         = IR_STA_RX_PIN,
    .pwm_pin          = PIN_MAX,
    .pwm_duty_default = 0,
    .pwm_resolution   = 0,
    .pwm_frequency    = 0,
};

const IR_Resource_t ir_left_res = {
    .en_pin           = IR_CHANNEL_CTRL_PIN,
    .read_pin         = IRR_RX_PIN,
    .pwm_pin          = IRR_PIN,
    .pwm_duty_default = 750,
    .pwm_resolution   = PWM_RESOLUTION_DEFAULT,
    .pwm_frequency    = PWM_FREQUENCY_DEFAULT,
};

const IR_Resource_t ir_right_res = {
    .en_pin           = IR_CHANNEL_CTRL_PIN,
    .read_pin         = IRL_RX_PIN,
    .pwm_pin          = IRL_PIN,
    .pwm_duty_default = 750,
    .pwm_resolution   = PWM_RESOLUTION_DEFAULT,
    .pwm_frequency    = PWM_FREQUENCY_DEFAULT,
};


void BSP_IR_Init(const void* ir)
{
  /* 初始化红外接收引脚 */
  IR_Resource_t* ir_res = (IR_Resource_t*)ir;
  pinMode((Pin_TypeDef)ir_res->read_pin, INPUT_ANALOG_DMA);
  pinMode((Pin_TypeDef)ir_res->en_pin, OUTPUT);
  
  uint8_t ch = PWM_Init(ir_res->pwm_pin, ir_res->pwm_resolution, ir_res->pwm_frequency);
  if ( IS_PIN(ir_res->pwm_pin) && ch == 0 ) {
    log_w("The Pin %d is not configured as PWM channel!", ir_res->pwm_pin);
  }
}

void BSP_IR_Enable(const void* ir)
{
  IR_Resource_t* ir_res = (IR_Resource_t*)ir;
  PinInfo_TypeDef pinInfo = PIN_MAP[ir_res->en_pin];
  GPIO_HIGH(pinInfo.GPIOx, pinInfo.GPIO_Pin_x);
  PWM_Write(ir_res->pwm_pin, ir_res->pwm_duty_default);
}

void BSP_IR_Disable(const void* ir)
{
  IR_Resource_t* ir_res = (IR_Resource_t*)ir;
  PinInfo_TypeDef pinInfo = PIN_MAP[ir_res->en_pin];
  GPIO_LOW(pinInfo.GPIOx, pinInfo.GPIO_Pin_x);
  PWM_Write(ir_res->pwm_pin, 0);
}

void BSP_IR_GetRawData(const void* ir, uint16_t* raw)
{
  IR_Resource_t* ir_res = (IR_Resource_t*)ir;
  PinInfo_TypeDef pinInfo = PIN_MAP[ir_res->read_pin];
  *raw = ADC_DMA_GetValue(pinInfo.ADC_Channel);
}

void BSP_IR_SetPWM(const void* ir, uint16_t duty)
{
  IR_Resource_t* ir_res = (IR_Resource_t*)ir;
  PWM_Write(ir_res->pwm_pin, duty);
}
