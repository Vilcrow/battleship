#ifndef PREPARATION_TIMEOUT_H
#define PREPARATION_TIMEOUT_H

#include "timeout_handler.h"

class GameSession;

/**
 * @brief The PreparationTimeout class
 */
class PreparationTimeout : public TimeoutHandler {
public:
    /**
     * @brief PreparationTimeout
     * @param p_sec
     * @param p_session
     */
    PreparationTimeout(long p_sec, GameSession *p_session);

    /**
     * @brief ~PreparationTimeout
     */
    virtual ~PreparationTimeout();
private:
    GameSession *session;

    void timeoutEvent();
};

/**
 * @brief The CheckStartFieldsTimeout class
 */
class CheckStartFieldsTimeout : public TimeoutHandler {
public:
    /**
     * @brief CheckStartFieldsTimeout
     * @param p_sec
     * @param p_session
     */
    CheckStartFieldsTimeout(long p_sec, GameSession *p_session);

    /**
     * @brief ~CheckStartFieldsTimeout
     */
    virtual ~CheckStartFieldsTimeout();
private:
    GameSession *session;

    void timeoutEvent();
};

/**
 * @brief The GameStartTimeout class
 */
class GameStartTimeout : public TimeoutHandler {
public:
    /**
     * @brief GameStartTimeout
     * @param p_sec
     * @param p_session
     */
    GameStartTimeout(long p_sec, GameSession *p_session);

    /**
     * @brief ~GameStartTimeout
     */
    virtual ~GameStartTimeout();
private:
    GameSession *session;

    void timeoutEvent();
};

/**
 * @brief The GameTurnTimeout class
 */
class GameTurnTimeout : public TimeoutHandler {
public:
    /**
     * @brief GameTurnTimeout
     * @param p_sec
     * @param p_session
     */
    GameTurnTimeout(long p_sec, GameSession *p_session);

    /**
     * @brief ~GameTurnTimeout
     */
    virtual ~GameTurnTimeout();
private:
    GameSession *session;

    void timeoutEvent();
};

/**
 * @brief The ChangeTurnTimeout class
 */
class ChangeTurnTimeout : public TimeoutHandler {
public:
    /**
     * @brief ChangeTurnTimeout
     * @param p_sec
     * @param p_session
     */
    ChangeTurnTimeout(long p_sec, GameSession *p_session);

    /**
     * @brief ~ChangeTurnTimeout
     */
    virtual ~ChangeTurnTimeout();
private:
    GameSession *session;

    void timeoutEvent();
};

/**
 * @brief The EndGameTimeout class
 */
class EndGameTimeout : public TimeoutHandler {
public:
    /**
     * @brief EndGameTimeout
     * @param p_sec
     * @param p_session
     */
    EndGameTimeout(long p_sec, GameSession *p_session);

    /**
     * @brief ~EndGameTimeout
     */
    virtual ~EndGameTimeout();
private:
    GameSession *session;

    void timeoutEvent();
};

#endif
