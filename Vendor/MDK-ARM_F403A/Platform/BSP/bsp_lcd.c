/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_lcd.c
 *
 * @author Yedou | UNION
 *
 * @brief Provide the BSP APIs of LCD and corresponding options.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2025-12-01 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 *
 *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "bsp_lcd.h"
#include "mcu_core.h"
#include "Logger.h"

/* Private define ------------------------------------------------------------*/
#define LCD_RESET_HIGH(pin) digitalWrite_HIGH(pin);
#define LCD_RESET_LOW(pin)  digitalWrite_LOW(pin);

#define LCD_BL_HIGH(pin)    digitalWrite_HIGH(pin);
#define LCD_BL_LOW(pin)     digitalWrite_LOW(pin);

typedef void (*LCD_Param_Init_f)(const struct _LCD_Resource_t *lcd_res);

typedef enum
{
  LCD_BUS_TYPE_GPIO, // 并行 GPIO（8080/6800）
  LCD_BUS_TYPE_XMC,  // 使用 XMC (Flexible Memory Controller)
  LCD_BUS_TYPE_UART, // 串口屏（UART）
  LCD_BUS_TYPE_SPI,  // SPI 接口
  LCD_BUS_TYPE_QSPI, // QSPI 接口
  LCD_BUS_TYPE_I2C,  // I2C 接口
} LCD_Bus_Type_e;

typedef enum
{
  LCD_SHINY_ST043A23_IPS, // 首荣 480x272 IPS 4.3寸屏, ST7796, mcu/8080 接口,
                          // 使用XMC连接
} LCD_Model_e;

/* ==================== GPIO 并行总线配置 ==================== */
typedef struct
{
  uint8_t rd_pin;        // Read strobe (RD#)
  uint8_t wr_pin;        // Write strobe (WR#)
  uint8_t cs_pin;        // Chip select (CS#)
  uint8_t dc_pin;        // Data/Command (RS/DC)
  uint8_t data_pins[16]; // 数据引脚 D0-D15（8位模式只用前8个）
  uint8_t data_width;    // 8 or 16
} LCD_GPIO_BusConfig_t;

/* ==================== GPIO XMC 总线配置 ==================== */
typedef struct
{
  /* XMC 硬件配置 */
  uint8_t  bank;      // XMC Bank 编号 (1-4)
  uint32_t cmd_addr;  // 命令地址
  uint32_t data_addr; // 数据地址

  /* XMC 需要配置的引脚（复用为 AF 功能） */
  uint8_t noe_pin;    // XMC_NOE (读使能，对应 RD#)
  uint8_t nwe_pin;    // XMC_NWE (写使能，对应 WR#)
  uint8_t ne_pin;     // XMC_NE1/2/3/4 (片选)
  uint8_t dc_pin;     // 连接到IC的dc引脚用于命令/数据选择
  uint8_t d_pins[16]; // XMC_D0-D15 数据线
  uint8_t data_width; // 8 or 16
} LCD_GPIOXMC_BusConfig_t;

/* ==================== UART 总线配置 ==================== */
typedef struct
{
  uint8_t  uart_instance; // UART1, UART2, etc.
  uint32_t baudrate;
  uint8_t  tx_pin; // UART TX 引脚
  uint8_t  rx_pin; // UART RX 引脚（如果是单向屏可以省略）
} LCD_UART_BusConfig_t;

/* ==================== SPI 总线配置 ==================== */
typedef struct
{
  uint8_t  spi_instance; // SPI1, SPI2, etc.
  uint32_t baudrate;     // SPI 时钟频率 (Hz)
  uint8_t  sck_pin;      // SPI SCK 引脚
  uint8_t  mosi_pin;     // SPI MOSI 引脚
  uint8_t  miso_pin;     // SPI MISO 引脚（只读屏幕可以设为 0xFF）
  uint8_t  cs_pin;       // Chip Select 引脚
  uint8_t  dc_pin;       // Data/Command 引脚
  uint8_t  spi_mode;     // SPI 模式 (0-3)
  uint8_t  bit_order;    // 0: MSB First, 1: LSB First
} LCD_SPI_BusConfig_t;

/* ==================== QSPI 总线配置 ==================== */
typedef struct
{
  uint8_t  qspi_instance; // QSPI1, QSPI2, etc.
  uint32_t baudrate;      // QSPI 时钟频率 (Hz)
  uint8_t  clk_pin;       // QSPI CLK 引脚
  uint8_t  cs_pin;        // QSPI CS 引脚
  uint8_t  io0_pin;       // QSPI IO0 引脚 (MOSI in SPI mode)
  uint8_t  io1_pin;       // QSPI IO1 引脚 (MISO in SPI mode)
  uint8_t  io2_pin;       // QSPI IO2 引脚 (WP# in Quad mode)
  uint8_t  io3_pin;       // QSPI IO3 引脚 (HOLD# in Quad mode)
  uint8_t  dc_pin;        // Data/Command 引脚 (部分 LCD 需要)
  uint8_t  data_lines;    // 数据线数量 (1=SPI, 2=Dual, 4=Quad)
  uint8_t  clk_mode;      // 时钟模式 (0-3, 类似 SPI Mode)
  uint8_t  addr_size;     // 地址宽度 (8/16/24/32 bit)
  uint8_t  cmd_size;      // 命令宽度 (通常为 8 bit)
} LCD_QSPI_BusConfig_t;

