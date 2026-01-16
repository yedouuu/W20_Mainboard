
#include "Common/common.h"
#include "DeviceManager/device_manager.h"
#include "drv_ir_wrapper.h"
#include "bsp_ir.h"

static Status_t __DRV_IR_Read_Adapter(Device_t *dev, uint8_t *buf, uint32_t len)
{
  if (len < sizeof(uint16_t))
  {
    log_e("buffer too small needs at least %d bytes", sizeof(uint16_t));
    return kStatus_InvalidArgument;
  }

  uint16_t raw_val = 0;
  Status_t ret     = DRV_IR_GetRawData(dev, &raw_val);

  if (ret == kStatus_Success) { memcpy(buf, &raw_val, 2); }

  return ret;
}

// clang-format off
static DRV_IR_Ops_t ir_ops = {
    .base =
    {
        .init   = DRV_IR_Init,
        .deinit = DRV_IR_DeInit,
        .open   = DRV_IR_Enable,
        .close  = DRV_IR_Disable,
        .read   = __DRV_IR_Read_Adapter,
        .write  = NULL,
        .ioctl  = NULL,
    },
    .magic      = DEVICE_MAGIC_IR,
    .BSP_Init   = BSP_IR_Init,
    .Enable     = BSP_IR_Enable,
    .Disable    = BSP_IR_Disable,
    .SetPWM     = BSP_IR_SetPWM,
    .GetRawData = BSP_IR_GetRawData,
};
// clang-format on

static DRV_IR_Priv_t ir_hopper_priv = {
    .type       = DRV_IR_HOPPER,
    .pwm_duty   = 0,
    .state      = DRV_IR_FREE,
    .close_data = 0,
    .open_data  = 0,
    .diff_thred = 0,
    .hold_tick  = 0,
    .free_tick  = 0,
};

static DRV_IR_Priv_t ir_stacker_priv = {
    .type       = DRV_IR_STACKER,
    .pwm_duty   = 0,
    .state      = DRV_IR_FREE,
    .close_data = 0,
    .open_data  = 0,
    .diff_thred = 0,
    .hold_tick  = 0,
    .free_tick  = 0,
};

static DRV_IR_Priv_t ir_right_priv = {
    .type       = DRV_IRR,
    .pwm_duty   = 0,
    .state      = DRV_IR_FREE,
    .close_data = 0,
    .open_data  = 0,
    .diff_thred = 0,
    .hold_tick  = 0,
    .free_tick  = 0,
};

static DRV_IR_Priv_t ir_left_priv = {
    .type       = DRV_IRL,
    .pwm_duty   = 0,
    .state      = DRV_IR_FREE,
    .close_data = 0,
    .open_data  = 0,
    .diff_thred = 0,
    .hold_tick  = 0,
    .free_tick  = 0,
};

Device_t ir_hopper = {
    .name      = "IR_HOPPER",
    .dev_class = DEV_CLASS_SENSOR,
    .ops       = (void *)&ir_ops,
    .priv      = (void *)&ir_hopper_priv,
    .res       = (void *)&ir_hopper_res,
};

Device_t ir_stacker = {
    .name      = "IR_STACKER",
    .dev_class = DEV_CLASS_SENSOR,
    .ops       = (void *)&ir_ops,
    .priv      = (void *)&ir_stacker_priv,
    .res       = (void *)&ir_stacker_res,
};

Device_t ir_right = {
    .name      = "IRR",
    .dev_class = DEV_CLASS_SENSOR,
    .ops       = (void *)&ir_ops,
    .priv      = (void *)&ir_right_priv,
    .res       = (void *)&ir_right_res,
};

Device_t ir_left = {
    .name      = "IRL",
    .dev_class = DEV_CLASS_SENSOR,
    .ops       = (void *)&ir_ops,
    .priv      = (void *)&ir_left_priv,
    .res       = (void *)&ir_left_res,
};

DEVICE_EXPORT(ir_hopper);
DEVICE_EXPORT(ir_stacker);
DEVICE_EXPORT(ir_right);
DEVICE_EXPORT(ir_left);
