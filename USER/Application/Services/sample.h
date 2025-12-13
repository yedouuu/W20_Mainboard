
#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "Common/common.h"


typedef enum {
    SAMPLING_MODE_USER,
    SAMPLING_MODE_CHECK,
    SAMPLING_MODE_VIEW,
    SAMPLING_MODE_STATIC
} sampling_mode_e;

typedef struct {
    sampling_mode_e mode;
    
    // 模式处理器（策略模式）
    sampling_mode_handler_t *handler;
    
    // FIFO管理
    sampling_fifo_t *fifo;
    
    // 状态
    sampling_state_e state;
    
    // 回调
    sampling_callbacks_t callbacks;
} sampling_engine_t;

// 接口
sampling_engine_t* sampling_engine_create(sampling_config_t *config);
status_t sampling_engine_set_mode(sampling_engine_t *engine, sampling_mode_e mode);
status_t sampling_engine_start(sampling_engine_t *engine);
status_t sampling_engine_stop(sampling_engine_t *engine);
void sampling_engine_process(sampling_engine_t *engine); // 在编码器中断中调用


// 职责：统一管理sample_buf和数据缓冲区
typedef struct {
    uint16_t *raw_buffer;      // 原始DMA缓冲（替代原sample_buf）
    size_t buffer_size;
    
    uint8_t *work_buffer;      // 工作缓冲（替代原save_data）
    size_t work_size;
    size_t allocated;
} sampling_buffer_t;

sampling_buffer_t* sampling_buffer_create(size_t raw_size, size_t work_size);
uint16_t sampling_buffer_read_sensor(sampling_buffer_t *buf, signal_type_e signal);
void* sampling_buffer_alloc(sampling_buffer_t *buf, size_t size);


#endif /* __SAMPLE_H__ */