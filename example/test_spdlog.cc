#include "log_lib/spdlog_logger.h"
#include <unistd.h>

int main()
{
    // init log
    LOG_INIT("debug", "log_example", "./logs", "2");
    
    LOG(DEBUG, "hello world");
    
    // reset log level
    SET_LOG_LEVEL(INFO);
    LOG(DEBUG, "This log will not be printed");
    LOG(INFO, "This log will be printed");
    
    // reset sync level
    SET_LOG_SYNC_LEVEL(DEBUG);
    LOG(INFO, "Hello world {}", true);        
    return 0;
}