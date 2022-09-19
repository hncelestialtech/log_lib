#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdio.h>

#include <fstream>

#include <json/json.h>

namespace logger_lib {
namespace details {

class ConfigParser {
public:
    ConfigParser(std::string config_path): 
        null_config(""), log_dir("./"), log_level("info"), cpuset_bind(null_config), config_path_(config_path) {
        std::ifstream config_in(config_path);
        Json::Reader config_reader;
        Json::Value config_value;

        if(!config_in.is_open()){
            fprintf(stderr, "Failed to open config file %s\n", config_path.c_str());
            return;
        }

        if(config_reader.parse(config_in,config_value)){
            get_config("log_dir", config_value, log_dir);
            get_config("log_level", config_value, log_level);
            get_config("LogCPUSet", config_value, cpuset_bind);
        }
    }
private:
    void get_config(std::string config_name, Json::Value& config_value, std::string& config) {
        if (config_value[config_name]) {
            config = config_value[config_name].asString();
        } else {
            fprintf(stderr, "Failed to get config %s, set to default value %s\n", config_name.c_str(), config.c_str());
        }
    }
    
public:
    std::string null_config;

    std::string log_dir;
    std::string log_level;
    std::string cpuset_bind;
private:
    std::string config_path_;
};

} // utils
} // logger_lib

#endif // CONFIG_PARSER_H