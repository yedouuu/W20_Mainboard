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

/* 使用elog的宏进行输出elog_i elog_w elog_e elog_d elog_v */

#endif // __LOGGER_H__
