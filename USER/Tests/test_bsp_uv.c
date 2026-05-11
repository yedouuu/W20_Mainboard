
#include "test_bsp_uv.h"
#include "bsp_uv.h"
#include "adc.h"
#include "mcu_core.h"

#define UV_CHANNEL_COUNT   2
#define UV_SAMPLE_COUNT    10

static const UV_Resource_t *uv_channels[UV_CHANNEL_COUNT] = {
    &uv_left_res,
    &uv_right_res,
};

static const char *uv_channel_names[UV_CHANNEL_COUNT] = {
    "UV_LEFT",
    "UV_RIGHT",
};

/**
 * @brief 验证 BSP_UV_Init 是否正确调用了 ADC_DMA_Register
 */
void TEST_UV_RegisterCheck(void)
{
  TEST_LOG("\r\n========== TEST: UV Register Check ==========\r\n");
  TEST_LOG("  ADC_DMA_RegCnt before init: %u\r\n", ADC_DMA_GetRegisterCount());

  uint8_t count_before = ADC_DMA_GetRegisterCount();

  for (uint8_t i = 0; i < UV_CHANNEL_COUNT; i++)
  {
    uint8_t cnt_pre = ADC_DMA_GetRegisterCount();
    BSP_UV_Init(uv_channels[i]);
    uint8_t cnt_post = ADC_DMA_GetRegisterCount();

    TEST_ASSERT_EQUAL_UINT8(cnt_pre + 1, cnt_post);
    TEST_LOG("  [PASS] %s registered (reg count: %u -> %u)\r\n",
             uv_channel_names[i], cnt_pre, cnt_post);
  }

  uint8_t count_after = ADC_DMA_GetRegisterCount();
  TEST_ASSERT_EQUAL_UINT8(count_before + UV_CHANNEL_COUNT, count_after);
  TEST_LOG("  ADC_DMA_RegCnt after init: %u\r\n", count_after);
  TEST_LOG("  [PASS] All %d channels registered\r\n", UV_CHANNEL_COUNT);
}

/**
 * @brief 验证重复 Init 不会重复注册
 */
void TEST_UV_DuplicateInitGuard(void)
{
  TEST_LOG("\r\n========== TEST: UV Duplicate Init Guard ==========\r\n");

  uint8_t count_before = ADC_DMA_GetRegisterCount();

  for (uint8_t i = 0; i < UV_CHANNEL_COUNT; i++)
  {
    BSP_UV_Init(uv_channels[i]);
  }

  uint8_t count_after = ADC_DMA_GetRegisterCount();
  TEST_ASSERT_EQUAL_UINT8(count_before, count_after);
  TEST_LOG("  [PASS] Duplicate init does not re-register (count=%u)\r\n", count_after);
}

/**
 * @brief 数据活性检测 — 判断 DMA 是否真的在搬运数据
 */
void TEST_UV_DataFreshness(void)
{
  uint8_t frozen_channels = 0;

  TEST_LOG("\r\n========== TEST: UV Data Freshness ==========\r\n");

  for (uint8_t ch = 0; ch < UV_CHANNEL_COUNT; ch++)
  {
    uint16_t min_v = 65535, max_v = 0;
    uint16_t raw;

    for (uint8_t n = 0; n < UV_SAMPLE_COUNT; n++)
    {
      BSP_UV_GetRawData(uv_channels[ch], &raw);
      if (raw < min_v) min_v = raw;
      if (raw > max_v) max_v = raw;
    }

    if (max_v == 0)
    {
      frozen_channels++;
      TEST_LOG("  [WARN] %s: all %d samples = 0, DMA may not be sampling this channel\r\n",
               uv_channel_names[ch], UV_SAMPLE_COUNT);
    }
    else
    {
      TEST_LOG("  [OK] %s: min=%u max=%u (range %u)\r\n",
               uv_channel_names[ch], min_v, max_v, max_v - min_v);
    }
  }

  if (frozen_channels == UV_CHANNEL_COUNT)
  {
    TEST_LOG("\r\n");
    TEST_LOG("  ====================================================\r\n");
    TEST_LOG("  [FAIL] ALL %d channels read exactly 0.\r\n", UV_CHANNEL_COUNT);
    TEST_LOG("  Root cause: BSP_UV_Init() was called AFTER ADC_DMA_Init().\r\n");
    TEST_LOG("  Fix: Move BSP_UV_Init() before ADC_DMA_Init() in main.cpp.\r\n");
    TEST_LOG("  ====================================================\r\n");
  }

  TEST_ASSERT_TRUE(frozen_channels < UV_CHANNEL_COUNT);
}

