#include "client_data.h"
#include "bs_server.h"
#include "game_session.h"
#include "message.h"
#include "tools/write_log.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

long long ClientData::msg_num = Message::START_REG_NUM;

ClientData::ClientData(BattleshipServer *p_server)
    : server(p_server)
    , game_session(0)
    , fd(-1)
{
    memset(&address, 0, sizeof(address));
}

ClientData::~ClientData()
{
    if(fd != -1) {
        int res = close(fd);
        if(res == -1) {
            WRITE_LOG->print("fd close error on ClientData");
        }
    }
}

long long ClientData::get_msg_num()
{
    return msg_num++;
}

sockaddr* ClientData::get_address()
{
    return (sockaddr*)&address;
}

bool ClientData::set_game_session(GameSession *p_session)
{
    if(!p_session || game_session) {
        return false;
    }
    if(p_session->set_player(this)) {
        game_session = p_session;
        return true;
    }
    else {
        return false;
    }
}

void ClientData::disconnect_from_game_session()
{
    NULL_PTR_CHECK_RETURN(game_session)

    game_session->disconnect_player(this);
    game_session = 0;
}

int ClientData::get_fd() const
{
    return fd;
}

void ClientData::set_fd(int p_fd)
{
    fd = p_fd;
}

bool ClientData::ready_write() const
{
    return to_send_msg.get_size();
}

BuffError ClientData::read_msg()
{
    if(fd == -1) {
        return INVALID_FD;
    }

    BuffError err = buf.read_msg(fd);
    if(err == BUFF_OK) {
        Message *msg;
        while((msg = buf.extract())) {
            to_process_msg.put(msg);
        }
    }
    else if(err == CONN_BROKEN) {
        disconnect_from_game_session();
        return CONN_BROKEN;
    }
    return BUFF_OK;
}

void ClientData::write_msg()
{
    if(fd == -1) {
        return;
    }

    int res = 0;
    int msg_size = 0;
    Message *msg = 0;
    WRITE_LOG->print("ClientData::write_msg() count = %d", to_send_msg.get_size());
    while(to_send_msg.get_size()) {
        msg = to_send_msg.get();
        MsgType type = msg->get_type();
        if(type == INVALID_MSG || type == UNDEFINED_MSG) {
            WRITE_LOG->print("Detected invalid msg which will be deleted");
            delete msg;
            msg = 0;
            continue;
        }
        if(msg->id == Message::NO_ID) {
            msg->id = get_msg_num();
        }
        char *data = msg->get_data();
        msg_size = Message::get_type_data_size(type);
        res = write(fd, data, msg_size);
        if(res == msg_size) {
            if(msg->type == RECEIPT_MSG) {
                delete msg;
                msg = 0;
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
            to_send_msg.put(msg);
            delete data;
            data = 0;
            break;
        }
        delete data;
        data = 0;
        WRITE_LOG->print("writed in client = %d", res);
    }

    delivery_control.resend(to_send_msg);
}

void ClientData::process_msg()
{
    WRITE_LOG->print("ClientData::process_msg");
    Message *msg = 0;
    while((msg = to_process_msg.get())) {
        switch(msg->type) {
        case UNDEFINED_MSG:
        case INVALID_MSG:
            WRITE_LOG->print("Invalid msg");
            Message::remove(msg);
            continue;
        case GAME_SESSION_REQ_MSG:
            _process_game_session_req_msg(static_cast<GameSessionReqMsg*>(msg));
            break;
        case GAME_FIELD_MSG:
            _process_game_field_msg(static_cast<GameFieldMsg*>(msg));
            break;
        case GAME_MOVE_MSG:
            _process_game_move_msg(static_cast<GameMoveMsg*>(msg));
            break;
        case GAME_STATE_MSG:
        case START_FIELD_REQ_MSG:
        case SERVER_CONN_STATUS_REQ_MSG:
        case SERVER_CONN_STATUS_MSG:
        case SERVER_CONN_REQ_MSG:
        case STOP_PREPARATION_MSG:
        case START_TURN_MSG:
        case STOP_TURN_MSG:
        case INFO_MSG:
        case ERROR_MSG:
            WRITE_LOG->print("Unsupported msg with type = %d", msg->type);
            break;
        case RECEIPT_MSG:
            _process_receipt_msg(static_cast<ReceiptMsg*>(msg));
            break;
        }

        if(msg->type != RECEIPT_MSG) {
            _send_receipt(msg->id);
        }
        Message::remove(msg);
    }
}

void ClientData::_send_receipt(long long id)
{
    ReceiptMsg *msg = new ReceiptMsg(id);
    send_msg(msg);
}

void ClientData::send_msg(Message *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    to_send_msg.put(msg);
}

void ClientData::_process_game_session_req_msg(const GameSessionReqMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    GameState result;
    if(game_session) {
        result = game_session->get_game_state();
    }
    else if(server->connect_player_to_game(this)) {
        result = game_session->get_game_state();
    }
    else {
        result = NO_VACANT_GAME;
    }

    if(result == WAIT_PREPARATION) {
        game_session->start_preparation();
    }
    else {
        GameStateMsg *answ = new GameStateMsg(result);
        to_send_msg.put(answ);
    }
}

void ClientData::_process_game_field_msg(const GameFieldMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    if(!game_session) {
        ErrorMsg *answ = new ErrorMsg("You are not in the game");
        to_send_msg.put(answ);
    }
    else {
        if(!msg->field.is_valid_start_field()) {
            ErrorMsg *answ = new ErrorMsg("Invalid game field. Correct it!");
            to_send_msg.put(answ);
        }
        else if(game_session->set_field(this, msg->field)) {
            ReceiptMsg *answ = new ReceiptMsg(msg->id);
            to_send_msg.put(answ);
        }
        else {
            ErrorMsg *answ = new ErrorMsg("Can't set the game field");
            to_send_msg.put(answ);
        }
    }
}

void ClientData::_process_game_move_msg(const GameMoveMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    if(!game_session) {
        ErrorMsg *answ = new ErrorMsg("You are not in the game");
        to_send_msg.put(answ);
    }
    else {
        GameState state = game_session->get_game_state();
        if(state == GAME_TURN || state == PROCESS_TURN) {
            if(game_session->set_last_shot(msg->point, this)) {
                game_session->end_turn();
            }
        }
    }
}

void ClientData::_process_receipt_msg(const ReceiptMsg *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    if(!delivery_control.process_check(msg)) {
        WRITE_LOG->print("NO MSG with #%lld type = %d, id = %d",
                         msg->req_msg_id, msg->type, msg->id);
    }
}
