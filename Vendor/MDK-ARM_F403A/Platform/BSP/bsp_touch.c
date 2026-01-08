/******************************************************************************
 * Copyright (C) 2025 UNION, or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_touch.c
 * 
 * @author Yedou | UNION
 * 
 * @brief Provide the BSP APIs of touch and corresponding options.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-12-02 @author Yedou
 *
 * @note 1 tab == 2 spaces!
 * 
 *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mcu_core.h"
#include "Logger.h"

#include "bsp_touch.h"
#include "i2c.h"

/* Private define ------------------------------------------------------------*/

typedef enum
{
  TOUCH_IC_NS2009,      // 电阻屏，通过 I2C 读压感
  TOUCH_IC_GT911,       // 电容屏
  TOUCH_IC_FT5X06,      // 电容屏
  TOUCH_IC_CST816,      // 电容屏
} Touch_IC_e;

typedef enum
{
  NS2009_ADDRESS_WR = 0x92,        /* b(1001 0010) */
  NS2009_ADDRESS_RD = 0x93,        /* b(1001 0011) */
  NS2009_LOW_POWER_READ_X  = 0xc4, /* 读取时关闭中断 1100 0000 -> 1100 0100 */
  NS2009_LOW_POWER_READ_Y  = 0xd4, /* 读取时关闭中断 1101 0000 -> 1101 0100 */
  NS2009_LOW_POWER_READ_Z1 = 0xe4, /* 读取时关闭中断 1110 0000 -> 1110 0100 */
  NS2009_LOW_POWER_READ_Z2 = 0xf4, /* 读取时关闭中断 1111 0000 -> 1111 0100 */
  NS2009_LONG_DRIVER_READ_X  = 0x80, /* 读取时开启中断 1000 0000 -> 1000 0000 */
  NS2009_LONG_DRIVER_READ_Y  = 0x90, /* 读取时开启中断 1001 0000 -> 1001 0000 */
  NS2009_LONG_DRIVER_READ_Z1 = 0xA0, /* 读取时开启中断 1010 0000 -> 1010 0000 */
  NS2009_LONG_DRIVER_READ_Z2 = 0xB0, /* 读取时开启中断 1011 0000 -> 1011 0000 */
  NS2009_ONLY_OPEN_IRQ       = 0x00, /* 只用于重新开启中断 */
} NS2009_Cmd_e;

typedef enum
{
  TOUCH_BUS_I2C,        // I2C，例如：GT911、FT5426、NS2009（I2C模式）
  TOUCH_BUS_SPI,        // SPI，例如：XPT2046
  TOUCH_BUS_ADC,        // ADC + GPIO（模拟电阻屏）
} Touch_Bus_Type_e;

typedef enum
{
  TOUCH_PRESSED  = 0,
  TOUCH_RELEASED = 1,
  TOUCH_IDLE = 2,
} Touch_State_e;

/* 通用触摸数据 */
typedef struct
{
  uint16_t x;
  uint16_t y;
  uint16_t z1;
  uint16_t z2;
  uint8_t  pressed;     // 是否按下
} TouchPoint_t;

/* 触摸读函数回调 */
typedef void (*Touch_Read_f)(const struct _Touch_Resource_t *res, TouchPoint_t *p);
typedef void (*Touch_Init_f)(const struct _Touch_Resource_t *res);
typedef void (*Touch_IRQ_Handler_f)(void);

/* I2C 总线 */
typedef struct
{
  I2C_TypeDef *i2c_instance;
  uint8_t  scl_pin;
  uint8_t  sda_pin;
  uint32_t speed;
  uint8_t  dev_addr_wr;
  uint8_t  dev_addr_rd;
} Touch_I2C_Bus_t;

/* SPI 总线 */
typedef struct
{
  uint8_t  spi_instance;
  uint8_t  sck_pin;
  uint8_t  miso_pin;
  uint8_t  mosi_pin;
  uint8_t  cs_pin;
  uint32_t speed;
} Touch_SPI_Bus_t;

/* ADC 方案 */
typedef struct
{
  uint8_t  adc_x;       // X+ 接 ADC 通道
  uint8_t  adc_y;       // Y+ 接 ADC 通道

  uint8_t  xp_pin;      // X+ GPIO
  uint8_t  xm_pin;      // X- GPIO
  uint8_t  yp_pin;      // Y+ GPIO
  uint8_t  ym_pin;      // Y- GPIO
} Touch_ADC_Bus_t;

