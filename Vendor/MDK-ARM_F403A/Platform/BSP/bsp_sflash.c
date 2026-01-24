/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_sflash.c
 *
 * @author Yedou | UNION
 *
 * @brief Provide the BSP APIs of SFlash and corresponding options.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2026-01-24 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 *
 *****************************************************************************/
#include "bsp_sflash.h"
#include "mcu_core.h"
#include <string.h>
#include <stdio.h>

#define EXT_FLASH_ID "WH_ZM_W25Q64JV_V1.0"
/* Define External SPI flash type, define only one ---------------------------*/
#define FLASH_SPIM_TYPE1 1
#define FLASH_SPIM_TYPE2 2

/* Define External SPI flash size --------------------------------------------*/
#define SFLASH_PAGE_SIZE 4096

/* Define External SPI flash unlock key --------------------------------------*/
#define FLASH_KEY1 ((UINT32)0x45670123)
#define FLASH_KEY2 ((UINT32)0xCDEF89AB)

uint32_t           ext_flash_lvgl_addr;
static const char *ext_flash_8M    = EXT_FLASH_8M;
static const char *ext_flash_16M   = EXT_FLASH_16M;
static const char *ext_flash_type  = NULL;
static uint32_t    jedec_id        = 0;
static uint8_t     spi_flash_ready = 0;

#define FLASH_HEAD_PARA_SIZE                                                   \
  SFLASH_SECTOR_SIZE // HEAD INFO,IN FLASH FIRST SECTOR
#define FLASH_TYPE_SIZE     SFLASH_SECTOR_SIZE // TYPE INFO,IN FLASH LAST SECTOR
#define EXT_FLASH_PARA_SIZE 262144             // 256K
#define EXT_FLASH_RESERVE                                                      \
  (FLASH_HEAD_PARA_SIZE + FLASH_TYPE_SIZE + EXT_FLASH_PARA_SIZE)
#define EXT_FLASH_8M_SIZE  8388608
#define EXT_FLASH_16M_SIZE 16777216

#define FLASH_IMG_OFFSET   0x1000
#define EXT_FLASH_MAX1     0x093FFFFF // 16M
#define EXT_FLASH_MAX2     0x08BFFFFF // 8M
#define EXT_FLASH_BASE     SPIM_FLASH_BASE
// #define EXT_FLASH_TYPE "16MB"
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Read JEDEC ID using software SPI
 * @retval 24-bit JEDEC ID (0xEF4017 for W25Q64)
 */
uint32_t BSP_SFlash_Read_JEDEC_ID(void)
{
  uint32_t jedec_id = 0;
  // uint8_t mfr_id, mem_type, capacity;

  sw_spi_config_t sw_spi = {.cs_pin   = SFLASH_CS_PIN,
                            .clk_pin  = SFLASH_CLK_PIN,
                            .mosi_pin = SFLASH_MOSI_PIN,
                            .miso_pin = SFLASH_MISO_PIN,
                            .wp_pin   = SFLASH_WP_PIN,
                            .hold_pin = SFLASH_HOLD_PIN};

  uint8_t tx_buf[]  = {0x9F};
  uint8_t tx_len    = 1;
  uint8_t rx_buf[3] = {0};
  uint8_t rx_len    = 3;

  sw_spi_init(&sw_spi);
  sw_spi_write_read(&sw_spi, tx_buf, tx_len, rx_buf, rx_len);

  uint8_t mfr_id   = rx_buf[0];
  uint8_t mem_type = rx_buf[1];
  uint8_t capacity = rx_buf[2];

  // 组合 JEDEC ID
  jedec_id = ((uint32_t)mfr_id << 16) | ((uint32_t)mem_type << 8) | capacity;

  printf("JEDEC ID: 0x%06X (MFR:0x%02X, TYPE:0x%02X, CAP:0x%02X)\r\n",
         jedec_id,
         mfr_id,
         mem_type,
         capacity);

  return jedec_id;
}

/**
 * @brief  Parse flash capacity from JEDEC ID
 * @param  jedec_id: 24-bit JEDEC ID
 * @retval Flash size in bytes, 0 if unknown
 */
