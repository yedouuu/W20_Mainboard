
#include "DeviceManager.h"
#include <string.h>

extern uint32_t Image$$RW_DEVICE_TABLE$$Base;
extern uint32_t Image$$RW_DEVICE_TABLE$$Limit;

/**
 * @brief  系统设备初始化
 * @note   遍历设备表并调用初始化函数
 */
void DM_DeviceInitALL(void)
{
  /* 获取设备表的起始和结束地址 */
  Device_t **dev_ptr_start = (Device_t **)&Image$$RW_DEVICE_TABLE$$Base;
  Device_t **dev_ptr_end   = (Device_t **)&Image$$RW_DEVICE_TABLE$$Limit;

  Device_t **ptr;

  /* 遍历指针数组 */
  for (ptr = dev_ptr_start; ptr < dev_ptr_end; ptr++)
  {
    Device_t *dev = *ptr;

    if (dev && dev->ops)
    {
      /* 强转为 Device_Ops_t 并调用 init */
      Device_Ops_t *ops = (Device_Ops_t *)dev->ops;
      if (ops->init)
      {
        ops->init(dev);
      }
    }
  }
}


/**
 * @brief  系统设备释放
 * @note   遍历设备表并调用释放函数
 */
void DM_DeviceDeinitALL(void)
{
  /* 获取设备表的起始和结束地址 */
  Device_t **dev_ptr_start = (Device_t **)&Image$$RW_DEVICE_TABLE$$Base;
  Device_t **dev_ptr_end   = (Device_t **)&Image$$RW_DEVICE_TABLE$$Limit;

  Device_t **ptr;

  /* 遍历指针数组 */
  for (ptr = dev_ptr_start; ptr < dev_ptr_end; ptr++)
  {
    Device_t *dev = *ptr;

    if (dev && dev->ops)
    {
      /* 强转为 Device_Ops_t 并调用 deinit */
      Device_Ops_t *ops = (Device_Ops_t *)dev->ops;
      if (ops->deinit)
      {
        ops->deinit(dev);
      }
    }
  }
}

DM_RET_TYPE DM_DeviceInit(char *name)
{
  Device_t *dev = DM_DeviceFind(name);
  if (dev && dev->ops)
  {
    Device_Ops_t *ops = (Device_Ops_t *)dev->ops;
    if (ops->init)
    {
      return ops->init(dev);
    }
  }
  return kStatus_DeviceError;
}


DM_RET_TYPE DM_DeviceDeinit(char *name)
{
  Device_t *dev = DM_DeviceFind(name);
  if (dev && dev->ops)
  {
    Device_Ops_t *ops = (Device_Ops_t *)dev->ops;
    if (ops->deinit)
    {
      return ops->deinit(dev);
    }
  }
  return kStatus_DeviceError;
}

/**
 * @brief  查找设备
 */
Device_t *DM_DeviceFind(const char *name)
{
  Device_t **dev_ptr_start = (Device_t **)&Image$$RW_DEVICE_TABLE$$Base;
  Device_t **dev_ptr_end   = (Device_t **)&Image$$RW_DEVICE_TABLE$$Limit;
  Device_t **ptr;

  for (ptr = dev_ptr_start; ptr < dev_ptr_end; ptr++)
  {
    Device_t *dev = *ptr;
    if (dev && dev->name && strcmp(dev->name, name) == 0)
    {
      return dev;
    }
  }
  return kStatus_DeviceMissing;
}


DM_RET_TYPE DM_DeviceOpen(const char *name)
{
  Device_t *dev = DM_DeviceFind(name);
  if (dev == NULL)
  {
    return kStatus_DeviceMissing;
  }

  if (dev->ops == NULL)
  {
    return kStatus_InvalidArgument;
  }

  Device_Ops_t *ops = (Device_Ops_t *)dev->ops;
  if (ops->open)
  {
    return ops->open(dev);
  }
  return kStatus_InvalidArgument;
}


DM_RET_TYPE DM_DeviceClose(const char *name)
{
  Device_t *dev = DM_DeviceFind(name);
  if (dev == NULL || dev->ops == NULL)
  {
    return kStatus_NotFound;
  }

  Device_Ops_t *ops = (Device_Ops_t *)dev->ops;
  if (ops->close)
  {
    return ops->close(dev);
  }
  return kStatus_InvalidArgument;
}


DM_RET_TYPE DM_DeviceRead(const char *name, uint8_t* buf, uint32_t* len)
{
  Device_t *dev = DM_DeviceFind(name);
  if (dev == NULL || dev->ops == NULL)
  {
    return kStatus_DeviceMissing;
  }
  if (buf == NULL || len == 0)
  {
    return kStatus_InvalidArgument;
  }

  Device_Ops_t *ops = (Device_Ops_t *)dev->ops;

  if (ops->read)
  {
    return ops->read(dev, buf, len);
  }

  return kStatus_InvalidArgument;
}


DM_RET_TYPE DM_DeviceWrite(const char *name, const uint8_t* buf, uint32_t len)
{
  Device_t *dev = DM_DeviceFind(name);
  if (dev == NULL || dev->ops == NULL)
  {
    return kStatus_NotFound;
  }
  if (buf == NULL || len == 0)
  {
    return kStatus_InvalidArgument;
  }

  Device_Ops_t *ops = (Device_Ops_t *)dev->ops;

  if (ops->write)
  {
    return ops->write(dev, buf, len);
  }

  return kStatus_InvalidArgument;
}

