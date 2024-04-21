#ifndef BS_GAME_H
#define BS_GAME_H

#include "game_interface.h"
#include "client_server.h"

/**
 * @brief The BattleshipGame class
 */
class BattleshipGame {
public:
    /**
     * @brief BattleshipGame
     */
    BattleshipGame();

    /**
     * @brief ~BattleshipGame
     */
    ~BattleshipGame();

    /**
     * @brief run
     * @return
     */
    int run();

    /**
     * @brief set_client_fd
     * @param p_fd
     */
    void set_client_fd(int p_fd);
private:
    GameInterface *interface;

    // The client server pipe descriptors.
    int client_fd;
    int server_write_fd;

    fd_set readfds, writefds;

    int old_fd;

    MsgBuffer buf;
    List<Message> from_server;
    List<Message> to_server;

    bool quit_flag;

    void _process_msg_from_server();
    void _write_msg_for_server();
    void _read_msg_from_server();
    void _req_server_conn();
    void _req_server_conn_status();
    void _req_game_session();

    void _process_game_state_msg(const GameStateMsg *msg);
    void _process_field_msg(const GameFieldMsg *msg);
    void _process_server_conn_status_msg(const ServerConnStatusMsg *msg);
    void _process_start_field_req_msg(const StartFieldReqMsg *msg);
    void _process_stop_preparation_msg(const StopPreparationMsg *msg);
    void _process_start_turn_msg(const StartTurnMsg *msg);
    void _process_stop_turn_msg(const StopTurnMsg *msg);
    void _process_info_msg(const InfoMsg *msg);
    void _process_error_msg(const ErrorMsg *msg);

    void _process_select_result(int result);
    void _process_fd_actions();
    void _fill_fd_sets();
    void _send_shot_pos();
};

#endif
