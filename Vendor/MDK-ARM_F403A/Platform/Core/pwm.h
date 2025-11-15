
#ifndef __PWM_H__
#define __PWM_H__

#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define pwmWrite(pin, value) PWM_Write(pin, value)

uint8_t PWM_Init(uint8_t Pin, uint32_t Resolution, uint32_t Frequency);
void PWM_Write(uint8_t Pin, uint32_t Value);

#ifdef __cplusplus
}
#endif


#endif