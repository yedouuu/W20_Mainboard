
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"
#include "bsp_config.h"

/* Private defines -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  LED2                                   = 0,
  LED3                                   = 1,
  LED4                                   = 2,
  LED5                                   = 3
} led_type;


/* Exported variables prototypes ---------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
void BSP_LED_Init(void);
void BSP_LED_On(uint8_t led);
void BSP_LED_Off(uint8_t led);
void BSP_LED_Toggle(uint8_t led);


#endif
