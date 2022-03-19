/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_dot_prod_q16p_xpulpv2.c
 * Description:  16-bit fixed point scalar dot product for XPULPV2 with interleaved access
 *
 * $Date:        04. Jun 2019
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
  @brief Parallel dot product with interleaved access of 16-bit fixed point vectors kernel for
  XPULPV2 extension.
  @param[in]  S     points to the instance structure for fixed point parallel dot product
  @return        none
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

    uint32_t blkCnt; /* Loop counter, temporal BlockSize */
    int32_t sum = 0; /* Temporary return variable */

#if defined(PLP_MATH_LOOPUNROLL)

    int32_t x0, x1 = 0;
    uint32_t loopCycles = (blkSizePE / nPE) & 0xFFFFFFFE;
    for (blkCnt = blkSizePE * core_id; blkCnt < blkSizePE * core_id + loopCycles; blkCnt += 2) {
        x0 = pSrcA[blkCnt] * pSrcB[blkCnt];
        x1 = pSrcA[blkCnt + 1] * pSrcB[blkCnt + 1];
        sum += __ADDROUNDNORM_REG(x0, x1, deciPoint);
    }
    if (blkSizePE & 1U) {
        sum += __ROUNDNORM_REG(pSrcA[blkCnt] * pSrcB[blkCnt], deciPoint);
    }

#else // PLP_MATH_LOOPUNROLL

    for (blkCnt = blkSizePE * core_id; blkCnt < blkSizePE * core_id + blkSizePE; blkCnt++) {
        sum += __ROUNDNORM_REG(pSrcA[blkCnt] * pSrcB[blkCnt], deciPoint);
    }

#endif // PLP_MATH_LOOPUNROLL

    *resBufferPE = sum;
}

/**
   @} end of BasicDotProdKernels group
*/
