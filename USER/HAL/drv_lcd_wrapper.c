
#include "drv_lcd_wrapper.h"

/**
 * @brief  内部辅助函数：安全获取并校验 Ops 指针
 * @note   集中处理类型检查，防止非法强转
 * @param  dev: 通用设备指针
 * @retval 有效的 DRV_LCD_Ops_t 指针，如果校验失败返回 NULL
 */
static DRV_LCD_Ops_t* __DRV_LCD_GetOps(Device_t* dev)
{
  if (dev == NULL) 
  {
    log_e("Device is NULL!");
    return NULL;
  }

  if (dev->dev_class != DEV_CLASS_DISPLAY)
  {
    log_e("Type Mismatch: Device %s is not a display device", dev->name);
    return NULL;
  }

  if (dev->ops == NULL)
  {
    log_e("Device %s has no ops!", dev->name);
    return NULL;
  }

  DRV_LCD_Ops_t* ops = (DRV_LCD_Ops_t*)dev->ops;
  if (ops->magic != DEVICE_MAGIC_LCD)
  {
    log_e("Magic Mismatch: Device %s is not a LCD Device!", dev->name);
    return NULL;
  }

  return ops;
}


Status_t DRV_LCD_Init(Device_t* lcd_dev)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  log_i("Init device: %s", lcd_dev->name);

  ops->BSP_Init(lcd_dev->res);

  return kStatus_Success;
}

Status_t DRV_LCD_DeInit(Device_t* lcd_dev)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  log_i("DeInit device: %s", lcd_dev->name);

  ops->BSP_DeInit(lcd_dev->res);

  return kStatus_Success;
}

Status_t DRV_LCD_Blk_On(Device_t* lcd_dev)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  ops->Blk_On(lcd_dev->res);

  return kStatus_Success;
}

Status_t DRV_LCD_Blk_Off(Device_t* lcd_dev)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  ops->Blk_Off(lcd_dev->res);

  return kStatus_Success;
}

Status_t DRV_LCD_SetBlock(Device_t* lcd_dev, uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  ops->SetBlock(lcd_dev->res, x_start, y_start, x_end, y_end);

  return kStatus_Success;
}

Status_t DRV_LCD_WriteBlock(Device_t* lcd_dev, uint16_t* color, uint32_t size)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  ops->WriteBlock(lcd_dev->res, color, size);

  return kStatus_Success;
}

Status_t DRV_LCD_WritePoint(Device_t* lcd_dev, uint16_t color)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  ops->WritePoint(lcd_dev->res, color);

  return kStatus_Success;
}

Status_t DRV_LCD_DrawPoint(Device_t* lcd_dev, uint16_t x, uint16_t y, uint16_t color)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  ops->DrawPoint(lcd_dev->res, x, y, color);

  return kStatus_Success;
}

Status_t DRV_LCD_Clear(Device_t* lcd_dev, uint16_t color)
{
  DRV_LCD_Ops_t* ops = __DRV_LCD_GetOps(lcd_dev);
  if (ops == NULL)
  {
    log_e("%s Invalid Argument!", lcd_dev->name);
    return kStatus_InvalidArgument;
  }

  ops->Clear(lcd_dev->res, color);

  return kStatus_Success;
}

