/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for AT32F403A SPIM (W25Q64).
 * Created on: 2016-04-23
 * Modified for AT32F403A SPIM: 2026-01-20
 */

#include <sfud.h>
#include <stdarg.h>
#include "bsp_config.h"
#include "bsp_sflash.h"
#include "sw_spi.h"

static char log_buf[256];

void sfud_log_debug(const char *file, const long line, const char *format, ...);

/**
 * @brief Initialize SPIM hardware
 */
static void spim_hw_init(void)
{
  gpio_init_type gpio_init_struct;

  /* Enable clocks */
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* Configure SPIM pin remap: EXT_SPIM_GMUX_1001
   * SPIM_SCK(PB1), SPIM_CS(PA8), SPIM_IO0(PB10), SPIM_IO1(PB11),
   * SPIM_IO2(PB7), SPIM_IO3(PB6) */
  gpio_pin_remap_config(EXT_SPIM_GMUX_1001, TRUE);

  /* Configure GPIO pins */
  pinMode(SFLASH_CS_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_CLK_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_HOLD_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_WP_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_MISO_PIN, OUTPUT_AF_PP);
  pinMode(SFLASH_MOSI_PIN, OUTPUT_AF_PP);

  /* Select SPIM model (MODEL2 for W25Q64) */
  flash_spim_model_select(FLASH_SPIM_MODEL2);

  flash_flag_clear(FLASH_SPIM_PRGMERR_FLAG);
}

static sw_spi_config_t sw_spi = {.cs_pin   = SFLASH_CS_PIN,
                                 .clk_pin  = SFLASH_CLK_PIN,
                                 .mosi_pin = SFLASH_MOSI_PIN,
                                 .miso_pin = SFLASH_MISO_PIN,
                                 .wp_pin   = SFLASH_WP_PIN,
                                 .hold_pin = SFLASH_HOLD_PIN};

static void sw_spi_hw_init(void)
{
  sw_spi_init(&sw_spi);
}

/**
 * @brief SPI write data then read data (using memory-mapped access via SPIM)
 * @note For SPIM, this is mainly used for read operations through memory
 * mapping
 */
static sfud_err spi_write_read(const sfud_spi *spi,
                               const uint8_t  *write_buf,
                               size_t          write_size,
                               uint8_t        *read_buf,
                               size_t          read_size)
{
  sfud_err result = SFUD_SUCCESS;
  sw_spi_write_read(&sw_spi, write_buf, write_size, read_buf, read_size);

  /* For read operations, SPIM supports direct memory access */
  // if (read_size > 0 && read_buf != NULL)
  // {
  //   /* Check if this is a simple read command (0x03) */
  //   if (write_size >= 4 && write_buf[0] == 0x03)
  //   {
  //     /* Extract 24-bit address from command */
  //     uint32_t addr = (write_buf[1] << 16) | (write_buf[2] << 8) | write_buf[3];
  //     uint32_t spim_addr = FLASH_SPIM_START_ADDR + addr;

  //     // ext_flash_write(write_buf, addr, write_size); // Send command and
  //     // address
  //     /* Direct memory read */
  //     uint8_t *src = (uint8_t *)spim_addr;
  //     for (size_t i = 0; i < read_size; i++) { read_buf[i] = src[i]; }
  //   }
  // }

  return result;
}

#ifdef SFUD_USING_QSPI
/**
 * @brief Read flash data by QSPI (using AT32 SPIM memory-mapped access)
 * @note SPIM provides hardware-accelerated memory-mapped read access
 */
static sfud_err qspi_read(const struct __sfud_spi   *spi,
                          uint32_t                   addr,
                          sfud_qspi_read_cmd_format *qspi_read_cmd_format,
                          uint8_t                   *read_buf,
                          size_t                     read_size)
{
  return SFUD_SUCCESS;
}
#endif /* SFUD_USING_QSPI */

/**
 * @brief Initialize SFUD SPI port for AT32F403A SPIM
 * @param flash: SFUD flash device
 * @return SFUD_SUCCESS or error code
 */
sfud_err sfud_spi_port_init(sfud_flash *flash)
{
  sfud_err result = SFUD_SUCCESS;

  // /* 1. Initialize SPIM hardware (GPIO, clocks, pin remap) */
  // spim_hw_init();

  // /* 2. Unlock SPIM for operations */
  // flash_spim_unlock();

  sw_spi_hw_init();

  /* 3. Configure flash SPI interface */
  flash->spi.wr = spi_write_read;


#ifdef SFUD_USING_QSPI
  /* Enable QSPI fast read through SPIM memory mapping */
  flash->spi.qspi_read = qspi_read;
#endif

  /* Note: SPIM handles CS automatically, no lock/unlock needed */
  flash->spi.lock      = NULL;
  flash->spi.unlock    = NULL;
  flash->spi.user_data = NULL;

  /* 4. Configure retry parameters */
  flash->retry.delay = NULL;
  flash->retry.times = 10000;

  /* Perform dummy read to complete SPIM initialization */
  flash_spim_dummy_read();

  return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...)
{
  va_list args;

  /* args point to the first variable parameter */
  va_start(args, format);
  printf("[SFUD](%s:%ld) ", file, line);
  /* must use vprintf to print */
  vsnprintf(log_buf, sizeof(log_buf), format, args);
  printf("%s\r\n", log_buf);
  va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...)
{
  va_list args;

  /* args point to the first variable parameter */
  va_start(args, format);
  printf("[SFUD]");
  /* must use vprintf to print */
  vsnprintf(log_buf, sizeof(log_buf), format, args);
  printf("%s\r\n", log_buf);
  va_end(args);
}
