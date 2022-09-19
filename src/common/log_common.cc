#include "common/log_common.h"

#include <pthread.h>

#include "common/cpuset.h"

namespace logger_lib {
namespace utils {

static int do_taskset(size_t setsize, cpu_set_t *set, pthread_t* pid_list, size_t pid_num)
{
    for(size_t i = 0; i < pid_num; ++i)
    {
        if(pthread_setaffinity_np(pid_list[i], setsize, set) < 0)
        {
            return -1;
        }
    }
    return 0;
}

int set_cpu_affinity(pthread_t* pid_list, size_t pidnum)
{
    int ncpus;
    const char* cpulist_env = getenv("LogCPUSet");
    if(cpulist_env == NULL)
    {
        return -1;
    }
    ncpus = get_max_number_of_cpus();
	if (ncpus <= 0)
	{
        fprintf(stderr, "Failed to get cpu nums\n");
        return -1;
    }
    size_t setsize;
    size_t nbits;
    cpu_set_t *set = cpuset_alloc(ncpus, &setsize, &nbits);
    if(set == NULL)
    {
        fprintf(stderr, "Failed to alloc cpuset\n");
        return -1;
    }
    if(cpulist_parse(cpulist_env, set, setsize, 0))
    {
        fprintf(stderr, "Failed to parse cpu list\n");
        return -1;
    }
    return do_taskset(setsize, set, pid_list, pidnum);
}

} // utils
} // logger