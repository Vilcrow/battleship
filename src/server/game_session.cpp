#include "game_session.h"
#include "bs_server.h"
#include "tools/write_log.h"
#include "preparation_timeout.h"
#include "random.h"
#include <stdlib.h>

bool GameSession::is_valid_player_num(int num)
{
    if(num >= FIRST_PLAYER && num < PLAYERS_COUNT) {
        return true;
    }
    return false;
}

GameSession::GameSession(BattleshipServer *p_server)
    : server(p_server)
    , state(NO_VACANT_GAME)
    , cur_turn_player_idx(FIRST_PLAYER)
    , last_shot(-1, -1)
    , turn_timeout(0)
{
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        players[i] = 0;
        fields[i] = GameField();
    }
}

GameSession::~GameSession()
{

}

GameState GameSession::get_game_state() const
{
    return state;
}

void GameSession::set_game_state(GameState p_state)
{
    state = p_state;
}

bool GameSession::set_player(ClientData *player)
{
    if(!player || !_is_vacant() || _has_player(player)) {
        return false;
    }

    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i] == 0) {
            players[i] = player;
            if(state == NO_VACANT_GAME) {
                state = WAIT_PLAYER;
            }
            else {
                state = WAIT_PREPARATION;
            }
            return true;
        }
    }
    return false;
}

void GameSession::disconnect_player(const ClientData *player)
{
    NULL_PTR_CHECK_RETURN(player)

    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i] == player) {
            players[i] = 0;
            if(state != WAIT_PLAYER) {
                state = WAIT_PLAYER;
            }
            else {
                state = NO_VACANT_GAME;
            }
            return;
        }
    }
    send_game_state_for_all();
}

bool GameSession::set_field(PlayerNumber num, const GameField &p_field)
{
    if(!is_valid_player_num(num)) {
        return false;
    }

    fields[num] = p_field;
    return true;
}

bool GameSession::set_field(ClientData *player, const GameField &p_field)
{
    if(!player || !_has_player(player) ||
            (state != PREPARATION && state != WAIT_START)) {
        return false;
    }

    int i = _get_player_index(player);
    fields[i] = p_field;
    return true;
}

GameField GameSession::get_field(PlayerNumber num, bool *ok) const
{
    if(!_has_player(num)) {
        if(ok) {
            *ok = false;
        }
        return GameField();
    }

    if(ok) {
        *ok = true;
    }
    return fields[num];
}

GameField GameSession::get_field(ClientData *player, bool own, bool *ok) const
{
    if(!_has_player(player)) {
        if(ok) {
            *ok = false;
        }
        return GameField();
    }

    if(ok) {
        *ok = true;
    }
    int i = _get_player_index(player, own);
    return fields[i];
}

GFCellState GameSession::process_move(ClientData *player, const Point &shot)
{
    int enemy_idx = _get_player_index(player, false);
    if(!_has_player(player) || !_has_player(enemy_idx)) {
        return GameField::CELL_EMPTY;
    }

    GFCellState ret = fields[enemy_idx].register_shot(shot);
    return ret;
}

void GameSession::send_for_all(Message *msg)
{
    NULL_PTR_CHECK_RETURN(msg)

    char *data = msg->get_data();
    MsgType type = msg->get_type();
    int size = msg->get_type_data_size(type);
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i]) {
            // Create copy of the message for all players.
            Message *msg_cp = Message::create_instance(data, size);
            players[i]->send_msg(msg_cp);
        }
    }
    delete data;
    Message::remove(msg);
}

void GameSession::start_preparation()
{
    WRITE_LOG->print("Start preparation");
    if(state != WAIT_PREPARATION) {
        return;
    }

    set_game_state(PREPARATION);
    time_t cur_time = time(NULL);
    time_t end_time = cur_time + PREPARATION_DURATION;
    StartFieldReqMsg *req_field_msg = new StartFieldReqMsg(end_time);
    send_for_all(req_field_msg);
    PreparationTimeout *pt = new PreparationTimeout(PREPARATION_DURATION, this);
    server->add_timeout_handler(pt);
}

