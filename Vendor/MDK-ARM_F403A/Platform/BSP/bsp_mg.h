/*
 * MIT License
 * Copyright (c) 2025 Yedouuu | UNION
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __BSP_MG_H__
#define __BSP_MG_H__

/* Includes ----------------------------------------------------------*/
#include "mcu_type.h"
#include "bsp_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MGS_LEFT_1  = 0,
  MGS_LEFT_2  = 1,
  MGS_LEFT_3  = 2,
  MGS_RIGHT_1 = 3,
  MGS_RIGHT_2 = 4,
  MGS_RIGHT_3 = 5,
  MGB_BM      = 6,
  MGB_HD      = 7,
  MG_MAX
} MG_type_e;

typedef struct _MG_Resource_t MG_Resource_t;

extern const MG_Resource_t mgs_left_1_res;
extern const MG_Resource_t mgs_left_2_res;
extern const MG_Resource_t mgs_left_3_res;
extern const MG_Resource_t mgs_right_1_res;
extern const MG_Resource_t mgs_right_2_res;
extern const MG_Resource_t mgs_right_3_res;
extern const MG_Resource_t mgb_bm_res;
extern const MG_Resource_t mgb_hd_res;

/* Exported functions prototypes ---------------------------------------------*/
void BSP_MG_Init(const void *mg);
void BSP_MG_GetRawData(const void *mg, uint16_t *raw);

#ifdef __cplusplus
}
#endif

#endif
