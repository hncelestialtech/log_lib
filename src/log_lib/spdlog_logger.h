#ifndef SPDLOG_LOGGER_H
#define SPDLOG_LOGGER_H

#include <pthread.h>

#include <string>
#include <memory>

#include "spdlog/logger.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "common/log_common.h"

#define DEBUG   spdlog::level::level_enum::debug
#define INFO    spdlog::level::level_enum::info
#define WARNING spdlog::level::level_enum::warn
#define ERROR   spdlog::level::level_enum::err

#define LOG(log_level,...)  \
do {    \
    if ((int)log_level >= (int)INFO)  \
        _SPD_SYNC_LOG(log_level,__VA_ARGS__);   \
    else    \
        _SPD_ASYNC_LOG(log_level, __VA_ARGS__); \
} while (0)

#define _SPD_SYNC_LOG(log_level,...)    \
do {    \
    _SPD_ASYNC_LOG(log_level,__VA_ARGS__);  \
    SYNC;  \
} while(0)

#define _SPD_ASYNC_LOG(log_level,...) logger_lib::details::spd_factory::instance().getLog()->log(log_level, __VA_ARGS__)

#define SYNC _SPD_SYNC
#define _SPD_SYNC logger_lib::details::spd_factory::instance().getLog()->flush()

#define SET_LOG_LEVEL(log_level)  _SPD_ASYNC_SET_LEVEL(log_level)
#define _SPD_ASYNC_SET_LEVEL(log_level)  logger_lib::details::spd_factory::instance().getLog()->set_level(log_level) 

using LoggerGuard = std::shared_ptr<spdlog::logger>;

namespace logger_lib {
namespace details {

class spdlogger
{
public:
#define TASK_COMM_LEN  16
    spdlogger()
    {
        char process_name[TASK_COMM_LEN];
        pthread_getname_np(pthread_self(), process_name, TASK_COMM_LEN);
        std::string wall_time = logger_lib::utils::getWallClock();
        std::string log_file = std::string("logs/") + std::string(process_name) + "-" + wall_time;
        logger_ = spdlog::basic_logger_mt<spdlog::async_factory>(process_name, log_file);
        if(logger_ == nullptr)
        {
            fprintf(stderr, "failed to create logger\n");
        }
    }
    LoggerGuard getLog()
    {
        return logger_;
    }
private:
    LoggerGuard logger_;
};

class spd_factory
{
public:
    static spdlogger& instance()
    {
        return logger_;
    }
private:
    spd_factory()=delete;
    ~spd_factory()=delete;

    static spdlogger logger_;
};

} // detail
} // logger_lib

#endif // SPDLOG_LOGGER_H