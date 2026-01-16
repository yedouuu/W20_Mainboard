#ifndef __LOGGER_H__
#define __LOGGER_H__

/* Includes ----------------------------------------------------------*/
#include <stdint.h>

#ifndef LVGL_WINDOWS_SIMULATOR
#include "elog.h"
#else
#define log_e(fmt, ...)                                                        \
  lv_log(fmt, ##__VA_ARGS__);                                                  \
  lv_log("\r\n", ##__VA_ARGS__)
#define log_i(fmt, ...)                                                        \
  lv_log(fmt, ##__VA_ARGS__);                                                  \
  lv_log("\r\n", ##__VA_ARGS__)
#define log_w(fmt, ...)                                                        \
  lv_log(fmt, ##__VA_ARGS__);                                                  \
  lv_log("\r\n", ##__VA_ARGS__)
#define log_d(fmt, ...)                                                        \
  lv_log(fmt, ##__VA_ARGS__);                                                  \
  lv_log("\r\n", ##__VA_ARGS__)
#define log_v(fmt, ...)                                                        \
  lv_log(fmt, ##__VA_ARGS__);                                                  \
  lv_log("\r\n", ##__VA_ARGS__)
#endif

/* Private defines -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* 定义日志级别 */
typedef enum
{
  LOG_LEVEL_ASSERT,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARN,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_VERBOSE,
} LogLevel_t;

typedef struct LoggerOps Logger_t;

/* Exported functions prototypes ---------------------------------------------*/

void loggerInit(LogLevel_t level);
void loggerSetLevel(LogLevel_t level);

/* 使用elog的宏进行输出elog_i elog_w elog_e elog_d elog_v */

#endif // __LOGGER_H__
