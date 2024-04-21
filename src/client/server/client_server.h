#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H

#include "list.h"
#include "msg_buffer.h"
#include "msg_delivery_control.h"
#include <netinet/in.h>

/**
 * @brief The ClientServer class
 */
class ClientServer {
public:
    /**
     * @brief ClientServer
     * @param p_server_ip
     * @param p_server_port
     */
    ClientServer(const char *p_server_ip, int p_server_port);

    /**
     * @brief ~ClientServer
     */
    ~ClientServer();

    /**
     * @brief run
     */
    void run();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief get_msg_num
     * @return
     */
    static long long get_msg_num();

    /**
     * @brief set_interface_fd
     * @param p_fd
     */
    void set_interface_fd(int p_fd);
private:
    int server_fd;
    sockaddr_in server_addr;
    bool has_server_connection;

    bool quit_flag;

    fd_set readfds, writefds;

    MsgBuffer buf_from_server;
    MsgBuffer buf_from_interface;

    List<Message> to_interface_msg;
    List<Message> to_server_msg;

    List<Message> from_server;
    List<Message> from_interface;

    int interface_fd;

    MsgDeliveryControl delivery_control;

    static long long msg_num;

    void _create_socket();

    void _process_select_result(int result);

    void _read_msg_from_server();
    void _read_msg_from_interface();
    void _write_msg_to_server();
    void _write_msg_to_interface();
    void _process_fd_actions();
    void _fill_fd_sets();

    void _process_msg_from_server();
    void _process_cmd_from_interface();
    void _redirect_to_server(Message *msg);
    void _redirect_to_interface(Message *msg);
    void _process_server_conn_status_req_msg(const ServerConnStatusReqMsg *msg);
    void _process_receipt_msg(const ReceiptMsg *msg);

    void _send_server_conn_status();
    bool _connect_to_server();
    void _process_server_conn_req_msg(const ServerConnReqMsg *msg);
    int _get_max_fd() const;

    void _on_broken_server_conn();
    void _send_receipt(long long id);
};

#endif
