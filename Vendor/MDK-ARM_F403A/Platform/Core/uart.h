/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file uart.h
 * 
 * @author Yedou | UNION
 * 
 * @brief Provide the HAL APIs of UART and corresponding options.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-11-1 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/

#ifndef __UART_H__
#define __UART_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"


/* Private defines -----------------------------------------------------------*/
#define PRINT_UART                       USART1
#define PRINT_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK
#define PRINT_UART_TX_PIN                GPIO_PINS_9
#define PRINT_UART_TX_GPIO               GPIOA
#define PRINT_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK
#define PRINT_UART_RX_PIN                GPIO_PINS_10
#define PRINT_UART_RX_GPIO               GPIOA
#define PRINT_UART_RX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK


/* Exported types ------------------------------------------------------------*/


/* Exported variables prototypes ---------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
void Uart_Print_Init(uint32_t baudrate);


#endif
