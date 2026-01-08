/**
 * @file lv_port_disp_template.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "device_manager.h"
#include "drv_lcd_wrapper.h"
#include "bsp_lcd.h"
/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
    //#warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    480
#endif

#ifndef MY_DISP_VER_RES
    //#warning Please define or replace the macro MY_DISP_VER_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    272
#endif

#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */

/**********************
 *      TYPEDEFS
 **********************/
#define USE_DMA 1
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

/**********************
 *  STATIC VARIABLES
 **********************/
static Device_t* lcd_dev = NULL;
static lv_display_t* lv_disp_drv_p = NULL;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#include "Common/common.h"

void DMA2_Channel2_IRQHandler(void)
{
	if(dma_interrupt_flag_get(DMA2_FDT2_FLAG) == SET)
	{
        dma_channel_enable(DMA2_CHANNEL2, FALSE);
		dma_flag_clear(DMA2_FDT2_FLAG);
		if(lv_disp_drv_p != NULL)
		{
			lv_display_flush_ready(lv_disp_drv_p); /* tell lvgl that flushing is done */
		}	
	}
    else if (dma_interrupt_flag_get(DMA2_DTERR2_FLAG) == SET)
    {
        dma_flag_clear(DMA2_DTERR2_FLAG);
        log_e("DMA2 Channel2 Transfer Error!");
    }
}


void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    lv_display_t * disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_flush_cb(disp, disp_flush);

    /* Example 1
     * One buffer for partial rendering*/
    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_1_1[(MY_DISP_HOR_RES * MY_DISP_VER_RES / 4) * BYTE_PER_PIXEL];            /*A buffer for 1/4 screen*/
    // lv_display_set_buffers(disp, buf_1_1, NULL, sizeof(buf_1_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 2
     * Two buffers for partial rendering
     * In flush_cb DMA or similar hardware should be used to update the display in the background.*/
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_1[MY_DISP_HOR_RES * 30 * BYTE_PER_PIXEL];

    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_2[MY_DISP_HOR_RES * 30 * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /* Example 3
     * Two buffers screen sized buffer for double buffering.
     * Both LV_DISPLAY_RENDER_MODE_DIRECT and LV_DISPLAY_RENDER_MODE_FULL works, see their comments*/
    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];

    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES * BYTE_PER_PIXEL];
    // lv_display_set_buffers(disp, buf_3_1, buf_3_2, sizeof(buf_3_1), LV_DISPLAY_RENDER_MODE_DIRECT);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    lcd_dev = DM_DeviceFind("LCD_MAIN");
    if (lcd_dev) {
        DRV_LCD_Blk_On(lcd_dev);	
    }
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)  
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display.
 *`px_map` contains the rendered image as raw pixel map and it should be copied to `area` on the display.
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_display_flush_ready()' has to be called when it's finished.*/
static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map)
{
#if USE_DMA == 1
    /*Using DMA for flushing the screen*/
    if(disp_flush_enabled) {
        lv_disp_drv_p = disp_drv;
        uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
        uint16_t * color_p = (uint16_t *)px_map;

        DRV_LCD_SetBlock(lcd_dev, area->x1,  area->y1, area->x2, area->y2);

        dma_channel_enable(DMA2_CHANNEL2, FALSE);
        dma_data_number_set(DMA2_CHANNEL2, size);

        DMA2_CHANNEL2->dtcnt = size * BYTE_PER_PIXEL;
        DMA2_CHANNEL2->maddr = (uint32_t)color_p;

        // lcd_dma2_config(size * BYTE_PER_PIXEL, color_p);

        dma_channel_enable(DMA2_CHANNEL2, TRUE);
        
    }
#else
    if(disp_flush_enabled) {
        lv_disp_drv_p = disp_drv;
        uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
        uint16_t * color_p = (uint16_t *)px_map;
        DRV_LCD_SetBlock(lcd_dev, area->x1,  area->y1, area->x2, area->y2);
        DRV_LCD_WriteBlock(lcd_dev, color_p, size);
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_display_flush_ready(disp_drv);
#endif
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
