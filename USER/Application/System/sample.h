

#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "Common/common.h"
#include "HAL/drv_wrapper.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * 数据采样模块设计
 *
 * 1. 多张纸币数据缓存区：在连续点钞时，需要缓存多张纸币的数据，以便后续处理
 * 2. 判断分张信号：用来区分不同纸币的数据单元
 * 3. 数据单元设计：
 *    每张纸币需要保存码盘计数、原始传感器值和转换后的电压值
 *    在每个码盘中断中，增加码盘计数，同时多次采样传感器数据，保存到当前纸币的数据单元中
 *
 */

typedef enum
{
  SIGNAL_TYPE_HOP = 0,
  SIGNAL_TYPE_STA,
  SIGNAL_TYPE_IRR,
  SIGNAL_TYPE_IRL,
  SIGNAL_TYPE_CP1, // Channel Point 1
  SIGNAL_TYPE_CP2, // Channel Point 2
  SIGNAL_TYPE_MGB,
  SIGNAL_TYPE_HD,
  SIGNAL_TYPE_LMGS,
  SIGNAL_TYPE_RMGS,
  SIGNAL_TYPE_UV_U,
  SIGNAL_TYPE_UV_D,
  SIGNAL_TYPE_MAX
} Signal_Type_e;

typedef struct
{
  Signal_Type_e type;
  uint8_t       adc_channel;
} Sample_Sensor_Conf_t;

typedef struct
{
  Signal_Type_e type;    // 传感器类型（如：HOP、STA、IRR、IRL等）
  uint16_t      enc_cnt; // 编码器计数
  uint16_t      start_enc;
  uint16_t      end_enc;
  uint16_t      brake_enc; // 刹车位置的编码器计数
  uint16_t      raw_value; // 原始传感器值
  float         voltage;   // 转换后的电压值
} Sample_Data_Unit_t;

typedef struct
{
  Sample_Data_Unit_t data_units[SIGNAL_TYPE_MAX]; // 数据单元数组
  size_t             unit_count;                  // 数据单元数量
} Sample_Data_t;

// typedef enum {
//     SAMPLING_MODE_USER,
//     SAMPLING_MODE_CHECK,
//     SAMPLING_MODE_VIEW,
//     SAMPLING_MODE_STATIC
// } sampling_mode_e;

// typedef struct {
//     sampling_mode_e mode;

//     // 模式处理器（策略模式）
//     sampling_mode_handler_t *handler;

//     // FIFO管理
//     sampling_fifo_t *fifo;

//     // 状态
//     sampling_state_e state;

//     // 回调
//     sampling_callbacks_t callbacks;
// } sampling_engine_t;

// // 接口
// sampling_engine_t* sampling_engine_create(sampling_config_t *config);
// status_t sampling_engine_set_mode(sampling_engine_t *engine, sampling_mode_e
// mode); status_t sampling_engine_start(sampling_engine_t *engine); status_t
// sampling_engine_stop(sampling_engine_t *engine); void
// sampling_engine_process(sampling_engine_t *engine); // 在编码器中断中调用

// // 职责：统一管理sample_buf和数据缓冲区
// typedef struct {
//     uint16_t *raw_buffer;      // 原始DMA缓冲（替代原sample_buf）
//     size_t buffer_size;

//     uint8_t *work_buffer;      // 工作缓冲（替代原save_data）
//     size_t work_size;
//     size_t allocated;
// } sampling_buffer_t;

// sampling_buffer_t* sampling_buffer_create(size_t raw_size, size_t work_size);
// uint16_t sampling_buffer_read_sensor(sampling_buffer_t *buf, signal_type_e
// signal); void* sampling_buffer_alloc(sampling_buffer_t *buf, size_t size);

#if defined(__cplusplus)
}
#endif

#endif /* __SAMPLE_H__ */