/* ==================== I2C 总线配置 ==================== */
typedef struct
{
  uint8_t  i2c_instance; // I2C1, I2C2, etc.
  uint32_t speed;        // I2C 速度 (100000=100kHz, 400000=400kHz)
  uint8_t  scl_pin;      // I2C SCL 引脚
  uint8_t  sda_pin;      // I2C SDA 引脚
  uint8_t  device_addr;  // 设备地址 (7-bit or 10-bit)
  uint8_t  addr_width;   // 地址宽度 (7 or 10 bit)
} LCD_I2C_BusConfig_t;

struct _LCD_Resource_t
{
  LCD_Model_e    model_no; // LCD 型号
  LCD_Bus_Type_e bus_type; // 总线类型
  uint16_t       hor_res;  // Horizontal resolution
  uint16_t       ver_res;  // Vertical resolution

  uint8_t  reset_pin;        // Reset pin
  uint8_t  blk_pin;          // backlight pin
  uint8_t  pwm_pin;          // 如果需要调制 PWM
  uint16_t pwm_duty_default; // 当前 PWM 占空比
  uint16_t pwm_resolution;   // PWM 分辨率
  uint16_t pwm_frequency;    // PWM 频率

  union
  {
    LCD_GPIO_BusConfig_t    gpio;
    LCD_GPIOXMC_BusConfig_t xmc;
    LCD_UART_BusConfig_t    uart;
    LCD_SPI_BusConfig_t     spi;
    LCD_QSPI_BusConfig_t    qspi;
    LCD_I2C_BusConfig_t     i2c;
  } bus;

  LCD_Param_Init_f param_init;
};

/* Private function prototypes -----------------------------------------------*/
void LCD_WR_DATA(const struct _LCD_Resource_t *lcd_res, uint8_t data);
void LCD_WR_COMMAND(const struct _LCD_Resource_t *lcd_res, uint8_t command);
void __BSP_LCD_ST043A23_Param_Init(const struct _LCD_Resource_t *lcd_res);

/* Private variables ---------------------------------------------------------*/

/* Global variables ---------------------------------------------------------*/
const struct _LCD_Resource_t lcd_main_res = {
    .model_no         = LCD_SHINY_ST043A23_IPS,
    .bus_type         = LCD_BUS_TYPE_XMC,
    .hor_res          = SCREEN_MAIN_HOR_RES,
    .ver_res          = SCREEN_MAIN_VER_RES,
    .reset_pin        = SCREEN_RESET_PIN,
    .blk_pin          = SCREEN_BLK_PIN,
    .pwm_pin          = SCREEN_BLK_PIN,
    .pwm_duty_default = 500,
    .pwm_resolution   = 1000,
    .pwm_frequency    = 10000,

    .bus.xmc =
        {
            .bank      = SCREEN_XMC_BANK, // 使用 XMC Bank1
            .cmd_addr  = 0x6000FFFF,      // Bank1 基址
            .data_addr = 0x60010000,      // 基址 + 偏移（A16 为高时为数据）

            /* XMC 引脚配置（需要设置为 AF 复用功能） */
            .noe_pin = SCREEN_RD_PIN, // XMC_NOE -> LCD_RD
            .nwe_pin = SCREEN_WR_PIN, // XMC_NWE -> LCD_WR
            .ne_pin  = SCREEN_CS_PIN, // XMC_NE1 -> LCD_CS
            .dc_pin  = SCREEN_DC_PIN, // DC 引脚

            .d_pins =
                {
                    // D0-D7
                    SCREEN_XMC_D0_PIN,
                    SCREEN_XMC_D1_PIN,
                    SCREEN_XMC_D2_PIN,
                    SCREEN_XMC_D3_PIN,
                    SCREEN_XMC_D4_PIN,
                    SCREEN_XMC_D5_PIN,
                    SCREEN_XMC_D6_PIN,
                    SCREEN_XMC_D7_PIN,
                },

            .data_width = 8,
        },

    .param_init = __BSP_LCD_ST043A23_Param_Init,
};

/* Private function -----------------------------------------------*/

