#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include <netinet/in.h>
#include "list.h"
#include "msg_buffer.h"
#include "msg_delivery_control.h"

class BattleshipServer;
class GameSession;
class ReceiptMsg;

/**
 * @brief A class for storing data of connected client
 */
class ClientData {
public:
    enum {
        CLIENT_BUFF_SIZE = 4096
    };

    /**
     * @brief Constructor
     * @param p_server pointer to the BattleshipServer object
     */
    ClientData(BattleshipServer *p_server);

    /**
     * @brief Destructor
     */
    ~ClientData();

    /**
     * @brief Returnst pointer to address of client
     */
    sockaddr* get_address();

    /**
     * @brief set_game_session
     * @param p_session pointer to the game session object
     * @return
     */
    bool set_game_session(GameSession *p_session);

    /**
     * @brief disconnect_from_game_session
     */
    void disconnect_from_game_session();

    /**
     * @brief get_fd
     * @return
     */
    int get_fd() const;

    /**
     * @brief set_fd
     * @param p_fd
     */
    void set_fd(int p_fd);

    /**
     * @brief ready_write
     * @return
     */
    bool ready_write() const;

    /**
     * @brief read_msg
     * @return
     */
    BuffError read_msg();

    /**
     * @brief write_msg
     */
    void write_msg();

    /**
     * @brief process_msg
     */
    void process_msg();

    /**
     * @brief send_msg
     * @param msg
     */
    void send_msg(Message *msg);

    /**
     * @brief get_msg_num
     * @return
     */
    static long long get_msg_num();
private:
    BattleshipServer *server;
    GameSession *game_session;

    int fd;
    sockaddr_in address;

    MsgBuffer buf;

    List<Message> to_send_msg;
    List<Message> to_process_msg;

    MsgDeliveryControl delivery_control;

    static long long msg_num;

    void _process_receipt_msg(const ReceiptMsg *msg);
    void _process_game_session_req_msg(const GameSessionReqMsg *msg);
    void _process_game_field_msg(const GameFieldMsg *msg);
    void _process_game_move_msg(const GameMoveMsg *msg);
    void _send_receipt(long long id);
};

#endif
