#ifndef NCURSES_INTERFACE_H
#define NCURSES_INTERFACE_H

#include "game_interface.h"
#include "cursor.h"
#include "timer.h"

#define INTERFACE_PATH "files/background"

#define ON_TXT "ON"
#define OFF_TXT "OFF"

/**
 * @brief The NcursesInterface class
 */
class NcursesInterface : public GameInterface {
public:
    enum {
        // Total size.
        WIDTH = 25,
        HEIGHT = 21,
        // Own field.
        OWN_FIELD_START_Y = 3,
        OWN_FIELD_START_X = 1,
        // Enemy field.
        ENEMY_FIELD_START_Y = 3,
        ENEMY_FIELD_START_X = 14,
        // Field size.
        FIELD_SIZE = 10,
        // Server state.
        SERVER_STATE_Y = 19,
        SERVER_STATE_X = 12,
        SERVER_STATE_WIDTH = 11,
        // Game status.
        GAME_STATUS_Y = 14,
        GAME_STATUS_X = 3,
        GAME_STATUS_WIDTH = 20,
        // Timer.
        TIMER_Y = 15,
        TIMER_X = 3,
        TIMER_WIDTH = 20,
        // User message.
        USER_MSG_Y = 16,
        USER_MSG_X = 3,
        USER_MSG_WIDTH = 20
    };

    enum ColorPair {
        ERROR_COL_PAIR,
        OK_COL_PAIR
    };

    /**
     * @brief NcursesInterface
     */
    NcursesInterface();

    /**
     * @brief ~NcursesInterface
     */
    virtual ~NcursesInterface();

    /**
     * @brief is_valid
     * @return
     */
    bool is_valid() const;

    /**
     * @brief process_input
     * @return
     */
    InputResult process_input();

    /**
     * @brief show
     */
    void show();

    /**
     * @brief set_game_state
     * @param p_state
     */
    void set_game_state(GameState p_state);

    /**
     * @brief set_server_conn_state
     * @param ok
     */
    void set_server_conn_state(bool ok);

    /**
     * @brief activate_preparation_mode
     * @param end_time
     */
    void activate_preparation_mode(time_t end_time);

    /**
     * @brief stop_preparation_mode
     */
    void stop_preparation_mode();

    /**
     * @brief activate_own_turn_mode
     * @param end_time
     */
    void activate_own_turn_mode(time_t end_time);

    /**
     * @brief stop_own_turn_mode
     */
    void stop_own_turn_mode();

    /**
     * @brief activate_enemy_turn_mode
     * @param end_time
     */
    void activate_enemy_turn_mode(time_t end_time);

    /**
     * @brief stop_enemy_turn_mode
     */
    void stop_enemy_turn_mode();
private:
    bool loaded;
    int background_img[WIDTH][HEIGHT];
    bool has_color;

    Cursor cursor;

    Timer timer;

    bool _load();
    void _init_curses_mode();
    void _init_color_pairs();
    void _on_color(ColorPair cp);
    void _off_color(ColorPair cp);

    void _clear_all();
    void _clear_own_field();
    void _clear_enemy_field();
    void _clear_server_state();
    void _clear_game_state();
    void _clear_user_msg();
    void _clear_timer();

    void _draw_background();
    void _draw_server_conn_state();
    void _draw_game_state();
    void _draw_timer();
    void _draw_own_field();
    void _draw_enemy_field();

    void _move_cursor_to_own_field();
    void _move_cursor_to_enemy_field();

    InputResult _process_move_input(int key);
    InputResult _process_enter_input();
};

#endif
