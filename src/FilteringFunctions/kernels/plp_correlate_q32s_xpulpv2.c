/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_correlate_q32s_xpulpv2.c
 * Description:
 *
 * $Date:        03.07.2020
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2020 ETH Zurich and University of Bologna.
 *
 * Author: Moritz Scherer, ETH Zurich
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
   @ingroup BasicCorrelation
*/

/**
   @addtogroup BasicCorrelationKernels
   @{
*/

/**
   @brief Correlation of 32-bit fixed point vectors kernel for XPULPV2 extension.
   @param[in]  pSrcA      points to the first input vector
   @param[in]  srcALen   Length of the first input vector
   @param[in]  pSrcB      points to the second input vector
   @param[in]  srcBLen   Length of the second input vector
   @param[out] pRes     output result returned here
   @return        none */

void plp_correlate_q32s_xpulpv2(const int32_t *pSrcA,
                                const uint32_t srcALen,
                                const int32_t *pSrcB,
                                const uint32_t srcBLen,
                                uint32_t fracBits,
                                int32_t *pRes) {

    const int32_t *pSrc1, *pSrc2;
    int32_t src1Len, src2Len;

    int8_t switchOn = 0;

    if (srcALen >= srcBLen) {
        pSrc1 = pSrcA;
        pSrc2 = pSrcB;
        src1Len = srcALen;
        src2Len = srcBLen;
        switchOn = 0;
    } else {
        pSrc2 = pSrcA;
        pSrc1 = pSrcB;
        src2Len = srcALen;
        src1Len = srcBLen;
        switchOn = 1;
    }

    int32_t temp = 0;
    const int32_t offset = src1Len - src2Len + 1;
    const int32_t totLen = 2 * (src2Len - 1) + offset - 1;
    pRes = pRes + (switchOn * totLen);
    // Stage 1

    if (switchOn == 0) {
        for (int i = 1; i < src2Len; i++) { // Length of overlap
            for (int j = 0; j < i; j++) {
                temp += (((pSrc1[j] * pSrc2[src2Len - i + j]) >> (fracBits - 1)) + 1) >> 1;
            }
            *pRes++ = temp;
            temp = 0;
        }
    } else {
        for (int i = 1; i < src2Len; i++) { // Length of overlap
            for (int j = 0; j < i; j++) {
                temp += (((pSrc1[j] * pSrc2[src2Len - i + j]) >> (fracBits - 1)) + 1) >> 1;
            }
            *pRes-- = temp;
            temp = 0;
        }
    }

    // Stage 2

    if (switchOn == 0) {

        for (int i = 0; i < offset; i++) {
            for (int j = 0; j < src2Len; j++) {
                temp += (((pSrc1[j + i] * pSrc2[j]) >> (fracBits - 1)) + 1) >> 1;
            }
            *pRes++ = temp;
            temp = 0;
        }
    } else {
        for (int i = 0; i < offset; i++) {
            for (int j = 0; j < src2Len; j++) {
                temp += (((pSrc1[j + i] * pSrc2[j]) >> (fracBits - 1)) + 1) >> 1;
            }
            *pRes-- = temp;
            temp = 0;
        }
    }

    // Stage 3
    if (switchOn == 0) {
        for (int i = src2Len - 1; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                temp += (((pSrc1[src1Len - i + j] * pSrc2[j]) >> (fracBits - 1)) + 1) >> 1;
            }
            *pRes++ = temp;
            temp = 0;
        }
    } else {
        for (int i = src2Len - 1; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                temp += (((pSrc1[src1Len - i + j] * pSrc2[j]) >> (fracBits - 1)) + 1) >> 1;
            }
            *pRes-- = temp;
            temp = 0;
        }
    }
}

/**
   @} end of BasicCorrelationKernels
*/
