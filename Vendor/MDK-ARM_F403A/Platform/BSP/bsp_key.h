
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"
#include "bsp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private defines -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  USER_BUTTON                            = 0,
  NO_BUTTON                              = 1
} button_type;

/* Exported variables prototypes ---------------------------------------------*/
extern uint8_t g_speed;

/* Exported functions prototypes ---------------------------------------------*/
void BSP_KEY_Init(void);
uint8_t BSP_KEY_GetState(uint8_t key);


#ifdef __cplusplus
}
#endif

#endif
