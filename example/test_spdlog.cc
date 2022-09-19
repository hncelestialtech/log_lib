#include "log_lib/spdlog_logger.h"

int main()
{
    LOG(DEBUG, "hello world {}", 1);
    while(true)
        ;
    return 0;
}