/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_cmplx_conj_f32_xpulpv2.c
 * Description:  32-bit integer complex dot product glue code
 *
 * $Date:        29. June 2020
 * $Revision:    V0
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2019 ETH Zurich and Ubiversity of Bologna.
 *
 * Author: Hanna Mueller, ETH Zurich
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
  ported and adopted for RISC-V PULP platform from ARM CMSIS DSP
 released under Copyright (C) 2010-2019 ARM Limited or its affiliates
  with Apache-2.0.
 */

#include "plp_math.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @defgroup cmplx_dot_prod Complex Dot Product
  Computes the dot product of two complex vectors.
  The vectors are multiplied element-by-element and then summed.
  The <code>pSrcA</code> points to the first complex input vector and
  <code>pSrcB</code> points to the second complex input vector.
  <code>numSamples</code> specifies the number of complex samples
  and the data in each array is stored in an interleaved fashion
  (real, imag, real, imag, ...).
  Each array has a total of <code>2*numSamples</code> values.
  The underlying algorithm is used:
  <pre>
  realResult = 0;
  imagResult = 0;
  for (n = 0; n < numSamples; n++) {
      realResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+0] - pSrcA[(2*n)+1] * pSrcB[(2*n)+1];
      imagResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+1] + pSrcA[(2*n)+1] * pSrcB[(2*n)+0];
  }
  </pre>
  There are separate functions for floating point, integer, and fixed point 32- 16- 8-bit data
  types.
 */

/**
  @addtogroup cmplx_dot_prod
  @{
 */

/**
  @brief         32-bit integer complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned here
  @return        none
 */

void plp_cmplx_dot_prod_i32_xpulpv2(const int32_t *pSrcA,
                                    const int32_t *pSrcB,
                                    uint32_t numSamples,
                                    int32_t *realResult,
                                    int32_t *imagResult) {
    uint32_t blkCnt;                    /* Loop counter */
    int32_t real_sum = 0, imag_sum = 0; /* Temporary result variables */
    int32_t a0, b0, c0, d0;

    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;
    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += a0 * c0;
        imag_sum += a0 * d0;
        real_sum -= b0 * d0;
        imag_sum += b0 * c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in destination buffer. */
    *realResult = real_sum;
    *imagResult = imag_sum;
    // printf("real %d imag %d\n", real_sum, imag_sum);
}
/**
  @} end of cmplx_dot_prod group
 */