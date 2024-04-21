#ifndef TIMEOUT_HANDLER_H
#define TIMEOUT_HANDLER_H

/**
 * @brief The abstract class to trigger an event after a certain time has
 * elapsed. The triggered event must be defined in the child class
 */
class TimeoutHandler {
public:
    /**
     * @brief Constructor
     * @param p_sec seconds
     * @param p_usec microseconds
     */
    TimeoutHandler(long p_sec, long p_usec);

    /**
     * @brief Destructor
     */
    virtual ~TimeoutHandler();

    /**
     * @brief Process the elapsed time. If the set time has expired,
     * the specified event is triggered
     * @param p_sec passed seconds
     * @param p_usec passed microseconds
     * @return true if the event was triggered, otherwise returns false
     */
    bool process(long p_sec, long p_usec);

    /**
     * @brief Returns true if the specified time has expired
     */
    bool elapsed() const;

    /**
     * @brief Zeroizes the timer. The event isn't triggered
     */
    void stop();

    /**
     * @brief Returns true if the remaining time is less than the other handler
     */
    bool operator<(const TimeoutHandler &other) const;
private:
    long sec;
    long usec;

    virtual void timeoutEvent() = 0;
};

#endif
