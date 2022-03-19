#include "cluster.h"
#include "pmsis.h"

static int test_entry(void) {

    struct pi_device cluster_dev;
    struct pi_cluster_conf cl_conf;
    struct pi_cluster_task cl_task;

    pi_cluster_conf_init(&cl_conf);
    pi_open_from_conf(&cluster_dev, &cl_conf);
    if (pi_cluster_open(&cluster_dev)) {
        return -1;
    }

#if defined(INT16_TEST)
    pi_cluster_send_task_to_cl(&cluster_dev, pi_cluster_task(&cl_task, cluster_entry_i16, NULL));
#elif defined(Q16_TEST)
    pi_cluster_send_task_to_cl(&cluster_dev, pi_cluster_task(&cl_task, cluster_entry_q16, NULL));
#elif defined(FLOAT16_TEST)
    pi_cluster_send_task_to_cl(&cluster_dev, pi_cluster_task(&cl_task, cluster_entry_f16, NULL));
#else
    pi_cluster_send_task_to_cl(&cluster_dev, pi_cluster_task(&cl_task, cluster_entry_i16, NULL));
#endif

    pi_cluster_close(&cluster_dev);

    return 0;
}

static void test_kickoff(void *arg) {
    int ret = test_entry();
    pmsis_exit(ret);
}

int main(void) { return pmsis_kickoff((void *)test_kickoff); }