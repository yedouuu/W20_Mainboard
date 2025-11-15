
#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdbool.h>
#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_GET_CLOCK_MAX(TIMx)           F_CPU

typedef void (*Timer_CallbackFunction_t)(void);

void     Timer_SetEnable(TIM_TypeDef* TIMx, bool Enable);
void     Timer_SetInterrupt(TIM_TypeDef* TIMx, uint32_t time, Timer_CallbackFunction_t Function);
void     Timer_SetInterruptTimeUpdate(TIM_TypeDef* TIMx, uint32_t Time);
bool     Timer_SetInterruptFreqUpdate(TIM_TypeDef* TIMx, uint32_t Freq);
void     Timer_SetInterruptBase(
    TIM_TypeDef* TIMx, 
    uint16_t Period, uint16_t Prescaler, 
    Timer_CallbackFunction_t Function, 
    uint8_t PreemptionPriority, uint8_t SubPriority
);
void     Timer_SetCompare(TIM_TypeDef* TIMx, uint8_t TimerChannel, uint32_t Compare);
void     Timer_SetPrescaler(TIM_TypeDef* TIMx, uint16_t Prescaler);
void     Timer_SetReload(TIM_TypeDef* TIMx, uint16_t Reload);
void     Timer_ClockCmd(TIM_TypeDef* TIMx, bool NewState);
uint32_t Timer_GetClockMax(TIM_TypeDef* TIMx);
uint32_t Timer_GetClockOut(TIM_TypeDef* TIMx);
uint16_t Timer_GetCompare(TIM_TypeDef* TIMx, uint8_t TimerChannel);
void     Timer_GenerateUpdate(TIM_TypeDef* TIMx);


#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */
