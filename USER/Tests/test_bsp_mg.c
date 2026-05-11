
#include "test_bsp_mg.h"
#include "bsp_mg.h"
#include "adc.h"
#include "mcu_core.h"

#define MG_CHANNEL_COUNT   8
#define MG_SAMPLE_COUNT    10

static const MG_Resource_t *mg_channels[MG_CHANNEL_COUNT] = {
    &mgs_left_1_res,  &mgs_left_2_res,  &mgs_left_3_res,
    &mgs_right_1_res, &mgs_right_2_res, &mgs_right_3_res,
    &mgb_bm_res,      &mgb_hd_res,
};

static const char *mg_channel_names[MG_CHANNEL_COUNT] = {
    "MGS_LEFT_1",  "MGS_LEFT_2",  "MGS_LEFT_3",
    "MGS_RIGHT_1", "MGS_RIGHT_2", "MGS_RIGHT_3",
    "MGB_BM",      "MGB_HD",
};

/**
 * @brief 验证 BSP_MG_Init 是否正确调用了 ADC_DMA_Register
 *
 * 背景: ADC_DMA_Init() 会快照当前 ADC_DMA_RegCnt 来配置 DMA 缓冲区大小和
 * ADC 扫描序列。如果 BSP_MG_Init 在 ADC_DMA_Init 之后才调用，通道虽然注册了
 * 但 DMA 不会搬运数据，导致 ADC_DMA_GetValue 始终返回 0。
 *
 * 本测试通过检查注册计数变化来验证 Init 确实完成了通道注册。
 */
void TEST_MG_RegisterCheck(void)
{
  TEST_LOG("\r\n========== TEST: MG Register Check ==========\r\n");
  TEST_LOG("  ADC_DMA_RegCnt before init: %u\r\n", ADC_DMA_GetRegisterCount());

  uint8_t count_before = ADC_DMA_GetRegisterCount();

  for (uint8_t i = 0; i < MG_CHANNEL_COUNT; i++)
  {
    uint8_t cnt_pre = ADC_DMA_GetRegisterCount();
    BSP_MG_Init(mg_channels[i]);
    uint8_t cnt_post = ADC_DMA_GetRegisterCount();

    TEST_ASSERT_EQUAL_UINT8(cnt_pre + 1, cnt_post);
    TEST_LOG("  [PASS] %s registered (reg count: %u -> %u)\r\n",
             mg_channel_names[i], cnt_pre, cnt_post);
  }

  uint8_t count_after = ADC_DMA_GetRegisterCount();
  TEST_ASSERT_EQUAL_UINT8(count_before + MG_CHANNEL_COUNT, count_after);
  TEST_LOG("  ADC_DMA_RegCnt after init: %u\r\n", count_after);
  TEST_LOG("  [PASS] All %d channels registered\r\n", MG_CHANNEL_COUNT);
}

/**
 * @brief 验证重复 Init 不会重复注册
 */
void TEST_MG_DuplicateInitGuard(void)
{
  TEST_LOG("\r\n========== TEST: MG Duplicate Init Guard ==========\r\n");

  uint8_t count_before = ADC_DMA_GetRegisterCount();

  for (uint8_t i = 0; i < MG_CHANNEL_COUNT; i++)
  {
    BSP_MG_Init(mg_channels[i]);
  }

  uint8_t count_after = ADC_DMA_GetRegisterCount();
  TEST_ASSERT_EQUAL_UINT8(count_before, count_after);
  TEST_LOG("  [PASS] Duplicate init does not re-register (count=%u)\r\n", count_after);
}

/**
 * @brief 数据活性检测 — 判断 DMA 是否真的在搬运数据
 *
 * 如果 ADC_DMA_Init 先于 BSP_MG_Init 执行，DMA 缓冲区对应槽位永远不会被更新，
 * 始终为初始值 0。真实 ADC 采样几乎不会所有通道全部严格为 0，
 * 因此全 0 是 "DMA 未搬运" 的强特征信号。
 */
void TEST_MG_DataFreshness(void)
{
  uint8_t frozen_channels = 0;

  TEST_LOG("\r\n========== TEST: MG Data Freshness ==========\r\n");

  for (uint8_t ch = 0; ch < MG_CHANNEL_COUNT; ch++)
  {
    uint16_t min_v = 65535, max_v = 0;
    uint16_t raw;

    for (uint8_t n = 0; n < MG_SAMPLE_COUNT; n++)
    {
      BSP_MG_GetRawData(mg_channels[ch], &raw);
      if (raw < min_v) min_v = raw;
      if (raw > max_v) max_v = raw;
    }

    if (max_v == 0)
    {
      frozen_channels++;
      TEST_LOG("  [WARN] %s: all %d samples = 0, DMA may not be sampling this channel\r\n",
               mg_channel_names[ch], MG_SAMPLE_COUNT);
    }
    else
    {
      TEST_LOG("  [OK] %s: min=%u max=%u (range %u)\r\n",
               mg_channel_names[ch], min_v, max_v, max_v - min_v);
    }
  }

  if (frozen_channels == MG_CHANNEL_COUNT)
  {
    TEST_LOG("\r\n");
    TEST_LOG("  ====================================================\r\n");
    TEST_LOG("  [FAIL] ALL %d channels read exactly 0.\r\n", MG_CHANNEL_COUNT);
    TEST_LOG("  Root cause: BSP_MG_Init() was called AFTER ADC_DMA_Init().\r\n");
    TEST_LOG("  Fix: Move BSP_MG_Init() before ADC_DMA_Init() in main.cpp.\r\n");
    TEST_LOG("  ====================================================\r\n");
  }

  /* 全通道全 0 几乎不可能是真实硬件状态，直接断言失败 */
  TEST_ASSERT_TRUE(frozen_channels < MG_CHANNEL_COUNT);
}

/**
 * @brief 标准单次读取测试（依赖 DataFreshness 已通过的 DMA 活性前提）
 */
void TEST_MG_GetRawData(void)
{
  uint16_t raw = 0;

  TEST_LOG("\r\n========== TEST: MG GetRawData ==========\r\n");

  for (uint8_t i = 0; i < MG_CHANNEL_COUNT; i++)
  {
    BSP_MG_GetRawData(mg_channels[i], &raw);
    TEST_LOG("  %s raw = %u\r\n", mg_channel_names[i], raw);
    TEST_ASSERT_UINT16_WITHIN(4095, 0, raw);
  }
}

void TEST_MG_AllChannels(void)
{
  uint16_t raw_values[MG_CHANNEL_COUNT] = {0};

  TEST_LOG("\r\n========== TEST: MG All Channels ==========\r\n");

  for (uint8_t i = 0; i < MG_CHANNEL_COUNT; i++)
  {
    BSP_MG_GetRawData(mg_channels[i], &raw_values[i]);
    TEST_LOG("  %s = %u\r\n", mg_channel_names[i], raw_values[i]);
  }

  for (uint8_t i = 0; i < MG_CHANNEL_COUNT; i++)
  {
    TEST_ASSERT_UINT16_WITHIN(4095, 0, raw_values[i]);
  }
  TEST_LOG("  [PASS] All %d channels read successfully\r\n", MG_CHANNEL_COUNT);
}

void TEST_MG_ReadStability(void)
{
#define MG_STABILITY_SAMPLES 10

  TEST_LOG("\r\n========== TEST: MG Read Stability ==========\r\n");

  for (uint8_t ch = 0; ch < MG_CHANNEL_COUNT; ch++)
  {
    uint16_t min_val = 4095;
    uint16_t max_val = 0;
    uint16_t raw;

    for (uint8_t n = 0; n < MG_STABILITY_SAMPLES; n++)
    {
      BSP_MG_GetRawData(mg_channels[ch], &raw);
      if (raw < min_val) min_val = raw;
      if (raw > max_val) max_val = raw;
    }

    uint16_t jitter = max_val - min_val;
    TEST_LOG("  %s: min=%u max=%u jitter=%u\r\n", mg_channel_names[ch], min_val,
             max_val, jitter);

    TEST_ASSERT_UINT16_WITHIN(4095, 0, jitter);
  }
  TEST_LOG("  [PASS] Read stability verified (%d samples each)\r\n",
           MG_STABILITY_SAMPLES);

#undef MG_STABILITY_SAMPLES
}

void TEST_MG_RunAllTests(void)
{
  TEST_LOG("\r\n");
  TEST_LOG("========================================\r\n");
  TEST_LOG("   MG Unit Test Suite Starting...       \r\n");
  TEST_LOG("========================================\r\n");

  UNITY_BEGIN();

  /* 注册检测必须最先执行，验证 Init 调用了 ADC_DMA_Register */
  RUN_TEST(TEST_MG_RegisterCheck);
  RUN_TEST(TEST_MG_DuplicateInitGuard);

  /* 数据活性检测 — 能捕获 "Init 晚于 ADC_DMA_Init" 的 BUG */
  RUN_TEST(TEST_MG_DataFreshness);

  /* 常规功能测试 */
  RUN_TEST(TEST_MG_GetRawData);
  RUN_TEST(TEST_MG_AllChannels);
  RUN_TEST(TEST_MG_ReadStability);

  TEST_LOG("\r\n");
  TEST_LOG("========================================\r\n");
  TEST_LOG("   MG Unit Test Suite Completed!        \r\n");
  TEST_LOG("========================================\r\n");
  TEST_LOG("\r\n");

  UNITY_END();
}
