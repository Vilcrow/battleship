#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "client_data.h"
#include "game_field.h"
#include "const.h"

class BattleshipServer;
class GameTurnTimeout;

/**
 * @brief The GameSession class
 */
class GameSession {
public:
    enum PlayerNumber {
        FIRST_PLAYER = 0,
        SECOND_PLAYER = 1,
        PLAYERS_COUNT
    };

    /**
     * @brief is_valid_player_num
     * @param num
     * @return
     */
    static bool is_valid_player_num(int num);

    /**
     * @brief Constructor
     * @param p_server
     */
    GameSession(BattleshipServer *p_server);

    /**
     * @brief Destructor
     */
    ~GameSession();

    /**
     * @brief get_game_state
     * @return
     */
    GameState get_game_state() const;

    /**
     * @brief set_player
     * @param player
     * @return
     */
    bool set_player(ClientData *player);

    /**
     * @brief disconnect_player
     * @param player
     */
    void disconnect_player(const ClientData *player);

    /**
     * @brief set_field
     * @param num
     * @param p_field
     * @return
     */
    bool set_field(PlayerNumber num, const GameField &p_field);

    /**
     * @brief set_field
     * @param player
     * @param p_field
     * @return
     */
    bool set_field(ClientData *player, const GameField &p_field);

    /**
     * @brief get_field
     * @param num
     * @param ok
     * @return
     */
    GameField get_field(PlayerNumber num, bool *ok = 0) const;

    /**
     * @brief get_field
     * @param player
     * @param own
     * @param ok
     * @return
     */
    GameField get_field(ClientData *player, bool own = true, bool *ok = 0) const;

    /**
     * @brief process_move
     * @param player
     * @param shot
     * @return
     */
    GFCellState process_move(ClientData *player, const Point &shot);

    /**
     * @brief send_game_state_for_all
     */
    void send_game_state_for_all();

    /**
     * @brief set_game_state
     * @param p_state
     */
    void set_game_state(GameState p_state);

    /**
     * @brief send_for_all
     * @param msg
     */
    void send_for_all(Message *msg);

    /**
     * @brief start_preparation
     */
    void start_preparation();

    /**
     * @brief stop_preparation
     */
    void stop_preparation();

    /**
     * @brief check_start_fields
     */
    void check_start_fields();

    /**
     * @brief get_players_count
     * @return
     */
    int get_players_count() const;

    /**
     * @brief change_cur_turn_player_idx
     */
    void change_cur_turn_player_idx();

    /**
     * @brief start_game
     */
    void start_game();

    /**
     * @brief send_start_fields
     */
    void send_start_fields();

    /**
     * @brief end_turn
     */
    void end_turn();

    /**
     * @brief change_turn_player
     */
    void change_turn_player();

    /**
     * @brief set_last_shot
     * @param shot
     * @param player
     * @return
     */
    bool set_last_shot(const Point &shot, const ClientData *player);

    /**
     * @brief get_last_shot
     * @return
     */
    const Point& get_last_shot() const;

    /**
     * @brief has_last_shot
     * @return
     */
    bool has_last_shot() const;

    /**
     * @brief clear_last_shot
     */
    void clear_last_shot();

    /**
     * @brief end_game
     */
    void end_game();
private:
    BattleshipServer *server;

    GameState state;

    ClientData* players[PLAYERS_COUNT];
    GameField fields[PLAYERS_COUNT];

    int cur_turn_player_idx;
    Point last_shot;
    GameTurnTimeout *turn_timeout;

    bool _is_vacant() const;
    bool _has_player(int num) const;
    bool _has_player(const ClientData *player) const;
    int _get_player_index(const ClientData *player, bool own = true) const;
    static int _get_first_turn_player_idx();
    static int _get_enemy_idx(int idx);
    void _start_new_turn();
    GFCellState _register_shot();
    void _send_fields();
    bool _is_game_over() const;
    void _send_game_result();
};

#endif
