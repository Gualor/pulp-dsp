/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_dot_prod_f16_parallel.c
 * Description:  16-bit float parallel dot product glue code
 *
 * $Date:        19. Mar 2022
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
  @ingroup groupMath
 */

/**
  @addtogroup BasicDotProd
  @{
 */

/**
  @brief Glue code for parallel dot product of 16-bit float vectors.
  @param[in]  pSrcA      points to the first input vector
  @param[in]  pSrcB      points to the second input vector
  @param[in]  blockSize  number of samples in each vector
  @param[in]  nPE        number of parallel processing units
  @param[out] pRes     output result returned here
  @return        none
 */

void plp_dot_prod_f16_parallel(const float16_t *__restrict__ pSrcA,
                               const float16_t *__restrict__ pSrcB,
                               uint32_t blockSize,
                               uint32_t nPE,
                               float32_t *__restrict__ pRes) {

    if (hal_cluster_id() == ARCHI_FC_CID) {
        printf("parallel processing supported only for cluster side\n");
        return;
    } else {

        uint32_t i, tmpblkSizePE = blockSize / nPE;
        float32_t resBuffer[hal_cl_nb_pe_cores()];

        plp_dot_prod_instance_f16 S;

        // Initialize the plp_dot_prod_instance
        S.pSrcA = pSrcA;
        S.pSrcB = pSrcB;
        S.blkSizePE = tmpblkSizePE;
        S.nPE = nPE;
        S.resBuffer = resBuffer;

        // Fork the dot product to nPE cores (i.e. processing units)
        hal_cl_team_fork(nPE, plp_dot_prod_f16p_xpulpv2, (void *)&S);

        float32_t sum = 0;
        for (i = 0; i < nPE; i++) {
            sum += resBuffer[i];
        }

        // Dot product on remaining blocks, loopunroll does not increase
        // performance here
        for (i = tmpblkSizePE * nPE; i < blockSize; i++) {
            sum += (pSrcA[i] * pSrcB[i]);
        }

        *pRes = sum;
    }
}

/**
  @} end of BasicDotProd group
 */
