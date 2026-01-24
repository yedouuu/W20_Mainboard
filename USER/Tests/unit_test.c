
#include "unit_test.h"
#include "fault_test.h"
#include "test_sfud.h"
#include "test_bsp_sflash.h"

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
  // RUN_TEST(TEST_SFUD_RunAllTests);
  
  // TEST_SFUD_RunAllTests();
  TEST_SPIM_RunAllTests();
  UNITY_END();

}
