
#include "Common/common.h"
#include "DeviceManager/device_manager.h"
#include "drv_encoder_wrapper.h"
#include "bsp_encoder.h"

// clang-format off
static DRV_Encoder_Ops_t encoder_ops = {
    .base =
    {
        .init   = DRV_Encoder_Init,
        .deinit = DRV_Encoder_DeInit,
        .open   = DRV_Encoder_Enable,
        .close  = DRV_Encoder_Disable,
        .read   = NULL,
        .write  = NULL,
        .ioctl  = NULL,
    },
    .magic      = DEVICE_MAGIC_ENCODER,
    .BSP_Init   = BSP_Encoder_Init,
    .Enable     = BSP_Encoder_Enable,
    .Disable    = BSP_Encoder_Disable,
};
// clang-format on

static DRV_Encoder_Priv_t encoder_priv = {
    .state = DRV_Encoder_NONE,
};

Device_t encoder = {
    .name      = "ENCODER",
    .dev_class = DEV_CLASS_SENSOR,
    .ops       = (void *)&encoder_ops,
    .priv      = (void *)&encoder_priv,
    .res       = NULL,
};

DEVICE_EXPORT(encoder);