/* TODO: Core层XMC解耦 */
void __BSP_LCD_XMC_Init(const struct _LCD_Resource_t *lcd_res)
{
  xmc_norsram_init_type        xmc_norsram_init_struct;
  xmc_norsram_timing_init_type rw_timing_struct, w_timing_struct;
  gpio_init_type               gpio_init_struct;

  /* enable the xmc clock */
  crm_periph_clock_enable(CRM_XMC_PERIPH_CLOCK, TRUE);

  /* GPIO 初始化 */
  gpio_pin_remap_config(XMC_NADV_GMUX, TRUE);

  pinMode(lcd_res->bus.xmc.noe_pin, OUTPUT_AF_PP);
  pinMode(lcd_res->bus.xmc.nwe_pin, OUTPUT_AF_PP);
  pinMode(lcd_res->bus.xmc.ne_pin, OUTPUT_AF_PP);
  pinMode(lcd_res->bus.xmc.dc_pin, OUTPUT_AF_PP);
  for (int i = 0; i < lcd_res->bus.xmc.data_width; i++)
  {
    pinMode(lcd_res->bus.xmc.d_pins[i], OUTPUT_AF_PP);
  }
  pinMode(lcd_res->reset_pin, OUTPUT);
  pinMode(lcd_res->blk_pin, PWM);

  /* XMC 初始化 */
  xmc_norsram_default_para_init(&xmc_norsram_init_struct);
  xmc_norsram_init_struct.subbank             = lcd_res->bus.xmc.bank;
  xmc_norsram_init_struct.data_addr_multiplex = XMC_DATA_ADDR_MUX_DISABLE;
  xmc_norsram_init_struct.device              = XMC_DEVICE_SRAM;
  xmc_norsram_init_struct.bus_type            = XMC_BUSTYPE_8_BITS;
  xmc_norsram_init_struct.burst_mode_enable   = XMC_BURST_MODE_DISABLE;
  xmc_norsram_init_struct.asynwait_enable     = XMC_ASYN_WAIT_DISABLE;
  xmc_norsram_init_struct.wait_signal_lv      = XMC_WAIT_SIGNAL_LEVEL_LOW;
  xmc_norsram_init_struct.wrapped_mode_enable = XMC_WRAPPED_MODE_DISABLE;
  xmc_norsram_init_struct.wait_signal_config  = XMC_WAIT_SIGNAL_SYN_BEFORE;
  xmc_norsram_init_struct.write_enable        = XMC_WRITE_OPERATION_ENABLE;
  xmc_norsram_init_struct.wait_signal_enable  = XMC_WAIT_SIGNAL_DISABLE;
  xmc_norsram_init_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  xmc_norsram_init_struct.write_burst_syn     = XMC_WRITE_BURST_SYN_DISABLE;
  xmc_nor_sram_init(&xmc_norsram_init_struct);

  /* timing configuration */
  xmc_norsram_timing_default_para_init(&rw_timing_struct, &w_timing_struct);
  rw_timing_struct.subbank             = lcd_res->bus.xmc.bank;
  rw_timing_struct.write_timing_enable = XMC_WRITE_TIMING_ENABLE;
  rw_timing_struct.addr_setup_time     = 0xb;
  rw_timing_struct.addr_hold_time      = 0x0;
  rw_timing_struct.data_setup_time     = 0xb;
  rw_timing_struct.bus_latency_time    = 0x0;
  rw_timing_struct.clk_psc             = 0x0;
  rw_timing_struct.data_latency_time   = 0x0;
  rw_timing_struct.mode                = XMC_ACCESS_MODE_A;
  w_timing_struct.subbank              = lcd_res->bus.xmc.bank;
  w_timing_struct.write_timing_enable  = XMC_WRITE_TIMING_ENABLE;
  w_timing_struct.addr_setup_time      = 0xb;
  w_timing_struct.addr_hold_time       = 0x0;
  w_timing_struct.data_setup_time      = 0xb;
  w_timing_struct.bus_latency_time     = 0x0;
  w_timing_struct.clk_psc              = 0x0;
  w_timing_struct.data_latency_time    = 0x0;
  w_timing_struct.mode                 = XMC_ACCESS_MODE_A;
  xmc_nor_sram_timing_config(&rw_timing_struct, &w_timing_struct);

  /* bus turnaround phase for consecutive read duration and consecutive write
   * duration */
  // xmc_ext_timing_config(lcd_res->bus.xmc.bank, 0x08, 0x08);

  /* enable lcd_res->bus.xmc.bank */
  xmc_nor_sram_enable(lcd_res->bus.xmc.bank, TRUE);
}

void __BSP_LCD_XMC_DMA_Init(const struct _LCD_Resource_t *lcd_res)
{
  dma_init_type dma_init_structure;
  crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);

  nvic_irq_enable(DMA2_Channel2_IRQn, 10, 0);
  dma_reset(DMA2_CHANNEL2);

  dma_default_para_init(&dma_init_structure);
  dma_init_structure.buffer_size = 0; // later set in transfer
  dma_init_structure.direction =
      DMA_DIR_MEMORY_TO_PERIPHERAL | DMA_DIR_MEMORY_TO_MEMORY;
  dma_init_structure.memory_base_addr      = 0; // later set in transfer
  dma_init_structure.memory_data_width     = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_structure.memory_inc_enable     = TRUE;
  dma_init_structure.peripheral_base_addr  = (lcd_res->bus.xmc.data_addr);
  dma_init_structure.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_structure.peripheral_inc_enable = FALSE;
  dma_init_structure.priority              = DMA_PRIORITY_HIGH;
  dma_init_structure.loop_mode_enable      = FALSE;
  dma_init(DMA2_CHANNEL2, &dma_init_structure);

  dma_flag_clear(DMA2_GL2_FLAG); // Clear all
  dma_interrupt_enable(DMA2_CHANNEL2, DMA_FDT_INT, TRUE);
  dma_interrupt_enable(DMA2_CHANNEL2, DMA_DTERR_INT, TRUE);
}

