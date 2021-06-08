/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_sqrt_q16s_xpulpv2.c
 * Description:  16-Bit fixed point square root kernel for RV32IM
 *
 * $Date:        02.07.2020
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

 *
 * Notice: project inspired by ARM CMSIS DSP and parts of source code
 * ported and adopted for RISC-V PULP platform from ARM CMSIS DSP
 * released under Copyright (C) 2010-2019 ARM Limited or its affiliates
 * with Apache-2.0.
 */

#define sqrt2 0b1011010100000100
#include "plp_math.h"

/**
   @ingroup sqrt
*/

/**
   @defgroup sqrtKernels Sqrt Kernels

*/

/**
   @addtogroup sqrtKernels
   @{
*/

/**
   @brief         Square root of a 16-bit fixed point number for XPULPV2 extension.
   @param[in]     pSrc       points to the input vector
   @param[out]    pRes    Square root returned here
   @return        none
*/

void plp_sqrt_q16s_xpulpv2(const int16_t *__restrict__ pSrc,
                           const uint32_t fracBits,
                           int16_t *__restrict__ pRes) {

    int16_t number, temp1, intermediate_fixpoint, signBits, half;
    int32_t bits_val1;
    float temp_float1;
    union {
        int32_t fracval;
        float floatval;
    } tempconv;

    number = *pSrc;

    /* If the input is a positive number then compute the signBits. */
    if (number > 0) {
        signBits = __builtin_clz(number) - 17;

        /* Shift by the number of signBits */
        if ((signBits % 2) == 0) {
            number = number << signBits;
        } else {
            number = number << (signBits - 1);
        }

        /* Calculate half value of the number */
        half = number >> 1;
        /* Store the number for later use */
        temp1 = number;

        /* Convert to float */
        temp_float1 = number * 3.051757812500000e-005f;
        /*Store as integer */
        tempconv.floatval = temp_float1;
        bits_val1 = tempconv.fracval;
        /* Subtract the shifted value from the magic number to give intial guess */
        bits_val1 = 0x5f3759df - (bits_val1 >> 1); /* gives initial guess */
        /* Store as float */
        tempconv.fracval = bits_val1;
        temp_float1 = tempconv.floatval;
        /* Convert to integer format */
        intermediate_fixpoint = (int32_t)(temp_float1 * 16384);

        intermediate_fixpoint =
            ((int16_t)((int32_t)intermediate_fixpoint *
                           (0x3000 - ((int16_t)((((int16_t)(((int32_t)intermediate_fixpoint *
                                                             intermediate_fixpoint) >>
                                                            15)) *
                                                 (int32_t)half) >>
                                                15))) >>
                       15))
            << 2;

        intermediate_fixpoint =
            ((int16_t)((int32_t)intermediate_fixpoint *
                           (0x3000 - ((int16_t)((((int16_t)(((int32_t)intermediate_fixpoint *
                                                             intermediate_fixpoint) >>
                                                            15)) *
                                                 (int32_t)half) >>
                                                15))) >>
                       15))
            << 2;

        intermediate_fixpoint =
            ((int16_t)((int32_t)intermediate_fixpoint *
                           (0x3000 - ((int16_t)((((int16_t)(((int32_t)intermediate_fixpoint *
                                                             intermediate_fixpoint) >>
                                                            15)) *
                                                 (int32_t)half) >>
                                                15))) >>
                       15))
            << 2;

        intermediate_fixpoint = ((int16_t)(((int32_t)temp1 * intermediate_fixpoint) >> 15)) << 1;

        if ((16 - fracBits) > 1) {
            intermediate_fixpoint = intermediate_fixpoint >> ((int32_t)(16 - fracBits) >> 1);
            if ((16 - fracBits) % 2 == 0) {
                intermediate_fixpoint = ((int32_t)intermediate_fixpoint * sqrt2) >> 15;
            }
        }

        if ((signBits % 2) == 0) {
            intermediate_fixpoint = intermediate_fixpoint >> (signBits / 2);
        } else {
            intermediate_fixpoint = intermediate_fixpoint >> ((signBits - 1) / 2);
        }
        *pRes = intermediate_fixpoint;

    } else {
        *pRes = 0;
    }
}
