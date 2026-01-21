
#include "spi.h"

static SPI_TypeDef* SPIx_Instance[] = {SPI1, SPI2, SPI3, SPI4};

Status_t SPIx_Init(SPI_TypeDef *SPIx, uint16_t mode, uint32_t baudRatePrescaler)
{
  spi_init_type spi_init_struct;
  spi_default_para_init(&spi_init_struct);

  if ( SPIx == SPI1 )
  {

  }
  else if ( SPIx == SPI2 )
  {

  }
  else if ( SPIx == SPI3 )
  {

  }
  else if ( SPIx == SPI4 )
  {

  }
  else
  {
    // log_e("SPIx_Init: Invalid SPI instance");
    return kStatus_InvalidArgument;
  }

  // spi_i2s_dma_transmitter_enable(SPIx, DISABLE);

  return kStatus_Success;
}
