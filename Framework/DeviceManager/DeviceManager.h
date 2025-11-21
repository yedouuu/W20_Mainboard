
#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "Common/errno.h"

/* Private defines -----------------------------------------------------------*/
#define DEVICE_SECTION __attribute__((used, section(".device_table")))
#define DEVICE_EXPORT(dev) \
    static Device_t * __device_##dev DEVICE_SECTION = &dev

#define DM_RET_TYPE Status_t


/* Exported types ------------------------------------------------------------*/
typedef enum {
    DEV_CLASS_MOTOR,
    DEV_CLASS_SCREEN,
    DEV_CLASS_SENSOR,
    DEV_CLASS_KEYBOARD,
    DEV_CLASS_UNKNOWN,
} Device_Class_e;


struct Device_t;
typedef DM_RET_TYPE (*Device_Init)    (struct Device_t*);
typedef DM_RET_TYPE (*Device_Deinit)  (struct Device_t*);
typedef DM_RET_TYPE (*Device_Open)    (struct Device_t*);
typedef DM_RET_TYPE (*Device_Close)   (struct Device_t*);
typedef DM_RET_TYPE (*Device_Read)    (struct Device_t*, uint8_t* buf, uint32_t len);
typedef DM_RET_TYPE (*Device_Write)   (struct Device_t*, const uint8_t* buf, uint32_t len);
typedef DM_RET_TYPE (*Device_IOCTL)   (struct Device_t*);


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
} Device_t;


/* Exported functions prototypes ---------------------------------------------*/
void DM_DeviceInitALL(void);
void DM_DeviceDeinitALL(void);
Device_t *DM_DeviceFind(const char *name);
DM_RET_TYPE DM_DeviceInit(char *name);
DM_RET_TYPE DM_DeviceDeinit(char *name);
DM_RET_TYPE DM_DeviceOpen(const char *name);
DM_RET_TYPE DM_DeviceClose(const char *name);
DM_RET_TYPE DM_DeviceRead(const char *name, uint8_t* buf, uint32_t* len);
DM_RET_TYPE DM_DeviceWrite(const char *name, const uint8_t* buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
