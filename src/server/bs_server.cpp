#include "bs_server.h"
#include "client_data.h"
#include "game_session.h"
#include "timeout_handler.h"
#include "tools/write_log.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>

BattleshipServer::BattleshipServer(int p_port)
    : port(p_port)
    , clients_queue(CLIENT_QUEUE_LEN)
    , clients_in_game(GAME_COUNT*2)
    , quit_flag(false)
{
    _create_socket();
    _fill_address();
    _bind_socket();
}

BattleshipServer::~BattleshipServer()
{

}

void BattleshipServer::run()
{
    if(!_start_listen()) {
        const char *err = strerror(errno);
        WRITE_LOG->print("Can't start the listening: %s", err);
        exit(1);
    }

    quit_flag = false;
    // The main cycle.
    while(!quit_flag) {
        _fill_fd_sets();
        _set_timeout();

        struct timeval before_select_time;
        gettimeofday(&before_select_time, NULL);

        int res = select(max_d+1, &readfds, &writefds, 0, &timeout);

        struct timeval after_select_time;
        gettimeofday(&after_select_time, NULL);

        after_select_time.tv_usec -= before_select_time.tv_usec;
        if(after_select_time.tv_usec < 0) {
            after_select_time.tv_sec -= 1;
            if(after_select_time.tv_sec >= 0) {
                after_select_time.tv_usec += 1000000;
            }
        }
        after_select_time.tv_sec -= before_select_time.tv_sec;

        _process_select_result(res, after_select_time);
    }
}

void BattleshipServer::_create_socket()
{
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1) {
        const char *err = strerror(errno);
        WRITE_LOG->print("Can't create socket: %s", err);
        exit(2);
    }
}

