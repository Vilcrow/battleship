#ifndef CONST_H
#define CONST_H

#define MAIN_SERVER_PORT 10463

#define NULL_PTR_CHECK_RETURN(p_name) \
{ \
    if(!p_name) { \
        WRITE_LOG->print("Passed null pointer: %s, %d", __FILE__, __LINE__); \
        return; \
    } \
}

#define NULL_PTR_CHECK_RET_ZERO(p_name) \
{ \
    if(!p_name) { \
        WRITE_LOG->print("Passed null pointer: %s, %d", __FILE__, __LINE__); \
        return 0; \
    } \
}

#ifndef MAX
#define MAX(a,b) (((a)<(b)) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

enum GameState {
    NO_VACANT_GAME,
    WAIT_PLAYER,
    WAIT_PREPARATION,
    PREPARATION,
    WAIT_START,
    GAME_TURN,
    PROCESS_TURN,
    OWN_TURN,
    ENEMY_TURN,
    WIN_GAME,
    LOSE_GAME
};

enum {
    TURN_DURATION = 15,
    PREPARATION_DURATION = 70,
    WAIT_DURATION = 2
};

enum MsgType {
    INVALID_MSG,
    GAME_STATE_MSG,
    GAME_SESSION_REQ_MSG,
    GAME_FIELD_MSG,
    START_FIELD_REQ_MSG,
    STOP_PREPARATION_MSG,
    START_TURN_MSG,
    STOP_TURN_MSG,
    GAME_MOVE_MSG,
    SERVER_CONN_STATUS_REQ_MSG,
    SERVER_CONN_STATUS_MSG,
    SERVER_CONN_REQ_MSG,
    RECEIPT_MSG,
    INFO_MSG,
    ERROR_MSG,
    UNDEFINED_MSG
};

enum InputResult {
    QUIT_IN,
    REQ_SERVER_CONN_STATUS_IN,
    REQ_SERVER_CONN_IN,
    REQ_GAME_SESSION_IN,
    PREPARE_FIELD_IN,
    CURSOR_MOVE_IN,
    SHOT_IN,
    INVALID_IN
};

enum BuffError {
    INVALID_FD,
    CONN_BROKEN,
    FULL_BUFF,
    EMPTY_BUFF,
    READ_ERR,
    TO_SMALL_MSG_ERR,
    INVALID_MSG_ERR,
    BUFF_OK
};

#endif
