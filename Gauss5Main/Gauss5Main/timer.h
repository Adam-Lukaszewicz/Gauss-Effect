#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <iostream>

namespace Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start_counting, stop_counting;
    std::chrono::duration<double> time;
    double count;

    void start()
    {
        start_counting = std::chrono::high_resolution_clock::now();
    }

    void stop()
    {
        stop_counting = std::chrono::high_resolution_clock::now();
        time = stop_counting - start_counting;
        count = time.count() * 1000;
        std::cout << "Zmierzony czas: " << count << "ms\n";
    }
};
#endif // !TIMER_H