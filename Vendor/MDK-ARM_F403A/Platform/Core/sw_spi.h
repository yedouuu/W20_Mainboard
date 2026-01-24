
#ifndef __SW_SPI_H__
#define __SW_SPI_H__

#include "mcu_type.h"
#include "mcu_core.h"

/* Software SPI Pin Configuration ----------------------------------------*/
typedef struct
{
  uint16_t cs_pin;   // Chip Select
  uint16_t clk_pin;  // Clock
  uint16_t mosi_pin; // Master Out Slave In
  uint16_t miso_pin; // Master In Slave Out
  uint16_t wp_pin;   // Write Protect (optional)
  uint16_t hold_pin; // Hold (optional)
} sw_spi_config_t;

/* Software SPI Functions ------------------------------------------------*/
void sw_spi_init(const sw_spi_config_t *sw_spi);
void sw_spi_deinit(const sw_spi_config_t *sw_spi);
void sw_spi_write_read(sw_spi_config_t *sw_spi,
                       const uint8_t   *tx_buf,
                       uint32_t         tx_len,
                       uint8_t         *rx_buf,
                       uint32_t         rx_len);

#endif /* __SW_SPI_H__ */
