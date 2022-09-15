#ifndef STDOUT_LOGGER_H
#define STDOUT_LOGGER_H

#include <stdio.h>

#define LOG(log_level,...) do { \
    fprintf(stdout, __VA_ARGS__)    \
} while(0)

#endif // STDOUT_LOGGER_H