void GameSession::stop_preparation()
{
    WRITE_LOG->print("Stop preparation");
    if(state != PREPARATION) {
        return;
    }

    StopPreparationMsg *msg = new StopPreparationMsg;
    send_for_all(msg);

    state = WAIT_START;
    CheckStartFieldsTimeout *ct = new CheckStartFieldsTimeout(WAIT_DURATION,
                                                              this);
    server->add_timeout_handler(ct);
    send_game_state_for_all();
}

void GameSession::check_start_fields()
{
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i]) {
            if(!(fields[i].is_valid_start_field())) {
                WRITE_LOG->print("Invalid field. Disconnect player");
                server->disconnect_client(players[i]);
            }
        }
    }

    int players_count = get_players_count();
    if(players_count == PLAYERS_COUNT) {
        send_start_fields();
        GameStartTimeout *start_timeout = new GameStartTimeout(WAIT_DURATION,
                                                               this);
        server->add_timeout_handler(start_timeout);
    }
    else if(players_count == 1) {
        state = WAIT_PLAYER;
        send_game_state_for_all();
    }
}

int GameSession::get_players_count() const
{
    int count = 0;
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i]) {
            ++count;
        }
    }
    return count;
}

int GameSession::_get_enemy_idx(int idx)
{
    if(idx == FIRST_PLAYER) {
        return SECOND_PLAYER;
    }
    else if(idx == SECOND_PLAYER) {
        return FIRST_PLAYER;
    }
    // Invalid argument.
    return PLAYERS_COUNT;
}

int GameSession::_get_first_turn_player_idx()
{
    return RANDOM->rand(2);
}

void GameSession::change_cur_turn_player_idx()
{
    cur_turn_player_idx = _get_enemy_idx(cur_turn_player_idx);
}

void GameSession::start_game()
{
    if(state != WAIT_START) {
        return;
    }

    cur_turn_player_idx = _get_first_turn_player_idx();
    _start_new_turn();
}

void GameSession::_start_new_turn()
{
    if(state != WAIT_START && state != PROCESS_TURN) {
        return;
    }

    state = GAME_TURN;
    time_t cur_time = time(0);
    time_t end_time = cur_time + TURN_DURATION;
    StartTurnMsg *for_cur_player = new StartTurnMsg(end_time, true);
    players[cur_turn_player_idx]->send_msg(for_cur_player);

    int enemy_idx = _get_enemy_idx(cur_turn_player_idx);
    StartTurnMsg *for_enemy = new StartTurnMsg(end_time, false);
    players[enemy_idx]->send_msg(for_enemy);

    GameTurnTimeout *timeout = new GameTurnTimeout(TURN_DURATION, this);
    server->add_timeout_handler(timeout);
    turn_timeout = timeout;
}

void GameSession::send_start_fields()
{
    if(state != WAIT_START) {
        return;
    }

    for(int i = 0; i < PLAYERS_COUNT; i++) {
        int enemy_idx = _get_enemy_idx(i);
        GameField own_field = fields[i];
        GameField enemy_field = fields[enemy_idx].get_for_enemy();
        GameFieldMsg *own_field_msg =
                new GameFieldMsg(own_field, true);
        GameFieldMsg *enemy_field_msg =
                new GameFieldMsg(enemy_field, false);
        players[i]->send_msg(own_field_msg);
        players[i]->send_msg(enemy_field_msg);
    }
}

void GameSession::end_turn()
{
    if(state != GAME_TURN) {
        return;
    }

    state = PROCESS_TURN;

    StopTurnMsg *for_cur_player = new StopTurnMsg(true);
    players[cur_turn_player_idx]->send_msg(for_cur_player);

    int enemy_idx = _get_enemy_idx(cur_turn_player_idx);
    StopTurnMsg *for_enemy = new StopTurnMsg(false);
    players[enemy_idx]->send_msg(for_enemy);

    ChangeTurnTimeout *timeout = new ChangeTurnTimeout(WAIT_DURATION, this);
    server->add_timeout_handler(timeout);

    if(turn_timeout) {
        turn_timeout->stop();
        turn_timeout = 0;
    }
}

