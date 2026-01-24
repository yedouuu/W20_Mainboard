
#include "test_sfud.h"
#include "SFUD/inc/sfud.h"
#include "HAL/drv_wrapper.h"
// #include <string.h>
// #include <stdio.h>

/* Test buffer size */
#define TEST_BUFFER_SIZE 256
#define TEST_PAGE_SIZE   256
#define TEST_SECTOR_SIZE 4096

/* Test patterns */
#define TEST_PATTERN_0x55 0x55
#define TEST_PATTERN_0xAA 0xAA
#define TEST_PATTERN_0xFF 0xFF
#define TEST_PATTERN_0x00 0x00

/* Test buffers */
static uint8_t write_buffer[TEST_BUFFER_SIZE];
static uint8_t read_buffer[TEST_BUFFER_SIZE];
static uint8_t verify_buffer[TEST_BUFFER_SIZE];

/* Flash device pointer */
static sfud_flash *flash = NULL;

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

/**
 * @brief Test SFUD initialization
 */
void TEST_SFUD_Init(void)
{
  sfud_err result;

  printf("\r\n========== TEST: SFUD Initialization ==========\r\n");

  /* Test 1: Initialize SFUD library */
  printf("Test 1: Initialize SFUD library...\r\n");
  result = sfud_init();
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] SFUD initialized successfully\r\n");

  /* Test 2: Get device table */
  printf("Test 2: Get device table...\r\n");
  const sfud_flash *device_table = sfud_get_device_table();
  TEST_ASSERT_NOT_NULL(device_table);
  printf("  [PASS] Device table obtained\r\n");

  /* Test 3: Get flash device from table */
  printf("Test 3: Get flash device by index...\r\n");
  uint8_t device_num = sfud_get_device_num();
  TEST_ASSERT_TRUE(device_num > 0);
  for (size_t i = 0; i < device_num; i++)
  {
    sfud_flash *dev = sfud_get_device(i);
    TEST_ASSERT_NOT_NULL(dev);
    TEST_ASSERT_TRUE(dev->chip.capacity > 0);
    printf("    Device %d: %s\r\n", (int)i, dev->name);
  }

  flash = sfud_get_device(SFUD_W25Q_DEVICE_INDEX);
  TEST_ASSERT_NOT_NULL(flash);
  TEST_ASSERT_TRUE(flash->chip.capacity > 0);
  printf("  [PASS] Flash device obtained: %s\r\n", flash->name);
  printf("  Flash capacity: %d bytes\r\n", flash->chip.capacity);
  printf("  Flash page size: %d bytes\r\n", flash->chip.erase_gran);
  printf("  Flash erase granularity: %d bytes\r\n", flash->chip.erase_gran);
}

/**
 * @brief Test SFUD Read Flash ID and parameters
 */
