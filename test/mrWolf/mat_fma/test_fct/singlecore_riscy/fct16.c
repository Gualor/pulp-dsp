#include "fct16.h"
#include <stdio.h>

// #define BLOCK_VERSION_PARALELL
// #define BLOCK_VERSION
#define BLOCK_VERSION_SHUFFLE

#ifdef BLOCK_VERSION_PARALELL

void plp_mat_mult_i16s_xpulpv2(
                              const int16_t * __restrict__ pSrcA,
                              const int16_t * __restrict__ pSrcB,
                              uint32_t M,
                              uint32_t N,
                              uint32_t O,
                              int32_t * __restrict__ pDstC) {
        
        uint32_t i; // loop counter for M
        uint32_t j; // loop counter for N
        uint32_t k; // loop counter for O
        
        for(i=0; i < M/4; i++){
          for(k=0; k < O; k++){

            int32_t sum00 = 0;
            // int32_t sum01 = 0;
            int32_t sum10 = 0;
            // int32_t sum11 = 0;
            int32_t sum20 = 0;
            // int32_t sum21 = 0;
            int32_t sum30 = 0;
            // v2s* Bpoint = (v2s*) &(pSrcB[k]);

            for(j=0; j<N/2; j++){
              
              v2s aVec0 = *((v2s*)&(pSrcA[(i*4  )*N + (j*2  )]));
              v2s aVec1 = *((v2s*)&(pSrcA[(i*4+1)*N + (j*2  )]));
              v2s aVec2 = *((v2s*)&(pSrcA[(i*4+2)*N + (j*2  )]));
              v2s aVec3 = *((v2s*)&(pSrcA[(i*4+3)*N + (j*2  )]));

              int16_t BVal00 = pSrcB[(j*2  )*O+(k )];
              // int16_t BVal01 = pSrcB[(j*2  )*O+(k*2+1)];
              int16_t BVal10 = pSrcB[(j*2+1)*O+(k )];
              // int16_t BVal11 = pSrcB[(j*2+1)*O+(k*2+1)];

              v2s bVec0 = {BVal00, BVal10};
              // v2s bVec1 = {BVal01, BVal11};

              sum00 = __SUMDOTP2(aVec0, bVec0, sum00);
              // sum01 = __SUMDOTP2(aVec0, bVec1, sum01);
              sum10 = __SUMDOTP2(aVec1, bVec0, sum10);
              // sum11 = __SUMDOTP2(aVec1, bVec1, sum11);
              sum20 = __SUMDOTP2(aVec2, bVec0, sum20);
              // sum21 = __SUMDOTP2(aVec2, bVec1, sum21);
              sum30 = __SUMDOTP2(aVec3, bVec0, sum30);

            }

            pDstC[(i*4  )*O +(k  )] = sum00;
            // pDstC[(i*4  )*O +(k*2+1)] = sum01;
            pDstC[(i*4+1)*O +(k  )] = sum10;
            // pDstC[(i*4+1)*O +(k*2+1)] = sum11;
            pDstC[(i*4+2)*O +(k  )] = sum20;
            // pDstC[(i*4+2)*O +(k*2+1)] = sum21;
            pDstC[(i*4+3)*O +(k  )] = sum30;

          }
        }

                
        // clean up code
        i = i*4;
        j = j*2;
        k = k;
        //check if every index is nicely finished
        if(i == M && j == N && k == O){
          return;
        } else {
          uint32_t iEnd = i;
          uint32_t jEnd = j;
          uint32_t kEnd = k;

          if(i == 0 || k == 0 || j == 0){
            for(; i < M; i++){
              for(; k < O; k++){
                int32_t sum = 0;
                for(; j<N; j++){
                  sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                }
                pDstC[i*O + k] = sum;
              }
            }
          } else {
            // clean up for j
            if(jEnd != N){
              for(i = 0; i < iEnd; i++){
                for(k = 0; k < kEnd; k++){
                  int32_t sum = 0;
                  for(j = jEnd; j < N; j++){
                    sum += sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                  }
                  pDstC[i*O+k] += sum;
                }
              }
            }

            // clean up for k
            if(kEnd != O){
              for(i = 0; i < iEnd; i++){
                for(k = kEnd; k < O; k++){
                  int32_t sum = 0;
                  for(j=0; j<N; j++){
                    sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                  }
                  pDstC[i*O + k] = sum;
                }
              }
            }
            
            // clean up for i
            for(i = iEnd; i < M; i++){
              for(k = 0; k < O; k++){
                int32_t sum = 0;
                for(j = 0; j < N; j++){
                  sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                }
                pDstC[i*O + k] = sum;
              }
            }
          }
        }

}

