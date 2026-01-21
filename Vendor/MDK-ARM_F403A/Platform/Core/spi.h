
#ifndef __SPI_H__
#define __SPI_H__

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"
#include "gpio.h"
#include "Common/errno.h"
#include "Common/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
typedef enum {
  SPI1_INSTANCE = 0,
  SPI2_INSTANCE,
  SPI3_INSTANCE,
  SPI4_INSTANCE,
} SPI_Instance_e; 


/* Exported functions prototypes ---------------------------------------------*/
Status_t SPIx_Init(SPI_TypeDef *SPIx, uint16_t mode, uint32_t baudRatePrescaler);
Status_t SPIx_Transmit(SPI_TypeDef *SPIx, uint8_t *txData, uint32_t length);
Status_t SPIx_Receive(SPI_TypeDef *SPIx, uint8_t *rxData, uint32_t length);
Status_t SPIx_TransmitReceive(SPI_TypeDef *SPIx,
                              uint8_t     *txData,
                              uint8_t     *rxData,
                              uint32_t     length);

#ifdef __cplusplus
}
#endif

#endif // __SPI_H__