#ifndef __HAL_TIME_H__
#define __HAL_TIME_H__

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

uint32_t HAL_GetMillis(void);
uint32_t HAL_GetMicros(void);
void HAL_DelayMs(uint32_t ms);
void HAL_DelayUs(uint32_t us);

#if defined(__cplusplus)
}
#endif

#endif // __HAL_TIME_H__
