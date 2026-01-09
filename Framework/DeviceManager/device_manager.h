/*
 * MIT License
 * Copyright (c) 2025 Yedouuu | UNION
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Common/errno.h"


/* ========================================================================== */
/*                            Device Magic Registry                           */
/* ========================================================================== */

/* 将4个字符组合成一个32位整数 (Big Endian 风格，内存中可读) */
#define DEVICE_MAGIC_MAKE(a, b, c, d)  ( ((uint32_t)(a) << 24) | \
                                         ((uint32_t)(b) << 16) | \
                                         ((uint32_t)(c) << 8)  | \
                                         ((uint32_t)(d)) )

/* 
 * 全局设备 Magic Number 注册表
 * 使用 ASCII 字符更易于识别和调试
 */
#define DEVICE_MAGIC_IR         DEVICE_MAGIC_MAKE('I', 'R', 'O', 'P') // 0x49524F50
#define DEVICE_MAGIC_MOTOR      DEVICE_MAGIC_MAKE('M', 'O', 'T', 'R') // 0x4D4F5452
#define DEVICE_MAGIC_LCD        DEVICE_MAGIC_MAKE('L', 'C', 'D', '_') // 0x4C43445F
#define DEVICE_MAGIC_TOUCH      DEVICE_MAGIC_MAKE('T', 'O', 'U', 'C') // 0x544F5543
#define DEVICE_MAGIC_KEYPAD     DEVICE_MAGIC_MAKE('K', 'E', 'Y', 'P') // 0x4B455950
#define DEVICE_MAGIC_PRINTER    DEVICE_MAGIC_MAKE('P', 'R', 'N', 'T') // 0x50524E54
#define DEVICE_MAGIC_UV         DEVICE_MAGIC_MAKE('U', 'V', 'S', 'N') // 0x5556534E
#define DEVICE_MAGIC_MAG        DEVICE_MAGIC_MAKE('M', 'A', 'G', 'N') // 0x4D41474E



/* Private defines -----------------------------------------------------------*/
#if defined(_MSC_VER) /* For Visual Studio */
    #define DEVICE_EXPORT(dev) \
        static void __init_##dev(void) __attribute__((constructor)); \
        static void __init_##dev(void) { DM_DeviceRegister(&dev); }

    #define DEVICE_FOREACH(dev) \
        for(Device_t *dev = g_device_list_head; dev != NULL; dev = dev->next)

#else /* For ARM Compiler */
    #define DEVICE_SECTION __attribute__((used, section(".device_table")))
    #define DEVICE_EXPORT(dev) \
        static Device_t * __device_##dev DEVICE_SECTION = &dev

    #define DEVICE_FOREACH(dev) \
        for(Device_t **ptr = (Device_t **)&Image$$RW_DEVICE_TABLE$$Base;  \
            ptr < (Device_t **)&Image$$RW_DEVICE_TABLE$$Limit; \
            ptr++) \
            for(Device_t *dev = *ptr; dev != NULL; dev = NULL) /* 内部循环只执行一次 */
#endif

#define DM_RET_TYPE Status_t


/* Exported types ------------------------------------------------------------*/
typedef enum {
    DEV_CLASS_MOTOR,
    DEV_CLASS_DISPLAY,
    DEV_CLASS_SENSOR,
    DEV_CLASS_INPUT,
    DEV_CLASS_UNKNOWN,
} Device_Class_e;


struct Device_t;
typedef DM_RET_TYPE (*Device_Init)    (struct Device_t*);
typedef DM_RET_TYPE (*Device_Deinit)  (struct Device_t*);
typedef DM_RET_TYPE (*Device_Open)    (struct Device_t*);
typedef DM_RET_TYPE (*Device_Close)   (struct Device_t*);
typedef DM_RET_TYPE (*Device_Read)    (struct Device_t*, uint8_t* buf, uint32_t len);
typedef DM_RET_TYPE (*Device_Write)   (struct Device_t*, const uint8_t* buf, uint32_t len);
typedef DM_RET_TYPE (*Device_IOCTL)   (struct Device_t*, uint32_t cmd, void* arg);


typedef struct Device_Ops {
    Device_Init    init;
    Device_Deinit  deinit;
    Device_Open    open;
    Device_Close   close;
    Device_Read    read;
    Device_Write   write;
    Device_IOCTL   ioctl;
} Device_Ops_t;
        

typedef struct Device_t {
    const char      *name;     // 设备名
    Device_Class_e   class;    // 设备类别
    const void      *ops;      // 指向各设备 ops 表（强转使用）
    void            *priv;     // 私有数据
    void            *res;      // 设备资源
    struct Device_t *next;     // 指向下一个设备，用于构成链表
} Device_t;


/* Exported functions prototypes ---------------------------------------------*/
void DM_DeviceRegister(Device_t *dev);
void DM_DeviceInitALL(void);
void DM_DeviceDeinitALL(void);
Device_t *DM_DeviceFind(const char *name);
DM_RET_TYPE DM_DeviceInit(char *name);
DM_RET_TYPE DM_DeviceDeinit(char *name);
DM_RET_TYPE DM_DeviceOpen(const char *name);
DM_RET_TYPE DM_DeviceClose(const char *name);
DM_RET_TYPE DM_DeviceRead(const char *name, uint8_t* buf, uint32_t len);
DM_RET_TYPE DM_DeviceWrite(const char *name, const uint8_t* buf, uint32_t len);
DM_RET_TYPE DM_DeviceIOCTL(const char *name, uint32_t cmd, void* arg);
#ifdef __cplusplus
}
#endif

#endif