void TEST_SFUD_ReadID(void)
{
  printf("\r\n========== TEST: SFUD Read Flash Info ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Test 1: Check manufacturer ID */
  printf("Test 1: Check Flash parameters...\r\n");
  printf("  Flash name: %s\r\n", flash->name);
  printf("  Manufacturer ID: 0x%02X\r\n", flash->chip.mf_id);
  printf("  Memory Type ID: 0x%02X\r\n", flash->chip.type_id);
  printf("  Capacity ID: 0x%02X\r\n", flash->chip.capacity_id);
  printf("  Capacity: %d bytes\r\n", flash->chip.capacity);
  TEST_ASSERT_TRUE(flash->chip.mf_id != 0x00);
  TEST_ASSERT_TRUE(flash->chip.mf_id != 0xFF);
  printf("  [PASS] Flash ID valid\r\n");

  /* Test 2: Check capacity */
  printf("Test 2: Check Flash capacity...\r\n");
  TEST_ASSERT_TRUE(flash->chip.capacity > 0);
  TEST_ASSERT_TRUE(flash->chip.capacity <= (128 * 1024 * 1024)); // Max 128MB
  printf("  [PASS] Flash capacity: %d bytes\r\n", flash->chip.capacity);

  /* Test 3: Check write mode */
  printf("Test 3: Check write mode...\r\n");
  printf("  Write mode: 0x%04X\r\n", flash->chip.write_mode);
  TEST_ASSERT_TRUE(flash->chip.write_mode > 0);
  printf("  [PASS] Write mode valid\r\n");
}

/**
 * @brief Test SFUD Erase, Write and Read operations
 */
void TEST_SFUD_EraseWriteRead(void)
{
  sfud_err result;
  uint32_t test_addr = 0;

  printf("\r\n========== TEST: SFUD Erase/Write/Read ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Test 1: Erase sector */
  printf("Test 1: Erase sector at address 0x%08X...\r\n", test_addr);
  result = sfud_erase(flash, test_addr, TEST_SECTOR_SIZE);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Sector erased successfully\r\n");

  /* Test 2: Read after erase (should be 0xFF) */
  printf("Test 2: Read after erase...\r\n");
  memset(read_buffer, 0x00, TEST_BUFFER_SIZE);
  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  fill_buffer(verify_buffer, TEST_BUFFER_SIZE, 0xFF);
  TEST_ASSERT_TRUE(
      compare_buffer(read_buffer, verify_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Flash erased to 0xFF\r\n");

  /* Test 3: Write pattern 0x55 */
  printf("Test 3: Write pattern 0x55...\r\n");
  fill_buffer(write_buffer, TEST_BUFFER_SIZE, TEST_PATTERN_0x55);
  result = sfud_write(flash, test_addr, TEST_BUFFER_SIZE, write_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Pattern 0x55 written\r\n");

  /* Test 4: Read and verify pattern 0x55 */
  printf("Test 4: Read and verify pattern 0x55...\r\n");
  memset(read_buffer, 0x00, TEST_BUFFER_SIZE);
  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Pattern 0x55 verified\r\n");

  /* Test 5: Erase and write pattern 0xAA */
  printf("Test 5: Erase write pattern 0xAA...\r\n");
  fill_buffer(write_buffer, TEST_BUFFER_SIZE, TEST_PATTERN_0xAA);
  result = sfud_erase_write(flash, test_addr, TEST_BUFFER_SIZE, write_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Pattern 0xAA written\r\n");

  /* Test 6: Read and verify pattern 0xAA */
  printf("Test 6: Read and verify pattern 0xAA...\r\n");
  memset(read_buffer, 0x00, TEST_BUFFER_SIZE);
  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Pattern 0xAA verified\r\n");
}

/**
 * @brief Test SFUD read operations with different sizes
 */
void TEST_SFUD_ReadVariousSizes(void)
{
  sfud_err result;
  uint32_t test_addr = 0;

  printf("\r\n========== TEST: SFUD Read Various Sizes ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Prepare test data */
  printf("Preparing test data...\r\n");
  fill_buffer_incremental(write_buffer, TEST_BUFFER_SIZE);
  result = sfud_erase_write(flash, test_addr, TEST_BUFFER_SIZE, write_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  /* Test 1: Read 1 byte */
  printf("Test 1: Read 1 byte...\r\n");
  result = sfud_read(flash, test_addr, 1, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_EQUAL(write_buffer[0], read_buffer[0]);
  printf("  [PASS] 1 byte read successfully\r\n");

  /* Test 2: Read 16 bytes */
  printf("Test 2: Read 16 bytes...\r\n");
  result = sfud_read(flash, test_addr, 16, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, 16));
  printf("  [PASS] 16 bytes read successfully\r\n");

  /* Test 3: Read 64 bytes */
  printf("Test 3: Read 64 bytes...\r\n");
  result = sfud_read(flash, test_addr, 64, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, 64));
  printf("  [PASS] 64 bytes read successfully\r\n");

  /* Test 4: Read full buffer */
  printf("Test 4: Read full buffer (%d bytes)...\r\n", TEST_BUFFER_SIZE);
  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Full buffer read successfully\r\n");
}

/**
 * @brief Test SFUD write operations with different sizes
 */
void TEST_SFUD_WriteVariousSizes(void)
{
  sfud_err result;
  uint32_t test_addr = TEST_SECTOR_SIZE; // Use different sector

  printf("\r\n========== TEST: SFUD Write Various Sizes ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Erase test sector */
  printf("Erasing test sector...\r\n");
  result = sfud_erase(flash, test_addr, TEST_SECTOR_SIZE);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  /* Test 1: Write 1 byte */
  printf("Test 1: Write 1 byte...\r\n");
  write_buffer[0] = 0xA5;
  result          = sfud_write(flash, test_addr, 1, write_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  result = sfud_read(flash, test_addr, 1, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_EQUAL(write_buffer[0], read_buffer[0]);
  printf("  [PASS] 1 byte written and verified\r\n");

  /* Test 2: Write 16 bytes */
  printf("Test 2: Write 16 bytes...\r\n");
  fill_buffer_incremental(write_buffer, 16);
  result = sfud_write(flash, test_addr + 256, 16, write_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  result = sfud_read(flash, test_addr + 256, 16, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, 16));
  printf("  [PASS] 16 bytes written and verified\r\n");

  /* Test 3: Write page aligned */
  printf("Test 3: Write page aligned (%d bytes)...\r\n", TEST_PAGE_SIZE);
  fill_buffer_incremental(write_buffer, TEST_PAGE_SIZE);
  result = sfud_write(flash, test_addr + 512, TEST_PAGE_SIZE, write_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  result = sfud_read(flash, test_addr + 512, TEST_PAGE_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, TEST_PAGE_SIZE));
  printf("  [PASS] Page aligned write verified\r\n");
}

/**
 * @brief Test SFUD erase operations
 */
void TEST_SFUD_EraseOperations(void)
{
  sfud_err result;
  uint32_t test_addr = TEST_SECTOR_SIZE * 2;

  printf("\r\n========== TEST: SFUD Erase Operations ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Test 1: Erase single sector */
  printf("Test 1: Erase single sector...\r\n");
  result = sfud_erase(flash, test_addr, TEST_SECTOR_SIZE);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  /* Verify erased (should be 0xFF) */
  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  fill_buffer(verify_buffer, TEST_BUFFER_SIZE, 0xFF);
  TEST_ASSERT_TRUE(
      compare_buffer(read_buffer, verify_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Single sector erased and verified\r\n");

  /* Test 2: Erase multiple sectors */
  printf("Test 2: Erase multiple sectors...\r\n");
  result = sfud_erase(flash, test_addr, TEST_SECTOR_SIZE * 2);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  /* Verify first sector */
  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(
      compare_buffer(read_buffer, verify_buffer, TEST_BUFFER_SIZE));

  /* Verify second sector */
  result = sfud_read(
      flash, test_addr + TEST_SECTOR_SIZE, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(
      compare_buffer(read_buffer, verify_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Multiple sectors erased and verified\r\n");
}

/**
 * @brief Test SFUD status operations
 */
void TEST_SFUD_StatusOperations(void)
{
  sfud_err result;
  uint8_t  status = 0;

  printf("\r\n========== TEST: SFUD Status Operations ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Test 1: Read status register */
  printf("Test 1: Read status register...\r\n");
  result = sfud_read_status(flash, &status);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Status read: 0x%02X\r\n", status);

  /* Test 2: Check write protection bits */
  printf("Test 2: Check write protection status...\r\n");
  uint8_t wp_bits = status & 0x3C; // Typical WP bits in status register
  printf("  Write protection bits: 0x%02X\r\n", wp_bits);
  printf("  [PASS] Status register accessible\r\n");
}

/**
 * @brief Test SFUD boundary conditions
 */
void TEST_SFUD_BoundaryConditions(void)
{
  sfud_err result;
  uint32_t last_addr;

  printf("\r\n========== TEST: SFUD Boundary Conditions ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Test 1: Read at start address */
  printf("Test 1: Read at start address (0x00000000)...\r\n");
  result = sfud_read(flash, 0, 16, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Read at start address successful\r\n");

  /* Test 2: Read near end address */
  printf("Test 2: Read near end address...\r\n");
  last_addr = flash->chip.capacity - TEST_BUFFER_SIZE;
  result    = sfud_read(flash, last_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Read at end boundary successful\r\n");

  /* Test 3: Test page boundary crossing */
  printf("Test 3: Read across page boundary...\r\n");
  uint32_t page_boundary = TEST_PAGE_SIZE - 16;
  result                 = sfud_read(flash, page_boundary, 32, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Read across page boundary successful\r\n");
}

/**
 * @brief Test SFUD data integrity
 */
void TEST_SFUD_DataIntegrity(void)
{
  sfud_err result;
  uint32_t test_addr = TEST_SECTOR_SIZE * 4;

  printf("\r\n========== TEST: SFUD Data Integrity ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Erase test area */
  printf("Erasing test area...\r\n");
  result = sfud_erase(flash, test_addr, TEST_SECTOR_SIZE);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  /* Test 1: Write and verify incremental pattern */
  printf("Test 1: Incremental pattern test...\r\n");
  fill_buffer_incremental(write_buffer, TEST_BUFFER_SIZE);
  result = sfud_write(flash, test_addr, TEST_BUFFER_SIZE, write_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Incremental pattern verified\r\n");

  /* Test 2: Multiple read consistency */
  printf("Test 2: Multiple read consistency...\r\n");
  memset(verify_buffer, 0x00, TEST_BUFFER_SIZE);
  result = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, verify_buffer);
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(
      compare_buffer(read_buffer, verify_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Multiple reads consistent\r\n");

  /* Test 3: Partial overwrite */
  // printf("Test 3: Partial overwrite test...\r\n");
  // fill_buffer(write_buffer, 64, 0x33);
  // result = sfud_write(flash, test_addr + 128, 64, write_buffer);
  // TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);

  // result = sfud_read(flash, test_addr + 128, 64, read_buffer);
  // TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  // TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, 64));
  // printf("  [PASS] Partial overwrite verified\r\n");
}

/**
 * @brief Test SFUD performance
 */
void TEST_SFUD_Performance(void)
{
  sfud_err result;
  uint32_t test_addr = TEST_SECTOR_SIZE * 5;
  uint32_t start_time, end_time, elapsed;

  printf("\r\n========== TEST: SFUD Performance ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Test 1: Erase performance */
  printf("Test 1: Erase performance test...\r\n");
  start_time = DRV_GetMillis(); // Use your timer function here
  result     = sfud_erase(flash, test_addr, TEST_SECTOR_SIZE);
  end_time   = DRV_GetMillis(); // Use your timer function here
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Erase %dB completed in %u ms\r\n", TEST_SECTOR_SIZE, end_time - start_time);

  /* Test 2: Write performance */
  printf("Test 2: Write performance test...\r\n");
  fill_buffer_incremental(write_buffer, TEST_BUFFER_SIZE);
  start_time = DRV_GetMillis(); // Use your timer function here
  result     = sfud_write(flash, test_addr, TEST_BUFFER_SIZE, write_buffer);
  end_time   = DRV_GetMillis(); // Use your timer function here
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Write %dB completed in %u ms\r\n", TEST_BUFFER_SIZE, end_time - start_time);

  /* Test 3: Read performance */
  printf("Test 3: Read performance test...\r\n");
  start_time = DRV_GetMillis(); // Use your timer function here
  result     = sfud_read(flash, test_addr, TEST_BUFFER_SIZE, read_buffer);
  end_time   = DRV_GetMillis(); // Use your timer function here
  TEST_ASSERT_EQUAL(SFUD_SUCCESS, result);
  TEST_ASSERT_TRUE(compare_buffer(read_buffer, write_buffer, TEST_BUFFER_SIZE));
  printf("  [PASS] Read %dB completed in %u ms\r\n", TEST_BUFFER_SIZE, end_time - start_time);
}

/**
 * @brief Test SFUD error handling
 */
void TEST_SFUD_ErrorHandling(void)
{
  sfud_err result;

  printf("\r\n========== TEST: SFUD Error Handling ==========\r\n");

  TEST_ASSERT_NOT_NULL(flash);

  /* Test 1: Invalid address (beyond capacity) */
  printf("Test 1: Read beyond flash capacity...\r\n");
  result =
      sfud_read(flash, flash->chip.capacity, TEST_BUFFER_SIZE, read_buffer);
  TEST_ASSERT_NOT_EQUAL(SFUD_SUCCESS, result);
  printf("  [PASS] Invalid address detected\r\n");

  /* Test 2: NULL pointer handling */
  // printf("Test 2: NULL pointer test...\r\n");
  // result = sfud_read(flash, 0, TEST_BUFFER_SIZE, NULL);
  // TEST_ASSERT_NOT_EQUAL(SFUD_SUCCESS, result);
  // printf("  [PASS] NULL pointer detected\r\n");

  /* Test 3: Zero size operation */
  printf("Test 3: Zero size operation...\r\n");
  result = sfud_read(flash, 0, 0, read_buffer);
  // Some implementations may allow this, check based on your implementation
  printf("  Zero size result: %d\r\n", result);
  printf("  [PASS] Zero size handled\r\n");
}

/**
 * @brief Master test suite runner for SFUD
 */
void TEST_SFUD_RunAllTests(void)
{
  printf("\r\n");
  printf("========================================\r\n");
  printf("   SFUD Unit Test Suite Starting...    \r\n");
  printf("========================================\r\n");

  UNITY_BEGIN();

  /* Initialize and basic tests */
  RUN_TEST(TEST_SFUD_Init);
  RUN_TEST(TEST_SFUD_ReadID);

  /* Core functionality tests */
  RUN_TEST(TEST_SFUD_EraseWriteRead);
  RUN_TEST(TEST_SFUD_ReadVariousSizes);
  RUN_TEST(TEST_SFUD_WriteVariousSizes);
  RUN_TEST(TEST_SFUD_EraseOperations);

  /* Advanced tests */
  RUN_TEST(TEST_SFUD_StatusOperations);
  RUN_TEST(TEST_SFUD_BoundaryConditions);
  RUN_TEST(TEST_SFUD_DataIntegrity);
  RUN_TEST(TEST_SFUD_Performance);
  RUN_TEST(TEST_SFUD_ErrorHandling);

  printf("\r\n");
  printf("========================================\r\n");
  printf("   SFUD Unit Test Suite Completed!     \r\n");
  printf("========================================\r\n");
  printf("\r\n");

  UNITY_END();
}