
#ifndef __MCU_CORE_H__
#define __MCU_CORE_H__

#define sei() (__set_PRIMASK(0))
#define cli() (__set_PRIMASK(1))

#include "systick.h"
#include "uart.h"
#include "adc.h"
#include "exti.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"
// #include "wdg.h"

#ifdef __cplusplus
extern "C" {
#endif

void Core_Init(void);

/**
 * @brief 快速设置引脚模式
 * 
 * @param pin 
 * @param mode 
 * @return uint8_t 
 */
uint8_t pinMode(Pin_TypeDef pin, PinMode_TypeDef mode);

#ifdef __cplusplus
}
#endif

#endif