void BattleshipServer::_fill_address()
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void BattleshipServer::_bind_socket()
{
    // Invalid arguments.
    if(listen_fd == -1) {
        WRITE_LOG->print("Invalid the listen fd = %d", listen_fd);
        exit(3);
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int ok = bind(listen_fd, (sockaddr*)&addr, sizeof(addr));
    if(ok == -1) {
        char *err = strerror(errno);
        WRITE_LOG->print("Can't bind socket: %s", err);
        exit(4);
    }
}

bool BattleshipServer::_start_listen()
{
    int ok;
    ok = listen(listen_fd, CLIENT_QUEUE_LEN);
    if(ok == -1) {
        return false;
    }
    return true;
}

void BattleshipServer::_accept_new_connection()
{
    if(clients_queue.isFull()) {
        // TODO: Send message.
        WRITE_LOG->print("Can't add the client");
        return;
    }

    int client_fd;
    ClientData *client = new ClientData(this);
    sockaddr *client_addr = client->get_address();
    socklen_t addr_len = sizeof(*client_addr);
    client_fd = accept(listen_fd, client_addr, &addr_len);
    if(client_fd == -1) {
        const char *err = strerror(errno);
        WRITE_LOG->print("Error on accept: %s", err);
        delete client;
        return;
    }

    clients_queue.add(client);
    client->set_fd(client_fd);
    WRITE_LOG->print("Client connected: fd = %d", client_fd);
}

void BattleshipServer::_fill_fd_sets()
{
    max_d = listen_fd;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(listen_fd, &readfds);

    int i;
    int fd;
    int client_cnt = clients_in_game.get_count();
    int unprocessed_cnt = client_cnt;
    for(i = 0; unprocessed_cnt > 0; i++) {
        const ClientData *client = clients_in_game[i];
        if(!client) {
            continue;
        }

        fd = client->get_fd();
        FD_SET(fd, &readfds);

        if(client->ready_write()) {
            FD_SET(fd, &writefds);
        }

        if(fd > max_d) {
            max_d = fd;
        }

        --unprocessed_cnt;
    }

    client_cnt = clients_queue.get_count();
    unprocessed_cnt = client_cnt;
    for(i = 0; unprocessed_cnt > 0; i++) {
        const ClientData *client = clients_queue[i];
        if(!client) {
            continue;
        }

        fd = client->get_fd();
        FD_SET(fd, &readfds);

        if(client->ready_write()) {
            FD_SET(fd, &writefds);
        }

        if(fd > max_d) {
            max_d = fd;
        }

        --unprocessed_cnt;
    }
}

void BattleshipServer::_set_timeout()
{
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
}

void BattleshipServer::_process_select_result(int result,
                                              const timeval &passed_time)
{
    if(result == -1) {
        _process_signals();
        return;
    }

    _process_timeouts(passed_time);
    if(result != 0) {
        if(FD_ISSET(listen_fd, &readfds)) {
            _accept_new_connection();
        }
        _process_fd_actions();
    }
}

void BattleshipServer::_process_fd_actions()
{
    int fd;
    int client_cnt = clients_in_game.get_count();
    int unprocessed_cnt = client_cnt;
    WRITE_LOG->print("client count %d", client_cnt);
    for(int i = 0; unprocessed_cnt > 0; i++) {
        ClientData *client = clients_in_game[i];
        if(!client) {
            continue;
        }
        fd = client->get_fd();
        WRITE_LOG->print("fd action fd = %d", fd);
        if(FD_ISSET(fd, &readfds)) {
            WRITE_LOG->print("read");
            BuffError err = client->read_msg();
            if(err == CONN_BROKEN) {
                disconnect_client(client);
                --unprocessed_cnt;
                continue;
            }
        }
        if(FD_ISSET(fd, &writefds)) {
            WRITE_LOG->print("write");
            client->write_msg();
        }
        client->process_msg();
        --unprocessed_cnt;
    }

    client_cnt = clients_queue.get_count();
    unprocessed_cnt = client_cnt;
    for(int i = 0; unprocessed_cnt > 0; i++) {
        ClientData *client = clients_queue[i];
        if(!client) {
            continue;
        }
        fd = client->get_fd();
        WRITE_LOG->print("fd action fd = %d", fd);
        if(FD_ISSET(fd, &readfds)) {
            BuffError err = client->read_msg();
            if(err == CONN_BROKEN) {
                disconnect_client(client);
                --unprocessed_cnt;
                continue;
            }
        }
        if(FD_ISSET(fd, &writefds)) {
            client->write_msg();
        }
        client->process_msg();
        --unprocessed_cnt;
    }
}

void BattleshipServer::_process_signals()
{
    // TODO: process signals.
    if(errno == EINTR) {
    }
    else {
    }
}

void BattleshipServer::_process_timeouts(const timeval &passed_time)
{
    if(timeout_handlers.is_empty()) {
        return;
    }

    List<TimeoutHandler> tmp;
    while(!timeout_handlers.is_empty()) {
        TimeoutHandler *handler = timeout_handlers.get();
        handler->process(passed_time.tv_sec, passed_time.tv_usec);
        if(!handler->elapsed()) {
            tmp.put(handler);
        }
        else {
            delete handler;
        }
    }
    timeout_handlers.swap(tmp);
}

bool BattleshipServer::connect_player_to_game(ClientData *player)
{
    NULL_PTR_CHECK_RET_ZERO(player)

    GameSession *session = 0;
    int sessionsCount = sessions.get_size();

    for(int i = 0; i < sessionsCount; i++) {
        session = sessions[i];
        if(player->set_game_session(session)) {
            return _change_client_queue(player);
        }
    }

    if(sessions.get_size() == GAME_COUNT) {
        return false;
    }
    else {
        session = new GameSession(this);
        sessions.put(session);
        player->set_game_session(session);
        return _change_client_queue(player);
    }
    return false;
}

void BattleshipServer::disconnect_client(ClientData *client)
{
    NULL_PTR_CHECK_RETURN(client)

    int res = close(client->get_fd());
    if(res == -1) {
        WRITE_LOG->print("fd close error on ClientData");
    }
    client->disconnect_from_game_session();
    clients_queue.remove(client);
    clients_in_game.remove(client);
    delete client;
}

void BattleshipServer::add_timeout_handler(TimeoutHandler *timeout_handler)
{
    NULL_PTR_CHECK_RETURN(timeout_handler)

    timeout_handlers.put(timeout_handler);
}

bool BattleshipServer::_change_client_queue(ClientData *player, bool to_game)
{
    NULL_PTR_CHECK_RET_ZERO(player)

    if(to_game) {
        if(clients_in_game.add(player)) {
            clients_queue.remove(player);
            return true;
        }
    }
    else {
        if(clients_queue.add(player)) {
            clients_in_game.remove(player);
            player->disconnect_from_game_session();
            return true;
        }
    }
    return false;
}
