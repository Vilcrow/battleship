#include "message.h"
#include "tools/write_log.h"
#include <string.h>
#include <stdio.h>

int Message::get_type_size(MsgType type)
{
    int ret = -1;
    switch(type) {
    case INVALID_MSG:
    case UNDEFINED_MSG:
        break;
    case GAME_STATE_MSG:
        ret = sizeof(GameStateMsg);
        break;
    case GAME_SESSION_REQ_MSG:
        ret = sizeof(GameSessionReqMsg);
        break;
    case GAME_FIELD_MSG:
        ret = sizeof(GameFieldMsg);
        break;
    case GAME_MOVE_MSG:
        ret = sizeof(GameMoveMsg);
        break;
    case SERVER_CONN_STATUS_REQ_MSG:
        ret = sizeof(ServerConnStatusReqMsg);
        break;
    case SERVER_CONN_STATUS_MSG:
        ret = sizeof(ServerConnStatusMsg);
        break;
    case SERVER_CONN_REQ_MSG:
        ret = sizeof(ServerConnReqMsg);
        break;
    case START_FIELD_REQ_MSG:
        ret = sizeof(StartFieldReqMsg);
        break;
    case STOP_PREPARATION_MSG:
        ret = sizeof(StopPreparationMsg);
        break;
    case START_TURN_MSG:
        ret = sizeof(StartTurnMsg);
        break;
    case STOP_TURN_MSG:
        ret = sizeof(StopTurnMsg);
        break;
    case RECEIPT_MSG:
        ret = sizeof(ReceiptMsg);
        break;
    case INFO_MSG:
        ret = sizeof(InfoMsg);
        break;
    case ERROR_MSG:
        ret = sizeof(ErrorMsg);
        break;
    }
    return ret;
}

int Message::get_type_data_size(MsgType type)
{
    int ret = 0;
    switch(type) {
    case INVALID_MSG:
    case UNDEFINED_MSG:
        break;
    case GAME_STATE_MSG:
        ret = GameStateMsg::get_data_size();
        break;
    case GAME_SESSION_REQ_MSG:
        ret = GameSessionReqMsg::get_data_size();
        break;
    case GAME_FIELD_MSG:
        ret = GameFieldMsg::get_data_size();
        break;
    case GAME_MOVE_MSG:
        ret = GameMoveMsg::get_data_size();
        break;
    case SERVER_CONN_STATUS_REQ_MSG:
        ret = ServerConnStatusReqMsg::get_data_size();
        break;
    case SERVER_CONN_STATUS_MSG:
        ret = ServerConnStatusMsg::get_data_size();
        break;
    case SERVER_CONN_REQ_MSG:
        ret = ServerConnReqMsg::get_data_size();
        break;
    case START_FIELD_REQ_MSG:
        ret = StartFieldReqMsg::get_data_size();
        break;
    case STOP_PREPARATION_MSG:
        ret = StopPreparationMsg::get_data_size();
        break;
    case START_TURN_MSG:
        ret = StartTurnMsg::get_data_size();
        break;
    case STOP_TURN_MSG:
        ret = StopTurnMsg::get_data_size();
        break;
    case RECEIPT_MSG:
        ret = ReceiptMsg::get_data_size();
        break;
    case INFO_MSG:
        ret = InfoMsg::get_data_size();
        break;
    case ERROR_MSG:
        ret = ErrorMsg::get_data_size();
        break;
    }
    return ret;
}

Message* Message::create_instance(const char *data, int size)
{
    if(!data || is_too_small(size)) {
        return 0;
    }

    unsigned char *type_p = (unsigned char*)data;
    ++type_p;
    MsgType type = *((MsgType*)type_p);
    Message *ret = 0;
    switch(type) {
    case INVALID_MSG:
    case UNDEFINED_MSG:
        break;
    case GAME_STATE_MSG:
        ret = GameStateMsg::create(data, size);
        break;
    case GAME_SESSION_REQ_MSG:
        ret = GameSessionReqMsg::create(data, size);
        break;
    case GAME_FIELD_MSG:
        ret = GameFieldMsg::create(data, size);
        break;
    case GAME_MOVE_MSG:
        ret = GameMoveMsg::create(data, size);
        break;
    case SERVER_CONN_STATUS_REQ_MSG:
        ret = ServerConnStatusReqMsg::create(data, size);
        break;
    case SERVER_CONN_STATUS_MSG:
        ret = ServerConnStatusMsg::create(data, size);
        break;
    case SERVER_CONN_REQ_MSG:
        ret = ServerConnReqMsg::create(data, size);
        break;
    case START_FIELD_REQ_MSG:
        ret = StartFieldReqMsg::create(data, size);
        break;
    case STOP_PREPARATION_MSG:
        ret = StopPreparationMsg::create(data, size);
        break;
    case START_TURN_MSG:
        ret = StartTurnMsg::create(data, size);
        break;
    case STOP_TURN_MSG:
        ret = StopTurnMsg::create(data, size);
        break;
    case RECEIPT_MSG:
        ret = ReceiptMsg::create(data, size);
        break;
    case INFO_MSG:
        ret = InfoMsg::create(data, size);
        break;
    case ERROR_MSG:
        ret = ErrorMsg::create(data, size);
        break;
    }
    return ret;
}

