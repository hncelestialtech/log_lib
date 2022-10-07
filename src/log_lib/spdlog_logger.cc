#include "log_lib/log_config.h"
#include "log_lib/spdlog_logger.h"

#include <thread>
#include <mutex>

std::once_flag log_init_flag;

namespace logger_lib {

SpdLoggerGuard logger;
std::string LogConfig::null_config="";

} // logger_lib