
#include "sample.h"

static Sample_Data_t  sample_data[4];        // 假设最多缓存4张纸币的数据
static Sample_Data_t *current_sample = NULL; // 当前正在采样的数据

static Device_t *ir_hop = NULL;
static Device_t *ir_sta = NULL;
static Device_t *ir_irr = NULL;
static Device_t *ir_irl = NULL;

static const Sample_Sensor_Conf_t sensors_conf[] = {
    {SIGNAL_TYPE_MGB, 1},
    {SIGNAL_TYPE_HD, 2},
    {SIGNAL_TYPE_HOP, 5},
    {SIGNAL_TYPE_STA, 6},
    {SIGNAL_TYPE_IRR, 7},
    {SIGNAL_TYPE_IRL, 8},
    {SIGNAL_TYPE_RMGS, 12},
    {SIGNAL_TYPE_LMGS, 13},
    {SIGNAL_TYPE_UV_U, 14},
    {SIGNAL_TYPE_UV_D, 15},
};

void Sample_Init(void)
{
  for (size_t i = 0; i < 4; i++)
  {
    if (sample_data[i].data_units == NULL)
    {
      log_e("Failed to allocate memory for sample data units");
      sample_data[i].unit_count = 0;
      return;
    }
  }

  for (size_t i = 0; i < 4; i++)
  {
    sample_data[i].unit_count = SIGNAL_TYPE_MAX;

    // 初始化数据单元
    for (size_t j = 0; j < sample_data[i].unit_count; j++)
    {
      sample_data[i].data_units[j].enc_cnt   = 0;
      sample_data[i].data_units[j].raw_value = 0;
      sample_data[i].data_units[j].voltage   = 0.0f;
    }
  }

  current_sample = &sample_data[0]; // 默认使用第一个数据单元
}


void Sample_StartSampling(void)
{
  // 启动采样逻辑，例如启动定时器或中断
  
}
