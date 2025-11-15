
#include "HAL.h"
#include "systick.h"
#include "Logger.h"

uint32_t HAL_GetMillis(void)
{
  return millis();
}


uint32_t HAL_GetMicros(void)
{
  return micros();
}


void HAL_DelayMs(uint32_t ms)
{
  delay_ms(ms);
}


void HAL_DelayUs(uint32_t us)
{
  delay_us(us);
}
