/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_touch.h
 * 
 * @author Yedou | UNION
 * 
 * @brief Provide the BSP APIs of touch and corresponding options.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-12-02 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/
#ifndef __BSP_TOUCH_H__
#define __BSP_TOUCH_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"
#include "bsp_config.h"


#ifdef __cplusplus
  extern "C" {
#endif

/* Exported Typedef --------------------------------------------------------*/
typedef struct _Touch_Resource_t Touch_Resource_t;

/* Exported Variables --------------------------------------------------------*/
extern const Touch_Resource_t touch_main_res;

/* Exported functions prototypes ---------------------------------------------*/
void BSP_Touch_Init(const void* touch);
void BSP_Touch_DeInit(const void* touch);
void BSP_Touch_Open(const void* touch);
void BSP_Touch_Close(const void* touch);
void BSP_Touch_Read(const void* touch, uint16_t* x, uint16_t* y, uint8_t* pressed);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_TOUCH_H__ */