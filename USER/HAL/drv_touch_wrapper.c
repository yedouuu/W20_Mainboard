
#include "drv_touch_wrapper.h"

/**
 * @brief  内部辅助函数：安全获取并校验 Ops 指针
 * @note   集中处理类型检查，防止非法强转
 * @param  dev: 通用设备指针
 * @retval 有效的 DRV_Touch_Ops_t 指针，如果校验失败返回 NULL
 */
static DRV_Touch_Ops_t *__DRV_Touch_GetOps(Device_t *dev)
{
  if (dev == NULL)
  {
    log_e("Device is NULL!");
    return NULL;
  }

  if (dev->dev_class != DEV_CLASS_INPUT)
  {
    log_e("Type Mismatch: Device %s is not a input device", dev->name);
    return NULL;
  }

  if (dev->ops == NULL)
  {
    log_e("Device %s has no ops!", dev->name);
    return NULL;
  }

  DRV_Touch_Ops_t *ops = (DRV_Touch_Ops_t *)dev->ops;
  if (ops->magic != DEVICE_MAGIC_TOUCH)
  {
    log_e("Magic Mismatch: Device %s is not a Touch Device!", dev->name);
    return NULL;
  }

  return ops;
}

Status_t DRV_Touch_Init(Device_t *touch_dev)
{
  DRV_Touch_Ops_t *ops = __DRV_Touch_GetOps(touch_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", touch_dev->name);
    return kStatus_InvalidArgument;
  }

  log_i("Init device: %s, type %d",
        touch_dev->name,
        ((DRV_Touch_Priv_t *)touch_dev->priv)->type);

  ops->BSP_Init(touch_dev->res);

  return kStatus_Success;
}

Status_t DRV_Touch_DeInit(Device_t *touch_dev)
{
  DRV_Touch_Ops_t *ops = __DRV_Touch_GetOps(touch_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->BSP_DeInit(touch_dev->res);

  return kStatus_Success;
}

Status_t DRV_Touch_Open(Device_t *touch_dev)
{
  DRV_Touch_Ops_t *ops = __DRV_Touch_GetOps(touch_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->Open(touch_dev->res);

  return kStatus_Success;
}

Status_t DRV_Touch_Close(Device_t *touch_dev)
{
  DRV_Touch_Ops_t *ops = __DRV_Touch_GetOps(touch_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->Close(touch_dev->res);

  return kStatus_Success;
}

Status_t DRV_Touch_Read(Device_t *touch_dev, DRV_Touch_Point_t *point)
{
  DRV_Touch_Ops_t *ops = __DRV_Touch_GetOps(touch_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->Read(touch_dev->res, &point->x, &point->y, &point->pressed);

  return kStatus_Success;
}
