# log_lib
## How to build
Go into the code repository and run
```bash
mkdir build && cd build
cmake .. && make
```
By default, it is compiled to the release version, and if debug information is needed, add -DCMAKE_BUILD_TYPE=Debug when cmake runs
## How to use
Log_lib provides users with three interfaces, LOG_INIT, LOG, SET_LOG_LEVEL, which need to be initialized by calling the LOG_INIT before use, logging with LOG, and modifying the log level with SET_LOG_LEVEL
Example below:
```c++
#include "log_lib/spdlog_logger.h"

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
```