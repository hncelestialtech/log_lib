#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <stdio.h>

#include <fstream>

#include <json/json.h>

namespace logger_lib {
namespace details {

class ConfigParser {
    using json = nlohmann::json;
public:
    ConfigParser(std::string config_path) {
        ifstream config_in(config_path,ios::binary);
        Json::Reader config_reader;
        Json::Value config_value;

        if(!config_in.is_open()){
            fprintf(stderr, "Failed to open config file %s", config_path.c_str());
            return;
        }

        if(config_reader.parse(config_in,config_value)){
            log_dir = root["log_dir"].asString();
            log_level = root["log_level"].asString();
            cpuset_bind = root["cpuset_bind"].asString();
        }
    }
    
public:
    std::string log_dir;
    std::string log_level;
    std::string cpuset_bind;
private:
    std::string config_path.
};

} // utils
} // logger_lib

#endif // CONFIG_PARSER_H