Message::Message(MsgType p_type)
    : start_mark(MSG_START)
    , type(p_type)
    , id(NO_ID)
{

}

Message::~Message()
{

}

MsgType Message::get_type() const
{
    return type;
}

int Message::get_size() const
{
    return get_type_size(type);
}

bool Message::is_too_small(int size)
{
    if(size < min_size()) {
        return true;
    }
    return false;
}

int Message::min_size()
{
    return sizeof(unsigned char) + sizeof(MsgType);
}

void Message::remove(Message *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    delete msg;
}

int Message::get_start_offset(const char *p_data)
{
    unsigned char *smp = (unsigned char*)p_data;
    ++smp;
    MsgType *mtp = (MsgType*)smp;
    ++mtp;
    long long *idp = (long long*)mtp;
    ++idp;
    return (char*)idp - p_data;
}

MsgType Message::get_type(const char *data, int size)
{
    MsgType ret = INVALID_MSG;
    if(!data || is_too_small(size)) {
        return ret;
    }
    unsigned char *tmp = (unsigned char*)data;
    ++tmp;
    MsgType type = *((MsgType*)tmp);
    switch(type) {
    case UNDEFINED_MSG:
    case INVALID_MSG:
        break;
    case GAME_STATE_MSG:
    case GAME_SESSION_REQ_MSG:
    case GAME_FIELD_MSG:
    case GAME_MOVE_MSG:
    case SERVER_CONN_STATUS_REQ_MSG:
    case SERVER_CONN_STATUS_MSG:
    case SERVER_CONN_REQ_MSG:
    case START_FIELD_REQ_MSG:
    case STOP_PREPARATION_MSG:
    case START_TURN_MSG:
    case STOP_TURN_MSG:
    case RECEIPT_MSG:
    case INFO_MSG:
    case ERROR_MSG:
        ret = type;
    }
    return ret;
}

int Message::get_data_size()
{
    int ret;
    ret = sizeof(unsigned char) + sizeof(MsgType) + sizeof(long long);
    return ret;
}

char* Message::set_header_data(char *r_data) const
{
    // TODO: Replace with the memcpy everywhere?
    unsigned char *smp = (unsigned char*)r_data;
    memcpy(smp, &start_mark, sizeof(start_mark));
    ++smp;
    MsgType *mtp = (MsgType*)smp;
    memcpy(mtp, &type, sizeof(type));
    ++mtp;
    long long *idp = (long long*)mtp;
    memcpy(idp, &id, sizeof(id));
    ++idp;
    return (char*)idp;
}

Message* Message::create(const char* /*data*/, int /*size*/)
{
    return 0;
}

long long Message::get_id(const char *data)
{
    unsigned char *smp = (unsigned char*)data;
    ++smp;
    MsgType *mtp = (MsgType*)smp;
    ++mtp;
    long long *idp = (long long*)mtp;
    return *idp;
}

ReceiptMsg::ReceiptMsg(long long p_req_id)
    : Message(RECEIPT_MSG)
    , req_msg_id(p_req_id)
{
    id = NO_REQ_ID;
}

ReceiptMsg::~ReceiptMsg()
{

}

int ReceiptMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(long long);
    return ret;
}

Message* ReceiptMsg::create(const char *data, int size)
{
    ReceiptMsg *ret = new ReceiptMsg(NO_ID);
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        memcpy(&ret->req_msg_id, data, sizeof(req_msg_id));
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* ReceiptMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    memcpy(tmp, &req_msg_id, sizeof(req_msg_id));
    return ret;
}

