#include "game_interface.h"
#include <ncurses.h>

GameInterface::GameInterface()
    : has_sever_conn(false)
    , own_field()
    , enemy_field()
    , state(NO_VACANT_GAME)
    , mode(IDLE_MODE)
    , last_shot(0, 0)
{

}

GameInterface::~GameInterface()
{

}

void GameInterface::set_game_state(GameState p_state)
{
    state = p_state;
}

void GameInterface::set_server_conn_state(bool connected)
{
    has_sever_conn = connected;
}

void GameInterface::set_own_field(const GameField &p_own_field)
{
    own_field = p_own_field;
}

const GameField& GameInterface::get_own_field() const
{
    return own_field;
}

void GameInterface::set_enemy_field(const GameField &p_enemy_field)
{
    enemy_field = p_enemy_field;
}

GameField GameInterface::get_enemy_field() const
{
    return enemy_field;
}

InterfaceMode GameInterface::get_mode() const
{
    return mode;
}

void GameInterface::set_mode(InterfaceMode p_mode)
{
    mode = p_mode;
}

const Point& GameInterface::get_last_shot() const
{
    return last_shot;
}

void GameInterface::set_last_shot(const Point &shot)
{
    last_shot = shot;
}
