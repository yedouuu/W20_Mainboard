
#ifndef __KEY_SCAN_H__
#define __KEY_SCAN_H__

#include "Common/common.h"

#if defined(__cplusplus)
extern "C" {
#endif

Status_t Key_Scan_Init(void);
void Key_ScanTask(void);


#if defined(__cplusplus)
}
#endif

#endif /* __KEY_SCAN_H__ */
