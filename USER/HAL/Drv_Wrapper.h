
#ifndef __HAL_H__
#define __HAL_H__

#include "Common/common.h"
#include "mcu_core.h"
#include "Drv_Delay_Wrapper.h"
#include "Drv_IR_Wrapper.h"

typedef bool (*CommitFunc_t)(void* info, void* userData);

void DRV_Init(void);
void DRV_Update(void);

/* LED */


/* Time */


/* Backlight */
// void Backlight_Init();
// uint16_t Backlight_GetValue();
// void Backlight_SetValue(int16_t val);
// void Backlight_SetGradual(uint16_t target, uint16_t time = 500);
// void Backlight_ForceLit(bool en);

/* Display */
// void Display_Init();
// void Display_DumpCrashInfo(const char* info);
// void Display_SetAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
// void Display_SendPixels(const uint16_t* pixels, uint32_t len);
    
// typedef void(*Display_CallbackFunc_t)(void);
// void Display_SetSendFinishCallback(Display_CallbackFunc_t func);
    
/* FaultHandle */
// void FaultHandle_Init();

#endif
