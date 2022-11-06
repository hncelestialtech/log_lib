#include "log_lib/spdlog_logger.h"
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <time.h>

void fmt_bench()
{
    int duration = 0;

    struct timespec begin{}, end{};
    // std::string s = "hello world";
    double s = 3.7363894947266278384;
    SET_LOG_SYNC_LEVEL(ERROR);
    for(int i = 0; i < 100; ++i) {
        clock_gettime(CLOCK_REALTIME, &begin);
        for(int i = 0; i < 1000; ++i) {
            LOG(DEBUG, "HELLO WORLD {} {} {} {} {} {} {} {} {} {} {} {} {} {}", s, s, s, s, s, s, s, s, s, s, s, s, s, s);
        }
        clock_gettime(CLOCK_REALTIME, &end);
        duration += ((end.tv_sec * 1000000000 + end.tv_nsec) - (begin.tv_sec * 1000000000 + begin.tv_nsec)) / 1000;
    }
    std::cout<<"fmt bench "<<duration/100<<std::endl;
}

void nofmt_bench()
{
    int duration = 0;

    struct timespec begin{}, end{};
    std::string s = "hello world";
    SET_LOG_SYNC_LEVEL(ERROR);
    for(int i = 0; i < 100; ++i) {
        clock_gettime(CLOCK_REALTIME, &begin);
        for(int i = 0; i < 1000; ++i) {
            LOG(DEBUG, "HELLO WORLD");
        }
        clock_gettime(CLOCK_REALTIME, &end);
        duration += ((end.tv_sec * 1000000000 + end.tv_nsec) - (begin.tv_sec * 1000000000 + begin.tv_nsec)) / 1000;
    }
    std::cout<<"no fmt bench "<<duration/100<<std::endl;
}

void snprintf_bench()
{
    int duration = 0;

    struct timespec begin{}, end{};
    std::string s = "hello world";
    SET_LOG_SYNC_LEVEL(ERROR);
    for(int i = 0; i < 100; ++i) {
        clock_gettime(CLOCK_REALTIME, &begin);
        for(int i = 0; i < 1000; ++i) {
            // LOG(DEBUG, "HELLO WORLD {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s}", s, s, s, s, s, s, s, s, s, s, s, s, s, s);
            char log_content[1024];
            snprintf(log_content, 1024, "HELLO WORLD %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str()
            , s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str());
            LOG(DEBUG, log_content);
        }
        clock_gettime(CLOCK_REALTIME, &end);
        duration += ((end.tv_sec * 1000000000 + end.tv_nsec) - (begin.tv_sec * 1000000000 + begin.tv_nsec)) / 1000;
        // std::cout<<"duration "<<duration<<std::endl;
    }
    std::cout<<"snprintf bench "<<duration/100<<std::endl;
}

int main()
{
    // // init log
    LOG_INIT("debug", "log_example", "./logs");
    
    // LOG(DEBUG, "hello world");
    
    // // reset log level
    // SET_LOG_LEVEL(INFO);
    // LOG(DEBUG, "This log will not be printed");
    // LOG(INFO, "This log will be printed");
    
    // // reset sync level
    // SET_LOG_SYNC_LEVEL(DEBUG);
    // int8_t c = 'a';
    // LOG(INFO, "Hello world {:c}", c);
    // struct timespec begin{}, end{};
    // std::string s = "hello world";
    // SET_LOG_SYNC_LEVEL(ERROR);
    // for(int i = 0; i < 100; ++i) {
    //     clock_gettime(CLOCK_REALTIME, &begin);
    //     for(int i = 0; i < 1000; ++i) {
    //         LOG(DEBUG, "HELLO WORLD {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s} {:s}", s, s, s, s, s, s, s, s, s, s, s, s, s, s);
    //         // char log_content[1024];
    //         // snprintf(log_content, 1024, "HELLO WORLD %s %s %s %s %s %s %s %s %s %s %s %s %s %s", s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str()
    //         // , s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str(), s.c_str());
    //         // LOG(DEBUG, log_content);
    //     }
    //     clock_gettime(CLOCK_REALTIME, &end);
    //     auto duration = (end.tv_sec * 1000000000 + end.tv_nsec) - (begin.tv_sec * 1000000000 + begin.tv_nsec);
    //     std::cout<<"duration "<<duration / 1000<<std::endl;
    // }
    fmt_bench();
    snprintf_bench();
    nofmt_bench();
    return 0;
}