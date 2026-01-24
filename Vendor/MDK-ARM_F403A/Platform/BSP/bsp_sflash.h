
#ifndef __BSP_SFLASH_H__
#define __BSP_SFLASH_H__

#include "mcu_type.h"
#include "bsp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Defines
 * ---------------------------------------------------------------------*/
typedef enum
{
  SFLASH_SUCC,
  SFLASH_FAIL,
} SFlash_Status_e;

#define SFLASH_SECTOR_SIZE            4096
#define EXT_FLASH_8M                  "8MB"
#define EXT_FLASH_16M                 "16MB"
#define SFLASH_SECTOR_START_ADD(addr) (addr & (~(SFLASH_SECTOR_SIZE - 1)))
extern uint32_t ext_flash_lvgl_addr;

/* Private function prototypes -----------------------------------------------*/
void            BSP_SFlash_Init(void);
SFlash_Status_e BSP_SFlash_ErasePage(uint32_t addr, uint32_t size);
SFlash_Status_e BSP_SFlash_Write(uint8_t *pBuffer,
                                 uint32_t dwWriteAddr,
                                 uint32_t dwNumByteToWrite);
SFlash_Status_e BSP_SFlash_Read(uint8_t *pBuffer,
                                uint32_t dwReadAddr,
                                uint32_t dwNumByteToRead);

/* Extern function prototypes -----------------------------------------------*/
const char *BSP_SFlash_TypeGet(void);
uint32_t    BSP_SFlash_SizeGet(void);
uint32_t    BSP_SFlash_ParaHeadAddrGet(void);
uint32_t    BSP_SFlash_ParaSizeGet(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_SFLASH_H__ */
