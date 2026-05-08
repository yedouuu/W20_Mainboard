
#include "drv_touch_wrapper.h"

#define TOUCH_PRESS_CONFIRM_CNT   3
#define TOUCH_RELEASE_CONFIRM_CNT 3
#define TOUCH_COORD_FILTER_SHIFT  2

static uint16_t __DRV_Touch_FilterCoord(uint16_t last, uint16_t current)
{
  uint32_t filtered;

  filtered = (((uint32_t)last * 3U) + current +
              (1U << (TOUCH_COORD_FILTER_SHIFT - 1U))) >>
             TOUCH_COORD_FILTER_SHIFT;

  return (uint16_t)filtered;
}

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

  DRV_Touch_Priv_t *priv = (DRV_Touch_Priv_t *)touch_dev->priv;
  DRV_Touch_Point_t raw_point;

  ops->Read(touch_dev->res, &raw_point.x, &raw_point.y, &raw_point.pressed);

  if (raw_point.pressed)
  {
    if (priv->press_count < TOUCH_PRESS_CONFIRM_CNT) { priv->press_count++; }

    priv->release_count = 0;

    if (priv->state != DRV_TOUCH_STATE_PRESSED &&
        priv->press_count >= TOUCH_PRESS_CONFIRM_CNT)
    {
      log_d("Touch Press Confirmed at (%d, %d)", raw_point.x, raw_point.y);
      priv->state  = DRV_TOUCH_STATE_PRESSED;
      priv->last_x = raw_point.x;
      priv->last_y = raw_point.y;
    }
    else if (priv->state == DRV_TOUCH_STATE_PRESSED)
    {
      log_d("Touch From (%d, %d) Move to (%d, %d)",
            priv->last_x,
            priv->last_y,
            raw_point.x,
            raw_point.y);
      priv->last_x = __DRV_Touch_FilterCoord(priv->last_x, raw_point.x);
      priv->last_y = __DRV_Touch_FilterCoord(priv->last_y, raw_point.y);
    }
  }
  else
  {
    if (priv->release_count < TOUCH_RELEASE_CONFIRM_CNT)
    {
      priv->release_count++;
    }

    priv->press_count = 0;

    if (priv->release_count >= TOUCH_RELEASE_CONFIRM_CNT)
    {
      log_d("Touch Release Confirmed at (%d, %d)", priv->last_x, priv->last_y);
      priv->state = DRV_TOUCH_STATE_RELEASED;
    }
  }

  point->x       = priv->last_x;
  point->y       = priv->last_y;
  point->pressed = (priv->state == DRV_TOUCH_STATE_PRESSED) ? 1U : 0U;

  return kStatus_Success;
}
