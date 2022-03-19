/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_dot_prod_i16p_xpulpv2.c
 * Description:  16-bit integer scalar dot product for XPULPV2 with interleaved access
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

/**
  @ingroup BasicDotProd
 */

/**
  @addtogroup BasicDotProdKernels
  @{
 */

/**
  @brief Parallel dot product with interleaved access of 16-bit integer vectors kernel for XPULPV2
  extension.
  @param[in]  S     points to the instance structure for integer parallel dot product
  @return        none
 */

void plp_dot_prod_i16p_xpulpv2(void *S) {

    int core_id = hal_core_id();
    plp_dot_prod_instance_i16 *args = (plp_dot_prod_instance_i16 *)S;

    int16_t *pSrcA = (int16_t *)(args->pSrcA);
    int16_t *pSrcB = (int16_t *)(args->pSrcB);
    uint32_t blkSizePE = args->blkSizePE;
    uint32_t nPE = args->nPE;
    int32_t *resBufferPE = &(args->resBuffer[core_id]);

    uint32_t blkCnt;        /* Loop counter, temporal BlockSize */
    int32_t sum1, sum2 = 0; /* Temporary return variable */

#if defined(PLP_MATH_LOOPUNROLL)

    // uint32_t endBlk = blkSizePE * core_id + (blkSizePE & 0xFFFFFFFE);

    uint32_t tmpBS = (blkSizePE >> 2);
    uint32_t coreOffset = blkSizePE * core_id;

    for (blkCnt = 0; blkCnt < tmpBS; blkCnt++) {
        /*
        if (core_id == 0)
            printf("a0 %d b0 %d a1 %d b1 %d\n", pSrcA + coreOffset + 4 * blkCnt,
                   pSrcB + coreOffset + 4 * blkCnt, pSrcA + coreOffset + 4 * blkCnt + 2,
                   pSrcB + coreOffset + 4 * blkCnt + 2);

        v2s a0 = *((v2s *)((void *)(pSrcA + coreOffset + 4 * blkCnt)));
        v2s b0 = *((v2s *)((void *)(pSrcB + coreOffset + 4 * blkCnt)));
        v2s a1 = *((v2s *)((void *)(pSrcA + coreOffset + 4 * blkCnt + 2)));
        v2s b1 = *((v2s *)((void *)(pSrcB + coreOffset + 4 * blkCnt + 2)));
        sum1 = __SUMDOTP2(a0, b0, sum1);
        sum2 = __SUMDOTP2(a1, b1, sum2);
        */
        sum1 = __MAC(sum1, pSrcA[coreOffset + 4 * blkCnt + 0], pSrcB[coreOffset + 4 * blkCnt + 0]);
        sum1 = __MAC(sum1, pSrcA[coreOffset + 4 * blkCnt + 1], pSrcB[coreOffset + 4 * blkCnt + 1]);
        sum2 = __MAC(sum2, pSrcA[coreOffset + 4 * blkCnt + 2], pSrcB[coreOffset + 4 * blkCnt + 2]);
        sum2 = __MAC(sum2, pSrcA[coreOffset + 4 * blkCnt + 3], pSrcB[coreOffset + 4 * blkCnt + 3]);
    }
    tmpBS = (blkSizePE % 4U);
    /* TODO fix bug
    for (int i = 0; i < tmpBS; i++) {
        sum1 = __MAC(sum1, pSrcA[coreOffset + 4 * blkCnt + i], pSrcB[coreOffset + 4 * blkCnt + i]);
    }
    */

#else // PLP_MATH_LOOPUNROLL

    for (blkCnt = blkSizePE * core_id; blkCnt < blkSizePE * core_id + blkSizePE; blkCnt++) {
        sum1 = __MAC(sum1, pSrcA[blkCnt], pSrcB[blkCnt]);
    }

#endif // PLP_MATH_LOOPUNROLL

    *resBufferPE = sum1 + sum2;
}

/**
   @} end of BasicDotProdKernels group
*/
