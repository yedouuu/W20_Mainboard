
#include "test_bsp_sflash.h"
#include "HAL/drv_wrapper.h"
#include "bsp_sflash.h"

/* Test buffer size */
#define TEST_BUFFER_SIZE 256
#define TEST_PAGE_SIZE   256
#define TEST_SECTOR_SIZE 4096
#define SPIM_BASE_ADDR   SPIM_FLASH_BASE

/* Test patterns */
#define TEST_PATTERN_0x55 0x55
#define TEST_PATTERN_0xAA 0xAA
#define TEST_PATTERN_0xFF 0xFF
#define TEST_PATTERN_0x00 0x00

/* Test buffers */
static uint8_t write_buffer[TEST_BUFFER_SIZE];
static uint8_t read_buffer[TEST_BUFFER_SIZE];
static uint8_t verify_buffer[TEST_BUFFER_SIZE];


/* Test helper functions */
static void fill_buffer(uint8_t *buffer, size_t size, uint8_t pattern)
{
  for (size_t i = 0; i < size; i++) { buffer[i] = pattern; }
}

static void fill_buffer_incremental(uint8_t *buffer, size_t size)
{
  for (size_t i = 0; i < size; i++) { buffer[i] = (uint8_t)i; }
}

static bool compare_buffer(const uint8_t *buf1,
                           const uint8_t *buf2,
                           size_t         size)
{
  return (memcmp(buf1, buf2, size) == 0);
}


void TEST_SPIM_Init(void)
{
  BSP_SFlash_Init();
}

void TEST_SPIM_EraseWriteRead(void)
{
  SFlash_Status_e result;
  uint32_t test_addr = SPIM_BASE_ADDR;

  printf("\r\n========== TEST: SPIM Erase/Write/Read ==========\r\n");

  /* Test 1: Erase sector */
  printf("Test 1: Erase sector at address 0x%08X...\r\n", test_addr);
  result = BSP_SFlash_ErasePage(test_addr, TEST_SECTOR_SIZE);
  TEST_ASSERT_EQUAL(SFLASH_SUCC, result);
  printf("  [PASS] Sector erased successfully\r\n");

  /* Test 2: Read after erase (should be 0xFF) */
  printf("Test 2: Read after erase...\r\n");
  memset(read_buffer, 0x00, TEST_BUFFER_SIZE);
  result = BSP_SFlash_Read(read_buffer, test_addr, TEST_BUFFER_SIZE);
  TEST_ASSERT_EQUAL(SFLASH_SUCC, result);

  fill_buffer(verify_buffer, TEST_BUFFER_SIZE, 0xFF);
  TEST_ASSERT_TRUE(
      compare_buffer(read_buffer, verify_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Flash erased to 0xFF\r\n");

  /* Test 3: Write pattern 0x55 */
  printf("Test 3: Write pattern 0x55...\r\n");
  fill_buffer(write_buffer, TEST_BUFFER_SIZE, TEST_PATTERN_0x55);
  result = BSP_SFlash_Write(write_buffer, test_addr, TEST_BUFFER_SIZE);
  TEST_ASSERT_EQUAL(SFLASH_SUCC, result);
  printf("  [PASS] Pattern 0x55 written\r\n");

  /* Test 4: Read and verify pattern 0x55 */
  printf("Test 4: Read and verify pattern 0x55...\r\n");
  memset(read_buffer, 0x00, TEST_BUFFER_SIZE);
  result = BSP_SFlash_Read(read_buffer, test_addr, TEST_BUFFER_SIZE);
  TEST_ASSERT_EQUAL(SFLASH_SUCC, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Pattern 0x55 verified\r\n");
}

/**
 * @brief Test SPIM performance
 */
void TEST_SPIM_Performance(void) 
{
  SFlash_Status_e result;
  uint32_t test_addr = SPIM_BASE_ADDR + TEST_SECTOR_SIZE * 5;
  uint32_t start_time, end_time, elapsed;

  printf("\r\n========== TEST: SPIM Performance ==========\r\n");
  /* Test 1: Erase performance */
  printf("Test 1: Erase performance test...\r\n");
  start_time = DRV_GetMillis(); // Use your timer function here
  result = BSP_SFlash_ErasePage(test_addr, TEST_SECTOR_SIZE);
  end_time   = DRV_GetMillis(); // Use your timer function here
  elapsed    = end_time - start_time;
  TEST_ASSERT_EQUAL(SFLASH_SUCC, result);
  printf("[PASS] Erase %dB completed in %u ms\r\n", TEST_SECTOR_SIZE, elapsed);

  /* Test 2: Write performance */
  printf("Test 2: Write performance test...\r\n");
  fill_buffer_incremental(write_buffer, TEST_BUFFER_SIZE);
  start_time = DRV_GetMillis(); // Use your timer function here
  result = BSP_SFlash_Write(write_buffer, test_addr, TEST_BUFFER_SIZE);
  end_time = DRV_GetMillis(); // Use your timer function here
  elapsed  = end_time - start_time;
  TEST_ASSERT_EQUAL(SFLASH_SUCC, result);
  printf("[PASS] Write %dB completed in %u ms\r\n", TEST_BUFFER_SIZE, elapsed);

  /* Test 3: Read performance */
  printf("Test 3: Read performance test...\r\n");
  start_time = DRV_GetMillis(); // Use your timer function here
  result = BSP_SFlash_Read(read_buffer, test_addr, TEST_BUFFER_SIZE);
  end_time = DRV_GetMillis(); // Use your timer function here
  elapsed  = end_time - start_time;
  TEST_ASSERT_EQUAL(SFLASH_SUCC, result);
  printf("[PASS] Read %dB completed in %u ms\r\n", TEST_BUFFER_SIZE, elapsed);
}

void TEST_SPIM_RunAllTests(void)
{
  printf("\r\n========== TEST: SPIM Initialization ==========\r\n");

  /* Test 1: Initialize SPIM flash */
  printf("Test 1: Initialize SPIM flash...\r\n");
  RUN_TEST(TEST_SPIM_Init);
  RUN_TEST(TEST_SPIM_EraseWriteRead);
  RUN_TEST(TEST_SPIM_Performance);
  printf("  [PASS] SPIM flash initialized successfully\r\n");
}

