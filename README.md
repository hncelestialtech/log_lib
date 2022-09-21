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
    LOG_INIT("./logs", "info");
    LOG(DEBUG, "hello world");
    LOG(DEBUG, "hello world");
    SET_LOG_LEVEL(DEBUG);
    LOG(DEBUG, "Hello world {}", 3);
    return 0;
}
```