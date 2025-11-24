#ifndef __HAL_TIME_H__
#define __HAL_TIME_H__

#include "Common/common.h"

#if defined(__cplusplus)
extern "C" {
#endif

uint32_t DRV_GetMillis(void);
uint32_t DRV_GetMicros(void);
void DRV_DelayMs(uint32_t ms);
void DRV_DelayUs(uint32_t us);

#if defined(__cplusplus)
}
#endif

#endif // __HAL_TIME_H__
