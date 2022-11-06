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
    logger_lib::details::SpdLogger::instance({__VA_ARGS__});    \
} while (0)

#define LOG(log_level,...)  \
do {  \
    if(logger_lib::get_default_logger())    \
        logger_lib::get_default_logger()->log(log_level, __VA_ARGS__);   \
} while (0)

#define SET_LOG_LEVEL(log_level)  _SPD_ASYNC_SET_LEVEL(log_level)
#define _SPD_ASYNC_SET_LEVEL(log_level)  do {   \
    if (logger_lib::get_default_logger())   \
        logger_lib::get_default_logger()->set_level(log_level); \
} while(0)

#define SET_LOG_SYNC_LEVEL(log_level)   do {    \
    if (logger_lib::get_default_logger())     \
        logger_lib::details::SpdLogger::instance().set_flush_level(log_level);  \
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
            return WARNING;
        else if (strncmp("err", log_level.c_str(), 3) == 0)
            return ERROR;
        else {
            fprintf(stderr, "Failed to parse log level %s, set level to INFO", log_level.c_str());
            return INFO;
        }
    }
}

namespace details {

class SpdLogger
{
public:
    using LoggerGuard = std::shared_ptr<spdlog::logger>;
public:
    LoggerGuard getLog()
    {
        return logger_;
    }

    void set_flush_level(SPDLOG_LEVEL flush_level) {
        flush_level_ = flush_level;
        logger_->flush_on(flush_level_);
    }

    static SpdLogger& instance(const LogConfig& config={}) {
        static SpdLogger logger{config};
        return logger;
    }
private:
#define TASK_COMM_LEN  16
    SpdLogger(const LogConfig& config):logger_(nullptr), config_(config)
    {
        std::string cpuset_bind = config_.cpuset_bind;
        if (cpuset_bind != LogConfig::null_config)
            setenv("LogCPUSet", cpuset_bind.c_str(), 1);

        std::string log_file;
        std::string log_name;
        if (config.log_name != LogConfig::null_config) 
            log_name = config_.log_name;
        else 
            log_name = get_default_log_name();

        std::string wall_time = logger_lib::utils::get_wall_clock();
        log_file = config_.log_dir + "/" + log_name + "-" + wall_time + ".log";

        logger_ = spdlog::basic_logger_mt<spdlog::async_factory>(log_name, log_file);
        if(logger_ == nullptr)
        {
            fprintf(stderr, "Failed to create logger\n");
            return;
        }
        auto log_level = utils::get_log_level(config_.log_level);
        logger_->set_level(log_level);
        set_flush_level(INFO);
        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%F][%n][%l] %v");
    }
    SpdLogger(const SpdLogger&)=delete;
    SpdLogger& operator=(const SpdLogger&)=delete;

    std::string get_default_log_name()
    {
        char process_name[TASK_COMM_LEN];
        int ret = pthread_getname_np(pthread_self(), process_name, TASK_COMM_LEN);
        if (ret < 0)
            fprintf(stderr, "Failed to get process name\n");
        std::string log_name = std::string(process_name);
        return log_name;
    }
private:
    LoggerGuard logger_;
    LogConfig config_;

    SPDLOG_LEVEL flush_level_;
};

} // detail

inline details::SpdLogger::LoggerGuard get_default_logger()
{
    return details::SpdLogger::instance().getLog();
}

} // logger_lib

#endif // SPDLOG_LOGGER_H