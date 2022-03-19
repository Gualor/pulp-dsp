/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_dot_prod_f16p_xpulpv2.c
 * Description:  16-bit float scalar dot product for XPULPV2 with interleaved access
 *
 * $Date:        03. Jun 2019
 * $Revision:    V0
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2019 ETH Zurich and University of Bologna.
 *
 * Author: Xiaying Wang, ETH Zurich
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Notice: project inspired by ARM CMSIS DSP and parts of source code
 * ported and adopted for RISC-V PULP platform from ARM CMSIS DSP
 * released under Copyright (C) 2010-2019 ARM Limited or its affiliates
 * with Apache-2.0.
 */

#include "plp_math.h"

#define MAC(Acc, A, B) Acc += (A * B);

/**
  @ingroup BasicDotProd
 */

/**
  @addtogroup BasicDotProdKernels
  @{
 */

/**
  @brief Parallel dot product with interleaved access of 16-bit float vectors kernel for XPULPV2
  extension.
  @param[in]  S     points to the instance structure for integer parallel dot product
  @return        none
 */

void plp_dot_prod_f16p_xpulpv2(void *S) {

    int core_id = hal_core_id();
    plp_dot_prod_instance_f16 *args = (plp_dot_prod_instance_f16 *)S;

    float16_t *pSrcA = (float16_t *)(args->pSrcA);
    float16_t *pSrcB = (float16_t *)(args->pSrcB);
    uint32_t blkSizePE = args->blkSizePE;
    uint32_t nPE = args->nPE;
    float32_t *resBufferPE = &(args->resBuffer[core_id]);

    uint32_t blkIdx, remBS;
    uint32_t coreOffset = blkSizePE * core_id;
    float16_t sum = 0;

#if defined(PLP_MATH_LOOPUNROLL)

    uint32_t blkSize2 = blkSizePE & 0xFFFFFFFE; // Make it divisible by 2
    for (blkIdx = coreOffset; blkIdx < coreOffset + blkSize2; blkIdx += 2) {
        sum = MAC(sum, pSrcA[blkIdx], pSrcB[blkIdx]);
        sum = MAC(sum, pSrcA[blkIdx + 1], pSrcB[blkIdx + 1]);
    }
    if (blkSizePE & 1U) {
        sum = MAC(sum, pSrcA[blkIdx], pSrcB[blkIdx]);
    }

#else // PLP_MATH_LOOPUNROLL

    for (blkIdx = coreOffset; blkIdx < coreOffset + blkSizePE; blkIdx++) {
        sum = MAC(sum, pSrcA[blkIdx], pSrcB[blkIdx]);
    }

#endif // PLP_MATH_LOOPUNROLL

    *resBufferPE = sum;
}

/**
   @} end of BasicDotProdKernels group
*/
