#ifndef __HAL_TIME_H__
#define __HAL_TIME_H__

#include "Common/common.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*TimerInterval_CB_t)(void);

typedef enum _TimerInterval_Type_e
{
  TIMER_INTERVAL_ONCE = 0,
  TIMER_INTERVAL_REPEAT,
} TimerInterval_Type_e;

typedef struct _DRV_Tick_Ops_t
{
  uint32_t (*GetMillis)(void);
  uint32_t (*GetMicros)(void);
  void     (*DelayMs)(uint32_t ms);
  void     (*DelayUs)(uint32_t us);
} DRV_Tick_Ops_t;

void     DRV_Tick_Init(DRV_Tick_Ops_t *ops);
uint32_t DRV_GetMillis(void);
uint32_t DRV_GetMicros(void);
void     DRV_DelayMs(uint32_t ms);
void     DRV_DelayUs(uint32_t us);
Status_t DRV_SetInterval(TimerInterval_CB_t   callback,
                         uint32_t             interval_ms,
                         TimerInterval_Type_e repeat);
Status_t DRV_ClearInterval(TimerInterval_CB_t callback);

#if defined(__cplusplus)
}
#endif

#endif // __HAL_TIME_H__