struct _Touch_Resource_t
{
  Touch_IC_e         ic_type;
  Touch_Bus_Type_e   bus_type;

  uint16_t           hor_res;
  uint16_t           ver_res;
  float              ratio_x;        // X 轴比例系数
  float              ratio_y;        // Y 轴比例系数
  float              pres_min;       // 最小压力值
  float              pres_max;       // 最大压力值

  uint8_t            rst_pin;
  uint8_t            irq_pin;
  EXTI_POLARITY_TYPE irq_polarity;

  /* 硬件地址控制位 */
  uint8_t            a0_pin;
  uint8_t            a1_pin;

  int rotation;         // 触摸旋转方向（0/90/180/270）

  union
  {
    Touch_I2C_Bus_t i2c;
    Touch_SPI_Bus_t spi;
    Touch_ADC_Bus_t adc;
  } bus;

  Touch_Init_f        init;
  Touch_Read_f        read;
  Touch_IRQ_Handler_f irq_handler;
};

typedef struct _Touch_Handler_t
{
  const Touch_Resource_t* resource;
  Touch_State_e           state;
  bool                    debounce_active;
  uint16_t                debounce_tick;
  uint16_t                noise_cnt;
} Touch_Handler_t;


/* Private function prototypes ----------------------------------------------*/
static void     __NS2009_PENIRQ_Handler(void);
static void     __NS2009_Init(const Touch_Resource_t* touch_res);
static uint16_t __NS2009_Read(const Touch_Resource_t* touch_res, NS2009_Cmd_e cmd);
static void     __NS2009_OpenPENIRQ(const Touch_Resource_t* touch_res);
static uint8_t  __NS2009_Calculate_Pressure(TouchPoint_t* p);
static void     __NS2009_ReadCoordinate(const Touch_Resource_t* touch_res, TouchPoint_t* p);


/* Private variables ---------------------------------------------------------*/
static Touch_State_e touch_main_state = TOUCH_RELEASED;
static Touch_Handler_t touch_main_handler = {
  .resource        = &touch_main_res,
  .state           = TOUCH_RELEASED,
  .debounce_active = false,
  .debounce_tick   = 0,
  .noise_cnt       = 0,
};


/* Global variables ---------------------------------------------------------*/
__IO uint8_t g_ns2009_irq_flag = 0;

const Touch_Resource_t touch_main_res =
{
  .ic_type      = TOUCH_IC_NS2009,
  .bus_type     = TOUCH_BUS_I2C,
  .hor_res      = SCREEN_MAIN_HOR_RES,
  .ver_res      = SCREEN_MAIN_VER_RES,
  .ratio_x      = (float)(SCREEN_MAIN_HOR_RES-1) / TOUCH_NS2009_ADC_RESO,
  .ratio_y      = (float)(SCREEN_MAIN_VER_RES-1) / TOUCH_NS2009_ADC_RESO,
  .pres_min     = TOUCH_NS2009_PRESSURE_MIN,
  .pres_max     = TOUCH_NS2009_PRESSURE_MAX,
  .a0_pin       = TOUCH_NS2009_A0_PIN,
  .irq_pin      = TOUCH_NS2009_INT_PIN,
  .irq_polarity = CHANGE,
  .rotation     = 0,

  .bus.i2c =
  {
    .i2c_instance = I2C1,
    .scl_pin      = TOUCH_NS2009_I2C_SCL_PIN,
    .sda_pin      = TOUCH_NS2009_I2C_SDA_PIN,
    .speed        = I2C_SPEED_STANDARD,
    .dev_addr_wr  = NS2009_ADDRESS_WR,
    .dev_addr_rd  = NS2009_ADDRESS_RD,
  },

  .init        = __NS2009_Init,
  .read        = __NS2009_ReadCoordinate,
  .irq_handler = __NS2009_PENIRQ_Handler,
};

/* Private function ---------------------------------------------------------*/
#include "lvgl.h"
extern lv_timer_t * volatile indev_touchpad_timer;
static void __NS2009_PENIRQ_Handler(void)
{
  uint32_t cur_state = digitalRead_FAST(touch_main_res.irq_pin) ? TOUCH_RELEASED : TOUCH_PRESSED;

  log_d("NS2009 PENIRQ Handler: State=%d", cur_state);

  // if ( cur_state != touch_main_handler.state ) 
  // {
  //   touch_main_handler.debounce_active = true;
  //   touch_main_handler.state = (Touch_State_e)cur_state;
  //   touch_main_handler.debounce_tick = 20; // 20ms 消抖时间
  //   touch_main_handler.noise_cnt = 0;
  // }

  /* TODO: 使用事件驱动 */
  g_ns2009_irq_flag = 1;
  lv_timer_resume(indev_touchpad_timer);
}


