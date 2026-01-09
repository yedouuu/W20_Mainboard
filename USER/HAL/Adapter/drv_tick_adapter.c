
#include "Common/common.h"
#include "HAL/drv_tick_wrapper.h"

#ifndef LVGL_WINDOWS_SIMULATOR
#  include "systick.h"
#endif


uint32_t __DRV_GetMillis_Adapter(void)
{
  return millis();
}

uint32_t __DRV_GetMicros_Adapter(void)
{
  return micros();
}

void __DRV_DelayMs_Adapter(uint32_t ms)
{
  delay_ms(ms);
}

void __DRV_DelayUs_Adapter(uint32_t us)
{
  delay_us(us);
}

static DRV_Tick_Ops_t Tick_Ops = {
  .GetMillis = __DRV_GetMillis_Adapter,
  .GetMicros = __DRV_GetMicros_Adapter,
  .DelayMs   = __DRV_DelayMs_Adapter,
  .DelayUs   = __DRV_DelayUs_Adapter,
};

void DRV_TickAdapter_Init(void)
{
  DRV_Tick_Init(&Tick_Ops);
}

