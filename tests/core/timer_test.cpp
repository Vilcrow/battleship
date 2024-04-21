#include "CppUTest/TestHarness.h"
#include <unistd.h>
#include "timer.h"
#include "tools/write_log.h"

TEST_GROUP(TimerGroup)
{
    Timer *timer = 0;
    time_t duration = 3;
    void setup()
    {
        timer = new Timer(duration);
    }
    void teardown()
    {
        delete timer;
    }
};

TEST(TimerGroup, InactiveTimer)
{
    CHECK_FALSE(timer->is_active());
    CHECK_EQUAL(0, timer->remain());
    timer->start();
    CHECK_EQUAL(duration, timer->remain());
}

TEST(TimerGroup, TimerWork)
{
    time_t remain_time = duration;
    timer->start();
    while(remain_time) {
        sleep(1);
        --remain_time;
        CHECK_EQUAL(remain_time, timer->remain());
    }
}
