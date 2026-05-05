#ifndef __APP_TASKS_H__
#define __APP_TASKS_H__

#include "Common/common.h"

#define APP_TASK_ENABLE_LVGL 1

#ifdef __cplusplus
extern "C" {
#endif

void AppTasks_Init(void);

// Runtime stats counter function
uint32_t GetRunTimeCounterValue(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_TASKS_H__ */
