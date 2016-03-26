#include "timer.h"

using namespace std::chrono;

Timer::Timer()
{
    t_start = clock.now();
    stopwatch = false;
}

long long Timer::GetElapsedTime()
{
    return chrono::duration_cast<milliseconds>(high_resolution_clock::now() - t_start).count();
}

void Timer::Sleep(long ms)
{
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void Timer::Reset()
{
    t_start = clock.now();
    stopwatch = false;
}

bool Timer::Stopwatch(long ms)
{
    if(stopwatch)
    {
        if(chrono::duration_cast<milliseconds>(high_resolution_clock::now() - s_start).count() >= ms)
            return true;
    } else {
        s_start = clock.now();
        stopwatch = true;
    }
    return false;
}