void __BSP_LCD_ST043A23_Param_Init(const struct _LCD_Resource_t *lcd_res)
{
  /* backlight PWM init */
  /* Reset LCD */
  LCD_RESET_HIGH(lcd_res->reset_pin);
  delay_ms(10);
  LCD_RESET_LOW(lcd_res->reset_pin);
  delay_ms(200);
  LCD_RESET_HIGH(lcd_res->reset_pin);
  delay_ms(120);

  LCD_WR_COMMAND(lcd_res, 0xff);
  LCD_WR_DATA(lcd_res, 0xa5);
  LCD_WR_COMMAND(lcd_res, 0xE7); // TE_output_en
  LCD_WR_DATA(lcd_res, 0x10);
  LCD_WR_COMMAND(lcd_res, 0x35); // TE_ interface_en
  LCD_WR_DATA(lcd_res, 0x00);    // 01
  LCD_WR_COMMAND(lcd_res, 0x36);
  LCD_WR_DATA(lcd_res, 0xC0); // hl4_3

  LCD_WR_COMMAND(lcd_res, 0x3A);
  LCD_WR_DATA(lcd_res, 0x01); // 01---565/00---666
  LCD_WR_COMMAND(lcd_res, 0x40);
  LCD_WR_DATA(lcd_res, 0x01); // 01:IPS/00:TN
  LCD_WR_COMMAND(lcd_res, 0x41);
  LCD_WR_DATA(lcd_res, 0x01);    // 01--8bit//03--16bit
  LCD_WR_COMMAND(lcd_res, 0x44); // VBP
  LCD_WR_DATA(lcd_res, 0x15);
  LCD_WR_COMMAND(lcd_res, 0x45); // VFP
  LCD_WR_DATA(lcd_res, 0x15);
  LCD_WR_COMMAND(lcd_res, 0x7d); // vdds_trim[2:0]
  LCD_WR_DATA(lcd_res, 0x03);
  LCD_WR_COMMAND(lcd_res,
                 0xc1); // avdd_clp_en avdd_clp[1:0] avcl_clp_en avcl_clp[1:0]
  LCD_WR_DATA(lcd_res, 0xbb);    // 0xbb 88 a2
  LCD_WR_COMMAND(lcd_res, 0xc2); // vgl_clp_en vgl_clp[2:0]
  LCD_WR_DATA(lcd_res, 0x05);
  LCD_WR_COMMAND(lcd_res, 0xc3); // vgl_clp_en vgl_clp[2:0]
  LCD_WR_DATA(lcd_res, 0x10);
  LCD_WR_COMMAND(lcd_res, 0xc6); // avdd_ratio_sel avcl_ratio_sel
                                 // vgh_ratio_sel[1:0] vgl_ratio_sel[1:0]
  LCD_WR_DATA(lcd_res, 0x3e);
  LCD_WR_COMMAND(lcd_res,
                 0xc7); // mv_clk_sel[1:0] avdd_clk_sel[1:0] avcl_clk_sel[1:0]
  LCD_WR_DATA(lcd_res, 0x25);
  LCD_WR_COMMAND(lcd_res, 0xc8); // VGL_CLK_sel
  LCD_WR_DATA(lcd_res, 0x21);
  LCD_WR_COMMAND(lcd_res, 0x7a); // user_vgsp
  LCD_WR_DATA(lcd_res, 0x51);    // 58
  LCD_WR_COMMAND(lcd_res, 0x6f); // user_gvdd
  LCD_WR_DATA(lcd_res, 0x49);    // 4F
  LCD_WR_COMMAND(lcd_res, 0x78); // user_gvcl
  LCD_WR_DATA(lcd_res, 0x57);    // 70
  LCD_WR_COMMAND(lcd_res, 0xc9);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_COMMAND(lcd_res, 0x67);
  LCD_WR_DATA(lcd_res, 0x11);
  // gate_ed
  LCD_WR_COMMAND(lcd_res, 0x51); // gate_st_o[7:0]
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_COMMAND(lcd_res, 0x52); // gate_ed_o[7:0]
  LCD_WR_DATA(lcd_res, 0x7D);    // 7A
  LCD_WR_COMMAND(lcd_res, 0x53); // gate_st_e[7:0]
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_COMMAND(lcd_res, 0x54); // gate_ed_e[7:0]
  LCD_WR_DATA(lcd_res, 0x7D);    // 7A
  // sorce
  LCD_WR_COMMAND(lcd_res, 0x46); // fsm_hbp_o[5:0]
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_COMMAND(lcd_res, 0x47); // fsm_hfp_o[5:0]
  LCD_WR_DATA(lcd_res, 0x2a);
  LCD_WR_COMMAND(lcd_res, 0x48); // fsm_hbp_e[5:0]
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_COMMAND(lcd_res, 0x49); // fsm_hfp_e[5:0]
  LCD_WR_DATA(lcd_res, 0x1a);
  LCD_WR_COMMAND(lcd_res, 0x44); // VBP
  LCD_WR_DATA(lcd_res, 0x15);
  LCD_WR_COMMAND(lcd_res, 0x45); // VFP
  LCD_WR_DATA(lcd_res, 0x15);
  LCD_WR_COMMAND(lcd_res, 0x73);
  LCD_WR_DATA(lcd_res, 0x08);
  LCD_WR_COMMAND(lcd_res, 0x74);
  LCD_WR_DATA(lcd_res, 0x10); // 0A
  /// test mode
  // LCD_WR_COMMAND(0xf8);
  // LCD_WR_DATA(0x16);
  // LCD_WR_COMMAND(0xf9);
  // LCD_WR_DATA(0x20);
  LCD_WR_COMMAND(lcd_res, 0x56); // src_ld_wd[1:0] src_ld_st[5:0]
  LCD_WR_DATA(lcd_res, 0x43);
  LCD_WR_COMMAND(lcd_res, 0x57); // pn_cs_en src_cs_st[5:0]
  LCD_WR_DATA(lcd_res, 0x42);
  LCD_WR_COMMAND(lcd_res, 0x58); // src_cs_p_wd[6:0]
  LCD_WR_DATA(lcd_res, 0x3c);
  LCD_WR_COMMAND(lcd_res, 0x59); // src_cs_n_wd[6:0]
  LCD_WR_DATA(lcd_res, 0x64);
  LCD_WR_COMMAND(lcd_res, 0x5a); // src_pchg_st_o[6:0]
  LCD_WR_DATA(lcd_res, 0x41);
  LCD_WR_COMMAND(lcd_res, 0x5b); // src_pchg_wd_o[6:0]
  LCD_WR_DATA(lcd_res, 0x3C);
  LCD_WR_COMMAND(lcd_res, 0x5c); // src_pchg_st_e[6:0]
  LCD_WR_DATA(lcd_res, 0x02);
  LCD_WR_COMMAND(lcd_res, 0x5d); // src_pchg_wd_e[6:0]
  LCD_WR_DATA(lcd_res, 0x3c);
  LCD_WR_COMMAND(lcd_res, 0x5e); // src_pol_sw[7:0]
  LCD_WR_DATA(lcd_res, 0x1f);
  LCD_WR_COMMAND(lcd_res, 0x60); // src_op_st_o[7:0]
  LCD_WR_DATA(lcd_res, 0x80);
  LCD_WR_COMMAND(lcd_res, 0x61); // src_op_st_e[7:0]
  LCD_WR_DATA(lcd_res, 0x3f);
  LCD_WR_COMMAND(lcd_res, 0x62); // src_op_ed_o[9:8] src_op_ed_e[9:8]
  LCD_WR_DATA(lcd_res, 0x21);
  LCD_WR_COMMAND(lcd_res, 0x63); // src_op_ed_o[7:0]
  LCD_WR_DATA(lcd_res, 0x07);
  LCD_WR_COMMAND(lcd_res, 0x64); // src_op_ed_e[7:0]
  LCD_WR_DATA(lcd_res, 0xe0);
  LCD_WR_COMMAND(lcd_res, 0x65); // chopper
  LCD_WR_DATA(lcd_res, 0x02);
  LCD_WR_COMMAND(lcd_res, 0xca); // avdd_mux_st_o[7:0]
  LCD_WR_DATA(lcd_res, 0x20);
  LCD_WR_COMMAND(lcd_res, 0xcb); // avdd_mux_ed_o[7:0]
  LCD_WR_DATA(lcd_res, 0x52);
  LCD_WR_COMMAND(lcd_res, 0xcc); // avdd_mux_st_e[7:0]
  LCD_WR_DATA(lcd_res, 0x10);
  LCD_WR_COMMAND(lcd_res, 0xcd); // avdd_mux_ed_e[7:0]
  LCD_WR_DATA(lcd_res, 0x42);
  LCD_WR_COMMAND(lcd_res, 0xd0); // avcl_mux_st_o[7:0]
  LCD_WR_DATA(lcd_res, 0x20);
  LCD_WR_COMMAND(lcd_res, 0xd1); // avcl_mux_ed_o[7:0]
  LCD_WR_DATA(lcd_res, 0x52);
  LCD_WR_COMMAND(lcd_res, 0xd2); // avcl_mux_st_e[7:0]
  LCD_WR_DATA(lcd_res, 0x10);
  LCD_WR_COMMAND(lcd_res, 0xd3); // avcl_mux_ed_e[7:0]
  LCD_WR_DATA(lcd_res, 0x42);
  LCD_WR_COMMAND(lcd_res, 0xd4); // vgh_mux_st[7:0]
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_COMMAND(lcd_res, 0xd5); // vgh_mux_ed[7:0]
  LCD_WR_DATA(lcd_res, 0x32);
  // gammma boe4.3
  LCD_WR_COMMAND(lcd_res, 0x80); // gam_vrp0
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_COMMAND(lcd_res, 0xa0); // gam_VRN0
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_COMMAND(lcd_res, 0x81); // gam_vrp1
  LCD_WR_DATA(lcd_res, 0x06);    // 07
  LCD_WR_COMMAND(lcd_res, 0xa1); // gam_VRN1
  LCD_WR_DATA(lcd_res, 0x08);    // 06
  LCD_WR_COMMAND(lcd_res, 0x82); // gam_vrp2
  LCD_WR_DATA(lcd_res, 0x03);    // 02
  LCD_WR_COMMAND(lcd_res, 0xa2); // gam_VRN2
  LCD_WR_DATA(lcd_res, 0x03);    // 01
  LCD_WR_COMMAND(lcd_res, 0x86); // gam_prp0
  LCD_WR_DATA(lcd_res, 0x14);    // 11
  LCD_WR_COMMAND(lcd_res, 0xa6); // gam_PRN0
  LCD_WR_DATA(lcd_res, 0x14);    // 10
  LCD_WR_COMMAND(lcd_res, 0x87); // gam_prp1
  LCD_WR_DATA(lcd_res, 0x2C);    // 27
  LCD_WR_COMMAND(lcd_res, 0xa7); // gam_PRN1
  LCD_WR_DATA(lcd_res, 0x26);    // 27
  LCD_WR_COMMAND(lcd_res, 0x83); // gam_vrp3
  LCD_WR_DATA(lcd_res, 0x37);
  LCD_WR_COMMAND(lcd_res, 0xa3); // gam_VRN3
  LCD_WR_DATA(lcd_res, 0x37);
  LCD_WR_COMMAND(lcd_res, 0x84); // gam_vrp4
  LCD_WR_DATA(lcd_res, 0x35);
  LCD_WR_COMMAND(lcd_res, 0xa4); // gam_VRN4
  LCD_WR_DATA(lcd_res, 0x35);
  LCD_WR_COMMAND(lcd_res, 0x85); // gam_vrp5
  LCD_WR_DATA(lcd_res, 0x3f);
  LCD_WR_COMMAND(lcd_res, 0xa5); // gam_VRN5
  LCD_WR_DATA(lcd_res, 0x3f);
  LCD_WR_COMMAND(lcd_res, 0x88); // gam_pkp0
  LCD_WR_DATA(lcd_res, 0x0A);    // 0b
  LCD_WR_COMMAND(lcd_res, 0xa8); // gam_PKN0
  LCD_WR_DATA(lcd_res, 0x0A);    // 0b
  LCD_WR_COMMAND(lcd_res, 0x89); // gam_pkp1
  LCD_WR_DATA(lcd_res, 0x13);    // 14
  LCD_WR_COMMAND(lcd_res, 0xa9); // gam_PKN1
  LCD_WR_DATA(lcd_res, 0x12);    // 13
  LCD_WR_COMMAND(lcd_res, 0x8a); // gam_pkp2
  LCD_WR_DATA(lcd_res, 0x18);    // 1a
  LCD_WR_COMMAND(lcd_res, 0xaa); // gam_PKN2
  LCD_WR_DATA(lcd_res, 0x19);    // 1a
  LCD_WR_COMMAND(lcd_res, 0x8b); // gam_PKP3
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_COMMAND(lcd_res, 0xab); // gam_PKN3
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_COMMAND(lcd_res, 0x8c); // gam_PKP4
  LCD_WR_DATA(lcd_res, 0x17);    // 14
  LCD_WR_COMMAND(lcd_res, 0xac); // gam_PKN4
  LCD_WR_DATA(lcd_res, 0x0B);    // 08
  LCD_WR_COMMAND(lcd_res, 0x8d); // gam_PKP5
  LCD_WR_DATA(lcd_res, 0x1A);    // 17
  LCD_WR_COMMAND(lcd_res, 0xad); // gam_PKN5
  LCD_WR_DATA(lcd_res, 0x09);    // 07
  LCD_WR_COMMAND(lcd_res, 0x8e); // gam_PKP6
  LCD_WR_DATA(lcd_res, 0x1A);    // 16 //16
  LCD_WR_COMMAND(lcd_res, 0xae); // gam_PKN6
  LCD_WR_DATA(lcd_res, 0x08);    // 06 //13
  LCD_WR_COMMAND(lcd_res, 0x8f); // gam_PKP7
  LCD_WR_DATA(lcd_res, 0x1F);    // 1B
  LCD_WR_COMMAND(lcd_res, 0xaf); // gam_PKN7
  LCD_WR_DATA(lcd_res, 0x00);    // 07
  LCD_WR_COMMAND(lcd_res, 0x90); // gam_PKP8
  LCD_WR_DATA(lcd_res, 0x08);    // 04
  LCD_WR_COMMAND(lcd_res, 0xb0); // gam_PKN8
  LCD_WR_DATA(lcd_res, 0x00);    // 04
  LCD_WR_COMMAND(lcd_res, 0x91); // gam_PKP9
  LCD_WR_DATA(lcd_res, 0x10);    // 0A
  LCD_WR_COMMAND(lcd_res, 0xb1); // gam_PKN9
  LCD_WR_DATA(lcd_res, 0x06);    // 0A
  LCD_WR_COMMAND(lcd_res, 0x92); // gam_PKP10
  LCD_WR_DATA(lcd_res, 0x19);    // 16
  LCD_WR_COMMAND(lcd_res, 0xb2); // gam_PKN10
  LCD_WR_DATA(lcd_res, 0x15);    // 15
  LCD_WR_COMMAND(lcd_res, 0xff);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_COMMAND(lcd_res, 0x2A);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x01);
  LCD_WR_DATA(lcd_res, 0xDF);

  LCD_WR_COMMAND(lcd_res, 0x2B);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x01);
  LCD_WR_DATA(lcd_res, 0x0F);

  LCD_WR_COMMAND(lcd_res, 0x11);
  delay_ms(120);
  LCD_WR_COMMAND(lcd_res, 0x29);
  delay_ms(20);
  LCD_WR_COMMAND(lcd_res, 0x2C);
}

