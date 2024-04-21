#include "client_server.h"
#include "tools/write_log.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

long long ClientServer::msg_num = Message::START_REG_NUM;

ClientServer::ClientServer(const char *p_server_ip, int p_server_port)
    : server_fd(-1)
    , has_server_connection(false)
    , interface_fd(-1)
{
    _create_socket();

    // Fill server address.
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(p_server_port);
    server_addr.sin_addr.s_addr = inet_addr(p_server_ip);
}

ClientServer::~ClientServer()
{

}

void ClientServer::run()
{
    quit_flag = false;
    while(!quit_flag) {
        int max_d = _get_max_fd();
        _fill_fd_sets();
        int res = select(max_d+1, &readfds, &writefds, 0, 0);
        _process_select_result(res);
    }
}

void ClientServer::stop()
{
    quit_flag = true;
}

int ClientServer::_get_max_fd() const
{
    return MAX(server_fd, interface_fd);
}

void ClientServer::_on_broken_server_conn()
{
    WRITE_LOG->print("Server connection closed");
    close(server_fd);
    _create_socket();
    has_server_connection = false;
    _send_server_conn_status();
}

long long ClientServer::get_msg_num()
{
    return msg_num++;
}

void ClientServer::set_interface_fd(int p_fd)
{
    interface_fd = p_fd;
}

void ClientServer::_create_socket()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) {
        WRITE_LOG->print("Can't create socket %s: %d", __FILE__, __LINE__);
    }
}

void ClientServer::_process_select_result(int result)
{
    if(result == -1) {
        //_process_signals();
        return;
    }

    if(result == 0) {
        //_process_timeouts();
    }

    _process_fd_actions();
    //_update_timeouts();
}

void ClientServer::_read_msg_from_server()
{
    if(!has_server_connection) {
        return;
    }

    BuffError err = buf_from_server.read_msg(server_fd);
    if(err == BUFF_OK) {
        Message *msg;
        while((msg = buf_from_server.extract())) {
            from_server.put(msg);
        }
    }
    else {
        WRITE_LOG->print("Error in read %d", err);
        if(err == CONN_BROKEN) {
            _on_broken_server_conn();
        }
    }
}

void ClientServer::_read_msg_from_interface()
{
    BuffError err = buf_from_interface.read_msg(interface_fd);
    if(err == BUFF_OK) {
        Message *msg;
        while((msg = buf_from_interface.extract())) {
            from_interface.put(msg);
        }
    }
    else {
        WRITE_LOG->print("Error in read %d", err);
        if(err == CONN_BROKEN) {
            WRITE_LOG->print("Interface connection closed %d", err);
            quit_flag = true;
        }
    }
}

void ClientServer::_write_msg_to_server()
{
    if(!has_server_connection) {
        return;
    }

    int res = 0;
    int msg_size = 0;
    Message *msg = 0;
    while((msg = to_server_msg.get())) {
        MsgType type = msg->get_type();
        if(type == INVALID_MSG || type == UNDEFINED_MSG) {
            WRITE_LOG->print("Detected invalid msg which will be deleted");
            delete msg;
            continue;
        }
        if(msg->id == Message::NO_ID) {
            msg->id = get_msg_num();
        }
        msg_size = msg->get_type_data_size(msg->type);
        char *data = msg->get_data();
        res = write(server_fd, data, msg_size);
        if(res == msg_size) {
            WRITE_LOG->print("Writed for server: size = %d type =%d id=%d",
                             res, msg->type, msg->id);
            if(msg->type == RECEIPT_MSG) {
                delete msg;
            }
            else if(!delivery_control.under_control(msg)) {
                delivery_control.add(msg);
            }
            else {
                delivery_control.decrease(msg);
            }
        }
        else {
            // Sending failed. Return back the message.
            to_server_msg.put(msg);
            WRITE_LOG->print("NOT Writed");
            delete data;
            break;
        }
        delete data;
    }
    delivery_control.resend(to_server_msg);
}

void ClientServer::_write_msg_to_interface()
{
    int res = 0;
    int msg_size = 0;
    Message *msg = 0;
    while(to_interface_msg.get_size()) {
        WRITE_LOG->print("Msg for interface count = %d",
                         to_interface_msg.get_size());
        msg = to_interface_msg.get();
        msg_size = msg->get_type_data_size(msg->type);
        char *data = msg->get_data();
        res = write(interface_fd, data, msg_size);
        if(res == msg_size) {
            WRITE_LOG->print("Writed to interface size = %d type=%d",
                             res, msg->type);
        }
        else {
            // Sending failed. Return back the message.
            to_interface_msg.put(msg);
            WRITE_LOG->print("NOT Writed");
            break;
        }
        delete data;
    }
}

