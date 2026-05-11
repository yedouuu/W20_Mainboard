
#ifndef __TEST_BSP_MG_H__
#define __TEST_BSP_MG_H__

#include "unit_test.h"

#ifdef __cplusplus
extern "C" {
#endif

void TEST_MG_RegisterCheck(void);
void TEST_MG_DuplicateInitGuard(void);
void TEST_MG_DataFreshness(void);
void TEST_MG_GetRawData(void);
void TEST_MG_AllChannels(void);
void TEST_MG_ReadStability(void);
void TEST_MG_RunAllTests(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_BSP_MG_H__ */
