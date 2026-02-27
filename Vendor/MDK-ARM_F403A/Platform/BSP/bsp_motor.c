
#include "bsp_motor.h"
#include "mcu_core.h"
#include "mcu_type.h"

#include "Common/common.h"

struct _Motor_Resource_t
{
  const char      *name;             // 电机名称
  Motor_Drv_Type_e drv_type;         // 驱动类型
  uint8_t          in1_pin;          // IN1/PWM引脚
  uint8_t          in2_pin;          // IN2/方向引脚
  uint8_t          pwm_pin;          // 如果需要调制 PWM
  uint16_t         pwm_duty_default; // 当前 PWM 占空比
  uint16_t         pwm_resolution;   // PWM 分辨率
  uint32_t         pwm_frequency;    // PWM 频率
  uint16_t         brake_pwm;        // 刹车时的PWM占空比（0-100）
};

const Motor_Resource_t motor_main_res = {
    .name             = "MOTOR_MAIN",
    .drv_type         = MOTOR_DRV_TYPE_AT8236,
    .in1_pin          = MOTOR_MAIN_IN1_PWM_PIN,
    .in2_pin          = MOTOR_MAIN_IN2_PIN,
    .pwm_pin          = MOTOR_MAIN_IN1_PWM_PIN,
    .pwm_duty_default = 0,
    .pwm_resolution   = PWM_RESOLUTION_DEFAULT,
    .pwm_frequency    = PWM_FREQUENCY_DEFAULT,
    .brake_pwm        = 70,
};

const Motor_Resource_t motor_stacker_res = {
    .name             = "MOTOR_STACKER",
    .drv_type         = MOTOR_DRV_TYPE_DIRECT,
    .in1_pin          = MOTOR_STACKER_EN_PWM_PIN,
    .in2_pin          = MOTOR_STACKER_EN_PWM_PIN,
    .pwm_pin          = MOTOR_STACKER_EN_PWM_PIN,
    .pwm_duty_default = 0,
    .pwm_resolution   = PWM_RESOLUTION_DEFAULT,
    .pwm_frequency    = PWM_FREQUENCY_DEFAULT,
    .brake_pwm        = 70,
};

void BSP_Motor_Init(const void *motor)
{
  /* 初始化电机控制引脚 */
  Motor_Resource_t *motor_res = (Motor_Resource_t *)motor;
  int8_t            ret       = 0;
  if (motor_res->drv_type == MOTOR_DRV_TYPE_DIRECT)
  {
    pinMode(motor_res->in1_pin, OUTPUT_AF_PP);

    ret = PWM_Init(motor_res->pwm_pin,
                   motor_res->pwm_resolution,
                   motor_res->pwm_frequency);
    if (ret < 0)
    {
      log_e("Failed to initialize PWM for DIRECT motor on pin %d",
            motor_res->pwm_pin);
      return;
    }
    PWM_Write(motor_res->pwm_pin, motor_res->pwm_duty_default);
  }
  else if (motor_res->drv_type == MOTOR_DRV_TYPE_AT8236)
  {
    pinMode(motor_res->in1_pin, OUTPUT_AF_PP);
    pinMode(motor_res->in2_pin, OUTPUT);

    ret = PWM_Init(motor_res->pwm_pin,
                   motor_res->pwm_resolution,
                   motor_res->pwm_frequency);

    if (ret < 0)
    {
      log_e("Failed to initialize PWM for AT8236 motor on pin %d",
            motor_res->pwm_pin);
      return;
    }
  }
}

void BSP_Motor_Enable(const void *motor)
{
  Motor_Resource_t *motor_res = (Motor_Resource_t *)motor;
  PWM_Enable(motor_res->pwm_pin, true);
}

void BSP_Motor_Disable(const void *motor)
{
  Motor_Resource_t *motor_res = (Motor_Resource_t *)motor;
  PWM_Enable(motor_res->pwm_pin, false);
}

void BSP_Motor_SetPWM(const void *motor, uint8_t duty_cycle)
{
  if ( duty_cycle > 100 )
  {
    duty_cycle = 100;
  }

  Motor_Resource_t *motor_res = (Motor_Resource_t *)motor;
  PWM_Write(motor_res->pwm_pin, motor_res->pwm_resolution / 100 * duty_cycle);
}

void BSP_Motor_Operate(const void *motor, BSP_Motor_Opt_e operate)
{
  Motor_Resource_t *motor_res = (Motor_Resource_t *)motor;

  if (motor_res->drv_type == MOTOR_DRV_TYPE_DIRECT)
  {
    switch (operate)
    {
      case MOTOR_OPT_STOP:
        BSP_Motor_Disable(motor_res);
        // PWM_Write(motor_res->pwm_pin, 0);
        break;
      case MOTOR_OPT_FORWARD:
        BSP_Motor_Enable(motor_res);
        // PWM_Write(motor_res->pwm_pin, 500);
        break;
      default:
        break;
    }
  }
  else if (motor_res->drv_type == MOTOR_DRV_TYPE_AT8236)
  {
    switch (operate)
    {
      case MOTOR_OPT_STOP:
        log_d("MOTOR_OPT_STOP");
        BSP_Motor_Disable(motor_res);
        digitalWrite_LOW(motor_res->in2_pin);
        break;
      case MOTOR_OPT_FORWARD:
        log_d("MOTOR_OPT_FORWARD");
        digitalWrite_LOW(motor_res->in2_pin);
        BSP_Motor_Enable(motor_res);
        break;
      case MOTOR_OPT_BACKWARD:
        digitalWrite_HIGH(motor_res->in2_pin);
        // PWM_Write(motor_res->pwm_pin, motor_res->brake_pwm);
        break;
      case MOTOR_OPT_BRAKE:
        digitalWrite_HIGH(motor_res->in2_pin);
        PWM_Write(motor_res->pwm_pin, 1000);
        break;
      default:
        BSP_Motor_Disable(motor_res);
        digitalWrite_LOW(motor_res->in2_pin);
        break;
    }
  }
}
