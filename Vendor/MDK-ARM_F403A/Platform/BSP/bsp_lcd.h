
#ifndef __BSP_LCD_H__
#define __BSP_LCD_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"
#include "bsp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0XF81F
#define GRED    0XFFE0
#define GBLUE   0X07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0XBC40
#define BRRED   0XFC07
#define GRAY    0X8430

/* Exported Typedef --------------------------------------------------------*/
typedef struct _LCD_Resource_t LCD_Resource_t;

/* Exported Variables --------------------------------------------------------*/
extern const LCD_Resource_t lcd_main_res;

/* Exported functions prototypes ---------------------------------------------*/
void BSP_LCD_Init(const void *lcd);
void BSP_LCD_DeInit(const void *lcd);
void BSP_LCD_Set_Block(const void *lcd,
                       uint16_t    x_start,
                       uint16_t    y_start,
                       uint16_t    x_end,
                       uint16_t    y_end);
void BSP_LCD_Write_One_Point(const void *lcd, uint16_t color);
void BSP_LCD_Write_One_Block(const void *lcd, uint16_t *color, uint32_t size);
void BSP_LCD_DrawPoint(const void *lcd, uint16_t x, uint16_t y, uint16_t color);
void BSP_LCD_Clear(const void *lcd, uint16_t color);
void BSP_LCD_Blk_On(const void *lcd);
void BSP_LCD_Blk_Off(const void *lcd);
void BSP_LCD_Blk_Set_PWM(const void *lcd, uint16_t duty);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LCD_H__ */
