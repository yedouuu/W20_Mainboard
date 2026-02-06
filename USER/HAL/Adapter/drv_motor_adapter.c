/*
 * MIT License
 * Copyright (c) 2026 Yedouuu | UNION
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Common/common.h"
#include "DeviceManager/device_manager.h"

#include "HAL/drv_motor_wrapper.h"
#include "BSP/bsp_motor.h"

/**
 * @brief 内部辅助函数：BSP 电机操作适配器
 * @note  将通用的 DRV_Motor_State_e 转换为 BSP_Motor_Opt_e 并调用 BSP 接口
 *
 * @param res
 * @param operate
 * @return Status_t
 */
static Status_t __BSP_Motor_Operate_Adapter(const void       *res,
                                            DRV_Motor_State_e operate)
{
  BSP_Motor_Opt_e bsp_operate;

  switch (operate)
  {
    case DRV_MOTOR_STOP:
      bsp_operate = MOTOR_OPT_STOP;
      break;
    case DRV_MOTOR_FORWARD:
      bsp_operate = MOTOR_OPT_FORWARD;
      break;
    case DRV_MOTOR_BACKWARD:
      bsp_operate = MOTOR_OPT_BACKWARD;
      break;
    case DRV_MOTOR_BRAKE:
      bsp_operate = MOTOR_OPT_BRAKE;
      break;
    default:
      log_e("Invalid operate command: %d", operate);
      return kStatus_InvalidArgument;
  }

  BSP_Motor_Operate(res, bsp_operate);
  return kStatus_Success;
}

// clang-format off
static DRV_Motor_Ops_t motor_ops = {
    .base =
    {
        .init   = DRV_Motor_Init,
        .deinit = NULL,
        .open   = NULL,
        .close  = NULL,
        .read   = NULL,
        .write  = NULL,
        .ioctl  = NULL,
    },
    .magic    = DEVICE_MAGIC_MOTOR,
    .BSP_Init = BSP_Motor_Init,
    .Operate  = __BSP_Motor_Operate_Adapter,
    .SetPWM   = BSP_Motor_SetPWM,
};
// clang-format on

static DRV_Motor_Priv_t motor_main_priv = {
    .type     = DRV_MOTOR_MAIN,
    .state    = DRV_MOTOR_STOP,
    .pwm_duty = 0,
};

static DRV_Motor_Priv_t motor_stacker_priv = {
    .type     = DRV_MOTOR_STACKER,
    .state    = DRV_MOTOR_STOP,
    .pwm_duty = 0,
};

Device_t motor_main = {
    .name      = "MOTOR_MAIN",
    .dev_class = DEV_CLASS_MOTOR,
    .ops       = (void *)&motor_ops,
    .priv      = (void *)&motor_main_priv,
    .res       = (void *)&motor_main_res,
};

Device_t motor_stacker = {
    .name      = "MOTOR_STACKER",
    .dev_class = DEV_CLASS_MOTOR,
    .ops       = (void *)&motor_ops,
    .priv      = (void *)&motor_stacker_priv,
    .res       = (void *)&motor_stacker_res,
};

DEVICE_EXPORT(motor_main);
DEVICE_EXPORT(motor_stacker);
