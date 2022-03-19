#include "cluster.h"
#include "data.h"
#include "plp_math.h"
#include "rtos_hal.h"
#include "stdio.h"

void cluster_entry_i16(void *args) {

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
    hal_perf_conf(&perf, (1 << HAL_PERF_CYCLES) |
                             (1 << HAL_PERF_INSTR)); // Note: on gvsoc you can activate as many
                                                     // counters as you want, while when you run on
                                                     // board, there is only one HW counter.

    // Reset HW counters now and start and stop counters so that we benchmark
    // only around the printf
    hal_perf_reset(&perf);
    hal_perf_start(&perf);

    plp_dot_prod_i16s_xpulpv2(a_i16_cl, b_i16_cl, VLEN, &result);

    hal_perf_stop(&perf);

    printf("The true result is %d, the calculated result is %d.\n", res_i16, result);
    printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
    printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

    printf("\nCompute with %d parallel cores\n\n", NUM_CORES);

    // Reset HW counters now and start and stop counters so that we benchmark
    // only around the printf
    hal_perf_reset(&perf);
    hal_perf_start(&perf);

    plp_dot_prod_i16_parallel(a_i16_cl, b_i16_cl, VLEN, NUM_CORES, &result);

    hal_perf_stop(&perf);

    printf("The true result is %d, the calculated result is %d.\n", res_i16, result);
    printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
    printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

    return;
}

void cluster_entry_q16(void *args) {

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
    hal_perf_conf(&perf, (1 << HAL_PERF_CYCLES) |
                             (1 << HAL_PERF_INSTR)); // Note: on gvsoc you can activate as many
                                                     // counters as you want, while when you run on
                                                     // board, there is only one HW counter.

    // Reset HW counters now and start and stop counters so that we benchmark
    // only around the printf
    hal_perf_reset(&perf);
    hal_perf_start(&perf);

    plp_dot_prod_q16s_xpulpv2(a_i16_cl, b_i16_cl, VLEN, DECIPOINT, &result);

    hal_perf_stop(&perf);

    printf("The true result is %.04f, the calculated result is %d.\n", res_q16, result);
    printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
    printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

    printf("\nCompute with %d parallel cores\n\n", NUM_CORES);

    // Reset HW counters now and start and stop counters so that we benchmark
    // only around the printf
    hal_perf_reset(&perf);
    hal_perf_start(&perf);

    plp_dot_prod_q16_parallel(a_i16_cl, b_i16_cl, VLEN, DECIPOINT, NUM_CORES, &result);

    hal_perf_stop(&perf);

    printf("The true result is %.04f, the calculated result is %d.\n", res_q16, result);
    printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
    printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

    return;
}

void cluster_entry_f16(void *args) {

    printf("Cluster entered. Compute for f16.\n\n");

    float32_t result = 0;

    printf("\nCompute with %d parallel cores\n\n", NUM_CORES);

    // We also count the number of cycles taken to compute it.
    // This structure will hold the configuration and also the results in the
    // cumulative mode
    hal_perf_t perf;

    // It must be initiliazed at least once, this will set all values in the
    // structure to zero.
    hal_perf_init(&perf);

    // Activate specified events
    hal_perf_conf(&perf, (1 << HAL_PERF_CYCLES) |
                             (1 << HAL_PERF_INSTR)); // Note: on gvsoc you can activate as many
                                                     // counters as you want, while when you run on
                                                     // board, there is only one HW counter.

    // Reset HW counters now and start and stop counters so that we benchmark
    // only around the printf
    hal_perf_reset(&perf);
    hal_perf_start(&perf);

    plp_dot_prod_f16_parallel(a_f16_cl, b_f16_cl, VLEN, NUM_CORES, &result);

    hal_perf_stop(&perf);

    printf("The true result is %.04f, the calculated result is %.04f.\n", res_f16, result);
    printf("Total cycles: %d\n", hal_perf_read(HAL_PERF_CYCLES));
    printf("Instructions: %d\n", hal_perf_read(HAL_PERF_INSTR));

    return;
}