void __BSP_LCD_3_5_Param_Init(const struct _LCD_Resource_t *lcd_res)
{
  /* Reset LCD */
  LCD_RESET_HIGH(lcd_res->reset_pin);
  delay_ms(10);
  LCD_RESET_LOW(lcd_res->reset_pin);
  delay_ms(10);
  LCD_RESET_HIGH(lcd_res->reset_pin);
  delay_ms(10);

  /*Software Reset*/
  LCD_WR_COMMAND(lcd_res, 0x01);
  delay_ms(20);
  /* Read ID*/
  /*	LCD_WR_COMMAND(0X00);
    //Delay_us(1);
    lcd->lcd_id = LCD_RD_DATA();
    LCD_WR_COMMAND(0XD3);
    lcd->lcd_id = LCD_RD_DATA();
    lcd->lcd_id = LCD_RD_DATA();
    lcd->lcd_id = LCD_RD_DATA();
    lcd->lcd_id = LCD_RD_DATA();
    lcd->lcd_id = lcd->lcd_id<<8;
    lcd->lcd_id |= LCD_RD_DATA();*/

  LCD_WR_COMMAND(lcd_res, 0xe0);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x0e);
  LCD_WR_DATA(lcd_res, 0x15);
  LCD_WR_DATA(lcd_res, 0x06);
  LCD_WR_DATA(lcd_res, 0x13);
  LCD_WR_DATA(lcd_res, 0x09);
  LCD_WR_DATA(lcd_res, 0x3a);
  LCD_WR_DATA(lcd_res, 0xac);
  LCD_WR_DATA(lcd_res, 0x4f);
  LCD_WR_DATA(lcd_res, 0x05);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x0b);
  LCD_WR_DATA(lcd_res, 0x33);
  LCD_WR_DATA(lcd_res, 0x3b);
  LCD_WR_DATA(lcd_res, 0x0f);

  LCD_WR_COMMAND(lcd_res, 0xe1);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x0e);
  LCD_WR_DATA(lcd_res, 0x16);
  LCD_WR_DATA(lcd_res, 0x05);
  LCD_WR_DATA(lcd_res, 0x13);
  LCD_WR_DATA(lcd_res, 0x08);
  LCD_WR_DATA(lcd_res, 0x3b);
  LCD_WR_DATA(lcd_res, 0x9a);
  LCD_WR_DATA(lcd_res, 0x50);
  LCD_WR_DATA(lcd_res, 0x0a);
  LCD_WR_DATA(lcd_res, 0x13);
  LCD_WR_DATA(lcd_res, 0x0f);
  LCD_WR_DATA(lcd_res, 0x31);
  LCD_WR_DATA(lcd_res, 0x36);
  LCD_WR_DATA(lcd_res, 0x0f);

  LCD_WR_COMMAND(lcd_res, 0xc0);
  LCD_WR_DATA(lcd_res, 0x10);
  LCD_WR_DATA(lcd_res, 0x10);

  LCD_WR_COMMAND(lcd_res, 0xc1);
  LCD_WR_DATA(lcd_res, 0x44);

  LCD_WR_COMMAND(lcd_res, 0xc5);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x05);
  LCD_WR_DATA(lcd_res, 0x80);

  LCD_WR_COMMAND(lcd_res, 0x3a);
  LCD_WR_DATA(lcd_res, 0x55);

  LCD_WR_COMMAND(lcd_res, 0x36);
  // LCD_WR_DATA((1<<5)|(0<<6)|(1<<3));
  // LCD_WR_DATA((1<<5)|(1<<6)|(1<<7)|(1<<3));
  LCD_WR_DATA(lcd_res, (1 << 5) | (1 << 3));
  LCD_WR_COMMAND(lcd_res, 0xb0);
  LCD_WR_DATA(lcd_res, 0x00);

  LCD_WR_COMMAND(lcd_res, 0xb1);
  LCD_WR_DATA(lcd_res, 0xd0);
  LCD_WR_DATA(lcd_res, 0x1f);

  LCD_WR_COMMAND(lcd_res, 0xb4);
  LCD_WR_DATA(lcd_res, 0x02);

  LCD_WR_COMMAND(lcd_res, 0xb6);
  LCD_WR_DATA(lcd_res, 0x02);
  LCD_WR_DATA(lcd_res, 0x22);

  LCD_WR_COMMAND(lcd_res, 0x35);
  LCD_WR_DATA(lcd_res, 0x00);

  LCD_WR_COMMAND(lcd_res, 0xf7);
  LCD_WR_DATA(lcd_res, 0xa9);
  LCD_WR_DATA(lcd_res, 0x51);
  LCD_WR_DATA(lcd_res, 0x2c);
  LCD_WR_DATA(lcd_res, 0x82);

  LCD_WR_COMMAND(lcd_res, 0x2a);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x01);
  LCD_WR_DATA(lcd_res, 0x3f);
  LCD_WR_COMMAND(lcd_res, 0x2b);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x00);
  LCD_WR_DATA(lcd_res, 0x01);
  LCD_WR_DATA(lcd_res, 0xDf);

  LCD_WR_COMMAND(lcd_res, 0x11); // Exit Sleep
  delay_ms(120);
  LCD_WR_COMMAND(lcd_res, 0x29); // display on
  delay_ms(10);
}