void NS2009_TickHandler(void)
{

  if (touch_main_handler.debounce_tick > 0) 
  {
    uint32_t cur_state = digitalRead_FAST(touch_main_res.irq_pin) ? TOUCH_RELEASED : TOUCH_PRESSED;
    
    if (cur_state == touch_main_handler.state)
    {
      // 状态与预期一致，继续计时
      touch_main_handler.debounce_tick--;
      
      if (touch_main_handler.debounce_tick == 0) 
      {
        touch_main_state = touch_main_handler.state;
        log_i("NS2009 Touch State Changed: %d", touch_main_state);

        if (touch_main_state == TOUCH_PRESSED) {
          g_ns2009_irq_flag = 1;
          touch_main_handler.state = TOUCH_IDLE;
          if(indev_touchpad_timer) lv_timer_resume(indev_touchpad_timer);
          // touch_main_handler.debounce_active = false;
        }
        // else if (touch_main_state == TOUCH_RELEASED) {
        //   g_ns2009_irq_flag = 1;
        //   /* 释放事件 */
        // }
      }
    }
    else //(cur_state != touch_main_handler.state) 
    {
      // 状态变化，记作噪声, 若干扰过多则放弃本次状态变化
      touch_main_handler.noise_cnt++;

      if (touch_main_handler.noise_cnt > 2) 
      {

        touch_main_handler.debounce_tick = 0;
        touch_main_handler.state = TOUCH_IDLE;
        // touch_main_handler.debounce_active = false;
        log_w("NS2009 Touch State Change Aborted Due to Excessive Noise");
      }
      else
      {
        // log_d("NS2009 Touch State Noise Detected: %d (Count=%d)", cur_state, touch_main_handler.noise_cnt);
      }

      return;
    }
  }
}

static void __NS2009_Init(const Touch_Resource_t* touch_res)
{
  log_i("Initializing NS2009 touch driver...");

  /* MSOP-10封装, A0引脚 */
  pinMode(touch_res->a0_pin, OUTPUT);
  digitalWrite_HIGH(touch_res->a0_pin);

  I2Cx_Init(touch_res->bus.i2c.i2c_instance, touch_res->bus.i2c.speed);

  attachInterrupt(touch_res->irq_pin, __NS2009_PENIRQ_Handler, touch_res->irq_polarity);

  uint8_t test_cmd = NS2009_ONLY_OPEN_IRQ;
  Status_t status = I2Cx_Master_Write(touch_res->bus.i2c.i2c_instance, 
                                      touch_res->bus.i2c.dev_addr_wr, 
                                      &test_cmd, 1);
  if (status == kStatus_Success) {
    log_i("NS2009 initialization successful");
  } else {
    log_e("NS2009 initialization failed: %d", status);
  }
}

static uint16_t __NS2009_Read(const Touch_Resource_t* touch_res, NS2009_Cmd_e cmd)
{
  Status_t status;
  uint8_t  buf[2];
  uint16_t value = 0;

  status = I2Cx_Master_Write(touch_res->bus.i2c.i2c_instance, touch_res->bus.i2c.dev_addr_wr, &cmd, 1);
  if (status != kStatus_Success) {
    log_e("NS2009 I2C write command failed: %d", status);
    return 0;
  }

  status = I2Cx_Master_Read(touch_res->bus.i2c.i2c_instance, touch_res->bus.i2c.dev_addr_rd, buf, 2);
  if (status != kStatus_Success) {
    log_e("NS2009 I2C read command failed: %d", status);
    return 0;
  }

  value = ((uint16_t)(buf[0] << 8) | buf[1]) >> 4; // 12位数据，右对齐

  return value;
}

static void __NS2009_OpenPENIRQ(const Touch_Resource_t* touch_res)
{ 
  uint8_t cmd = NS2009_ONLY_OPEN_IRQ;
  I2Cx_Master_Write(touch_res->bus.i2c.i2c_instance, touch_res->bus.i2c.dev_addr_wr, &cmd, 1);
}

