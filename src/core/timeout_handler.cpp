#include "timeout_handler.h"

TimeoutHandler::TimeoutHandler(long p_sec, long p_usec)
    : sec(p_sec)
    , usec(p_usec)
{

}

TimeoutHandler::~TimeoutHandler()
{

}

bool TimeoutHandler::process(long p_sec, long p_usec)
{
    if(elapsed()) {
        return false;
    }

    usec -= p_usec;
    if(usec < 0) {
        --sec;
        if(sec >= 0) {
            usec += 1000000;
        }
        else {
            usec = 0;
        }
    }
    sec -= p_sec;
    if(sec < 0) {
        sec = 0;
        usec = 0;
    }

    if(elapsed()) {
        timeoutEvent();
        return true;
    }
    return false;
}

bool TimeoutHandler::elapsed() const
{
    if(sec <= 0 && usec <= 0) {
        return true;
    }
    return false;
}

void TimeoutHandler::stop()
{
    sec = 0;
    usec = 0;
}

bool TimeoutHandler::operator<(const TimeoutHandler &other) const
{
    return (sec < other.sec || (sec == other.sec && usec < other.usec));
}
