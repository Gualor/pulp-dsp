/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_dot_prod_i8s_xpulpv2.c
 * Description:  8-bit integer vectorized dot product for XPULPV2
 *
 * $Date:        25. May 2019
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
  @brief Vectorized dot product of 8-bit integer vectors singlecore kernel for XPULPV2 extension.
  @param[in]  pSrcA      points to the first input vector [8 bit]
  @param[in]  pSrcB      points to the second input vector [8 bit]
  @param[in]  blockSize  number of samples in each vector
  @param[out] pRes     output result returned here [32 bit]
  @return        none

  @par Exploiting SIMD instructions
  The 8 bit values are packed four by four into 32 bit vectors and then the four dot products are
  performed on 32 bit vectors, with 32 bit accumulator.
 */

void ex_dot_prod_i8s_xpulpv2(const int8_t *__restrict__ pSrcA,
                              const int8_t *__restrict__ pSrcB,
                              uint32_t blockSize,
                              int32_t *__restrict__ pRes) {
    uint32_t blkCnt, tmpBS;     /* Loop counter, temporal BlockSize */
    int32_t sum1 = 0, sum2 = 0; /* Temporary return variable */

#if defined(PLP_MATH_LOOPUNROLL)

    tmpBS = (blockSize >> 3);

    for (blkCnt = 0; blkCnt < tmpBS; blkCnt++) {

        v4s a0 = *((v4s *)((void *)(pSrcA + 8 * blkCnt)));
        v4s b0 = *((v4s *)((void *)(pSrcB + 8 * blkCnt)));
        v4s a1 = *((v4s *)((void *)(pSrcA + 8 * blkCnt + 4)));
        v4s b1 = *((v4s *)((void *)(pSrcB + 8 * blkCnt + 4)));
        sum1 = __SUMDOTP4(a0, b0, sum1);
        sum2 = __SUMDOTP4(a1, b1, sum2);

        // sum = __MAC(sum, (*pSrcA++), (*pSrcB++));
        // sum = __MAC(sum, (*pSrcA++), (*pSrcB++));
    }

    tmpBS = (blockSize % 8U);

    for (blkCnt = 0; blkCnt < tmpBS; blkCnt++) {
        int8_t a = *((int8_t *)(pSrcA + 8 * (blockSize / 8) + blkCnt));
        int8_t b = *((int8_t *)(pSrcB + 8 * (blockSize / 8) + blkCnt));
        sum1 += a * b;
        // sum = __MAC(sum, (*pSrcA++), (*pSrcB++));
    }

#else // PLP_MATH_LOOPUNROLL

    printf("loop not unrolled\n");

    for (blkCnt = 0; blkCnt < blockSize; blkCnt++) {
        sum1 = __MAC(sum1, (*pSrcA++), (*pSrcB++));
    }

#endif // PLP_MATH_LOOPUNROLL

    *pRes = sum1 + sum2;
}

/**
  @} end of BasicDotProdKernels group
 */
