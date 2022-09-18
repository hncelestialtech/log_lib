#ifndef SPDLOG_LOGGER_H
#define SPDLOG_LOGGER_H

#include <pthread.h>

#include <string>
#include <memory>
#include <stdlib.h>

#include "spdlog/logger.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "common/log_common.h"
#include "log_lib/config_parser.h"

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

namespace utils {
    spdlog::level::level_enum get_log_level(std::string log_level)
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

class spdlogger
{
public:
#define TASK_COMM_LEN  16
    spdlogger():config_("./config.json")
    {
        std::string cpuset_bind = config_.cpuset_bind;
        setenv("LogCPUSet", cpuset_bind.c_str(), 1);

        std::string wall_time = logger_lib::utils::getWallClock();
        std::string log_file = config_.log_dir + "/" + std::string(process_name) + "-" + wall_time;
        logger_ = spdlog::basic_logger_mt<spdlog::async_factory>(process_name, log_file);
        if(logger_ == nullptr)
        {
            fprintf(stderr, "failed to create logger\n");
        }
        auto level = utils::get_log_level(ocnfig_.log_level);
        logger_->set_level(level);
    }
    LoggerGuard getLog()
    {
        return logger_;
    }
private:
    LoggerGuard logger_;
    ConfigParser config_;
};

} // detail

class spd_factory
{
public:
    static details::spdlogger& instance()
    {
        return logger_;
    }
private:
    spd_factory()=delete;
    ~spd_factory()=delete;

    static details::spdlogger logger_;
};

} // logger_lib

#endif // SPDLOG_LOGGER_H