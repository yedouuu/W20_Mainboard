
#ifndef __DRV_TOUCH_WRAPPER_H__
#define __DRV_TOUCH_WRAPPER_H__

/* Includes ------------------------------------------------------------------*/
#include "Common/common.h"
#include "DeviceManager/device_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types -----------------------------------------------------------*/
typedef enum
{
  DRV_TOUCH_MAIN      = 0,
  DRV_TOUCH_SECONDARY = 1,
  DRV_TOUCH_THIRD     = 2,
  DRV_TOUCH_MAX
} DRV_Touch_Type_t;

typedef enum
{
  DRV_TOUCH_STATE_IDLE     = 0,
  DRV_TOUCH_STATE_PRESSED  = 1,
  DRV_TOUCH_STATE_RELEASED = 2,
} DRV_Touch_State_e;

typedef struct __DRV_Touch_Point_t
{
  uint16_t x;
  uint16_t y;
  uint8_t  pressed;
} DRV_Touch_Point_t;

typedef struct __DRV_Touch_Ops_t
{
  Device_Ops_t   base;
  const uint32_t magic;
  void (*BSP_Init)(const void *resource);
  void (*BSP_DeInit)(const void *resource);
  void (*Open)(const void *resource);
  void (*Close)(const void *resource);
  void (*Read)(const void *resource,
               uint16_t   *x,
               uint16_t   *y,
               uint8_t    *pressed);
} DRV_Touch_Ops_t;

typedef struct __DRV_Touch_Priv_t
{
  const DRV_Touch_Type_t type;         /* 触摸面板类型 */
  DRV_Touch_State_e      state;        /* 触摸面板当前状态 */
  uint16_t               last_x;       /* 上次触摸的 X 坐标 */
  uint16_t               last_y;       /* 上次触摸的 Y 坐标 */
  uint16_t               press_count;  /* 连续按下计数 */
  uint16_t               press_tick;   /* 按下时间戳 */
  uint16_t               release_tick; /* 按下和释放时间戳 */
} DRV_Touch_Priv_t;

/* Exported functions prototypes ---------------------------------------------*/
Status_t DRV_Touch_Init(Device_t *touch_dev);
Status_t DRV_Touch_DeInit(Device_t *touch_dev);
Status_t DRV_Touch_Open(Device_t *touch_dev);
Status_t DRV_Touch_Close(Device_t *touch_dev);
Status_t DRV_Touch_Read(Device_t *touch_dev, DRV_Touch_Point_t *point);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_TOUCH_WRAPPER_H__ */