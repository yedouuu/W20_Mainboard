
#include "drv_wrapper.h"

#include "HAL/Adapter/drv_tick_adapter.h"

void DRV_Init(void)
{
  // Initialize HAL components here
  DRV_TickAdapter_Init();

  DRV_SetInterval(DRV_Update, 10, TIMER_INTERVAL_REPEAT);
}

/**
 * @brief Circular update function for HAL
 * For example, updating sensors, etc.
 */
void DRV_Update(void)
{
  // Update HAL components here
}
