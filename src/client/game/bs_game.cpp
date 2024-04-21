#include "bs_game.h"
#include "ncurses_interface.h"
#include "tools/write_log.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

BattleshipGame::BattleshipGame()
    : interface(0)
    , quit_flag(false)
{
    interface = new NcursesInterface;
}

BattleshipGame::~BattleshipGame()
{
    delete interface;
}

int BattleshipGame::run()
{
    if(!interface->is_valid()) {
        WRITE_LOG->print("Invalid interface");
        return 1;
    }

    interface->show();

    InputResult input;
    quit_flag = false;
    while(!quit_flag) {
        input = interface->process_input();
        switch(input) {
        case QUIT_IN:
            return 0;
        case REQ_SERVER_CONN_STATUS_IN:
            _req_server_conn_status();
            break;
        case REQ_SERVER_CONN_IN:
            _req_server_conn();
            break;
        case REQ_GAME_SESSION_IN:
            _req_game_session();
            break;
        case INVALID_IN:
            break;
        case PREPARE_FIELD_IN:
            break;
        case CURSOR_MOVE_IN:
            break;
        case SHOT_IN:
            _send_shot_pos();
            break;
        }

        int max_d = client_fd;
        _fill_fd_sets();
        struct timeval t;
        t.tv_sec = 0;
        t.tv_usec = 100000;
        int res = select(max_d+1, &readfds, &writefds, 0, &t);
        _process_select_result(res);
        interface->show();
    }
    return 0;
}

void BattleshipGame::_send_shot_pos()
{
    interface->stop_own_turn_mode();
    GameMoveMsg *msg = new GameMoveMsg(interface->get_last_shot());
    to_server.put(msg);
}

void BattleshipGame::_fill_fd_sets()
{
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(client_fd, &readfds);

    if(to_server.get_size()) {
        FD_SET(client_fd, &writefds);
    }
}
void BattleshipGame::_process_select_result(int result)
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

void BattleshipGame::_process_fd_actions()
{
    if(FD_ISSET(client_fd, &readfds)) {
        _read_msg_from_server();
    }

    _process_msg_from_server();

    if(FD_ISSET(client_fd, &writefds)) {
        _write_msg_for_server();
    }
}

void BattleshipGame::set_client_fd(int p_fd)
{
    client_fd = p_fd;
}

void BattleshipGame::_req_server_conn_status()
{
    ServerConnStatusReqMsg *msg = new ServerConnStatusReqMsg;
    to_server.put(msg);
}

void BattleshipGame::_req_server_conn()
{
    ServerConnReqMsg *msg = new ServerConnReqMsg();
    to_server.put(msg);
}

void BattleshipGame::_req_game_session()
{
    GameSessionReqMsg *msg = new GameSessionReqMsg;
    to_server.put(msg);
}

void BattleshipGame::_process_msg_from_server()
{
    Message *msg = 0;
    while((msg = from_server.get())) {
        switch(msg->type) {
        case UNDEFINED_MSG:
        case INVALID_MSG:
        case GAME_SESSION_REQ_MSG:
        case GAME_MOVE_MSG:
        case SERVER_CONN_STATUS_REQ_MSG:
        case SERVER_CONN_REQ_MSG:
        case RECEIPT_MSG:
            break;
        case GAME_STATE_MSG:
            _process_game_state_msg(static_cast<GameStateMsg*>(msg));
            break;
        case GAME_FIELD_MSG:
            _process_field_msg(static_cast<GameFieldMsg*>(msg));
            break;
        case SERVER_CONN_STATUS_MSG:
            _process_server_conn_status_msg(static_cast<ServerConnStatusMsg*>(msg));
            break;
        case START_FIELD_REQ_MSG:
            _process_start_field_req_msg(static_cast<StartFieldReqMsg*>(msg));
            break;
        case STOP_PREPARATION_MSG:
            _process_stop_preparation_msg(static_cast<StopPreparationMsg*>(msg));
            break;
        case START_TURN_MSG:
            _process_start_turn_msg(static_cast<StartTurnMsg*>(msg));
            break;
        case STOP_TURN_MSG:
            _process_stop_turn_msg(static_cast<StopTurnMsg*>(msg));
            break;
        case INFO_MSG:
            _process_info_msg(static_cast<InfoMsg*>(msg));
            break;
        case ERROR_MSG:
            _process_error_msg(static_cast<ErrorMsg*>(msg));
            break;
        }
        Message::remove(msg);
    }
}

void BattleshipGame::_process_start_turn_msg(const StartTurnMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)
    if(msg->is_own) {
        interface->activate_own_turn_mode(msg->end_time);
        interface->set_game_state(OWN_TURN);
    }
    else {
        interface->activate_enemy_turn_mode(msg->end_time);
        interface->set_game_state(ENEMY_TURN);
    }
}

void BattleshipGame::_process_stop_turn_msg(const StopTurnMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)
    if(msg->is_own) {
        interface->stop_own_turn_mode();
    }
    else {
        interface->stop_enemy_turn_mode();
    }
    interface->set_game_state(PROCESS_TURN);
}

void BattleshipGame::_write_msg_for_server()
{
    Message *msg = 0;
    int msg_size;
    int res;
    while((msg = to_server.get())) {
        msg_size = msg->get_type_data_size(msg->type);
        char *data = msg->get_data();
        res = write(client_fd, data, msg_size);
        delete data;
        if(res != msg_size) {
            WRITE_LOG->print("Can't write msg for server id=%d type=%d",
                             msg->id, msg->type);
            to_server.put(msg);
            break;
        }
    }
}

void BattleshipGame::_read_msg_from_server()
{
    BuffError err = buf.read_msg(client_fd);
    if(err == BUFF_OK) {
        Message *msg;
        while((msg = buf.extract())) {
            from_server.put(msg);
        }
    }
    else {
        if(err == CONN_BROKEN) {
            quit_flag = true;
        }
    }
}

void BattleshipGame::_process_start_field_req_msg(const StartFieldReqMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    WRITE_LOG->print("Activate PREPARATION MODE");
    interface->activate_preparation_mode(msg->end_time);
    interface->set_game_state(PREPARATION);
}

void BattleshipGame::_process_stop_preparation_msg(const StopPreparationMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    WRITE_LOG->print("Stop PREPARATION MODE");
    interface->stop_preparation_mode();
    interface->set_game_state(WAIT_START);
    GameField field(interface->get_own_field());
    GameFieldMsg *field_msg = new GameFieldMsg(field);
    to_server.put(field_msg);
}

void BattleshipGame::_process_game_state_msg(const GameStateMsg *msg )
{
    NULL_PTR_CHECK_RETURN(msg)

    GameState state = msg->state;
    interface->set_game_state(state);
}

void BattleshipGame::_process_field_msg(const GameFieldMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    if(msg->is_own_field) {
        interface->set_own_field(msg->field);
    }
    else {
        interface->set_enemy_field(msg->field);
    }
}

void BattleshipGame::_process_server_conn_status_msg(const ServerConnStatusMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    interface->set_server_conn_state(msg->connected);
}

void BattleshipGame::_process_info_msg(const InfoMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)
    // TODO
}

void BattleshipGame::_process_error_msg(const ErrorMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)
    // TODO
}