ErrorMsg::ErrorMsg(const char *p_msg)
    : Message(ERROR_MSG)
{
    memset(buf, 0, BUF_SIZE);
    strncpy(buf, p_msg, BUF_SIZE);
    buf[BUF_SIZE-1] = '\0';
}

ErrorMsg::~ErrorMsg()
{

}

const char* ErrorMsg::get_error() const
{
    return buf;
}

int ErrorMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += BUF_SIZE;
    return ret;
}

Message* ErrorMsg::create(const char *data, int size)
{
    ErrorMsg *ret = new ErrorMsg("");
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        memcpy(ret->buf, data, sizeof(ret->buf));
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* ErrorMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    memcpy(tmp, buf, sizeof(buf));
    return ret;
}

GameStateMsg::GameStateMsg(GameState p_state)
    : Message(GAME_STATE_MSG)
    , state(p_state)
{

}

GameStateMsg::~GameStateMsg()
{

}

int GameStateMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(GameState);
    return ret;
}

Message* GameStateMsg::create(const char *data, int size)
{
    GameStateMsg *ret = new GameStateMsg(NO_VACANT_GAME);
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        ret->state = *((GameState*)data);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* GameStateMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    memcpy(tmp, &state, sizeof(state));
    return ret;
}

GameSessionReqMsg::GameSessionReqMsg()
    : Message(GAME_SESSION_REQ_MSG)
{

}

GameSessionReqMsg::~GameSessionReqMsg()
{

}

int GameSessionReqMsg::get_data_size()
{
    return Message::get_data_size();
}

Message* GameSessionReqMsg::create(const char *data, int size)
{
    GameSessionReqMsg *ret = new GameSessionReqMsg;
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* GameSessionReqMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    set_header_data(ret);
    return ret;
}

GameFieldMsg::GameFieldMsg(const GameField &p_field,
                           bool p_is_own_field)
    : Message(GAME_FIELD_MSG)
    , field(p_field)
    , is_own_field(p_is_own_field)
{

}

GameFieldMsg::~GameFieldMsg()
{

}

int GameFieldMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(GameField) + sizeof(bool);
    return ret;
}

Message* GameFieldMsg::create(const char *data, int size)
{
    GameFieldMsg *ret = new GameFieldMsg(GameField(), NO_ID);
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        const GameField *gf_ptr = (GameField*)data;
        ret->field = *gf_ptr;
        ++gf_ptr;
        ret->is_own_field = *((bool*)gf_ptr);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* GameFieldMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    memcpy(tmp, &field, sizeof(field));
    GameField *gf_ptr = (GameField*)tmp;
    ++gf_ptr;
    bool *iof_ptr = (bool*)gf_ptr;
    memcpy(iof_ptr, &is_own_field, sizeof(bool));
    return ret;
}

GameMoveMsg::GameMoveMsg(const Point &p_point)
    : Message(GAME_MOVE_MSG)
    , point(p_point)
{

}

GameMoveMsg::~GameMoveMsg()
{

}

int GameMoveMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(Point);
    return ret;
}

Message* GameMoveMsg::create(const char *data, int size)
{
    GameMoveMsg *ret = new GameMoveMsg(Point());
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        ret->point = *((Point*)data);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* GameMoveMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    memcpy(tmp, &point, sizeof(point));
    return ret;
}

InfoMsg::InfoMsg(const char *p_msg)
    : Message(INFO_MSG)
{
    memset(buf, 0, BUF_SIZE);
    strncpy(buf, p_msg, BUF_SIZE);
    buf[BUF_SIZE-1] = '\0';
}

InfoMsg::~InfoMsg()
{

}

const char* InfoMsg::get_info() const
{
    return buf;
}

int InfoMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += BUF_SIZE;
    return ret;
}

Message* InfoMsg::create(const char *data, int size)
{
    InfoMsg *ret = new InfoMsg("");
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        memcpy(ret->buf, data, sizeof(ret->buf));
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* InfoMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    memcpy(tmp, buf, sizeof(buf));
    return ret;
}

ServerConnStatusReqMsg::ServerConnStatusReqMsg()
    : Message(SERVER_CONN_STATUS_REQ_MSG)
{

}

ServerConnStatusReqMsg::~ServerConnStatusReqMsg()
{

}

int ServerConnStatusReqMsg::get_data_size()
{
    return Message::get_data_size();
}

