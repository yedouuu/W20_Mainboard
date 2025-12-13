
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

#ifdef __cplusplus
}
#endif

#endif
