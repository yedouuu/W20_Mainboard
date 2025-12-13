/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_NS2009.h
 * 
 * @author Yedou | UNION
 * 
 * @brief NS2009 resistive touch screen driver interface.
 * 
 * @version V1.0 2025-12-05 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/

#ifndef __BSP_NS2009_H__
#define __BSP_NS2009_H__

#include "bsp_touch.h"

/* NS2009 Command Definitions */
typedef enum
{
  NS2009_ADDRESS_WR        = 0x92,  /* b(1001 0010) */
  NS2009_ADDRESS_RD        = 0x93,  /* b(1001 0011) */
  NS2009_LOW_POWER_READ_X  = 0xC4,  /* 读取时关闭中断 1100 0000 -> 1100 0100 */
  NS2009_LOW_POWER_READ_Y  = 0xD4,  /* 读取时关闭中断 1101 0000 -> 1101 0100 */
  NS2009_LOW_POWER_READ_Z1 = 0xE4,
  NS2009_LOW_POWER_READ_Z2 = 0xF4,
  NS2009_ONLY_OPEN_IRQ     = 0x00,  /* 只用于重新开启中断 */
} NS2009_Cmd_e;

/* NS2009 Configuration Constants */
#define NS2009_ADC_RESOLUTION     4096
#define NS2009_PRESSURE_THRESHOLD 0.3f  // 压力阈值

/* NS2009 Pin Definitions (should be defined in board config) */
#ifndef TOUCH_NS2009_A0_PIN
  #define TOUCH_NS2009_A0_PIN       PA0
#endif

#ifndef TOUCH_NS2009_INT_PIN
  #define TOUCH_NS2009_INT_PIN      PA1
#endif

#ifndef TOUCH_NS2009_I2C_SCL_PIN
  #define TOUCH_NS2009_I2C_SCL_PIN  PB6
#endif

#ifndef TOUCH_NS2009_I2C_SDA_PIN
  #define TOUCH_NS2009_I2C_SDA_PIN  PB7
#endif

/* Global Variables */
extern volatile uint8_t ns2009_irq_flag;

/* NS2009 Public Functions */
void     NS2009_Init(const Touch_Resource_t* touch_res);
void     NS2009_Read_Coordinate(const Touch_Resource_t* touch_res, TouchPoint_t* p);
uint16_t NS2009_Read_Channel(const Touch_Resource_t* touch_res, NS2009_Cmd_e cmd);

/* NS2009 Device Resource (defined in bsp_NS2009.c) */
extern const Touch_Resource_t touch_main_res;

#endif /* __BSP_NS2009_H__ */