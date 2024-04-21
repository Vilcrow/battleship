#ifndef TIMER_H
#define TIMER_H

#include <time.h>

/**
 * @brief The timer class. The time should be set in seconds
 */
class Timer {
public:
    /**
     * @brief Constructor
     * @param p_duration the timer duration
     */
    Timer(time_t p_duration);

    /**
     * @brief Destructor
     */
    ~Timer();

    /**
     * @brief Starts the timer
     */
    void start();

    /**
     * @brief Stops the timer
     */
    void stop();

    /**
     * @brief Sets the duration of the timer
     * @param p_duration the timer duration.
     * When pasing a negative value, a zero value is set
     */
    void set_duration(time_t p_duration);

    /**
     * @brief Returns the remaining time
     */
    time_t remain() const;

    /**
     * @brief Returns true if the timer is started
     * and the duration time hasn't passed
     */
    bool is_active() const;
private:
    time_t start_time;
    time_t duration;

    enum {
        INACTIVE = -1
    };
};

#endif