#elif defined(BLOCK_VERSION)

void plp_mat_mult_i16s_xpulpv2(
                              const int16_t * __restrict__ pSrcA,
                              const int16_t * __restrict__ pSrcB,
                              uint32_t M,
                              uint32_t N,
                              uint32_t O,
                              int32_t * __restrict__ pDstC) {
        
        uint32_t i; // loop counter for M
        uint32_t j; // loop counter for N
        uint32_t k; // loop counter for O
        
        for(i=0; i < M/2; i++){
          for(k=0; k < O/2; k++){

            int32_t sum00 = 0;
            int32_t sum01 = 0;
            int32_t sum10 = 0;
            int32_t sum11 = 0;

            // v2s* Bpoint = (v2s*) &(pSrcB[k]);

            for(j=0; j<N/2; j++){
              
              v2s aVec0 = *((v2s*)&(pSrcA[(i*2  )*N + (j*2  )]));
              v2s aVec1 = *((v2s*)&(pSrcA[(i*2+1)*N + (j*2  )]));

              int16_t BVal00 = pSrcB[(j*2  )*O+(k*2 )];
              int16_t BVal01 = pSrcB[(j*2  )*O+(k*2+1)];
              int16_t BVal10 = pSrcB[(j*2+1)*O+(k*2 )];
              int16_t BVal11 = pSrcB[(j*2+1)*O+(k*2+1)];

              v2s bVec0 = {BVal00, BVal10};
              v2s bVec1 = {BVal01, BVal11};

              sum00 = __SUMDOTP2(aVec0, bVec0, sum00);
              sum01 = __SUMDOTP2(aVec0, bVec1, sum01);
              sum10 = __SUMDOTP2(aVec1, bVec0, sum10);
              sum11 = __SUMDOTP2(aVec1, bVec1, sum11);

            }

            pDstC[(i*2  )*O +(k*2  )] = sum00;
            pDstC[(i*2  )*O +(k*2+1)] = sum01;
            pDstC[(i*2+1)*O +(k*2  )] = sum10;
            pDstC[(i*2+1)*O +(k*2+1)] = sum11;

          }
        }
                
        // clean up code
        i = i*2;
        j = j*2;
        k = k*2;
        //check if every index is nicely finished
        if(i == M && j == N && k == O){
          return;
        } else {
          uint32_t iEnd = i;
          uint32_t jEnd = j;
          uint32_t kEnd = k;

          if(i == 0 || k == 0 || j == 0){
            for(; i < M; i++){
              for(; k < O; k++){
                int32_t sum = 0;
                for(; j<N; j++){
                  sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                }
                pDstC[i*O + k] = sum;
              }
            }
          } else {
            // clean up for j
            if(jEnd != N){
              for(i = 0; i < iEnd; i++){
                for(k = 0; k < kEnd; k++){
                  int32_t sum = 0;
                  for(j = jEnd; j < N; j++){
                    sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                  }
                  pDstC[i*O+k] += sum;
                }
              }
            }

            // clean up for k
            if(kEnd != O){
              for(i = 0; i < iEnd; i++){
                for(k = kEnd; k < O; k++){
                  int32_t sum = 0;
                  for(j=0; j<N; j++){
                    sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                  }
                  pDstC[i*O + k] = sum;
                }
              }
            }
            
            // clean up for i
            for(i = iEnd; i < M; i++){
              for(k = 0; k < O; k++){
                int32_t sum = 0;
                for(j = 0; j < N; j++){
                  sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                }
                pDstC[i*O + k] = sum;
              }
            }
          }
        }
}

#elif defined(BLOCK_VERSION_SHUFFLE)

