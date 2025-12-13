
#include "drv_wrapper.h"
#include "systick.h"
#include "Logger.h"

uint32_t DRV_GetMillis(void)
{
  return millis();
}


uint32_t DRV_GetMicros(void)
{
  return micros();
}


void DRV_DelayMs(uint32_t ms)
{
  delay_ms(ms);
}


void DRV_DelayUs(uint32_t us)
{
  delay_us(us);
}
