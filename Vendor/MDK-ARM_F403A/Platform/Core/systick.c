/**
 * @file systick.c
 * @author yedouuu (l1530396447@gmail.com)
 * @brief 提供毫秒和微秒级的时间函数, 使用RTOS时基于定时器实现, 否则使用SysTick实现
 * @version 0.1
 * @date 2026-05-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "systick.h"
#include "timer.h"

#if USE_OS

#define TIMEBASE_TIMER TIM5

static bool timebase_ready = false;

static void Timebase_TimerInit(void)
{
  uint32_t timer_clock;
  uint32_t prescaler;

  timer_clock = Timer_GetClockMax(TIMEBASE_TIMER);
  prescaler   = (timer_clock / 1000000U);

  if (prescaler == 0U)
  {
    return;
  }

  Timer_ClockCmd(TIMEBASE_TIMER, true);
  tmr_reset(TIMEBASE_TIMER);
  tmr_32_bit_function_enable(TIMEBASE_TIMER, TRUE);
  tmr_cnt_dir_set(TIMEBASE_TIMER, TMR_COUNT_UP);
  tmr_base_init(TIMEBASE_TIMER, 0xFFFFFFFFUL, prescaler - 1U);
  tmr_counter_value_set(TIMEBASE_TIMER, 0U);
  tmr_counter_enable(TIMEBASE_TIMER, TRUE);

  timebase_ready = true;
}

void SysTick_Init(void)
{
  system_core_clock_update();
  Timebase_TimerInit();
}

uint32_t millis(void)
{
  return micros() / 1000U;
}

uint32_t micros(void)
{
  if (!timebase_ready)
  {
    return 0;
  }

  return tmr_counter_value_get(TIMEBASE_TIMER);
}

void delay_ms(uint32_t ms)
{
  while (ms--)
  {
    delay_us(1000U);
  }
}

void delay_us(uint32_t us)
{
  uint32_t start;

  if (!timebase_ready)
  {
    return;
  }

  start = micros();

  while ((micros() - start) < us)
  {
  }
}

#else

#ifndef SYSTICK_TICK_FREQ
#  define SYSTICK_TICK_FREQ 1000 // Hz
#endif

#define SYSTICK_TICK_INTERVAL (1000U / SYSTICK_TICK_FREQ)
#define SYSTICK_LOAD_VALUE    (F_CPU / SYSTICK_TICK_FREQ)
#define SYSTICK_MILLIS        (SystemTickCount * SYSTICK_TICK_INTERVAL)

static volatile uint32_t SystemTickCount = 0;

void SysTick_Init(void)
{
  system_core_clock_update();
  SystemTickCount = 0;
  SysTick_Config(SYSTICK_LOAD_VALUE);
  NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIORITY);
}

void SysTick_Handler(void)
{
  SystemTickCount++;
}

uint32_t millis(void)
{
  return SYSTICK_MILLIS;
}

uint32_t micros(void)
{
  return (SYSTICK_MILLIS * 1000U +
          (SYSTICK_LOAD_VALUE - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}

void delay_ms(uint32_t ms)
{
  uint32_t start     = SystemTickCount;
  uint32_t waitTicks = ms / SYSTICK_TICK_INTERVAL;

  if ((ms % SYSTICK_TICK_INTERVAL) != 0U)
  {
    waitTicks++;
  }

  while ((SystemTickCount - start) < waitTicks)
  {
  }
}

void delay_us(uint32_t us)
{
  uint32_t start_val = SysTick->VAL;
  uint32_t cycles    = us * CYCLES_PER_MICROSECOND;

  while (1)
  {
    uint32_t current_val = SysTick->VAL;
    uint32_t elapsed;

    if (current_val <= start_val)
    {
      elapsed = start_val - current_val;
    }
    else
    {
      elapsed = start_val + (SYSTICK_LOAD_VALUE - current_val);
    }

    if (elapsed >= cycles)
    {
      break;
    }
  }
}

#endif