void ClientServer::_process_msg_from_server()
{
    Message *msg = 0;
    bool need_del = true;
    while((msg = from_server.get())) {
        need_del = true;
        switch(msg->type) {
        case UNDEFINED_MSG:
        case INVALID_MSG:
            WRITE_LOG->print("Invalid msg");
            Message::remove(msg);
            continue;
        case GAME_SESSION_REQ_MSG:
        case SERVER_CONN_STATUS_REQ_MSG:
        case GAME_MOVE_MSG:
        case SERVER_CONN_REQ_MSG:
            WRITE_LOG->print("Unsupported msg");
            break;
        case GAME_STATE_MSG:
        case GAME_FIELD_MSG:
        case START_FIELD_REQ_MSG:
        case SERVER_CONN_STATUS_MSG:
        case STOP_PREPARATION_MSG:
        case START_TURN_MSG:
        case STOP_TURN_MSG:
        case INFO_MSG:
        case ERROR_MSG:
            _redirect_to_interface(msg);
            need_del = false;
            break;
        case RECEIPT_MSG:
            _process_receipt_msg(static_cast<ReceiptMsg*>(msg));
            break;
        }

        if(msg->type != RECEIPT_MSG) {
            _send_receipt(msg->id);
        }
        if(need_del) {
            Message::remove(msg);
        }
    }
}

void ClientServer::_process_cmd_from_interface()
{
    Message *msg = 0;
    bool need_del = true;
    while((msg = from_interface.get())) {
        need_del = true;
        switch(msg->type) {
        case UNDEFINED_MSG:
        case INVALID_MSG:
            WRITE_LOG->print("Invalid msg from interface");
            break;
        case GAME_SESSION_REQ_MSG:
        case GAME_FIELD_MSG:
        case GAME_MOVE_MSG:
            _redirect_to_server(msg);
            need_del = false;
            break;
        case SERVER_CONN_STATUS_REQ_MSG:
            _process_server_conn_status_req_msg(static_cast<ServerConnStatusReqMsg*>(msg));
            break;
        case SERVER_CONN_REQ_MSG:
            _process_server_conn_req_msg(static_cast<ServerConnReqMsg*>(msg));
            break;
        case GAME_STATE_MSG:
        case START_FIELD_REQ_MSG:
        case SERVER_CONN_STATUS_MSG:
        case STOP_PREPARATION_MSG:
        case START_TURN_MSG:
        case STOP_TURN_MSG:
        case RECEIPT_MSG:
        case INFO_MSG:
        case ERROR_MSG:
            WRITE_LOG->print("Unsupported msg from interface");
            break;
        }
        if(need_del) {
            Message::remove(msg);
        }
    }
}

void ClientServer::_process_server_conn_req_msg(const ServerConnReqMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    _connect_to_server();
    _send_server_conn_status();
}

void ClientServer::_redirect_to_server(Message *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    to_server_msg.put(msg);
}

void ClientServer::_redirect_to_interface(Message *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    to_interface_msg.put(msg);
}

void ClientServer::_send_receipt(long long id)
{
    ReceiptMsg *msg = new ReceiptMsg(id);
    to_server_msg.put(msg);
}

void ClientServer::_process_server_conn_status_req_msg(const ServerConnStatusReqMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    _send_server_conn_status();
}

void ClientServer::_send_server_conn_status()
{
    ServerConnStatusMsg *answ = new ServerConnStatusMsg(has_server_connection);
    WRITE_LOG->print("Send server status = %d", answ->connected);
    to_interface_msg.put(answ);
}

bool ClientServer::_connect_to_server()
{
    if(has_server_connection) {
        return true;
    }

    int ok;
    ok = connect(server_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    if(ok == -1) {
        const char *err = strerror(errno);
        WRITE_LOG->print("Can't connect to server: %s", err);
        return false;
    }
    else {
        has_server_connection = true;
        return true;
    }

    return false;
}

void ClientServer::_process_receipt_msg(const ReceiptMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    if(!delivery_control.process_check(msg)) {
        WRITE_LOG->print("NO MSG with #%lld type = %d, id = %d",
                         msg->req_msg_id, msg->type, msg->id);
    }
}

void ClientServer::_process_fd_actions()
{
    if(has_server_connection) {
        if(FD_ISSET(server_fd, &readfds)) {
            _read_msg_from_server();
        }

        _process_msg_from_server();

        if(FD_ISSET(server_fd, &writefds)) {
            _write_msg_to_server();
        }
    }

    if(FD_ISSET(interface_fd, &readfds)) {
        _read_msg_from_interface();
    }

    _process_cmd_from_interface();

    if(FD_ISSET(interface_fd, &writefds)) {
        _write_msg_to_interface();
    }
}

void ClientServer::_fill_fd_sets()
{
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(interface_fd, &readfds);

    if(has_server_connection) {
        FD_SET(server_fd, &readfds);
        if(to_server_msg.get_size()) {
            FD_SET(server_fd, &writefds);
        }
    }

    if(to_interface_msg.get_size()) {
        FD_SET(interface_fd, &writefds);
    }
}