Message* ServerConnStatusReqMsg::create(const char *data, int size)
{
    ServerConnStatusReqMsg *ret = new ServerConnStatusReqMsg;
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* ServerConnStatusReqMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    set_header_data(ret);
    return ret;
}

ServerConnStatusMsg::ServerConnStatusMsg(bool p_connected)
    : Message(SERVER_CONN_STATUS_MSG)
    , connected(p_connected)
{

}

ServerConnStatusMsg::~ServerConnStatusMsg()
{

}

int ServerConnStatusMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(bool);
    return ret;
}

Message* ServerConnStatusMsg::create(const char *data, int size)
{
    ServerConnStatusMsg *ret = new ServerConnStatusMsg(false);
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        ret->connected = *((bool*)data);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* ServerConnStatusMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    bool *cop = (bool*)tmp;
    memcpy(cop, &connected, sizeof(connected));
    return ret;
}

ServerConnReqMsg::ServerConnReqMsg()
    : Message(SERVER_CONN_REQ_MSG)
{

}

ServerConnReqMsg::~ServerConnReqMsg()
{

}

int ServerConnReqMsg::get_data_size()
{
    return Message::get_data_size();
}

Message* ServerConnReqMsg::create(const char *data, int size)
{
    ServerConnReqMsg *ret = new ServerConnReqMsg;
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* ServerConnReqMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    set_header_data(ret);
    return ret;
}

StartFieldReqMsg::StartFieldReqMsg(time_t p_end_time)
    : Message(START_FIELD_REQ_MSG)
    , end_time(p_end_time)
{

}

StartFieldReqMsg::~StartFieldReqMsg()
{

}

int StartFieldReqMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(time_t);
    return ret;
}

Message* StartFieldReqMsg::create(const char *data, int size)
{
    StartFieldReqMsg *ret = new StartFieldReqMsg(0);
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        time_t *sp = (time_t*)data;
        ret->end_time = *sp;
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* StartFieldReqMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    memcpy(tmp, &end_time, sizeof(end_time));
    return ret;
}

StopPreparationMsg::StopPreparationMsg()
    : Message(STOP_PREPARATION_MSG)
{

}

StopPreparationMsg::~StopPreparationMsg()
{

}

int StopPreparationMsg::get_data_size()
{
    return Message::get_data_size();
}

Message* StopPreparationMsg::create(const char *data, int size)
{
    StopPreparationMsg *ret = new StopPreparationMsg;
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* StopPreparationMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    set_header_data(ret);
    return ret;
}

StartTurnMsg::StartTurnMsg(time_t p_end_time, bool p_is_own)
    : Message(START_TURN_MSG)
    , end_time(p_end_time)
    , is_own(p_is_own)
{

}

StartTurnMsg::~StartTurnMsg()
{

}

int StartTurnMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(time_t);
    ret += sizeof(bool);
    return ret;
}

Message* StartTurnMsg::create(const char *data, int size)
{

    StartTurnMsg *ret = new StartTurnMsg(0, true);
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        time_t *sp = (time_t*)data;
        ret->end_time = *sp;
        ++sp;
        bool *iop = (bool*)sp;
        ret->is_own = *iop;
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* StartTurnMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    time_t *tp = (time_t*)tmp;
    memcpy(tp, &end_time, sizeof(end_time));
    ++tp;
    bool *iop = (bool*)tp;
    memcpy(iop, &is_own, sizeof(is_own));
    return ret;
}

StopTurnMsg::StopTurnMsg(bool p_is_own)
    : Message(STOP_TURN_MSG)
    , is_own(p_is_own)
{

}

StopTurnMsg::~StopTurnMsg()
{

}

int StopTurnMsg::get_data_size()
{
    int ret = Message::get_data_size();
    ret += sizeof(bool);
    return ret;
}

Message* StopTurnMsg::create(const char *data, int size)
{
    StopTurnMsg *ret = new StopTurnMsg(0);
    if(size == ret->get_data_size()) {
        ret->id = get_id(data);
        data += get_start_offset(data);
        bool *iop = (bool*)data;
        ret->is_own = *iop;
    }
    else {
        Message::remove(ret);
        ret = 0;
    }
    return ret;
}

char* StopTurnMsg::get_data() const
{
    int size = get_data_size();
    char *ret = new char[size];
    char *tmp = set_header_data(ret);
    bool *iop = (bool*)tmp;
    memcpy(iop, &is_own, sizeof(is_own));
    return ret;
}