uint32_t ext_flash_parse_capacity(uint32_t jedec_id)
{
  uint8_t manufacturer_id = (jedec_id >> 16) & 0xFF;
  uint8_t memory_type     = (jedec_id >> 8) & 0xFF;
  uint8_t capacity_id     = jedec_id & 0xFF;

  // Winbond W25Q series
  if (manufacturer_id == 0xEF)
  {
    switch (capacity_id)
    {
      case 0x17: // W25Q64 - 64Mbit = 8MB
        printf("Detected: Winbond W25Q64 (8MB)\r\n");
        ext_flash_type = ext_flash_8M;
        return 8 * 1024 * 1024;

      case 0x18: // W25Q128 - 128Mbit = 16MB
        printf("Detected: Winbond W25Q128 (16MB)\r\n");
        ext_flash_type = ext_flash_16M;
        return 16 * 1024 * 1024;

      case 0x19: // W25Q256 - 256Mbit = 32MB
        printf("Detected: Winbond W25Q256 (32MB)\r\n");
        return 32 * 1024 * 1024;

      default:
        break;
    }
  }
  // GigaDevice GD25Q series
  else if (manufacturer_id == 0xC8)
  {
    if (capacity_id == 0x17)
    {
      printf("Detected: GigaDevice GD25Q64 (8MB)\r\n");
      ext_flash_type = ext_flash_8M;
      return 8 * 1024 * 1024;
    }
    else if (capacity_id == 0x18)
    {
      printf("Detected: GigaDevice GD25Q128 (16MB)\r\n");
      ext_flash_type = ext_flash_16M;
      return 16 * 1024 * 1024;
    }
  }

  printf("Unknown or unsupported flash! (JEDEC ID: 0x%06X)\r\n", jedec_id);
  return 0;
}

/**
 * @brief  Configures the ext.flash
 * @param  None
 * @retval None
 */
// void BSP_SFlash_Init(void)
void BSP_SFlash_Init(void)
{
  jedec_id            = BSP_SFlash_Read_JEDEC_ID();
  uint32_t flash_size = ext_flash_parse_capacity(jedec_id);
  // printf("External Flash Size: %lu bytes\r\n", flash_size);

  gpio_init_type gpio_init_struct;

  /* Enable clocks */
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* spim_sck(pb1), spim_cs(pa8), spim_io0(pb10), spim_io1(pb11), spim_io2(pb7),
   * spim_sio3(pb6)  */
  gpio_pin_remap_config(EXT_SPIM_GMUX_1001, TRUE);

  /* Configure GPIO pins */
  pinMode(SFLASH_CS_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_CLK_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_HOLD_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_WP_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_MISO_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_MOSI_PIN, OUTPUT_AF_PP);

  /* In this example, use W25Q64JV as ext.flash */
  flash_spim_model_select(FLASH_SPIM_MODEL2);

  /* Unlock the ext.flash program erase controller */
  flash_flag_clear(FLASH_SPIM_PRGMERR_FLAG);
  while (flash_flag_get(FLASH_SPIM_OBF_FLAG));
  flash_spim_unlock();
  while (flash_flag_get(FLASH_SPIM_ODF_FLAG));

  /* If the data written to ext.flash need to be scrambled, please specify the
   * scrambled range */
  FLASH->da = 0;

  ext_flash_lvgl_addr = EXT_FLASH_BASE + FLASH_HEAD_PARA_SIZE;

  return;
}

/**
 * @brief  Erases a specified FLASH page.
 * @note   This function can be used for all at32f4xx devices.
 * @param  Page_Address: The page address to be erased.
 * @retval FLASH Status: The returned value can be: FLASH_BSY, FLASH_PGRM_FLR,
 *         FLASH_WRPRT_FLR, FLASH_PRC_DONE or FLASH_TIMEOUT.
 */
