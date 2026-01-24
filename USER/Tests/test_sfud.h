

#ifndef __TEST_SFUD_H__
#define __TEST_SFUD_H__

#include "unit_test.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Basic test functions */
void TEST_SFUD_Init(void);
void TEST_SFUD_ReadID(void);
void TEST_SFUD_EraseWriteRead(void);

/* Extended test functions for comprehensive coverage */
void TEST_SFUD_ReadVariousSizes(void);
void TEST_SFUD_WriteVariousSizes(void);
void TEST_SFUD_EraseOperations(void);
void TEST_SFUD_StatusOperations(void);
void TEST_SFUD_BoundaryConditions(void);
void TEST_SFUD_DataIntegrity(void);
void TEST_SFUD_Performance(void);
void TEST_SFUD_ErrorHandling(void);

/* Master test suite runner */
void TEST_SFUD_RunAllTests(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEST_SFUD_H__ */
