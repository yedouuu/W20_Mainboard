/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file i2c.c
 * 
 * @author Yedou | UNION
 * 
 * @brief Provide the Core APIs of I2C and corresponding options.
 * 
 * Processing flow:
 * 1. I2C_Init() - Initialize I2C peripheral with SCL/SDA pins
 * 2. I2C_Write/Read() - Perform I2C transactions
 * 3. I2C_DeInit() - Release I2C peripheral resources
 * 
 * @version V1.0 2025-12-02 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/
#include <string.h>

#include "Logger.h"
#include "gpio.h"
#include "systick.h"

#include "i2c.h"
#include "i2c_application.h"

/* Private defines -----------------------------------------------------------*/
#define I2C_FLAG_TIMEOUT    100  // Timeout for I2C flag operations (in ms)

/* Private variables ---------------------------------------------------------*/


/* Private Functions ---------------------------------------------------------*/

/**
 * @brief  Convert I2C library status to project Status_t
 */
static Status_t I2C_ConvertStatus(i2c_status_type i2c_status)
{
  if (i2c_status != I2C_OK)
  {
    log_e("I2C Error: %d", i2c_status);
  }
  
  switch (i2c_status)
  {
    case I2C_OK:
      return kStatus_Success;
    case I2C_ERR_TIMEOUT:
      return kStatus_Timeout;
    case I2C_ERR_ACKFAIL:
      return kStatus_NackReceived;
    default:
      return kStatus_Fail;
  }
}

/* Public Functions ----------------------------------------------------------*/
Status_t I2Cx_Init(I2C_TypeDef *I2Cx, uint32_t baudRate)
{
  i2c_reset(I2Cx);

  if( I2Cx == I2C1 )
  {
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_I2C1_PERIPH_CLOCK, TRUE);
    gpio_pin_remap_config(I2C1_MUX, TRUE);
    pinMode(PB8, OUTPUT_AF_OD); // SCL
    pinMode(PB9, OUTPUT_AF_OD); // SDA
  }
  else if( I2Cx == I2C2 )
  {
    crm_periph_clock_enable(CRM_I2C2_PERIPH_CLOCK, TRUE);
    pinMode(PB10, OUTPUT_AF_OD); // SCL
    pinMode(PB11, OUTPUT_AF_OD); // SDA
  }
  else if( I2Cx == I2C3 )
  {
    crm_periph_clock_enable(CRM_I2C3_PERIPH_CLOCK, TRUE);
    pinMode(PC8, OUTPUT_AF_OD); // SCL
    pinMode(PC9, OUTPUT_AF_OD); // SDA
  }
  i2c_init(I2Cx, I2C_FSMODE_DUTY_2_1, baudRate);

  i2c_enable(I2Cx, TRUE);
  return kStatus_Success;
}

Status_t I2Cx_Master_Read(I2C_TypeDef *I2Cx, uint8_t slaveAddr, \
                          void *buf, uint32_t length)
{
  Status_t status;
  i2c_status_type i2c_status;
  i2c_handle_type hi2c;
  memset(&hi2c, 0, sizeof(i2c_handle_type));
  hi2c.i2cx = I2Cx;
  i2c_status = i2c_master_receive(&hi2c, slaveAddr, buf, length, I2C_TIMEOUT_DEFAULT_MS);
  return I2C_ConvertStatus(i2c_status);
}

Status_t I2Cx_Master_Write(I2C_TypeDef *I2Cx, uint8_t slaveAddr, \
                           void *buf, uint32_t length)
{
  Status_t status;
  i2c_status_type i2c_status;
  i2c_handle_type hi2c;
  memset(&hi2c, 0, sizeof(i2c_handle_type));
  hi2c.i2cx = I2Cx;

  // log_d("I2C Write: SlaveAddr=0x%02X, Length=%d", slaveAddr, length);
  i2c_status = i2c_master_transmit(&hi2c, slaveAddr, buf, length, I2C_TIMEOUT_DEFAULT_MS);
  return I2C_ConvertStatus(i2c_status);
}