SFlash_Status_e BSP_SFlash_ErasePage(uint32_t addr, uint32_t size)
{
  Disable_ALL_IRQ;
  uint32_t start_addr = SFLASH_SECTOR_START_ADD(addr);
  uint16_t sector_num = size / SFLASH_SECTOR_SIZE;
  for (uint16_t i = 0; i < sector_num; i++)
  {
    flash_status_type status =
        flash_sector_erase(start_addr + i * SFLASH_SECTOR_SIZE);
    if (status != FLASH_OPERATE_DONE)
    {
      Enable_ALL_IRQ;
      printf("[%s:%d] Ext.flash erase fail(%d) at addr: 0x%08X\r\n",
             __FUNCTION__,
             __LINE__,
             status,
             start_addr + i * SFLASH_SECTOR_SIZE);
      return SFLASH_FAIL;
    }
  }
  Enable_ALL_IRQ;
  return SFLASH_SUCC;
}

/**
 * @brief  Programs a word at a specified address.
 * @note   This function can be used for all at32f4xx devices.
 * @param  Address: specifies the address to be programmed.
 * @param  Data: specifies the data to be programmed.
 * @retval FLASH Status: The returned value can be: FLASH_PGRM_FLR,
 *         FLASH_WRPRT_FLR, FLASH_PRC_DONE or FLASH_TIMEOUT.
 */
SFlash_Status_e BSP_SFlash_Write(uint8_t *pBuffer,
                                 uint32_t dwWriteAddr,
                                 uint32_t dwNumByteToWrite)
{
  Disable_ALL_IRQ;
  //	uint32_t word_num = dwNumByteToWrite&0xfffffffc;
  uint32_t          write_offset = 0;
  flash_status_type status;

  while (write_offset < dwNumByteToWrite)
  {
    status = flash_word_program(dwWriteAddr + write_offset,
                                *(uint32_t *)(pBuffer + write_offset));
    if (status != FLASH_OPERATE_DONE)
    {
      printf("Ext.flash write fail(%d) at addr: 0x%08X\r\n",
             status,
             dwWriteAddr + write_offset);
      return SFLASH_FAIL;
    }
    write_offset += 4;
  }
  Enable_ALL_IRQ;
  return SFLASH_SUCC;
}

/**
 * @brief  Read one page from a specified address in ext.flash
 * @param  adr: specifies the page starting address
 * @param  sz: page size in bytes
 * @param  buf: the content read from ext.flash
 * @retval None
 */
SFlash_Status_e BSP_SFlash_Read(uint8_t *pBuffer,
                                uint32_t dwReadAddr,
                                uint32_t dwNumByteToRead)
{
  uint32_t word_num    = dwNumByteToRead & 0xfffffffc;
  uint32_t read_offset = 0;
  while (read_offset < word_num)
  {
    *(uint32_t *)pBuffer = *(uint32_t *)dwReadAddr;
    read_offset += sizeof(uint32_t);
    dwReadAddr += sizeof(uint32_t);
    pBuffer += sizeof(uint32_t);
  }

  while (read_offset < dwNumByteToRead)
  {
    *(uint8_t *)pBuffer = *(uint8_t *)dwReadAddr;
    read_offset += sizeof(uint8_t);
    dwReadAddr += sizeof(uint8_t);
    pBuffer += sizeof(uint8_t);
  }

  return SFLASH_SUCC;
}

const char *BSP_SFlash_TypeGet(void)
{
  return ext_flash_type;
}

uint32_t BSP_SFlash_SizeGet(void)
{
  if (ext_flash_type == ext_flash_8M)
  {
    return EXT_FLASH_8M_SIZE - EXT_FLASH_RESERVE;
  }
  else if (ext_flash_type == ext_flash_16M)
  {
    return EXT_FLASH_16M_SIZE - EXT_FLASH_RESERVE;
  }
  return 0;
}

uint32_t BSP_SFlash_ParaHeadAddrGet(void)
{
  return EXT_FLASH_BASE + FLASH_HEAD_PARA_SIZE + BSP_SFlash_SizeGet();
}

uint32_t BSP_SFlash_ParaSizeGet(void)
{
  return EXT_FLASH_PARA_SIZE;
}
