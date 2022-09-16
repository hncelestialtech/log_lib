#include "log_lib/spdlog_logger.h"

int main()
{
    SET_LOG_LEVEL(DEBUG);
    LOG(DEBUG, "hello world");
    return 0;
}