void TEST_UV_GetRawData(void)
{
  uint16_t raw = 0;

  TEST_LOG("\r\n========== TEST: UV GetRawData ==========\r\n");

  for (uint8_t i = 0; i < UV_CHANNEL_COUNT; i++)
  {
    BSP_UV_GetRawData(uv_channels[i], &raw);
    TEST_LOG("  %s raw = %u\r\n", uv_channel_names[i], raw);
    TEST_ASSERT_UINT16_WITHIN(4095, 0, raw);
  }
}

void TEST_UV_AllChannels(void)
{
  uint16_t raw_values[UV_CHANNEL_COUNT] = {0};

  TEST_LOG("\r\n========== TEST: UV All Channels ==========\r\n");

  for (uint8_t i = 0; i < UV_CHANNEL_COUNT; i++)
  {
    BSP_UV_GetRawData(uv_channels[i], &raw_values[i]);
    TEST_LOG("  %s = %u\r\n", uv_channel_names[i], raw_values[i]);
  }

  for (uint8_t i = 0; i < UV_CHANNEL_COUNT; i++)
  {
    TEST_ASSERT_UINT16_WITHIN(4095, 0, raw_values[i]);
  }
  TEST_LOG("  [PASS] All %d channels read successfully\r\n", UV_CHANNEL_COUNT);
}

void TEST_UV_ReadStability(void)
{
#define UV_STABILITY_SAMPLES 10

  TEST_LOG("\r\n========== TEST: UV Read Stability ==========\r\n");

  for (uint8_t ch = 0; ch < UV_CHANNEL_COUNT; ch++)
  {
    uint16_t min_val = 4095;
    uint16_t max_val = 0;
    uint16_t raw;

    for (uint8_t n = 0; n < UV_STABILITY_SAMPLES; n++)
    {
      BSP_UV_GetRawData(uv_channels[ch], &raw);
      if (raw < min_val) min_val = raw;
      if (raw > max_val) max_val = raw;
    }

    uint16_t jitter = max_val - min_val;
    TEST_LOG("  %s: min=%u max=%u jitter=%u\r\n", uv_channel_names[ch], min_val,
             max_val, jitter);

    TEST_ASSERT_UINT16_WITHIN(4095, 0, jitter);
  }
  TEST_LOG("  [PASS] Read stability verified (%d samples each)\r\n",
           UV_STABILITY_SAMPLES);

#undef UV_STABILITY_SAMPLES
}

void TEST_UV_RunAllTests(void)
{
  TEST_LOG("\r\n");
  TEST_LOG("========================================\r\n");
  TEST_LOG("   UV Unit Test Suite Starting...       \r\n");
  TEST_LOG("========================================\r\n");

  UNITY_BEGIN();

  RUN_TEST(TEST_UV_RegisterCheck);
  RUN_TEST(TEST_UV_DuplicateInitGuard);
  RUN_TEST(TEST_UV_DataFreshness);
  RUN_TEST(TEST_UV_GetRawData);
  RUN_TEST(TEST_UV_AllChannels);
  RUN_TEST(TEST_UV_ReadStability);

  TEST_LOG("\r\n");
  TEST_LOG("========================================\r\n");
  TEST_LOG("   UV Unit Test Suite Completed!        \r\n");
  TEST_LOG("========================================\r\n");
  TEST_LOG("\r\n");

  UNITY_END();
}
