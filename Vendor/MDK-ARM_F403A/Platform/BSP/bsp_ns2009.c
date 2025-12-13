/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_NS2009.c
 * 
 * @author Yedou | UNION
 * 
 * @brief NS2009 resistive touch screen driver implementation.
 * 
 * Processing flow:
 * 1. Initialize I2C and GPIO
 * 2. Attach interrupt to PENIRQ pin
 * 3. On interrupt, read X/Y/Z1/Z2 values
 * 4. Calculate pressure and determine if touched
 * 
 * @version V1.0 2025-12-05 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/

#include "mcu_core.h"
#include "Logger.h"
#include "bsp_NS2009.h"
#include "exti.h"

/* Private variables --------------------------------------------------------*/
volatile uint8_t ns2009_irq_flag = 0;

/* Private function prototypes ----------------------------------------------*/
static void     NS2009_PENIRQ_Handler(void);
static void     NS2009_Open_IRQ(const Touch_Resource_t* touch_res);
static uint8_t  NS2009_Calculate_Pressure(TouchPoint_t* p);

/* Device Resource Definition -----------------------------------------------*/
const Touch_Resource_t touch_main_res =
{
  .ic_type  = TOUCH_IC_NS2009,
  .bus_type = TOUCH_BUS_I2C,
  .hor_res  = 480,
  .ver_res  = 272,
  .a0_pin   = TOUCH_NS2009_A0_PIN,
  .irq_pin  = TOUCH_NS2009_INT_PIN,
  .rotation = 0,

  .bus.i2c =
  {
    .i2c_instance = I2C1,
    .scl_pin      = TOUCH_NS2009_I2C_SCL_PIN,
    .sda_pin      = TOUCH_NS2009_I2C_SDA_PIN,
    .speed        = I2C_SPEED_STANDARD,
    .dev_addr_wr  = NS2009_ADDRESS_WR,
    .dev_addr_rd  = NS2009_ADDRESS_RD,
  },

  .init = NS2009_Init,
  .read = NS2009_Read_Coordinate,
};

/* Private Functions --------------------------------------------------------*/

/**
 * @brief  NS2009 PENIRQ interrupt handler
 * @retval None
 */
static void NS2009_PENIRQ_Handler(void)
{
  uint32_t val = digitalRead_FAST(touch_main_res.irq_pin);

  if (val == 0) {
    log_d("NS2009 PENIRQ triggered. Touch Pressed.");
  } else {
    log_d("NS2009 PENIRQ triggered. Touch Released.");
  }
  
  ns2009_irq_flag = 1;
  detachInterrupt(touch_main_res.irq_pin);
}

/**
 * @brief  Re-enable NS2009 interrupt
 * @param  touch_res: Pointer to Touch_Resource_t structure
 * @retval None
 */
static void NS2009_Open_IRQ(const Touch_Resource_t* touch_res)
{
  uint8_t cmd = NS2009_ONLY_OPEN_IRQ;
  I2Cx_Master_Write(touch_res->bus.i2c.i2c_instance, 
                    touch_res->bus.i2c.dev_addr_wr, 
                    &cmd, 1);
}

/**
 * @brief  Calculate touch pressure and determine if pressed
 * @param  p: Pointer to TouchPoint_t structure
 * @retval 0=not pressed, 1=pressed
 */
static uint8_t NS2009_Calculate_Pressure(TouchPoint_t* p)
{
  float    pressure;
  uint16_t x  = p->x;
  uint16_t z1 = p->z1;
  uint16_t z2 = p->z2;

  // NS2009 压力计算公式：Pressure = (X/4096) * [(Z2/Z1) - 1]
  if (z1 == 0) {
    return 0; // 避免除零错误
  }

  pressure = ((float)x / (float)NS2009_ADC_RESOLUTION) * 
             (((float)z2 / (float)z1) - 1.0f);
  
  log_d("NS2009 Calculated Pressure: %.2f", pressure);

  return (pressure > NS2009_PRESSURE_THRESHOLD) ? 1 : 0;
}

/* Public Functions ---------------------------------------------------------*/

/**
 * @brief  Initialize NS2009 touch driver
 * @param  touch_res: Pointer to Touch_Resource_t structure
 * @retval None
 */
