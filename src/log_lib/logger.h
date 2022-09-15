#ifndef LOGGER_H
#define LOGGER_H

#ifdef SPDLOGGER
#include "log_lib/spdlog_logger.h"
#elif NANOLOG
#    error Not yet supported
// #include "log_lib/nano_logger.h"
#else
#include "log_lib/stdout_logger.h"
#endif 

#endif // LOGGER_H