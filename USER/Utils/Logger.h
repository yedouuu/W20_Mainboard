#ifndef __LOGGER_H__
#define __LOGGER_H__


/* Includes ----------------------------------------------------------*/
#include <stdint.h>
#include "elog.h"
/* Private defines -----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/

/* 定义日志级别 */
typedef enum {
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


/**
 * @brief 获取全局唯一的日志对象实例
 * @return const Logger_t* 指向日志对象的指针
 */
// const Logger_t* loggerGetInstance(void);


/* ------------------ 应用层推荐使用的宏 ------------------ */

/*
 * 使用示例:
 * LOG_I("Task started successfully, value = %d", 123);
 * LOG_E("Failed to open device: %s", deviceName);
 */
// #define LOG_E(format, ...) loggerGetInstance()->log(LOG_LEVEL_ERROR,   format, ##__VA_ARGS__)
// #define LOG_W(format, ...) loggerGetInstance()->log(LOG_LEVEL_WARN,    format, ##__VA_ARGS__)
// #define LOG_I(format, ...) loggerGetInstance()->log(LOG_LEVEL_INFO,    format, ##__VA_ARGS__)
// #define LOG_D(format, ...) loggerGetInstance()->log(LOG_LEVEL_DEBUG,   format, ##__VA_ARGS__)
// #define LOG_V(format, ...) loggerGetInstance()->log(LOG_LEVEL_VERBOSE, format, ##__VA_ARGS__)
// #define LOG_ERROR(format, ...)   LOG_E(format, ##__VA_ARGS__)
// #define LOG_WARN(format, ...)    LOG_W(format, ##__VA_ARGS__)
// #define LOG_INFO(format, ...)    LOG_I(format, ##__VA_ARGS__)
// #define LOG_DEBUG(format, ...)   LOG_D(format, ##__VA_ARGS__)
// #define LOG_VERBOSE(format, ...) LOG_V(format, ##__VA_ARGS__)
#endif // __LOGGER_H__
