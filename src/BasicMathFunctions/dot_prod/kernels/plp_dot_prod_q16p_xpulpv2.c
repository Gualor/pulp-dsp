/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_dot_prod_q16p_xpulpv2.c
 * Description:  16-bit fixed point scalar dot product for XPULPV2 with interleaved access
 *
 * $Date:        20. Mar 2022
 * $Revision:    V0
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2019 ETH Zurich and University of Bologna.
 *
 * Author: Lorenzo Gualniera
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
  @brief Parallel dot product with interleaved access of 16-bit fixed point vectors kernel for
  XPULPV2 extension.
  @param[in]  S     points to the instance structure for fixed point parallel dot product
  @return           none
 */

void plp_dot_prod_q16p_xpulpv2(void *S) {

    int core_id = hal_core_id();
    plp_dot_prod_instance_q16 *args = (plp_dot_prod_instance_q16 *)S;

    int16_t *pSrcA = (int16_t *)(args->pSrcA);
    int16_t *pSrcB = (int16_t *)(args->pSrcB);
    uint32_t blkSizePE = args->blkSizePE;
    uint32_t deciPoint = args->deciPoint;
    uint32_t nPE = args->nPE;
    int32_t *resBufferPE = &(args->resBuffer[core_id]);

    uint32_t blkCnt, tmpBS, remBS; /* Loop counter, temporal BlockSize */
    int32_t sum = 0;

    uint32_t blkSize = (blkSizePE / nPE) & 0xFFFFFFFC; // Makes it divisible by 4

    // set pSrcA and pSrcB to the correct location
    pSrcA += core_id * blkSize;
    pSrcB += core_id * blkSize;

    // set the block size for the last core
    if (core_id == nPE - 1) {
        blkSize = blkSizePE - (nPE - 1) * blkSize;
    }

    if (blkSize == 0) {
        *resBufferPE = 0;
        return;
    }

#if defined(PLP_MATH_LOOPUNROLL)

    tmpBS = (blkSize >> 2);

    for (blkCnt = 0; blkCnt < tmpBS; blkCnt++) {
        v2s a0 = *((v2s *)((void *)(pSrcA + 4 * blkCnt)));
        v2s b0 = *((v2s *)((void *)(pSrcB + 4 * blkCnt)));
        v2s a1 = *((v2s *)((void *)(pSrcA + 4 * blkCnt + 2)));
        v2s b1 = *((v2s *)((void *)(pSrcB + 4 * blkCnt + 2)));
        asm volatile("" ::: "memory"); // Inhibits compiler reordering of memory accesses
        int32_t x0 = __DOTP2(a0, b0);
        int32_t x1 = __DOTP2(a1, b1);
        sum += __ADDROUNDNORM_REG(x0, x1, deciPoint);
    }

    remBS = (blkSize % 4U);

    for (blkCnt = 0; blkCnt < remBS; blkCnt++) {
        int16_t a = *(pSrcA + 4 * tmpBS + blkCnt);
        int16_t b = *(pSrcB + 4 * tmpBS + blkCnt);
        sum += __ROUNDNORM_REG(a * b, deciPoint);
    }

#else // PLP_MATH_LOOPUNROLL

    for (blkCnt = 0; blkCnt < blkSize; blkCnt++) {
        sum += __ROUNDNORM_REG((*pSrcA++) * (*pSrcB++), deciPoint);
    }

#endif // PLP_MATH_LOOPUNROLL

    *resBufferPE = sum;
}

/**
   @} end of BasicDotProdKernels group
*/
