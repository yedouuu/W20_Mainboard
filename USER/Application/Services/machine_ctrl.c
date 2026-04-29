
#include "DeviceManager/device_manager.h"
#include "Common/common.h"
#include "HAL/drv_motor_wrapper.h"
#include "HAL/drv_tick_wrapper.h"

#include "Services/pocket_detect.h"
#include "machine_ctrl.h"

static uint8_t   g_machine_status = MACHINE_IDLE;
static Device_t *g_motor_main     = NULL;
static Device_t *g_motor_sta      = NULL;

void Machine_Ctrl_Init(void)
{
  g_machine_status = MACHINE_IDLE;
  g_motor_main     = DM_DeviceFind("MOTOR_MAIN");
  g_motor_sta      = DM_DeviceFind("MOTOR_STACKER");
  if (g_motor_main == NULL) { log_e("Failed to find MOTOR_MAIN device!"); }
  if (g_motor_sta == NULL) { log_e("Failed to find MOTOR_STACKER device!"); }
}

void Conveyer_Start(void)
{
  DRV_Motor_SetPWM(g_motor_main, 70);
  DRV_Motor_Operate(g_motor_main, DRV_MOTOR_FORWARD);
  DRV_Motor_SetPWM(g_motor_sta, 30);
  DRV_Motor_Operate(g_motor_sta, DRV_MOTOR_FORWARD);
}

void Conveyer_Stop(void)
{
  DRV_Motor_Operate(g_motor_main, DRV_MOTOR_STOP);
  DRV_Motor_Operate(g_motor_sta, DRV_MOTOR_STOP);
}

void Conveyer_Brake(void)
{
  DRV_Motor_Operate(g_motor_main, DRV_MOTOR_BRAKE);
  DRV_Motor_Operate(g_motor_sta, DRV_MOTOR_BRAKE);
}

void Conveyer_Reverse(void)
{
  DRV_Motor_SetPWM(g_motor_main, 70);
  DRV_Motor_Operate(g_motor_main, DRV_MOTOR_BACKWARD);
}

void Machine_Stop(void)
{
  g_machine_status = MACHINE_STOP;
  Conveyer_Stop();
}

void Machine_FSM_Run(void)
{

  switch (g_machine_status)
  {
    case MACHINE_IDLE:
      // log_d("Machine is idle. Waiting for hopper hold...");
      if (Pocket_Detect_IsHopperHold())
      {
        log_d("Hopper hold detected, starting machine.");
        g_machine_status = MACHINE_STARTUP;
      }
      else
      {
        // Ensure machine is stopped
        Conveyer_Stop();
      }
      break;

    case MACHINE_STARTUP:
      // log_d("Machine starting up. Checking IR sensors...");
      if (Pocket_Detect_IsIRLBlocked() || Pocket_Detect_IsIRRBlocked())
      {
        log_d("IR sensor blocked, returning to idle.");
        g_machine_status = MACHINE_IDLE;
      }
      else
      {
        g_machine_status = MACHINE_RUNNING;
      }
      break;
    case MACHINE_RUNNING:
      // log_d("Machine is running. Monitoring hopper and stacker...");
      if (!Pocket_Detect_IsHopperHold())
      {
        DRV_SetInterval((TimerInterval_CB_t)Machine_Stop, 5000, TIMER_INTERVAL_ONCE);
        return;
      }
      Conveyer_Start();
      break;
    case MACHINE_STANDBY:
      if (Pocket_Detect_IsStackerHold())
      {
        // log_d("stacker released, stopping machine.");
        Conveyer_Stop();
      }
      else
      {
        g_machine_status = MACHINE_IDLE;
      }
      break;
    case MACHINE_PAUSE: {
    }
    break;
    case MACHINE_RESUME: {
    }
    break;
    case MACHINE_BREAK: {
    }
    break;
    case MACHINE_BLOCK: {
    }
    break;
    case MACHINE_STOP: {
      if (Pocket_Detect_IsStackerHold()) { g_machine_status = MACHINE_STANDBY; }
      else
      {
        g_machine_status = MACHINE_IDLE;
      }
    }
    break;

    default:
      break;
  }
}