inline void LCD_WR_COMMAND(const struct _LCD_Resource_t *lcd_res,
                           uint8_t                       command)
{
  switch (lcd_res->bus_type)
  {
    case LCD_BUS_TYPE_GPIO:
      break;
    case LCD_BUS_TYPE_XMC:
      *(__IO uint8_t *)lcd_res->bus.xmc.cmd_addr = command;
      break;
    case LCD_BUS_TYPE_I2C:
      break;
    case LCD_BUS_TYPE_SPI:
      break;
    case LCD_BUS_TYPE_QSPI:
      break;
    case LCD_BUS_TYPE_UART:
      break;
  }
}

inline void LCD_WR_DATA(const struct _LCD_Resource_t *lcd_res, uint8_t data)
{
  switch (lcd_res->bus_type)
  {
    case LCD_BUS_TYPE_GPIO:
      break;
    case LCD_BUS_TYPE_XMC:
      *(__IO uint8_t *)lcd_res->bus.xmc.data_addr = data;
      break;
    case LCD_BUS_TYPE_I2C:
      break;
    case LCD_BUS_TYPE_SPI:
      break;
    case LCD_BUS_TYPE_QSPI:
      break;
    case LCD_BUS_TYPE_UART:
      break;
  }
}

/* Public function --------------------------------------------------*/
void BSP_LCD_Init(const void *lcd)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;

  switch (lcd_res->bus_type)
  {
    case LCD_BUS_TYPE_GPIO:
      break;
    case LCD_BUS_TYPE_XMC:
      log_i("Initializing LCD via XMC interface...");
      __BSP_LCD_XMC_Init(lcd_res);
      __BSP_LCD_XMC_DMA_Init(lcd_res);
      // delay_ms(50);
      lcd_res->param_init(lcd_res);

      PWM_Write(lcd_res->pwm_pin, lcd_res->pwm_duty_default);
      tmr_channel_enable(TMR2, TMR_SELECT_CHANNEL_1, TRUE);
      break;
    case LCD_BUS_TYPE_I2C:
      break;
    case LCD_BUS_TYPE_SPI:
      break;
    case LCD_BUS_TYPE_QSPI:
      break;
    case LCD_BUS_TYPE_UART:
      break;
  }
}

