#include "log_lib/spdlog_logger.h"
#include <unistd.h>

int main()
{
    LOG_INIT("./logs", "debug");
    // LOG(DEBUG, "hello world {}", true);
    // LOG_INIT("./logs", "debug");
    // LOG(DEBUG, "hello world {}", 2);
    // SET_LOG_LEVEL(DEBUG);
    // LOG(DEBUG, "Hello world {}", 3);
    // // SET_LOG_SYNC_LEVEL(DEBUG);
    // LOG(DEBUG, "Hello world {}", true);
    while (true) {
        LOG(DEBUG, "hello world");
        usleep(100);
    }
        
    return 0;
}