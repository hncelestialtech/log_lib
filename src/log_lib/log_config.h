#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdio.h>

#include <fstream>


namespace logger_lib {

class LogConfig {
public:
    LogConfig(std::string dir="./", std::string level="info", std::string cpuset=""):
        null_config(""), log_dir(dir), log_level(level), cpuset_bind(cpuset)
    {}
    std::string null_config;
    std::string log_dir;
    std::string log_level;
    std::string cpuset_bind;
};

} // logger_lib

#endif // CONFIG_PARSER_H