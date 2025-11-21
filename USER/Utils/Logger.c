#include "Logger.h"
#include <stdio.h>
#include <stdarg.h>

#include "SEGGER_RTT.h"

#define LOGGER_TAG "LOG"

/* ------------------ Private Methods ------------------ */
static void _eloggerInit(void);
static void _eloggerSetLevel(LogLevel_t level);
static void _eloggerLog(LogLevel_t level, const char *format, ...);

/**
 * @brief 日志对象的操作接口定义
 */
struct LoggerOps
{
  void (*init)(void);
  void (*setLevel)(LogLevel_t level);
  void (*log)(LogLevel_t level, const char *format, ...);
};


/**
 * @brief 定义并初始化全局唯一的日志对象实例
 */
static const Logger_t g_loggerInstance = {
  .init     = _eloggerInit,
  .setLevel = _eloggerSetLevel,
  .log      = _eloggerLog,
};

/* ------------------ 公共API实现 (Public API) ------------------ */

void loggerInit(LogLevel_t level)
{
  g_loggerInstance.init();
  g_loggerInstance.setLevel(level);
}

void loggerSetLevel(LogLevel_t level)
{
  g_loggerInstance.setLevel(level);
}

/**
 * @brief 获取全局唯一的日志对象实例
 */
// const Logger_t* loggerGetInstance(void)
// {
//   return &g_loggerInstance;
// }

/* ------------------ 私有方法实现 (Private Methods Implementation) ------------------ */

static void _eloggerInit(void)
{
  SEGGER_RTT_Init();
  elog_init();

  elog_set_fmt(ELOG_LVL_ASSERT,  ELOG_FMT_TIME | ELOG_FMT_LVL  | ELOG_FMT_DIR | \
                                 ELOG_FMT_FUNC | ELOG_FMT_LINE);

  elog_set_fmt(ELOG_LVL_ERROR,   ELOG_FMT_TIME | ELOG_FMT_LVL  | ELOG_FMT_DIR | \
                                 ELOG_FMT_FUNC | ELOG_FMT_LINE);

  elog_set_fmt(ELOG_LVL_WARN,    ELOG_FMT_TIME | ELOG_FMT_LVL  | ELOG_FMT_DIR | \
                                 ELOG_FMT_FUNC | ELOG_FMT_LINE);

  elog_set_fmt(ELOG_LVL_INFO,    ELOG_FMT_TIME | ELOG_FMT_LVL  | \
                                 ELOG_FMT_FUNC | ELOG_FMT_LINE);

  elog_set_fmt(ELOG_LVL_DEBUG,   ELOG_FMT_TIME | ELOG_FMT_LVL  | \
                                 ELOG_FMT_FUNC | ELOG_FMT_LINE);

  elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_TIME | ELOG_FMT_LVL  | ELOG_FMT_DIR | \
                                 ELOG_FMT_FUNC | ELOG_FMT_LINE);

  elog_start();
}

static void _eloggerSetLevel(LogLevel_t level)
{
  switch(level)
  {
    case LOG_LEVEL_ASSERT:  elog_set_filter_lvl(ELOG_LVL_ASSERT);  break;
    case LOG_LEVEL_ERROR:   elog_set_filter_lvl(ELOG_LVL_ERROR);   break;
    case LOG_LEVEL_WARN:    elog_set_filter_lvl(ELOG_LVL_WARN);    break;
    case LOG_LEVEL_INFO:    elog_set_filter_lvl(ELOG_LVL_INFO);    break;
    case LOG_LEVEL_DEBUG:   elog_set_filter_lvl(ELOG_LVL_DEBUG);   break;
    case LOG_LEVEL_VERBOSE: elog_set_filter_lvl(ELOG_LVL_VERBOSE); break;
    default:                elog_set_filter_lvl(ELOG_LVL_INFO);    break;
  }
}

static void _eloggerLog(LogLevel_t level, const char *format, ...)
{
  // va_list args;
  // va_start(args, format);

  // switch (level)
  // {
  //   case LOG_LEVEL_ERROR:   elog_output(LOG_LEVEL_ERROR,   LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, format, args); break;
  //   case LOG_LEVEL_WARN:    elog_output(LOG_LEVEL_WARN,    LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, format, args); break;
  //   case LOG_LEVEL_INFO:    elog_output(LOG_LEVEL_INFO,    LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, format, args); break;
  //   case LOG_LEVEL_DEBUG:   elog_output(LOG_LEVEL_DEBUG,   LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, format, args); break;
  //   case LOG_LEVEL_VERBOSE: elog_output(LOG_LEVEL_VERBOSE, LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, format, args); break;
  //   default: break;
  // }

  // va_end(args);
}


