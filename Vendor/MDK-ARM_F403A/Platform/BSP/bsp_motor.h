
#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "bsp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 电机标识枚举
 */
typedef enum
{
  MOTOR_ID_MAIN,    // 大电机（AT8236驱动）
  MOTOR_ID_STACKER, // 小电机（直接驱动）
  MOTOR_ID_MAX
} Motor_ID_e;

/**
 * @brief 电机操作枚举
 */
typedef enum
{
  MOTOR_OPT_STOP,     // 停止（自然停止）
  MOTOR_OPT_FORWARD,  // 正转
  MOTOR_OPT_BACKWARD, // 反转
  MOTOR_OPT_BRAKE,    // 刹车（快速停止）
} BSP_Motor_Opt_e;

/**
 * @brief 电机驱动类型
 */
typedef enum
{
  MOTOR_DRV_TYPE_AT8236, // AT8236芯片驱动
  MOTOR_DRV_TYPE_DIRECT, // 直接GPIO驱动
} Motor_Drv_Type_e;

typedef struct _Motor_Resource_t Motor_Resource_t;

extern const Motor_Resource_t motor_main_res;
extern const Motor_Resource_t motor_stacker_res;

/**
 * @brief 电机运行时状态
 */
typedef struct
{
  BSP_Motor_Opt_e current_opt;    // 当前操作状态
  uint8_t     current_pwm;    // 当前PWM占空比（0-100）
  uint8_t     is_initialized; // 初始化标志
} Motor_State_t;

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief 初始化单个电机
 * @param motor 电机资源指针
 */
void BSP_Motor_Init(const void *motor);

/**
 * @brief 电机操作控制
 * @param motor 电机资源指针
 * @param operate 操作类型
 */
void BSP_Motor_Operate(const void *motor, BSP_Motor_Opt_e operate);

/**
 * @brief 设置电机PWM占空比
 * @param motor 电机资源指针
 * @param duty_cycle PWM占空比 (0-100)
 */
void BSP_Motor_SetPWM(const void *motor, uint8_t duty_cycle);


#ifdef __cplusplus
}
#endif

#endif /*__BSP_MOTOR_H__*/
