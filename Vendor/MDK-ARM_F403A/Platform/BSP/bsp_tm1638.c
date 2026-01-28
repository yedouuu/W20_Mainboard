
#include "bsp_tm1638.h"
#include "mcu_core.h"
#include "Common/common.h"

#define TM1638_STB_HIGH   digitalWrite_HIGH(TM1638_SPI_STB_PIN)
#define TM1638_STB_LOW    digitalWrite_LOW(TM1638_SPI_STB_PIN)
#define TM1638_CLK_HIGH   digitalWrite_HIGH(TM1638_SPI_CLK_PIN)
#define TM1638_CLK_LOW    digitalWrite_LOW(TM1638_SPI_CLK_PIN)
#define TM1638_DIO_HIGH   digitalWrite_HIGH(TM1638_SPI_DIO_PIN)
#define TM1638_DIO_LOW    digitalWrite_LOW(TM1638_SPI_DIO_PIN)
#define TM1638_READ_DIO   digitalRead_FAST(TM1638_SPI_DIO_PIN)

#define TM1638_DELAY      40
#define TM1638_HALF_DELAY 20

static void __BSP_TM1638_Delay(uint32_t ticks)
{
  volatile uint32_t i;
  for (i = 0; i < ticks; i++); // 根据系统时钟调整延时
}

static void __BSP_TM1638_Write_Byte(uint8_t tx_byte)
{
  uint8_t i;
  pinMode(TM1638_SPI_DIO_PIN, OUTPUT_OPEN_DRAIN);

  for (i = 0; i < 8; i++)
  {
    TM1638_CLK_LOW;
    __BSP_TM1638_Delay(TM1638_HALF_DELAY);
    // 发送最低位
    if (tx_byte & 0x01) { TM1638_DIO_HIGH; }
    else
    {
      TM1638_DIO_LOW;
    }
    __BSP_TM1638_Delay(TM1638_HALF_DELAY);

    TM1638_CLK_HIGH;
    __BSP_TM1638_Delay(TM1638_DELAY);

    tx_byte >>= 1;
  }
}

static void __BSP_TM1638_Read_Byte(uint8_t *read_byte)
{
  uint8_t rx_byte = 0;
  pinMode(TM1638_SPI_DIO_PIN, INPUT);

  for (uint8_t i = 0; i < 8; i++)
  {
    TM1638_CLK_LOW;
    __BSP_TM1638_Delay(TM1638_DELAY);

    TM1638_CLK_HIGH;
    __BSP_TM1638_Delay(TM1638_HALF_DELAY);
    
    /* 上升沿读取 */
    if (TM1638_READ_DIO) {
      rx_byte |= (1 << i);
    }
    __BSP_TM1638_Delay(TM1638_DELAY);
  }

  *read_byte = rx_byte;
}

static void __BSP_TM1638_Write_Read(const uint8_t *tx_buf,
                                    uint32_t       tx_len,
                                    uint8_t       *rx_buf,
                                    uint32_t       rx_len)
{
  TM1638_STB_LOW;
  __BSP_TM1638_Delay(TM1638_DELAY);

  uint8_t i = 0;

  // 发送数据
  for (i = 0; i < tx_len; i++) { __BSP_TM1638_Write_Byte(tx_buf[i]); }

  // 接收数据
  for (i = 0; i < rx_len; i++) { __BSP_TM1638_Read_Byte(&rx_buf[i]); }

  __BSP_TM1638_Delay(TM1638_DELAY);
  TM1638_STB_HIGH;
}

void BSP_TM1638_Init(void)
{
  uint8_t write_buf[16] = {0};
  uint8_t write_size    = 1;

  pinMode(TM1638_SPI_STB_PIN, OUTPUT_OPEN_DRAIN);
  pinMode(TM1638_SPI_CLK_PIN, OUTPUT_OPEN_DRAIN);
  pinMode(TM1638_SPI_DIO_PIN, OUTPUT_OPEN_DRAIN);

  TM1638_STB_HIGH;
  __BSP_TM1638_Delay(TM1638_DELAY);

  /* 1. Set display mode 地址自增模式 */
  write_buf[0] = 0x40;
  write_size   = 1;
  __BSP_TM1638_Write_Read(write_buf, write_size, NULL, 0);

  /* 2. 设置起始地址 */
  write_buf[0] = 0xC0;
  write_size   = 1;
  __BSP_TM1638_Write_Read(write_buf, write_size, NULL, 0);

  /* 3. 发送显示数据 */
  write_buf[0] = 0x00; // 清零显存
  write_size   = 16;
  __BSP_TM1638_Write_Read(write_buf, write_size, NULL, 0);

  /* 4. 设置亮度 */
  write_buf[0] = 0x8F;
  write_size   = 1;
  __BSP_TM1638_Write_Read(write_buf, write_size, NULL, 0);

  /* 5. 读取按键扫描结果, 返回4字节数据 */
  // write_buf[0] = 0x42;
}

void BSP_TM1638_ReadKey(void)
{
  // TODO: 显示刷新和按键读取互斥处理

  uint8_t write_buf[1] = {0};
  uint8_t write_size   = 1;
  uint8_t read_buf[4]  = {0};
  uint8_t read_size    = 4;

  write_buf[0] = 0x42;
  __BSP_TM1638_Write_Read(write_buf, write_size, read_buf, read_size);

  log_d("Read Key: [%d][%d][%d][%d]",
        read_buf[0],
        read_buf[1],
        read_buf[2],
        read_buf[3]);

  return;
}
