
#include "drv_wrapper.h"

#define TIMER_INTERVAL_MAX 10

typedef struct
{
  TimerInterval_CB_t   callback;
  uint32_t             interval_ms;
  TimerInterval_Type_e repeat;
  uint32_t             last_tick;
} TimerInterval_t;

static DRV_Tick_Ops_t *Tick_Ops                                 = NULL;
static TimerInterval_t Interval_CBFunctions[TIMER_INTERVAL_MAX] = {0};

void DRV_Tick_Init(DRV_Tick_Ops_t *ops)
{
  if (ops != NULL) { Tick_Ops = ops; }
}

/**
 * @brief get ms time
 * 
 * @return uint32_t 
 */
uint32_t DRV_GetTickMs(void)
{
  return Tick_Ops->GetMillis();
}

/**
 * @brief get us time
 * 
 * @return uint32_t 
 */
uint32_t DRV_GetTickUs(void)
{
  return Tick_Ops->GetMicros();
}

void DRV_DelayMs(uint32_t ms)
{
  Tick_Ops->DelayMs(ms);
}

void DRV_DelayUs(uint32_t us)
{
  Tick_Ops->DelayUs(us);
}

void DRV_TimerIntervalCore(void)
{
  for (int i = 0; i < TIMER_INTERVAL_MAX; i++)
  {
    if (Interval_CBFunctions[i].callback != NULL)
    {
      uint32_t current_tick = DRV_GetTickMs();
      if ((current_tick - Interval_CBFunctions[i].last_tick) >=
          Interval_CBFunctions[i].interval_ms)
      {
        // 执行回调
        Interval_CBFunctions[i].callback();
        Interval_CBFunctions[i].last_tick = current_tick;
        // 如果是单次执行，清除回调
        if (Interval_CBFunctions[i].repeat == TIMER_INTERVAL_ONCE)
        {
          Interval_CBFunctions[i].callback    = NULL;
          Interval_CBFunctions[i].interval_ms = 0;
        }
      }
    }
  }
}

Status_t DRV_SetInterval(TimerInterval_CB_t   callback,
                         uint32_t             interval_ms,
                         TimerInterval_Type_e repeat)
{
  for (int i = 0; i < TIMER_INTERVAL_MAX; i++)
  {
    if (Interval_CBFunctions[i].callback == NULL)
    {
      Interval_CBFunctions[i].callback    = callback;
      Interval_CBFunctions[i].interval_ms = interval_ms;
      Interval_CBFunctions[i].repeat      = repeat;
      log_d("Set interval callback at index %d, interval %d ms, repeat %d",
            i,
            interval_ms,
            repeat);
      return kStatus_Success;
    }
  }
  log_w("No available slot to set interval callback");
  return kStatus_Fail;
}

Status_t DRV_ClearInterval(TimerInterval_CB_t callback)
{
  for (int i = 0; i < TIMER_INTERVAL_MAX; i++)
  {
    if (Interval_CBFunctions[i].callback == callback)
    {
      Interval_CBFunctions[i].callback    = NULL;
      Interval_CBFunctions[i].interval_ms = 0;
      log_d("Cleared interval callback at index %d", i);
      return kStatus_Success;
    }
  }
  log_w("Interval callback not found to clear");
  return kStatus_NotFound;
}
