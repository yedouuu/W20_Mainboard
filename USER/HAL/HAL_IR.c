
#include "HAL_IR.h"
#include "Logger.h"

Status_t HAL_IR_Init(Device_t* ir_dev)
{
  if (ir_dev == NULL || ir_dev->ops == NULL || ((HAL_IR_Ops_t*)ir_dev->ops)->base.init == NULL)
  {
    return kStatus_InvalidArgument;
  }

  log_d("HAL_IR_Init called for device: %s", ir_dev->name);

  HAL_IR_Ops_t *ops = (HAL_IR_Ops_t*)ir_dev->ops;
  ops->BSP_Init(ir_dev->res);

  return kStatus_Success;
}

Status_t HAL_IR_DeInit(Device_t* ir_dev)
{
  if (ir_dev == NULL || ir_dev->ops == NULL || ((HAL_IR_Ops_t*)ir_dev->ops)->base.deinit == NULL)
  {
    return kStatus_InvalidArgument;
  }

  HAL_IR_Ops_t *ops = (HAL_IR_Ops_t*)ir_dev->ops;
  HAL_IR_Priv_t* priv = (HAL_IR_Priv_t*)ir_dev->priv;
  memset(priv, 0, sizeof(HAL_IR_Priv_t));

  ops->base.deinit(ir_dev->res);
  return kStatus_Success;
}

Status_t HAL_IR_Enable(Device_t* ir_dev)
{
  if (ir_dev == NULL || ir_dev->ops == NULL || ((HAL_IR_Ops_t*)ir_dev->ops)->Enable == NULL)
  {
    return kStatus_InvalidArgument;
  }

  log_d("HAL_IR_Enable called for device: %s", ir_dev->name);

  HAL_IR_Ops_t *ops = (HAL_IR_Ops_t*)ir_dev->ops;
  ops->Enable(ir_dev->res);
  return kStatus_Success;
}


Status_t HAL_IR_Disable(Device_t* ir_dev)
{
  if (ir_dev == NULL || ir_dev->ops == NULL || ((HAL_IR_Ops_t*)ir_dev->ops)->Disable == NULL)
  {
    return kStatus_InvalidArgument;
  }

  log_d("HAL_IR_Disable called for device: %s", ir_dev->name);

  HAL_IR_Ops_t *ops = (HAL_IR_Ops_t*)ir_dev->ops;
  ops->Disable(ir_dev->res);
  return kStatus_Success;
}


Status_t HAL_IR_GetRawData(Device_t* ir_dev, uint8_t* buf, uint32_t* len)
{
  if (ir_dev == NULL || ir_dev->ops == NULL || \ 
      ((HAL_IR_Ops_t*)ir_dev->ops)->GetRawData == NULL || \
      buf == NULL || len == 0)
  {
    return kStatus_InvalidArgument;
  }
  HAL_IR_Ops_t *ops = (HAL_IR_Ops_t*)ir_dev->ops;
  
  uint16_t raw_data = 0;
  ops->GetRawData(ir_dev->res, &raw_data);
  
  buf[0] = raw_data & 0xFF;
  buf[1] = (raw_data >> 8) & 0xFF;
  *len = 2;
  return kStatus_Success;
}


Status_t HAL_IR_SetPWM(Device_t* ir_dev, HAL_IR_PWM_Duty_e duty)
{
  if (ir_dev == NULL || ir_dev->ops == NULL || \
      ((HAL_IR_Ops_t*)ir_dev->ops)->SetPWM == NULL)
  {
    return kStatus_InvalidArgument;
  }
  HAL_IR_Ops_t *ops = (HAL_IR_Ops_t*)ir_dev->ops;
  ops->SetPWM(ir_dev->res, (uint16_t)duty);
  return kStatus_Success;
}
