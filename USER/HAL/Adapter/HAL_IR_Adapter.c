
#include "Common/common.h"
#include "DeviceManager.h"
#include "HAL_IR.h"
#include "bsp_IR.h"


static Status_t _HAL_IR_Read_Adapter(Device_t* dev, uint8_t* buf, uint32_t len)
{
  if (len < sizeof(uint16_t))
  {
    log_e("buffer too small needs at least %d bytes", sizeof(uint16_t));
    return kStatus_InvalidArgument;
  }

  uint16_t raw_val = 0;
  Status_t ret = HAL_IR_GetRawData(dev, &raw_val);
  
  if (ret == kStatus_Success)
  {
    memcpy(buf, &raw_val, 2); 
  }
  
  return ret;
}


static HAL_IR_Ops_t ir_ops = {
    .base = {
        .init    = HAL_IR_Init,
        .deinit  = HAL_IR_DeInit,
        .open    = HAL_IR_Enable,
        .close   = HAL_IR_Disable,
        .read    = _HAL_IR_Read_Adapter,
        .write   = NULL,
        .ioctl   = NULL,
    },
    .magic       = DEVICE_MAGIC_IR,
    .BSP_Init    = BSP_IR_Init,
    .Enable      = BSP_IR_Enable,
    .Disable     = BSP_IR_Disable,
    .SetPWM      = BSP_IR_SetPWM,
    .GetRawData  = BSP_IR_GetRawData,
};


static HAL_IR_Priv_t ir_hopper_priv = {
    .type       = HAL_IR_HOPPER,
    .pwm_duty   = 0,
    .status     = HAL_IR_FREE,
    .last_data  = 0,
};

static HAL_IR_Priv_t ir_stacker_priv = {
    .type       = HAL_IR_STACKER,
    .pwm_duty   = 0,
    .status     = HAL_IR_FREE,
    .last_data  = 0,
};

static HAL_IR_Priv_t ir_right_priv = {
    .type       = HAL_IRR,
    .pwm_duty   = 0,
    .status     = HAL_IR_FREE,
    .last_data  = 0,
};

static HAL_IR_Priv_t ir_left_priv = {
    .type       = HAL_IRL,
    .pwm_duty   = 0,
    .status     = HAL_IR_FREE,
    .last_data  = 0,
};


Device_t ir_hopper = {
    .name  = "IR_HOPPER",
    .class = DEV_CLASS_SENSOR,
    .ops   = (void *)&ir_ops,
    .priv  = (void *)&ir_hopper_priv,
    .res   = (void *)&ir_hopper_res,
};

Device_t ir_stacker = {
    .name  = "IR_STACKER",
    .class = DEV_CLASS_SENSOR,
    .ops   = (void *)&ir_ops,
    .priv  = (void *)&ir_stacker_priv,
    .res   = (void *)&ir_stacker_res,
};

Device_t ir_right = {
    .name  = "IRR",
    .class = DEV_CLASS_SENSOR,
    .ops   = (void *)&ir_ops,
    .priv  = (void *)&ir_right_priv,
    .res   = (void *)&ir_right_res,
};

Device_t ir_left = {
    .name  = "IRL",
    .class = DEV_CLASS_SENSOR,
    .ops   = (void *)&ir_ops,
    .priv  = (void *)&ir_left_priv,
    .res   = (void *)&ir_left_res,
};


DEVICE_EXPORT(ir_hopper);
DEVICE_EXPORT(ir_stacker);
DEVICE_EXPORT(ir_right);
DEVICE_EXPORT(ir_left);

