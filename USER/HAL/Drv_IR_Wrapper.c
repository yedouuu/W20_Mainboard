

#include "drv_ir_wrapper.h"


/**
 * @brief  内部辅助函数：安全获取并校验 Ops 指针
 * @note   集中处理类型检查，防止非法强转
 * @param  dev: 通用设备指针
 * @retval 有效的 DRV_IR_Ops_t 指针，如果校验失败返回 NULL
 */
static DRV_IR_Ops_t* __DRV_IR_GetOps(Device_t* dev)
{
  if (dev == NULL) 
  {
    log_e("Device is NULL!");
    return NULL;
  }

  if (dev->class != DEV_CLASS_SENSOR)
  {
    log_e("Type Mismatch: Device %s is not a Sensor!", dev->name);
    return NULL;
  }

  if (dev->ops == NULL)
  {
    log_e("Device %s has no ops!", dev->name);
    return NULL;
  }

  DRV_IR_Ops_t* ops = (DRV_IR_Ops_t*)dev->ops;
  if (ops->magic != DEVICE_MAGIC_IR)
  {
    log_e("Magic Mismatch: Device %s is not an IR Device!", dev->name);
    return NULL;
  }

  return ops;
}


Status_t DRV_IR_Init(Device_t* ir_dev)
{
  DRV_IR_Ops_t* ops = __DRV_IR_GetOps(ir_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  log_i("Init device: %s, type %d", ir_dev->name, ((DRV_IR_Priv_t*)ir_dev->priv)->type);

  ops->BSP_Init(ir_dev->res);

  return kStatus_Success;
}

Status_t DRV_IR_DeInit(Device_t* ir_dev)
{
  DRV_IR_Ops_t* ops = __DRV_IR_GetOps(ir_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  DRV_IR_Priv_t* priv = (DRV_IR_Priv_t*)ir_dev->priv;
  memset(priv, 0, sizeof(DRV_IR_Priv_t));

  ops->base.deinit(ir_dev->res);
  return kStatus_Success;
}

Status_t DRV_IR_Enable(Device_t* ir_dev)
{
  DRV_IR_Ops_t* ops = __DRV_IR_GetOps(ir_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->Enable(ir_dev->res);
  return kStatus_Success;
}


Status_t DRV_IR_Disable(Device_t* ir_dev)
{
  DRV_IR_Ops_t* ops = __DRV_IR_GetOps(ir_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  ops->Disable(ir_dev->res);
  return kStatus_Success;
}


Status_t DRV_IR_GetRawData(Device_t* ir_dev, uint16_t* raw_data)
{
  DRV_IR_Ops_t* ops = __DRV_IR_GetOps(ir_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }
  
  ops->GetRawData(ir_dev->res, raw_data);

  return kStatus_Success;
}


Status_t DRV_IR_GetState(Device_t* ir_dev, DRV_IR_State_e* state)
{
  DRV_IR_Priv_t* priv = (DRV_IR_Priv_t*)ir_dev->priv;
  if (priv == NULL)
  {
    log_e("IR Device %s has no private data!", ir_dev->name);
    return kStatus_InvalidArgument;
  }

  *state = priv->state;
  return kStatus_Success;
}


Status_t DRV_IR_SetPWM(Device_t* ir_dev, DRV_IR_PWM_Duty_e duty)
{
  DRV_IR_Ops_t* ops = __DRV_IR_GetOps(ir_dev);
  if (ops == NULL)
  {
    log_e("Invalid Argument!");
    return kStatus_InvalidArgument;
  }

  if (duty >= DRV_IR_PWM_DUTY_MAX)
  {
    log_w("Invalid duty cycle value: %d, use default duty instead", duty);
  }

  uint16_t duty_val = 0;
  switch( duty )
  {
    case DRV_IR_PWM_DUTY_0:   duty_val = 0;    break;
    case DRV_IR_PWM_DUTY_25:  duty_val = 250;  break;
    case DRV_IR_PWM_DUTY_50:  duty_val = 500;  break;
    case DRV_IR_PWM_DUTY_75:  duty_val = 750;  break;
    case DRV_IR_PWM_DUTY_100: duty_val = 1000; break;
    default:                  duty_val = 500;  break;
  }

  DRV_IR_Priv_t* priv = (DRV_IR_Priv_t*)ir_dev->priv;
  if (priv == NULL)
  {
    log_e("IR Device %s has no private data!", ir_dev->name);
    return kStatus_InvalidArgument;
  }

  priv->pwm_duty = duty;
  ops->SetPWM(ir_dev->res, (uint16_t)duty_val);

  return kStatus_Success;
}
