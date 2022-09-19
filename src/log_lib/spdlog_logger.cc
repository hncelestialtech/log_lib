#include "log_lib/spdlog_logger.h"

#include <thread>
#include <mutex>

std::once_flag log_init_flag;


namespace logger_lib {

SpdLoggerGuard logger;

SPDLOG_LEVEL details::flush_level = SPDLOG_LEVEL::info;
} // logger_lib