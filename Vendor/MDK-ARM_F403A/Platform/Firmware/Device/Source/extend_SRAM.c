/**
  ******************************************************************************
  * File   : SRAM/extend_SRAM/EXTEND_SRAM/extend_SRAM.c
  * Version: V1.2.4
  * Date   : 2020-08-26
  * Brief  : This file contains the function extend_SRAM_test used to extend SRAM size
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "extend_SRAM.h" 
// #include "at32f4xx.h"   
#include "at32f403a_407.h"
#include "stdio.h"   

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

 /** @addtogroup SRAM_Extended_SRAM
  * @{
  */

#define EXTEND_SRAM_224K

/* Private typedef -----------------------------------------------------------*/

#define TEST_RAM_SIZE  0x800

/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/
/**
  * @brief  To extend SRAM size
  * @param  None
  * @retval None
  */
void extend_SRAM(void)
{
/* Target set_SRAM_96K is selected */
#ifdef EXTEND_SRAM_96K
/* check if RAM has been set to 96K, if not, change EOPB0 */
  if(((UOPTB->EOPB0)&0xFF)!=0xFF)
  {
    // /* Unlock Option Bytes Program Erase controller */
    // FLASH_Unlock();
    // /* Erase Option Bytes */
    // FLASH_EraseUserOptionBytes();
    // /* Change SRAM size to 96KB */
    // FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFF);
    // NVIC_SystemReset();

    flash_unlock();
    flash_user_system_data_erase();
    flash_user_system_data_program((uint32_t)&USD->eopb0, 0xFF);
    NVIC_SystemReset();
  }
#endif

/* Target set_SRAM_224K is selected */
#ifdef EXTEND_SRAM_224K
/* check if RAM has been set to 224K, if not, change EOPB0 */
  if(((USD->eopb0)&0xFF)!=0xFE)
  {
    /* Unlock Option Bytes Program Erase controller */
    // FLASH_Unlock();
    // /* Erase Option Bytes */
    // FLASH_EraseUserOptionBytes();
    // /* Change SRAM size to 224KB */
    // FLASH_ProgramUserOptionByteData((uint32_t)&USD->eopb0,0xFE);
    // NVIC_SystemReset();

    flash_unlock();
    
    flash_user_system_data_erase();
    flash_user_system_data_program((uint32_t)&USD->eopb0, 0xFE);
    NVIC_SystemReset();
  }
#endif
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
