
#ifndef __MACHINE_CTRL_H__
#define __MACHINE_CTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  MACHINE_IDLE,
  MACHINE_STARTUP,
  MACHINE_RUNNING,
  MACHINE_STANDBY,
  MACHINE_RESUME,
  MACHINE_PAUSE,
  MACHINE_BREAK,
  MACHINE_BLOCK,
  MACHINE_STOP,
} machine_status_e;

void Machine_Ctrl_Init(void);
void Machine_FSM_Run(void);


#ifdef __cplusplus
}
#endif

#endif /* __MACHINE_CTRL_H__ */
