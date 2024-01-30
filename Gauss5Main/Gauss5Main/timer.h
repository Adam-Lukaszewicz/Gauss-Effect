#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <iostream>
#include <intrin.h>

namespace Timer
{
    unsigned long long countStart, countEnd, countCycle;
    std::chrono::time_point<std::chrono::steady_clock> start_counting, stop_counting;
    std::chrono::duration<double> time;
    double count;

    void start()
    {
        start_counting = std::chrono::high_resolution_clock::now();
        countStart = __rdtsc();
    }

    void check() {
        stop_counting = std::chrono::high_resolution_clock::now();
        time = stop_counting - start_counting;
        count = time.count() * 1000;
        std::cout << "Aktualny czas od startu watku: " << count << "ms\n";
    }

    double stop()
    {
        stop_counting = std::chrono::high_resolution_clock::now();
        countEnd = __rdtsc();
        time = stop_counting - start_counting;
        countCycle = countEnd - countStart;
        count = time.count() * 1000;
        std::cout << "Zmierzony czas: " << count << "ms\n";
        std::cout << "Zmierzone cykle: " << countCycle << "\n";
        return count;
    }
};
#endif // !TIMER_H