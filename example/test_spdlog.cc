#include "log_lib/spdlog_logger.h"

int main()
{
    LOG_INIT("./logs", "info");
    LOG(DEBUG, "hello world {}", true);
    LOG_INIT("./logs", "debug");
    LOG(DEBUG, "hello world {}", 2);
    SET_LOG_LEVEL(DEBUG);
    LOG(DEBUG, "Hello world {}", 3);
    // SET_LOG_SYNC_LEVEL(DEBUG);
    LOG(DEBUG, "Hello world {}", true);
    while(true)
        ;
    return 0;
}