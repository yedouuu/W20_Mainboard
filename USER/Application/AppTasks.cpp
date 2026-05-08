/**
 * @file AppTasks.cpp
 * @author yedouuu (l1530396447@gmail.com)
 * @brief
 * @version 0.1
 * @date 2026-05-05
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "AppTasks.h"
#include "App.h"

#include "bsp_led.h"
#include "drv_wrapper.h"
#include "Logger.h"
#include "Services/machine_ctrl.h"

#if APP_TASK_ENABLE_LVGL
#  include "lvgl.h"
#  include "lv_port_indev.h"
#endif

#include "FreeRTOS.h"
#include "task.h"

// Runtime stats timer counter (using systick)
static uint32_t runtime_counter = 0;

uint32_t GetRunTimeCounterValue(void)
{
  return runtime_counter++;
}

// 优先级规划（范围：0-15，0为空闲任务保留）
// 1-3: 低优先级（测试、日志等）
// 4-7: 中等优先级（业务逻辑、控制）
// 8-11: 高优先级（实时响应、UI）
// 12-15: 最高优先级（中断后处理、系统服务）
#define TASK_PRIORITY_TEST           2
#define TASK_PRIORITY_SYSTEM_SERVICE 1
#define TASK_PRIORITY_MACHINE        1
#define TASK_PRIORITY_LVGL           3
#define TASK_PRIORITY_MONITOR        2

#define TASK_STACK_TEST              128
#define TASK_STACK_SYSTEM_SERVICE    256
#define TASK_STACK_MACHINE           256
#define TASK_STACK_LVGL              2048
#define TASK_STACK_MONITOR           256

__IO uint16_t dma_trans_complete_flag = 0;

static void TEST_Task(void *pvParameters)
{
  (void)pvParameters;

  while (1)
  {
    BSP_LED_Toggle(LED1);
    log_i("Running test task 1...");
    DRV_DelayMs(500);
  }
}

static void System_Service_Task(void *pvParameters)
{
  (void)pvParameters;

  while (1)
  {
    if (dma_trans_complete_flag > 0) { dma_trans_complete_flag--; }

    DRV_DelayMs(1);
  }
}

static void Machine_Task(void *pvParameters)
{
  (void)pvParameters;

  while (1)
  {
    Machine_FSM_Run();
    DRV_DelayMs(10);
  }
}

#if APP_TASK_ENABLE_LVGL
static void LVGL_Task(void *pvParameters)
{
  (void)pvParameters;

  while (1)
  {
    // log_i("Running LVGL task...");
    lv_port_touchpad_irq_process();
    lv_timer_handler();
    DRV_DelayMs(5);
  }
}

// static void LVGL_Task(void *pvParameters)
// {
//   (void)pvParameters;

//   TickType_t last = xTaskGetTickCount();

//   while (1)
//   {
//     lv_timer_handler();
//     DRV_DelayMsUntil(5);
//   }
// }
#endif

static void Monitor_Task(void *pvParameters)
{
  (void)pvParameters;

  // char *task_list_buffer;
  // char *runtime_stats_buffer;
  TaskStatus_t *task_status_array;
  UBaseType_t   task_count;
  uint32_t      total_runtime;

  // 分配缓冲区
  // task_list_buffer = (char *)pvPortMalloc(1024);
  // runtime_stats_buffer = (char *)pvPortMalloc(1024);

  log_i("Task Monitor started.");

  while (1)
  {
    // 获取任务数量
    task_count = uxTaskGetNumberOfTasks();

    // 分配任务状态数组
    task_status_array = (TaskStatus_t *)pvPortMalloc(task_count *
                                                     sizeof(TaskStatus_t));

    if (task_status_array != NULL)
    {
      
      // 获取任务状态信息
      task_count = uxTaskGetSystemState(task_status_array,
                                        task_count,
                                        &total_runtime);

      log_i("============== Task Monitor Report =============");
      log_i("Total Tasks: %lu", task_count);
      log_i("System Uptime: %lu ms", xTaskGetTickCount() * portTICK_PERIOD_MS);

      // 打印任务信息
      for (UBaseType_t i = 0; i < task_count; i++)
      {
        const char *task_state;
        switch (task_status_array[i].eCurrentState)
        {
          case eRunning:
            task_state = "Running";
            break;
          case eReady:
            task_state = "Ready";
            break;
          case eBlocked:
            task_state = "Blocked";
            break;
          case eSuspended:
            task_state = "Suspended";
            break;
          case eDeleted:
            task_state = "Deleted";
            break;
          default:
            task_state = "Unknown";
            break;
        }

        // 计算CPU使用率
        uint32_t cpu_percent = 0;
        if (total_runtime > 0)
        {
          cpu_percent = (task_status_array[i].ulRunTimeCounter * 100) /
                        total_runtime;
        }

        log_i("Task[%lu]: %s \t| State: %s \t| Priority: %lu \t| Stack HWM: %u "
              "\t| CPU: %lu%%",
              i,
              task_status_array[i].pcTaskName,
              task_state,
              task_status_array[i].uxCurrentPriority,
              task_status_array[i].usStackHighWaterMark,
              cpu_percent);

        // 检查堆栈使用情况
        if (task_status_array[i].usStackHighWaterMark < 50)
        {
          log_w("WARNING: Task '%s' has low stack space (%u words)",
                task_status_array[i].pcTaskName,
                task_status_array[i].usStackHighWaterMark);
        }
      }

      // 打印堆内存使用情况
      size_t free_heap     = xPortGetFreeHeapSize();
      size_t min_free_heap = xPortGetMinimumEverFreeHeapSize();
      log_i("Free Heap: %u bytes, Min Ever Free: %u bytes",
            free_heap,
            min_free_heap);

      if (free_heap < 1024)
      {
        log_w("WARNING: Low heap memory (%u bytes)", free_heap);
      }

      vPortFree(task_status_array);
    }
    else
    {
      log_e("Failed to allocate memory for task monitoring");
    }

    log_i("================= END ================");
    // 每10秒监控一次
    DRV_DelayMs(10000);
  }
}

void AppTasks_Init(void)
{
  xTaskCreate(TEST_Task,
              "TEST_Task",
              TASK_STACK_TEST,
              NULL,
              TASK_PRIORITY_TEST,
              NULL);

  // xTaskCreate(System_Service_Task,
  //             "SysService",
  //             TASK_STACK_SYSTEM_SERVICE,
  //             NULL,
  //             TASK_PRIORITY_SYSTEM_SERVICE,
  //             NULL);

  // xTaskCreate(Machine_Task,
  //             "Machine",
  //             TASK_STACK_MACHINE,
  //             NULL,
  //             TASK_PRIORITY_MACHINE,
  //             NULL);

#if APP_TASK_ENABLE_LVGL
  xTaskCreate(LVGL_Task,
              "LVGL",
              TASK_STACK_LVGL,
              NULL,
              TASK_PRIORITY_LVGL,
              NULL);
#endif

  // 创建任务监控器
  xTaskCreate(Monitor_Task,
              "TaskMonitor",
              TASK_STACK_MONITOR,
              NULL,
              TASK_PRIORITY_MONITOR,
              NULL);
}