void plp_mat_mult_i16s_xpulpv2(
                              const int16_t * __restrict__ pSrcA,
                              const int16_t * __restrict__ pSrcB,
                              uint32_t M,
                              uint32_t N,
                              uint32_t O,
                              int32_t * __restrict__ pDstC) {
        
        uint32_t i=0; // loop counter for M
        uint32_t j=0; // loop counter for N
        uint32_t k=0; // loop counter for O
        
        for(i=0; i < M/4; i++){
          for(k=0; k < O/2; k++){

            int32_t sum00 = 0;
            int32_t sum01 = 0;
            int32_t sum10 = 0;
            int32_t sum11 = 0;
            int32_t sum20 = 0;
            int32_t sum21 = 0;
            int32_t sum30 = 0;
            int32_t sum31 = 0;

            // v2s* Bpoint = (v2s*) &(pSrcB[k]);

            for(j=0; j<N/2; j++){
              
              v2s aVec0 = *((v2s*)&(pSrcA[(i*4  )*N + (j*2  )]));
              v2s aVec1 = *((v2s*)&(pSrcA[(i*4+1)*N + (j*2  )]));
              v2s aVec2 = *((v2s*)&(pSrcA[(i*4+2)*N + (j*2  )]));
              v2s aVec3 = *((v2s*)&(pSrcA[(i*4+3)*N + (j*2  )]));

              // int16_t BVal00 = pSrcB[(j*2  )*O+(k*2 )];
              // int16_t BVal01 = pSrcB[(j*2  )*O+(k*2+1)];
              // int16_t BVal10 = pSrcB[(j*2+1)*O+(k*2 )];
              // int16_t BVal11 = pSrcB[(j*2+1)*O+(k*2+1)];

              // v2s bVec0 = {BVal00, BVal10};
              // v2s bVec1 = {BVal01, BVal11};

              v2s bTemp0 = *((v2s*)&(pSrcB[(j*2  )*O + (k*2  )]));
              v2s bTemp1 = *((v2s*)&(pSrcB[(j*2+1)*O + (k*2  )]));

              v2s bVec0 = __builtin_shuffle(bTemp0, bTemp1, (v2s){0,2});
              v2s bVec1 = __builtin_shuffle(bTemp0, bTemp1, (v2s){1,3});

              sum00 = __SUMDOTP2(aVec0, bVec0, sum00);
              sum01 = __SUMDOTP2(aVec0, bVec1, sum01);
              sum10 = __SUMDOTP2(aVec1, bVec0, sum10);
              sum11 = __SUMDOTP2(aVec1, bVec1, sum11);
              sum20 = __SUMDOTP2(aVec2, bVec0, sum20);
              sum21 = __SUMDOTP2(aVec2, bVec1, sum21);
              sum30 = __SUMDOTP2(aVec3, bVec0, sum30);
              sum31 = __SUMDOTP2(aVec3, bVec1, sum31);

            }

            pDstC[(i*4  )*O +(k*2  )] = sum00;
            pDstC[(i*4  )*O +(k*2+1)] = sum01;
            pDstC[(i*4+1)*O +(k*2  )] = sum10;
            pDstC[(i*4+1)*O +(k*2+1)] = sum11;
            pDstC[(i*4+2)*O +(k*2  )] = sum20;
            pDstC[(i*4+2)*O +(k*2+1)] = sum21;
            pDstC[(i*4+3)*O +(k*2  )] = sum30;
            pDstC[(i*4+3)*O +(k*2+1)] = sum31;

          }
        }
                
        // clean up code
        i = i*4;
        j = j*2;
        k = k*2;
        //check if every index is nicely finished
        if(i == M && j == N && k == O){
          
        } else {
          uint32_t iEnd = i;
          uint32_t jEnd = j;
          uint32_t kEnd = k;

          // clean up for j
          if(jEnd != N){
            for(i = 0; i < iEnd; i++){
              for(k = 0; k < kEnd; k++){
                int32_t sum = 0;
                for(j = jEnd; j < N; j++){
                  sum += sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                }
                pDstC[i*O+k] += sum;
              }
            }
          }

          // clean up for k
          if(kEnd != O){
            for(i = 0; i < iEnd; i++){
              for(k = kEnd; k < O; k++){
                int32_t sum = 0;
                for(j=0; j<N; j++){
                  sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
                }
                pDstC[i*O + k] = sum;
              }
            }
          }

          // clean up for i
          for(i = iEnd; i < M; i++){
            for(k = 0; k < O; k++){
              int32_t sum = 0;
              for(j = 0; j < N; j++){
                sum = sum + pSrcA[i*N + j]*pSrcB[j*O + k];
              }
              pDstC[i*O + k] = sum;
            }
          }
        }
}

#else

void plp_mat_mult_i16s_xpulpv2(
                              const int16_t * __restrict__ pSrcA,
                              const int16_t * __restrict__ pSrcB,
                              uint32_t M,
                              uint32_t N,
                              uint32_t O,
                              int32_t * __restrict__ pDstC) {
        
        uint32_t i; // loop counter
        uint32_t j; // loop counter
        uint32_t k; // loop counter

        for(i=0; i < M; i++){
          for(k=0; k < O; k++){
            int16_t sum = 0;
            for(j=0; j<N; j++){
              sum = sum + pSrcA[i*N + j]*pSrcB[j*O+k];
            }
            pDstC[i*O +k] = sum;
          }
        }
}

#endif
