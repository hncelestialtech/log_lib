#ifndef COMMON_H
#define COMMON_H

#include <time.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <string>
#include <vector>

#define NOCOPY(x)   \
x& x(const x&)=delete;  \
x& operator=(const x&)=delete;  \
x& x(x&&)=delete;   \
x& operator=(x&&)=delete;

static inline std::string getWallClock()
{
    time_t now = time(NULL);
    tm* tm_t = localtime(&now);
    std::string wall_clock = std::to_string(tm_t->tm_year + 1900) + "-" + 
                            std::to_string(tm_t->tm_mon + 1) + "-" +
                            std::to_string(tm_t->tm_mday) + "-" +
                            std::to_string(tm_t->tm_hour) + ":" +
                            std::to_string(tm_t->tm_min) + ":" + 
                            std::to_string(tm_t->tm_sec);
    return wall_clock;
}

/*
 * Gets the cpu list from the environment variable and binds the thread
 * Return: 0 if success, otherwise -1
 */
extern int set_cpu_affinity(pthread_t* pid_list, size_t pidnum);

#endif // COMMON_H