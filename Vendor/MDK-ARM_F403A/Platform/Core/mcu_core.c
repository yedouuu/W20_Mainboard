
#include "mcu_core.h"

void Core_Init(void)
{
  // 初始化MCU核心相关设置
  // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  // GPIO_JTAG_Disable();
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  system_clock_config();
  SysTick_Init();
  Uart_Print_Init(115200);
}
