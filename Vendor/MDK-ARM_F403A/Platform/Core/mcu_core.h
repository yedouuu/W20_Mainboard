
#ifndef __MCU_CORE_H__
#define __MCU_CORE_H__

#define sei()           (__set_PRIMASK(0))
#define cli()           (__set_PRIMASK(1))
// #define Enable_ALL_IRQ  (__set_PRIMASK(0))
// #define Disable_ALL_IRQ (__set_PRIMASK(1))
#define Enable_ALL_IRQ  
#define Disable_ALL_IRQ 

#include "systick.h"
#include "uart.h"
#include "adc.h"
#include "exti.h"
#include "gpio.h"
#include "pwm.h"
#include "timer.h"
#include "sw_spi.h"
// #include "wdg.h"

#ifdef __cplusplus
extern "C" {
#endif

void Core_Init(void);

#ifdef __cplusplus
}
#endif

#endif
