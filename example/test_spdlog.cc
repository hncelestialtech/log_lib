#include "log_lib/spdlog_logger.h"

int main()
{
    LOG_INIT("./logs", "info");
    LOG(DEBUG, "hello world {}", 1);
    LOG_INIT("./logs", "debug");
    LOG(DEBUG, "hello world {}", 2);
    // while(true)
    //     ;
    return 0;
}