void NS2009_Init(const Touch_Resource_t* touch_res)
{
  log_i("Initializing NS2009 touch driver...");

  /* MSOP-10封装, A0引脚配置 */
  pinMode(touch_res->a0_pin, OUTPUT);
  digitalWrite_HIGH(touch_res->a0_pin);

  /* 初始化I2C */
  I2Cx_Init(touch_res->bus.i2c.i2c_instance, touch_res->bus.i2c.speed);

  /* 配置中断引脚 - 使用FALLING避免读取时的干扰 */
  attachInterrupt(touch_res->irq_pin, NS2009_PENIRQ_Handler, FALLING);

  /* 发送初始化命令 */
  uint8_t  test_cmd = NS2009_ONLY_OPEN_IRQ;
  Status_t status   = I2Cx_Master_Write(touch_res->bus.i2c.i2c_instance,
                                        touch_res->bus.i2c.dev_addr_wr,
                                        &test_cmd, 1);
  
  if (status == kStatus_Success) {
    log_i("NS2009 initialization successful");
  } else {
    log_e("NS2009 initialization failed: %d", status);
  }
}

/**
 * @brief  Read NS2009 single channel data
 * @param  touch_res: Pointer to Touch_Resource_t structure
 * @param  cmd: NS2009 command
 * @retval 12-bit ADC value
 */
uint16_t NS2009_Read_Channel(const Touch_Resource_t* touch_res, NS2009_Cmd_e cmd)
{
  Status_t status;
  uint8_t  buf[2];
  uint16_t value = 0;

  /* 发送读取命令 */
  status = I2Cx_Master_Write(touch_res->bus.i2c.i2c_instance,
                             touch_res->bus.i2c.dev_addr_wr,
                             (uint8_t*)&cmd, 1);
  if (status != kStatus_Success) {
    log_e("NS2009 I2C write command failed: %d", status);
    return 0;
  }

  /* 读取数据 */
  status = I2Cx_Master_Read(touch_res->bus.i2c.i2c_instance,
                            touch_res->bus.i2c.dev_addr_rd,
                            buf, 2);
  if (status != kStatus_Success) {
    log_e("NS2009 I2C read data failed: %d", status);
    return 0;
  }

  /* 12位数据，右对齐 */
  value = ((uint16_t)(buf[0] << 8) | buf[1]) >> 4;

  return value;
}

/**
 * @brief  Read NS2009 touch coordinates
 * @param  touch_res: Pointer to Touch_Resource_t structure
 * @param  p: Pointer to TouchPoint_t structure to store results
 * @retval None
 */
void NS2009_Read_Coordinate(const Touch_Resource_t* touch_res, TouchPoint_t* p)
{
  uint16_t x, y, z1, z2;

  /* 禁用中断，防止读取过程中触发 */
  detachInterrupt(touch_res->irq_pin);

  /* 读取 X, Y, Z1, Z2 */
  x  = NS2009_Read_Channel(touch_res, NS2009_LOW_POWER_READ_X);
  y  = NS2009_Read_Channel(touch_res, NS2009_LOW_POWER_READ_Y);
  z1 = NS2009_Read_Channel(touch_res, NS2009_LOW_POWER_READ_Z1);
  z2 = NS2009_Read_Channel(touch_res, NS2009_LOW_POWER_READ_Z2);

  /* 重新开启中断 */
  NS2009_Open_IRQ(touch_res);

  /* 填充数据 */
  p->x  = x;
  p->y  = y;
  p->z1 = z1;
  p->z2 = z2;

  /* 计算压力并判断是否按下 */
  p->pressed = NS2009_Calculate_Pressure(p);

  log_d("NS2009 Read X=%d, Y=%d, Z1=%d, Z2=%d, Pressed=%d", 
        x, y, z1, z2, p->pressed);

  /* 清除可能的EXTI标志位，防止立即触发 */
  exint_flag_clear(1 << GPIO_GetPinNum(touch_res->irq_pin));

  /* 重新使能中断 */
  attachInterrupt(touch_res->irq_pin, NS2009_PENIRQ_Handler, FALLING);
}