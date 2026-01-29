
#include "Common/common.h"
#include "HAL/drv_tick_wrapper.h"
#include "lvgl.h"

#ifndef LVGL_WINDOWS_SIMULATOR
#  include "systick.h"
#endif


uint32_t __DRV_GetTickMs_Adapter(void)
{
    return 0;
}

uint32_t __DRV_GetTickUs_Adapter(void)
{
    return lv_tick_get();
}

void __DRV_DelayMs_Adapter(uint32_t ms)
{
    lv_delay_ms(ms);
}

void __DRV_DelayUs_Adapter(uint32_t us)
{
    return;
}

static DRV_Tick_Ops_t Tick_Ops = {
  .GetMillis = __DRV_GetTickMs_Adapter,
  .GetMicros = __DRV_GetTickUs_Adapter,
  .DelayMs = __DRV_DelayMs_Adapter,
  .DelayUs = __DRV_DelayUs_Adapter,
};

void DRV_TickAdapter_Init(void)
{
    DRV_Tick_Init(&Tick_Ops);
}

