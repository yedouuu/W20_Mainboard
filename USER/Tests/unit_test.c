
/* Unit test switches: 1 = enable, 0 = disable */
#define UNIT_TEST_MG_ENABLE    1
#define UNIT_TEST_UV_ENABLE    1
#define UNIT_TEST_SPIM_ENABLE  0
#define UNIT_TEST_SFUD_ENABLE  0

#include "unit_test.h"
#include "fault_test.h"

#if UNIT_TEST_SFUD_ENABLE
#  include "test_sfud.h"
#endif

#if UNIT_TEST_SPIM_ENABLE
#  include "test_bsp_sflash.h"
#endif

#if UNIT_TEST_MG_ENABLE
#  include "test_bsp_mg.h"
#endif

#if UNIT_TEST_UV_ENABLE
#  include "test_bsp_uv.h"
#endif

void setUp(void) {

}

void tearDown(void) {

}

int32_t divide_func(int32_t a, int32_t b)
{
  if (b == 0) {
    return -1; // 或者其他错误处理
  }
  return a / b;
}


void test_func(void) {
  int32_t result = divide_func(10, 2);
  TEST_ASSERT_EQUAL_INT32(5, result);
}

void test_divide_by_zero(void) {
  int32_t result = divide_func(10, 0);
  TEST_ASSERT_EQUAL_INT32(-2, result);
}

void TEST_main(void)
{
  UNITY_BEGIN();

#if UNIT_TEST_SFUD_ENABLE
  TEST_SFUD_RunAllTests();
#endif

#if UNIT_TEST_SPIM_ENABLE
  TEST_SPIM_RunAllTests();
#endif

#if UNIT_TEST_MG_ENABLE
  TEST_MG_RunAllTests();
#endif

#if UNIT_TEST_UV_ENABLE
  TEST_UV_RunAllTests();
#endif

  UNITY_END();

}
