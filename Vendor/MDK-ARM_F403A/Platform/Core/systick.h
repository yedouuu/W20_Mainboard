/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file systick.h
 * 
 * @author Yedou | UNION
 * 
 * @brief Provide the HAL APIs of Systick and corresponding options.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-10-29 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"


/* Private defines -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported variables prototypes ---------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Init(void);
uint32_t millis(void);
uint32_t micros(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTICK_H__ */