/**
 * @brief  计算触摸压力并判断是否按下
 * @param  p: 触摸点数据
 * @return 0=未按下, 1=按下
 */
static uint8_t __NS2009_Calculate_Pressure(TouchPoint_t* p)
{
  float pressure;
  uint16_t x, y, z1, z2;
  x  = p->x;
  y  = p->y;
  z1 = p->z1;
  z2 = p->z2;

  // NS2009 压力计算公式：
  // Pressure = (X/4096) * [(Z2/Z1) - 1]
  if (z1 == 0) {
    return 0; // 避免除零错误
  }
  
  pressure = ((float)x / TOUCH_NS2009_ADC_RESO) * (((float)z2 / (float)z1) - 1.0f);
  // log_d("NS2009 Calculated Pressure: %.2f", pressure);
  
  if (pressure < touch_main_res.pres_min || pressure > touch_main_res.pres_max) {
    return 0; // 超出压力范围，认为未按下
  }

  return 1; // 按下
}

static void __NS2009_ReadCoordinate(const Touch_Resource_t* touch_res, TouchPoint_t* p)
{
  /* 防止重复触发中断 */
  detachInterrupt(touch_main_res.irq_pin);

  uint8_t buf[4];
  uint16_t x, y, z1, z2;

  x  = __NS2009_Read(touch_res, NS2009_LOW_POWER_READ_X);
  y  = __NS2009_Read(touch_res, NS2009_LOW_POWER_READ_Y);
  z1 = __NS2009_Read(touch_res, NS2009_LOW_POWER_READ_Z1);
  z2 = __NS2009_Read(touch_res, NS2009_LOW_POWER_READ_Z2);
  __NS2009_OpenPENIRQ(touch_res);


  /* 关键：等待 NS2009 完成 IRQ 引脚配置（约 100us） */
  // delay_us(100);

  p->x = x;
  p->y = y;
  p->z1 = z1;
  p->z2 = z2;
  p->pressed = __NS2009_Calculate_Pressure(p);

  log_d("NS2009 Read X=%d, Y=%d, Z1=%d, Z2=%d Press=%d", x, y, z1, z2, p->pressed);

  attachInterrupt(touch_res->irq_pin, touch_res->irq_handler, touch_res->irq_polarity);
}


/* Public function -------------------------------------------------------------*/
void BSP_Touch_Init(const void* touch)
{
  const Touch_Resource_t* touch_res = (const Touch_Resource_t*)touch;
  touch_res->init(touch_res);
}

void BSP_Touch_DeInit(const void* touch)
{
  const Touch_Resource_t* touch_res = (const Touch_Resource_t*)touch;
  EXTIx_DeInit(touch_res->irq_pin);
}

void BSP_Touch_Open(const void* touch)
{
  const Touch_Resource_t* touch_res = (const Touch_Resource_t*)touch;
  attachInterrupt(touch_res->irq_pin, touch_res->irq_handler, touch_res->irq_polarity);
}

void BSP_Touch_Close(const void* touch)
{
  const Touch_Resource_t* touch_res = (const Touch_Resource_t*)touch;
  detachInterrupt(touch_res->irq_pin);
}

void BSP_Touch_Read(const void* touch, uint16_t* x, uint16_t* y, uint8_t* pressed)
{
  const Touch_Resource_t* touch_res = (const Touch_Resource_t*)touch;
  TouchPoint_t p;
  touch_res->read(touch_res, &p);

  /* 先应用比例系数 */
  uint16_t raw_x = (uint16_t)(p.x * touch_res->ratio_x);
  uint16_t raw_y = (uint16_t)(p.y * touch_res->ratio_y);
  

  /* NS2009 Y轴镜像修正（如果硬件接线导致Y轴反向） */
  raw_y = touch_res->ver_res - raw_y - 1;

  /* 旋转处理（使用临时变量避免覆盖） */
  switch (touch_res->rotation) {
    case 0:
      *x = raw_x;
      *y = raw_y;
      break;
    
    case 90:
      *x = touch_res->ver_res - raw_y - 1;
      *y = raw_x;
      break;
    
    case 180:
      *x = touch_res->hor_res - raw_x - 1;
      *y = touch_res->ver_res - raw_y - 1;
      break;
    
    case 270:
      *x = raw_y;
      *y = touch_res->hor_res - raw_x - 1;
      break;
    
    default:
      *x = raw_x;
      *y = raw_y;
      break;
  }

  *pressed = p.pressed;
}


