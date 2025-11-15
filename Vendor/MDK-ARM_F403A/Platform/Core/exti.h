
#ifndef __EXTI_H__
#define __EXTI_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private defines -----------------------------------------------------------*/
#define CHANGE  EXINT_TRIGGER_BOTH_EDGE
#define FALLING EXINT_TRIGGER_FALLING_EDGE
#define RISING  EXINT_TRIGGER_RISING_EDGE

#define EXTI_POLARITY_TYPE exint_polarity_config_type


typedef void(*EXTI_CallbackFunction_t)(void);
/* Exported types ------------------------------------------------------------*/


/* Exported variables prototypes ---------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

void EXTIx_Init(
  uint8_t Pin,
  EXTI_CallbackFunction_t Function,
  EXTI_POLARITY_TYPE line_polarity,
  uint8_t PreemptionPriority,
  uint8_t SubPriority
);

void EXTIx_DeInit(uint8_t Pin);

void attachInterrupt(
  uint8_t Pin,
  EXTI_CallbackFunction_t Function,
  EXTI_POLARITY_TYPE polarity_config
);

void detachInterrupt(uint8_t Pin);


#ifdef __cplusplus
}
#endif

#endif
