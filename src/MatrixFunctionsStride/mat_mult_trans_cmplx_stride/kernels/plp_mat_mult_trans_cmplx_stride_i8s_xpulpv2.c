/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_mat_mult_trans_cmplx_stride_i8s_xpulpv2.c
 * Description:  8-bit integer complex strided matrix transpose matrix multiplication for XPULPV2
 *
 * $Date:        18. July 2020
 * $Revision:    V0
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2020 ETH Zurich and Ubiversity of Bologna. All rights reserved.
 *
 * Author: Tibor Schneider, ETH Zurich
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
 */

#include "plp_math.h"

/**
  @ingroup MatMultTransCmplxStride
 */

/**
  @addtogroup MatMultTransCmplxStrideKernels
  @{
 */

/**
  @brief      Strided strided matrix transpose matrix multiplication for complex 8-bit integers on
              XpulpV2
  @param[in]  pSrcA   Points to the first input matrix of shape MxN
  @param[in]  pSrcB   Points to the second input matrix of shape OxN
  @param[in]  M       Height of matrix SrcA and DstC
  @param[in]  N       Width of matrix SrcA and SrcB
  @param[in]  O       Height of matrix SrcB and width of matrix DstC
  @param[in]  strideA Stride of input matrix A (elements between each row)
  @param[in]  strideB Stride of input matrix B (elements between each row)
  @param[in]  strideC Stride of output matrix C (Elements between each row)
  @param[out] pDstC   Points to the output matrix of shape MxO
  @return     none
 */

void plp_mat_mult_trans_cmplx_stride_i8s_xpulpv2(const int8_t *__restrict__ pSrcA,
                                                 const int8_t *__restrict__ pSrcB,
                                                 uint32_t M,
                                                 uint32_t N,
                                                 uint32_t O,
                                                 uint32_t strideA,
                                                 uint32_t strideB,
                                                 uint32_t strideC,
                                                 int32_t *__restrict__ pDstC) {

#define BASIC_VERSION // if used don't forget to also use the undefine at end of file
#ifdef BASIC_VERSION

    for (int m = 0; m < M; m++) {
        for (int o = 0; o < O; o++) {
            int32_t sum_re = 0;
            int32_t sum_im = 0;
            for (int n = 0; n < N; n++) {
                int32_t a_re = (int32_t)pSrcA[(m * strideA + n) * 2 + 0];
                int32_t a_im = (int32_t)pSrcA[(m * strideA + n) * 2 + 1];
                int32_t b_re = (int32_t)pSrcB[(o * strideB + n) * 2 + 0];
                int32_t b_im = (int32_t)pSrcB[(o * strideB + n) * 2 + 1];
                sum_re += a_re * b_re - a_im * b_im;
                sum_im += a_re * b_im + a_im * b_re;
            }
            pDstC[(m * strideC + o) * 2 + 0] = sum_re;
            pDstC[(m * strideC + o) * 2 + 1] = sum_im;
        }
    }

#else

    // TODO: Hackathon

#endif
#undef BASIC_VERSION
}
/**
   @} end of MatMultTransCmplxStrideKernels group
*/
