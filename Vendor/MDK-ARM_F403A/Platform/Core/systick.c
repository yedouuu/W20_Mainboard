
#include "systick.h"

#ifndef SYSTICK_TICK_FREQ
#define SYSTICK_TICK_FREQ 1000 // Hz
#endif

#define SYSTICK_TICK_INTERVAL (1000 / SYSTICK_TICK_FREQ)
#define SYSTICK_LOAD_VALUE    (F_CPU / SYSTICK_TICK_FREQ)
#define SYSTICK_MILLIS        (SystemTickCount * SYSTICK_TICK_INTERVAL)

static volatile uint32_t SystemTickCount = 0;

void SysTick_Init(void)
{
  system_core_clock_update();
  // Default Configure SysTick for 1ms interrupts
  SysTick_Config(SYSTICK_LOAD_VALUE);
  NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIORITY);
}

void SysTick_Handler(void)
{
  SystemTickCount++;
}

inline uint32_t millis(void)
{
  return SYSTICK_MILLIS;
}

inline uint32_t micros(void)
{
  return (SYSTICK_MILLIS * 1000 +
          (SYSTICK_LOAD_VALUE - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}

inline void delay_ms(uint32_t ms)
{
  uint32_t start     = SystemTickCount;
  uint32_t waitTicks = ms / SYSTICK_TICK_INTERVAL;
  while ((SystemTickCount - start) < waitTicks);
}

inline void delay_us(uint32_t us)
{
  uint32_t start_val = SysTick->VAL;
  uint32_t cycles    = us * CYCLES_PER_MICROSECOND;

  while (1)
  {
    uint32_t current_val = SysTick->VAL;
    uint32_t elapsed;

    if (current_val <= start_val) { elapsed = start_val - current_val; }
    else
    {
      elapsed = start_val + (SYSTICK_LOAD_VALUE - current_val);
    }

    if (elapsed >= cycles) { break; }
  }
}
