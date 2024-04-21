#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "const.h"
#include "game_field.h"
#include <time.h>

enum InterfaceMode {
    IDLE_MODE,
    WAIT_MODE,
    PREPARATION_MODE,
    OWN_TURN_MODE,
    ENEMY_TURN_MODE
};

/**
 * @brief The GameInterface class
 */
class GameInterface {
public:
    /**
     * @brief GameInterface
     */
    GameInterface();

    /**
     * @brief ~GameInterface
     */
    virtual ~GameInterface();

    /**
     * @brief is_valid
     * @return
     */
    virtual bool is_valid() const = 0;

    /**
     * @brief process_input
     * @return
     */
    virtual InputResult process_input() = 0;

    /**
     * @brief show
     */
    virtual void show() = 0;

    /**
     * @brief set_game_state
     * @param p_state
     */
    virtual void set_game_state(GameState p_state);

    /**
     * @brief set_server_conn_state
     * @param connected
     */
    virtual void set_server_conn_state(bool connected);

    /**
     * @brief set_own_field
     * @param p_own_field
     */
    void set_own_field(const GameField &p_own_field);

    /**
     * @brief get_own_field
     * @return
     */
    const GameField& get_own_field() const;

    /**
     * @brief set_enemy_field
     * @param p_enemy_field
     */
    void set_enemy_field(const GameField &p_enemy_field);

    /**
     * @brief get_enemy_field
     * @return
     */
    GameField get_enemy_field() const;

    /**
     * @brief get_mode
     * @return
     */
    InterfaceMode get_mode() const;

    /**
     * @brief set_mode
     * @param p_mode
     */
    void set_mode(InterfaceMode p_mode);

    /**
     * @brief activate_preparation_mode
     * @param end_time
     */
    virtual void activate_preparation_mode(time_t end_time) = 0;

    /**
     * @brief stop_preparation_mode
     */
    virtual void stop_preparation_mode() = 0;

    /**
     * @brief activate_own_turn_mode
     * @param end_time
     */
    virtual void activate_own_turn_mode(time_t end_time) = 0;

    /**
     * @brief stop_own_turn_mode
     */
    virtual void stop_own_turn_mode() = 0;

    /**
     * @brief activate_enemy_turn_mode
     * @param end_time
     */
    virtual void activate_enemy_turn_mode(time_t end_time) = 0;

    /**
     * @brief stop_enemy_turn_mode
     */
    virtual void stop_enemy_turn_mode() = 0;

    /**
     * @brief get_last_shot
     * @return
     */
    const Point& get_last_shot() const;

    /**
     * @brief set_last_shot
     * @param shot
     */
    void set_last_shot(const Point &shot);
protected:
    // Server.
    bool has_sever_conn;

    // Fields.
    GameField own_field;
    GameField enemy_field;

    GameState state;

    InterfaceMode mode;
private:
    Point last_shot;
};

#endif
