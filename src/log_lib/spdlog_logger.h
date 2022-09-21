#ifndef SPDLOG_LOGGER_H
#define SPDLOG_LOGGER_H

#include <pthread.h>
#include <stdlib.h>

#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <utility>

#include "spdlog/logger.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "common/log_common.h"
#include "log_lib/log_config.h"

#define SPDLOG_LEVEL spdlog::level::level_enum
/**
 * @brief Logger log level
 * 
 */
#define DEBUG   SPDLOG_LEVEL::debug
#define INFO    SPDLOG_LEVEL::info
#define WARNING SPDLOG_LEVEL::warn
#define ERROR   SPDLOG_LEVEL::err

extern std::once_flag log_init_flag;

/**
 * @brief API for using spdlogger
 *  Logger must be initialized before LOG_INIT must be called, 
 *  and the construction parameters of the LogConfig struct need to be passed
 *  e.g LOG_INIT("./", info)
 *  @param level
 *  @param content 
 *  e.g LOG(DEBUG, "hello world")
 *  NOTE:
 *  LOG_INIT is thread-safe and only loads once
 */
#define LOG_INIT(...)      \
do {    \
    std::call_once(log_init_flag, [&]{  \
        logger_lib::LogConfig config{__VA_ARGS__};  \
        logger_lib::logger = std::move(logger_lib::SpdFactory::create_logger(config));  \
    }); \
} while (0)

#define LOG(log_level,...)  \
do {    \
    if (logger_lib::logger) {   \
        if ((int)log_level >= (int)logger_lib::details::flush_level)  \
            _SPD_SYNC_LOG(log_level,__VA_ARGS__);   \
        else    \
            _SPD_ASYNC_LOG(log_level, __VA_ARGS__); \
    }   \
} while (0)

#define _SPD_SYNC_LOG(log_level,...)    \
do {    \
    _SPD_ASYNC_LOG(log_level,__VA_ARGS__);  \
    SYNC;  \
} while(0)

#define _SPD_ASYNC_LOG(log_level,...) do {  \
    if (logger_lib::logger)   \
        logger_lib::logger->getLog()->log(log_level, __VA_ARGS__);   \
} while (0)

#define SYNC _SPD_SYNC
#define _SPD_SYNC logger_lib::logger->getLog()->flush()

#define SET_LOG_LEVEL(log_level)  _SPD_ASYNC_SET_LEVEL(log_level)
#define _SPD_ASYNC_SET_LEVEL(log_level)  do {   \
    if (logger_lib::logger)   \
        logger_lib::logger->getLog()->set_level(log_level); \
} while(0)

#define SET_LOG_SYNC_LEVEL(log_level)   do {    \
    logger_lib::details::flush_level = log_level;   \
} while (0)

namespace logger_lib {

namespace utils {
    inline spdlog::level::level_enum get_log_level(std::string log_level)
    {
        if (strncmp("debug", log_level.c_str(), 5) == 0)
            return DEBUG;
        else if (strncmp("info", log_level.c_str(), 4) == 0)
            return INFO;
        else if (strncmp("warn", log_level.c_str(), 4) == 0)
            return INFO;
        else if (strncmp("err", log_level.c_str(), 3) == 0)
            return INFO;
        else {
            fprintf(stderr, "Failed to parse log level %s, set level to INFO", log_level.c_str());
            return INFO;
        }
    }
}

namespace details {

extern SPDLOG_LEVEL flush_level;

class SpdLogger
{
    using LoggerGuard = std::shared_ptr<spdlog::logger>;
public:
#define TASK_COMM_LEN  16
    SpdLogger(const LogConfig& config):config_(config)
    {
        std::string cpuset_bind = config_.cpuset_bind;
        if (cpuset_bind != config_.null_config) {
            setenv("LogCPUSet", cpuset_bind.c_str(), 1);
        }

        char process_name[TASK_COMM_LEN];
        int ret = pthread_getname_np(pthread_self(), process_name, TASK_COMM_LEN);
        if (ret < 0)
            fprintf(stderr, "Failed to get process name\n");
        std::string wall_time = logger_lib::utils::getWallClock();
        std::string log_file = config_.log_dir + "/" + std::string(process_name) + "-" + wall_time;
        
        logger_ = spdlog::basic_logger_mt<spdlog::async_factory>(process_name, log_file);
        if(logger_ == nullptr)
        {
            fprintf(stderr, "Failed to create logger\n");
        }
        auto level = utils::get_log_level(config_.log_level);
        logger_->set_level(level);
    }
    LoggerGuard getLog()
    {
        return logger_;
    }
private:
    LoggerGuard logger_;
    LogConfig config_;
};

} // detail

using SpdLoggerGuard = std::unique_ptr<details::SpdLogger>;

extern SpdLoggerGuard logger;

class SpdFactory
{
public:
    static SpdLoggerGuard create_logger(const LogConfig& config)
    {
        return SpdLoggerGuard(new details::SpdLogger(config));
    }
private:
    SpdFactory()=delete;
    ~SpdFactory()=delete;
};

} // logger_lib

#endif // SPDLOG_LOGGER_H