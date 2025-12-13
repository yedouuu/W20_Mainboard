
#ifndef __DRV_LCD_WRAPPER_H__
#define __DRV_LCD_WRAPPER_H__


/* Includes ------------------------------------------------------------------*/

typedef enum
{
  DRV_LCD_MAIN      = 0,
  DRV_LCD_SECONDARY = 1,
  DRV_LCD_THIRD     = 2,
  DRV_LCD_MAX
} DRV_LCD_Type_t;


typedef struct __DRV_LCD_Ops_t
{
  Device_Ops_t   base;
  const uint32_t magic;
  void         (*BSP_Init)(const void* resource);
  void         (*BSP_DeInit)(const void* resource);
  void         (*Blk_On)(const void* resource);
  void         (*Blk_Off)(const void* resource);
  void         (*SetBlock)(const void* resource, \
                           uint16_t x_start, uint16_t y_start, \
                           uint16_t x_end,   uint16_t y_end);
  void         (*WriteBlock)(const void* resource, uint16_t color, uint32_t size);
  void         (*WritePoint)(const void* resource, uint16_t color);
  void         (*DrawPoint)(const void* resource, uint16_t x, uint16_t y, uint16_t color);
  void         (*Clear)(const void* resource, uint16_t color);
} DRV_LCD_Ops_t;


/* Exported Function prototypes ---------------------------------------------*/
Status_t DRV_LCD_Init(Device_t* lcd_dev);
Status_t DRV_LCD_DeInit(Device_t* lcd_dev);
Status_t DRV_LCD_Blk_On(Device_t* lcd_dev);
Status_t DRV_LCD_Blk_Off(Device_t* lcd_dev);
Status_t DRV_LCD_SetBlock(Device_t* lcd_dev, uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
Status_t DRV_LCD_WriteBlock(Device_t* lcd_dev, uint16_t color, uint32_t size);
Status_t DRV_LCD_WritePoint(Device_t* lcd_dev, uint16_t color);
Status_t DRV_LCD_DrawPoint(Device_t* lcd_dev, uint16_t x, uint16_t y, uint16_t color);
Status_t DRV_LCD_Clear(Device_t* lcd_dev, uint16_t color);

#endif /* __DRV_LCD_WRAPPER_H__ */