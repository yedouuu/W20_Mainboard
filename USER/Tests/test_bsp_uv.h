
#ifndef __TEST_BSP_UV_H__
#define __TEST_BSP_UV_H__

#include "unit_test.h"

#ifdef __cplusplus
extern "C" {
#endif

void TEST_UV_RegisterCheck(void);
void TEST_UV_DuplicateInitGuard(void);
void TEST_UV_DataFreshness(void);
void TEST_UV_GetRawData(void);
void TEST_UV_AllChannels(void);
void TEST_UV_ReadStability(void);
void TEST_UV_RunAllTests(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_BSP_UV_H__ */
