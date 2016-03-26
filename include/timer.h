#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std;

class Timer
{
private:
    bool stopwatch;
    chrono::high_resolution_clock clock;
    chrono::high_resolution_clock::time_point t_start;
    chrono::high_resolution_clock::time_point s_start;

public:
    Timer();
    ~Timer() {}

    long long GetElapsedTime();				// returns reset time
    void Sleep(long ms);					// suspend thread execution
    void Reset();							// reset clock
    bool Stopwatch(long ms);				// returns true once time has elapsed
};

#endif // TIMER_H
