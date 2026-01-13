
#include "Common/common.h"
#include "device_manager.h"

#include "drv_touch_wrapper.h"
#include "bsp_touch.h"

static Status_t __DRV_Touch_Read_Adapter(Device_t* dev, uint8_t* buf, uint32_t len)
{
  if (len < sizeof(DRV_Touch_Point_t))
  {
    log_e("buffer too small needs at least %d bytes", sizeof(DRV_Touch_Point_t));
    return kStatus_InvalidArgument;
  }

  DRV_Touch_Point_t point;
  Status_t ret = DRV_Touch_Read(dev, &point);
  
  if (ret == kStatus_Success)
  {
    memcpy(buf, &point, sizeof(DRV_Touch_Point_t)); 
  }
  
  return ret;
}

static DRV_Touch_Ops_t touch_ops = {
  .base = {
    .init    = DRV_Touch_Init,
    .deinit  = DRV_Touch_DeInit,
    .open    = DRV_Touch_Open,
    .close   = DRV_Touch_Close,
    .read    = __DRV_Touch_Read_Adapter,
    .write   = NULL,
    .ioctl   = NULL,
  },
  .magic       = DEVICE_MAGIC_TOUCH,
  .BSP_Init    = BSP_Touch_Init,
  .BSP_DeInit  = BSP_Touch_DeInit,
  .Open        = BSP_Touch_Open,
  .Close       = BSP_Touch_Close,
  .Read        = BSP_Touch_Read,
};

static DRV_Touch_Priv_t touch_main_priv = {
  .type         = DRV_TOUCH_MAIN,
  .state        = DRV_TOUCH_STATE_IDLE,
  .last_x       = 0,
  .last_y       = 0,
  .press_count  = 0,
  .press_tick   = 0,
  .release_tick = 0,
};

Device_t touch_main = {
  .name      = "TOUCH_MAIN",
  .dev_class = DEV_CLASS_INPUT,
  .ops       = (void *)&touch_ops,
  .priv      = (void *)&touch_main_priv,
  .res       = (void *)&touch_main_res,
};

DEVICE_EXPORT(touch_main);
