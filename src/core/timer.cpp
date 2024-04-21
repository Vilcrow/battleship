#include "timer.h"

Timer::Timer(time_t p_duration)
    : start_time(INACTIVE)
    , duration(p_duration)
{

}

Timer::~Timer()
{

}

void Timer::start()
{
    start_time = time(NULL);
}

void Timer::stop()
{
    start_time = INACTIVE;
}

void Timer::set_duration(time_t p_duration)
{
    duration = p_duration;
    if(duration < 0) {
        duration = 0;
    }
}

time_t Timer::remain() const
{
    if(!is_active()) {
        return 0;
    }

    time_t cur_time = time(NULL);
    time_t passed_time = cur_time - start_time;
    time_t ret = duration - passed_time;
    if(ret < 0) {
        ret = 0;
    }
    return ret;
}

bool Timer::is_active() const
{
    return start_time != INACTIVE && duration > 0;
}
