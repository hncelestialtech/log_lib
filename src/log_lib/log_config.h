#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdio.h>

#include <string>
#include <fstream>


namespace logger_lib {

class LogConfig {
public:
    LogConfig(std::string level="info", std::string name=LogConfig::null_config, std::string dir="./", std::string cpuset="0"):
        log_name(name), log_dir(dir), log_level(level), cpuset_bind(cpuset)
    {}
    static constexpr const char* null_config = "";
    std::string log_name;
    std::string log_dir;
    std::string log_level;
    std::string cpuset_bind;
};

} // logger_lib

#endif // CONFIG_PARSER_H