void GameSession::change_turn_player()
{
    if(state != PROCESS_TURN) {
        return;
    }

    GFCellState shot_res = _register_shot();
    _send_fields();

    if(_is_game_over()) {
        _send_game_result();
        EndGameTimeout *timeout = new EndGameTimeout(WAIT_DURATION, this);
        server->add_timeout_handler(timeout);
        return;
    }

    if(shot_res != GameField::CELL_DAMAGED) {
        change_cur_turn_player_idx();
    }
    _start_new_turn();
}

void GameSession::_send_game_result()
{
    GameStateMsg *for_winner = new GameStateMsg(WIN_GAME);
    players[cur_turn_player_idx]->send_msg(for_winner);
    GameStateMsg *for_loser = new GameStateMsg(LOSE_GAME);
    int enemy_idx = _get_enemy_idx(cur_turn_player_idx);
    players[enemy_idx]->send_msg(for_loser);
}

GFCellState GameSession::_register_shot()
{
    int enemy_idx = _get_enemy_idx(cur_turn_player_idx);
    GFCellState shot_res = GameField::CELL_EMPTY;
    if(!has_last_shot()) {
        int x = RANDOM->rand(10);
        int y = RANDOM->rand(10);
        last_shot = Point(x, y);
    }
    while(shot_res == GameField::CELL_EMPTY) {
        shot_res = fields[enemy_idx].register_shot(last_shot);
        int x = RANDOM->rand(10);
        int y = RANDOM->rand(10);
        last_shot = Point(x, y);
    }
    clear_last_shot();
    return shot_res;
}

bool GameSession::_is_game_over() const
{
    int enemy_idx = _get_enemy_idx(cur_turn_player_idx);
    return fields[enemy_idx].all_ships_destroyed();
}

void GameSession::_send_fields()
{
    int enemy_idx = _get_enemy_idx(cur_turn_player_idx);
    GameField enemy_field = fields[enemy_idx];
    GameFieldMsg *field_msg =
            new GameFieldMsg(enemy_field.get_for_enemy(), false);
    GameFieldMsg *field_msg_for_enemy =
            new GameFieldMsg(enemy_field, true);
    players[cur_turn_player_idx]->send_msg(field_msg);
    players[enemy_idx]->send_msg(field_msg_for_enemy);

}

bool GameSession::set_last_shot(const Point &shot, const ClientData *player)
{
    int idx = _get_player_index(player);
    if(idx == cur_turn_player_idx && !has_last_shot()) {
        last_shot = shot;
        return true;
    }
    return false;
}

const Point& GameSession::get_last_shot() const
{
    return last_shot;
}

bool GameSession::has_last_shot() const
{
    return last_shot != Point(-1, -1);
}

void GameSession::clear_last_shot()
{
    last_shot = Point(-1, -1);
}

void GameSession::end_game()
{
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        server->disconnect_client(players[i]);
    }
}

void GameSession::send_game_state_for_all()
{
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i]) {
            GameStateMsg *msg = new GameStateMsg(state);
            players[i]->send_msg(msg);
        }
    }
}

bool GameSession::_is_vacant() const
{
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i] == 0) {
            return true;
        }
    }
    return false;
}

bool GameSession::_has_player(int num) const
{
    if(!is_valid_player_num(num)) {
        return false;
    }
    return players[num] != 0;
}

bool GameSession::_has_player(const ClientData *player) const
{
    if(!player) {
        return false;
    }
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i] == player) {
            return true;
        }
    }
    return false;
}

int GameSession::_get_player_index(const ClientData *player, bool own) const
{
    int idx = -1;
    for(int i = 0; i < PLAYERS_COUNT; i++) {
        if(players[i] == player) {
            idx = i;
            break;
        }
    }

    if(!own && idx != -1) {
        idx = (idx == FIRST_PLAYER) ? SECOND_PLAYER : FIRST_PLAYER;
    }
    return idx;
}
