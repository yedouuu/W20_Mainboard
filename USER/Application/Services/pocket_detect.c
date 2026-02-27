
#include "pocket_detect.h"
#include "device_manager.h"
#include "drv_ir_wrapper.h"

static Device_t *ir_hop = NULL;
static Device_t *ir_sta = NULL;
static Device_t *ir_irr = NULL;
static Device_t *ir_irl = NULL;

Status_t Pocket_Detect_Init(void)
{
  ir_hop = DM_DeviceFind("IR_HOPPER");
  ir_sta = DM_DeviceFind("IR_STACKER");
  ir_irr = DM_DeviceFind("IRR");
  ir_irl = DM_DeviceFind("IRL");

  if (ir_hop == NULL || ir_sta == NULL || ir_irr == NULL || ir_irl == NULL)
  {
    log_e("IR devices not found! IRHOPPER:%p, IRSTACKER:%p, IRR:%p, IRL:%p",
          ir_hop,
          ir_sta,
          ir_irr,
          ir_irl);
    return kStatus_NotFound;
  }

  DRV_IR_Enable(ir_hop);
  DRV_IR_Enable(ir_sta);
  DRV_IR_Enable(ir_irr);
  DRV_IR_Enable(ir_irl);

  return kStatus_Success;
}

bool Pocket_Detect_IsHopperHold(void)
{
  if (ir_hop == NULL)
  {
    log_e("IR Hopper device not found!");
    return false;
  }

  uint16_t raw_hop = 0;
  DRV_IR_GetRawData(ir_hop, &raw_hop);
  return raw_hop < 1000; // 根据实际情况调整阈值
}

bool Pocket_Detect_IsStackerHold(void)
{
  if (ir_sta == NULL)
  {
    log_e("IR Stacker device not found!");
    return false;
  }

  uint16_t raw_sta = 0;
  DRV_IR_GetRawData(ir_sta, &raw_sta);
  return raw_sta > 3000; // 根据实际情况调整阈值
}

bool Pocket_Detect_IsIRRBlocked(void)
{
  if (ir_irr == NULL)
  {
    log_e("IRR device not found!");
    return false;
  }

  uint16_t raw_irr = 0;
  DRV_IR_GetRawData(ir_irr, &raw_irr);
  return raw_irr < 1000; // 根据实际情况调整阈值
}

bool Pocket_Detect_IsIRLBlocked(void)
{
  if (ir_irl == NULL)
  {
    log_e("IRL device not found!");
    return false;
  }

  uint16_t raw_irl = 0;
  DRV_IR_GetRawData(ir_irl, &raw_irl);
  return raw_irl < 1000; // 根据实际情况调整阈值
}


Status_t Pocket_Detect_Update(void)
{
  if (ir_hop == NULL || ir_sta == NULL || ir_irr == NULL || ir_irl == NULL)
  {
    log_e("IR devices not found! IRHOPPER:%p, IRSTACKER:%p, IRR:%p, IRL:%p",
          ir_hop,
          ir_sta,
          ir_irr,
          ir_irl);
    return kStatus_NotFound;
  }

  uint16_t raw_hop = 0;
  uint16_t raw_sta = 0;
  uint16_t raw_irr = 0;
  uint16_t raw_irl = 0;
  DRV_IR_GetRawData(ir_hop, &raw_hop);
  DRV_IR_GetRawData(ir_sta, &raw_sta);
  DRV_IR_GetRawData(ir_irr, &raw_irr);
  DRV_IR_GetRawData(ir_irl, &raw_irl);

  if (raw_hop < 1000)
  {
    if (raw_irl < 1000 || raw_irr < 1000)
    {
      log_w("IRL or IRR detected! Please Check Channel. Raw: %d, %d",
            raw_irl,
            raw_irr);
    }
    else
    {
      log_d("Pocket Hopper Detected! Ready to start. Raw: %d", raw_hop);
    }
  }

  if (raw_sta > 3000)
  {
    log_d("Pocket Stacker Hold! Clear Data. Raw: %d", raw_sta);
  }

  return kStatus_Success;
}
