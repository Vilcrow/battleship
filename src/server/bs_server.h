#ifndef BATTLESHIP_SERVER_H
#define BATTLESHIP_SERVER_H

#include <netinet/in.h>
#include "client_array.h"
#include "game_session.h"
#include "list.h"

class TimeoutHandler;

/**
 * @brief The BattleshipServer class
 */
class BattleshipServer {
public:
    /**
     * @brief Constructor
     * @param p_port port number
     */
    BattleshipServer(int p_port);

    /**
     * @brief Destructor
     */
    ~BattleshipServer();

    /**
     * @brief Enters the main event loop
     */
    void run();

    /**
     * @brief Connects the player to game if it is possible
     * @param player pointer to the player data
     * @return true if the player was connected to game,
     * otherwise returns false
     */
    bool connect_player_to_game(ClientData *player);

    /**
     * @brief Disconnects the player from him game session
     * @param client pointer to the player data
     */
    void disconnect_client(ClientData *client);

    /**
     * @brief Adds the given timout handler to the processing queue in
     * the main loop
     * @param timeout_handler pointer to the added timeout handler
     */
    void add_timeout_handler(TimeoutHandler *timeout_handler);
private:
    enum {
        CLIENT_QUEUE_LEN = 10
    };

    enum {
        GAME_COUNT = 5
    };

    int port;
    int listen_fd;
    sockaddr_in addr;

    List<GameSession> sessions;
    List<TimeoutHandler> timeout_handlers;
    ClientArray clients_queue;
    ClientArray clients_in_game;

    bool quit_flag;

    fd_set readfds, writefds;
    int max_d;
    struct timeval timeout;

    void _create_socket();
    void _fill_address();
    void _bind_socket();

    bool _start_listen();

    void _fill_fd_sets();
    void _set_timeout();
    void _process_select_result(int result, const struct timeval &passed_time);
    void _accept_new_connection();
    void _process_fd_actions();
    void _process_signals();
    void _process_timeouts(const struct timeval &passed_time);

    bool _change_client_queue(ClientData *player, bool to_game = true);
};

#endif