void BSP_LCD_DeInit(const void *lcd)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;

  switch (lcd_res->bus_type)
  {
    case LCD_BUS_TYPE_GPIO:
      break;
    case LCD_BUS_TYPE_XMC:
      log_i("DeInitializing LCD ...");
      // xmc_nor_sram_enable(lcd_res->bus.xmc.bank, FALSE);
      break;
    case LCD_BUS_TYPE_I2C:
      break;
    case LCD_BUS_TYPE_SPI:
      break;
    case LCD_BUS_TYPE_QSPI:
      break;
    case LCD_BUS_TYPE_UART:
      break;
  }
}

/**
 * @brief  this function is set row&column coordinates for lcd.
 * @param  xstart : row coordinates starting vaule.
 * @param  ystart : column coordinates starting vaule.
 * @param  xend : row coordinates ending vaule.
 * @param  yend : column coordinates ending vaule.
 * @retval none
 */
void BSP_LCD_Set_Block(const void *lcd,
                       uint16_t    xstart,
                       uint16_t    ystart,
                       uint16_t    xend,
                       uint16_t    yend)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  /* set row coordinates */
  LCD_WR_COMMAND(lcd_res, 0x2a);
  LCD_WR_DATA(lcd_res, xstart >> 8);
  LCD_WR_DATA(lcd_res, xstart & 0xff);
  LCD_WR_DATA(lcd_res, xend >> 8);
  LCD_WR_DATA(lcd_res, xend & 0xff);

  /* set column coordinates */
  LCD_WR_COMMAND(lcd_res, 0x2b);
  LCD_WR_DATA(lcd_res, ystart >> 8);
  LCD_WR_DATA(lcd_res, ystart & 0xff);
  LCD_WR_DATA(lcd_res, yend >> 8);
  LCD_WR_DATA(lcd_res, yend & 0xff);
  /* enable write menory */
  LCD_WR_COMMAND(lcd_res, 0x2c);
}

