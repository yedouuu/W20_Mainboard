
#include "Common/common.h"
#include "HAL/drv_tick_wrapper.h"

#ifndef LVGL_WINDOWS_SIMULATOR
#  include "systick.h"
#endif

#if USE_OS
#  include "FreeRTOS.h"
#  include "task.h"
#endif

uint32_t __DRV_GetTickMs_Adapter(void)
{
#if USE_OS
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    if (xPortIsInsideInterrupt())
    {
      return xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;
    }
    else
    {
      return xTaskGetTickCount() * portTICK_PERIOD_MS;
    }
  }
#endif

  return millis();
}

/**
 * @brief 获取us时间
 *
 * 若使用了RTOS, 则返回0, 因为RTOS的tick通常是以ms为单位的,
 * 无法提供us级别的时间精度
 *
 * @return uint32_t
 */
uint32_t __DRV_GetTickUs_Adapter(void)
{
#if USE_OS
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { return 0; }
#endif

  return micros();
}

void __DRV_DelayMs_Adapter(uint32_t ms)
{
#if USE_OS
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
  {
    vTaskDelay(pdMS_TO_TICKS(ms));
    return;
  }
#endif

  delay_ms(ms);
}

void __DRV_DelayMsUntil_Adapter(uint32_t target_ms)
{
#if USE_OS
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
  {
    TickType_t last = xTaskGetTickCount();
    vTaskDelayUntil(&last, pdMS_TO_TICKS(target_ms));
  }
#else
  delay_ms(target_ms);
#endif
}

void __DRV_DelayUs_Adapter(uint32_t us)
{
  delay_us(us);
}

static DRV_Tick_Ops_t Tick_Ops = {
    .GetMillis = __DRV_GetTickMs_Adapter,
    .GetMicros = __DRV_GetTickUs_Adapter,
    .DelayMs   = __DRV_DelayMs_Adapter,
    .DelayUs   = __DRV_DelayUs_Adapter,
};

void DRV_TickAdapter_Init(void)
{
  DRV_Tick_Init(&Tick_Ops);
}
