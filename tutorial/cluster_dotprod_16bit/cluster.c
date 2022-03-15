#include "rtos_hal.h"
#include "stdio.h"
#include "plp_math.h"
#include "data.h"


void cluster_entry_i16(void* args) {

  printf("Cluster entered. Compute for i16.\n\n");

  int32_t result = 0;

  printf("\nCompute with single core\n\n");

  // We also count the number of cycles taken to compute it.
  // This structure will hold the configuration and also the results in the
  // cumulative mode
  hal_perf_t perf;

  // It must be initiliazed at least once, this will set all values in the
  // structure to zero.
  hal_perf_init(&perf);

  // Activate specified events
  hal_perf_conf(&perf, (1<<HAL_PERF_CYCLES) | (1<<HAL_PERF_INSTR)); // Note: on gvsoc you can activate as many counters as you want, while when you run on board, there is only one HW counter.

  // Reset HW counters now and start and stop counters so that we benchmark
  // only around the printf
  hal_perf_reset(&perf);
  hal_perf_start(&perf);

  plp_dot_prod_i16s_xpulpv2(a_cl, b_cl, VLEN, &result);

  hal_perf_stop(&perf);

  printf("The true result is %d, the calculated result is %d.\n", res_i16, result);
  printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
  printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

  printf("\nCompute with 8 parallel cores\n\n");

  // Reset HW counters now and start and stop counters so that we benchmark
  // only around the printf
  hal_perf_reset(&perf);
  hal_perf_start(&perf);

  plp_dot_prod_i16_parallel(a_cl, b_cl, VLEN, 8, &result);

  hal_perf_stop(&perf);

  printf("The true result is %d, the calculated result is %d.\n", res_i16, result);
  printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
  printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

  return;
}


void cluster_entry_q16(void* args) {

  printf("Cluster entered. Compute for q16.\n\n");

  int32_t result = 0;

  printf("\nCompute with single core\n\n");

  // We also count the number of cycles taken to compute it.
  // This structure will hold the configuration and also the results in the
  // cumulative mode
  hal_perf_t perf;

  // It must be initiliazed at least once, this will set all values in the
  // structure to zero.
  hal_perf_init(&perf);

  // Activate specified events
  hal_perf_conf(&perf, (1<<HAL_PERF_CYCLES) | (1<<HAL_PERF_INSTR)); // Note: on gvsoc you can activate as many counters as you want, while when you run on board, there is only one HW counter.

  // Reset HW counters now and start and stop counters so that we benchmark
  // only around the printf
  hal_perf_reset(&perf);
  hal_perf_start(&perf);

  plp_dot_prod_q16s_xpulpv2(a_cl, b_cl, VLEN, DECIPOINT, &result);

  hal_perf_stop(&perf);

  printf("The true result is %d, the calculated result is %d.\n", res_q16, result);
  printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
  printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

  printf("\nCompute with 8 parallel cores\n\n");

  // Reset HW counters now and start and stop counters so that we benchmark
  // only around the printf
  hal_perf_reset(&perf);
  hal_perf_start(&perf);

  plp_dot_prod_q16_parallel(a_cl, b_cl, VLEN, DECIPOINT, 8, &result);

  hal_perf_stop(&perf);

  printf("The true result is %d, the calculated result is %d.\n", res_q16, result);
  printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
  printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

  return;
}


void cluster_entry_i32(void* args) {

  printf("Cluster entered. Compute for i32.\n\n");

  int32_t result = 0;

  printf("\nCompute with single core\n\n");

  // We also count the number of cycles taken to compute it.
  // This structure will hold the configuration and also the results in the
  // cumulative mode
  hal_perf_t perf;

  // It must be initiliazed at least once, this will set all values in the
  // structure to zero.
  hal_perf_init(&perf);

  // Activate specified events
  hal_perf_conf(&perf, (1<<HAL_PERF_CYCLES) | (1<<HAL_PERF_INSTR)); // Note: on gvsoc you can activate as many counters as you want, while when you run on board, there is only one HW counter.

  // Reset HW counters now and start and stop counters so that we benchmark
  // only around the printf
  hal_perf_reset(&perf);
  hal_perf_start(&perf);

  plp_dot_prod_i32s_xpulpv2(a_cl, b_cl, VLEN, &result);

  hal_perf_stop(&perf);

  printf("The calculated result is %d.\n", result);
  printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
  printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

  printf("\nCompute with 8 parallel cores\n\n");

  // Reset HW counters now and start and stop counters so that we benchmark
  // only around the printf
  hal_perf_reset(&perf);
  hal_perf_start(&perf);

  plp_dot_prod_i32_parallel(a_cl, b_cl, VLEN, 8, &result);

  hal_perf_stop(&perf);

  printf("The calculated result is %d.\n", result);
  printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
  printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

  return;
}
