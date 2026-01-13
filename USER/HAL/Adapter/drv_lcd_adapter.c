
#include "Common/common.h"
#include "device_manager.h"

#include "drv_lcd_wrapper.h"
#include "bsp_lcd.h"

static DRV_LCD_Ops_t lcd_ops = {
  .base = {
    .init    = DRV_LCD_Init,
    .deinit  = DRV_LCD_DeInit,
    .open    = DRV_LCD_Blk_On,
    .close   = DRV_LCD_Blk_Off,
    .read    = NULL,
    .write   = NULL,
    .ioctl   = NULL,
  },
  .magic       = DEVICE_MAGIC_LCD,
  .BSP_Init    = BSP_LCD_Init,
  .BSP_DeInit  = NULL,
  .Blk_On      = BSP_LCD_Blk_On,
  .Blk_Off     = BSP_LCD_Blk_Off,
  .SetBlock    = BSP_LCD_Set_Block,
  .WriteBlock  = BSP_LCD_Write_One_Block,
  .WritePoint  = BSP_LCD_Write_One_Point,
  .DrawPoint   = BSP_LCD_DrawPoint,
  .Clear       = BSP_LCD_Clear,
};

// static DRV_Touch_Priv_t lcd_main_priv = {
//   .type = DRV_LCD_MAIN,
// };


Device_t lcd_main = {
  .name      = "LCD_MAIN",
  .dev_class = DEV_CLASS_DISPLAY,
  .ops       = (void *)&lcd_ops,
  .priv      = NULL,
  .res       = (void *)&lcd_main_res,
};

DEVICE_EXPORT(lcd_main);
