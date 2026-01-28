/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file sw_spi.c
 *
 * @author Yedou | UNION
 *
 * @brief Software SPI implementation using GPIO bit-banging.
 *
 * Processing flow:
 *
 * 1. Call sw_spi_init() with pin configuration
 * 2. Use sw_spi_write_read() for data transfer
 * 3. Call sw_spi_deinit() when done
 *
 * @version V1.0 2026-01-24 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 *
 *****************************************************************************/
#include "sw_spi.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Software SPI delay for timing control
 */
static void sw_spi_delay(void)
{
  volatile uint32_t i;
  for (i = 0; i < 50; i++); // 根据系统时钟调整延时
}

/**
 * @brief  Initialize software SPI
 * @param  config: Pin configuration structure
 * @retval None
 */
void sw_spi_init(const sw_spi_config_t *sw_spi)
{
  // 使能时钟
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  // 配置 GPIO 为软件 SPI 模式（普通 GPIO，不是复用功能）
  pinMode(sw_spi->cs_pin, OUTPUT);         // CS 输出
  pinMode(sw_spi->clk_pin, OUTPUT);        // CLK 输出
  pinMode(sw_spi->mosi_pin, OUTPUT);       // MOSI 输出
  pinMode(sw_spi->miso_pin, INPUT_PULLUP); // MISO 输入上拉

  if (sw_spi->wp_pin != 0) pinMode(sw_spi->wp_pin, OUTPUT); // WP# 输出（可选）
  if (sw_spi->hold_pin != 0)
    pinMode(sw_spi->hold_pin, OUTPUT); // HOLD# 输出（可选）

  // 初始化 GPIO 状态
  digitalWrite_HIGH(sw_spi->cs_pin); // CS 拉高（未选中）
  digitalWrite_LOW(sw_spi->clk_pin); // CLK 初始低电平（SPI Mode 0）

  if (sw_spi->wp_pin != 0) digitalWrite_HIGH(sw_spi->wp_pin);     // 禁用写保护
  if (sw_spi->hold_pin != 0) digitalWrite_HIGH(sw_spi->hold_pin); // 禁用 HOLD

  delay_ms(1); // 等待稳定
}

/**
 * @brief  Deinitialize software SPI (restore to hardware SPIM mode)
 * @retval None
 */
void sw_spi_deinit(const sw_spi_config_t *sw_spi)
{
}

/**
 * @brief  Pull CS low to start SPI transaction
 * @retval None
 */
static void __sw_spi_cs_low(const sw_spi_config_t *sw_spi)
{
  digitalWrite_LOW(sw_spi->cs_pin);
  sw_spi_delay();
}

/**
 * @brief  Pull CS high to end SPI transaction
 * @retval None
 */
static void __sw_spi_cs_high(const sw_spi_config_t *sw_spi)
{
  digitalWrite_HIGH(sw_spi->cs_pin);
  sw_spi_delay();
}

static void __sw_spi_transfer_byte(const sw_spi_config_t *sw_spi,
                                   uint8_t                tx_byte,
                                   uint8_t               *rx_byte)
{
  uint8_t i;
  uint8_t received_byte = 0;

  for (i = 0; i < 8; i++)
  {
    // 发送最高位
    if (tx_byte & 0x80)
      digitalWrite_HIGH(sw_spi->mosi_pin);
    else
      digitalWrite_LOW(sw_spi->mosi_pin);

    sw_spi_delay();

    // 上升沿
    digitalWrite_HIGH(sw_spi->clk_pin);
    sw_spi_delay();

    // 读取 MISO
    received_byte <<= 1;
    if (digitalRead_FAST(sw_spi->miso_pin)) received_byte |= 0x01;

    // 下降沿
    digitalWrite_LOW(sw_spi->clk_pin);
    sw_spi_delay();

    // 移动到下一位
    tx_byte <<= 1;
  }

  if (rx_byte != NULL) { *rx_byte = received_byte; }
}

/**
 * @brief  Software SPI Write and Read transaction
 *
 * @param  sw_spi: Pointer to SPI configuration structure
 * @param  tx_buf: Pointer to data buffer to be transmitted (can be NULL if
 * tx_len is 0)
 * @param  tx_len: Number of bytes to transmit
 * @param  rx_buf: Pointer to buffer for received data (can be NULL if rx_len is
 * 0)
 * @param  rx_len: Number of bytes to receive
 */
void sw_spi_write_read(sw_spi_config_t *sw_spi,
                       const uint8_t   *tx_buf,
                       uint32_t         tx_len,
                       uint8_t         *rx_buf,
                       uint32_t         rx_len)
{
  uint32_t i;
  __sw_spi_cs_low(sw_spi);

  // 发送数据
  for (i = 0; i < tx_len; i++)
  {
    __sw_spi_transfer_byte(sw_spi, tx_buf[i], NULL);
  }

  // 接收数据
  for (i = 0; i < rx_len; i++)
  {
    __sw_spi_transfer_byte(sw_spi, 0xFF, &rx_buf[i]); // 发送空字节以接收数据
  }

  __sw_spi_cs_high(sw_spi);
}
