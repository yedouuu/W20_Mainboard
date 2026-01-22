
#ifndef __FAULT_TEST_H__
#define __FAULT_TEST_H__

#include "Common/common.h"

#ifdef __cplusplus
  extern "C" {
#endif

void fault_test_by_unalign(void);
void fault_test_by_div0(void);


#ifdef __cplusplus
}
#endif

#endif // __FAULT_TEST_H__
