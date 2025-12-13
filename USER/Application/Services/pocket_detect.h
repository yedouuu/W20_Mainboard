
#ifndef __POCKET_DETECT_H__
#define __POCKET_DETECT_H__

#include "Common/common.h"


typedef enum {
 POCKET_HOPPER,
 POCKET_STACKER,
 POCKET_IRR,
 POCKET_IRL,
 POCKET_REJECT,  
 POCKET_REJECT_FULL,
 POCKET_REJECT_CHANNEL,
 POCKET_MAX	
} Pocket_Type_e;




Status_t Pocket_Detect_Init(void);
Status_t Pocket_Detect_Update(void);


#endif /* __POCKET_DETECT_H__ */