void BSP_LCD_Write_One_Point(const void *lcd, uint16_t color)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  LCD_WR_DATA(lcd_res, color >> 8);
  LCD_WR_DATA(lcd_res, color);
}

void BSP_LCD_Write_One_Block(const void *lcd, uint16_t *color, uint32_t size)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  uint32_t                      i;
  for (i = 0; i < size; i++)
  {
    // BSP_LCD_Write_One_Point(lcd_res, color);
    LCD_WR_DATA(lcd_res, color[i] >> 8);
    LCD_WR_DATA(lcd_res, color[i]);
  }
}

void BSP_LCD_DrawPoint(const void *lcd, uint16_t x, uint16_t y, uint16_t color)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  BSP_LCD_Set_Block(lcd_res, x, y, x, y);
  BSP_LCD_Write_One_Point(lcd_res, color);
}

void BSP_LCD_Clear(const void *lcd, uint16_t color)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  uint32_t                      i;
  BSP_LCD_Set_Block(lcd_res, 0, 0, lcd_res->hor_res - 1, lcd_res->ver_res - 1);
  for (i = 0; i < lcd_res->hor_res * lcd_res->ver_res; i++)
  {
    BSP_LCD_Write_One_Point(lcd_res, color);
  }
}

void BSP_LCD_Blk_On(const void *lcd)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  PWM_Write(lcd_res->pwm_pin, lcd_res->pwm_duty_default);
}

void BSP_LCD_Blk_Off(const void *lcd)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  PWM_Write(lcd_res->pwm_pin, 0);
}

void BSP_LCD_Blk_Set_PWM(const void *lcd, uint16_t duty)
{
  const struct _LCD_Resource_t *lcd_res = (const struct _LCD_Resource_t *)lcd;
  PWM_Write(lcd_res->pwm_pin, duty);
}
