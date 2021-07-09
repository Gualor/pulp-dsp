/* ----------------------------------------------------------------------
 * Project:      PULP DSP Library
 * Title:        plp_dct2_f32.c
 * Description:  Floating-point DCT of type 2 on real input data
 *
 * $Date:        04. July 2021
 * $Revision:    V0
 *
 * Target Processor: PULP cores with "F" support (wolfe, vega)
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2019 ETH Zurich and University of Bologna. All rights reserved.
 *
 * Author: Aron Szakacs, ETH Zürich
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
  @ingroup groupTransforms
 */

/**
  @defgroup fft  FFT transforms
  This module contains the code to perform FFT transforms.


 */

/**
   @addtogroup fft
   @{
*/

/**
   @brief Floating-point DCT on real input data. Implementation of
   			John Makhoul's "A Fast Cosine Transform in One
			and Two Dimensions" 1980 IEEE paper. This
			implementation assumes norm = None for pytorch's DCT.
   @param[in]   S       points to an instance of the floating-point FFT
   			structure with FFTLength = DCTLength
   @param[in]   pShift  points to twiddle coefficient table of 4*FFTLength,
   			of which only the first quarter is necessary.
   @param[in]   pSrc    points to the input buffer (real data) of size
			FFTLength
   @param[out]  pBuf    points to buffer of size 2*FFTLength, used for 
   			computation.
   @param[out]  pDst    points to output buffer (real data) of size FFTLength,
   			may be the same as pSrc.
   @return      none
*/
void plp_dct2_f32(const plp_fft_instance_f32 *S,
                  const Complex_type_f32 *__restrict__ pShift,
                  const float32_t *__restrict__ pSrc,
                  float32_t *__restrict__ pBuf,
                  float32_t *__restrict__ pDst) {

    // 1: reordering (moves input from pSrc to pDst)
    uint32_t N = S->FFTLength;
    // write odd indices from Src into buffer
    for (int i=0;i<N/2;i++){
	pBuf[i] = pSrc[2*i + 1];
    }
    // squeeze remaining even indices closer together (decimate signal to odd indices) and write into Dst
    for (int i=0;i<N/2;i++){
	pDst[i] = pSrc[2*i];
    }
    // reverse and write saved odd indices into second half of Dst
    for (int i=0;i<N/2;i++){
	pDst[N/2+i] = pBuf[N/2-1-i];
    }
    // 2: RFFT of reordered sequence, result written into buffer
    plp_rfft_f32(S, pDst, pBuf);
    // RFFT must be extended to all FFTLength complex coefficients,
    // using X[k] = X*[-k] for real x[t]
    for (int i=0;i<N/2-1;i++) {
	pBuf[2*(N/2+1+i)] = pBuf[2*(N/2-1-i)]; 
	pBuf[2*(N/2+1+i)+1] = (-1)*pBuf[2*(N/2-1-i)+1];
    }
    // 3: shift FFT in place in buffer
    plp_cmplx_mult_cmplx_f32(pBuf, (float32_t*)pShift, pBuf, N);
    // 4: take real part, moving from buffer to output
    for (int i=0;i<N;i++){
	pDst[i] = pBuf[2*i];
    }
    // 5: multiply by 2 in place in output
    plp_scale_f32(pDst, 2, pDst, N);
}

/**
   @} end of FFT group
*/
