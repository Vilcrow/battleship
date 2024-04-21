#include "preparation_timeout.h"
#include "game_session.h"

PreparationTimeout::PreparationTimeout(long p_sec, GameSession *p_session)
    : TimeoutHandler(p_sec, 0)
    , session(p_session)
{

}

PreparationTimeout::~PreparationTimeout()
{

}

void PreparationTimeout::timeoutEvent()
{
    if(session) {
        session->stop_preparation();
    }
}

CheckStartFieldsTimeout::CheckStartFieldsTimeout(long p_sec,
                                                 GameSession *p_session)
    : TimeoutHandler(p_sec, 0)
    , session(p_session)
{

}

CheckStartFieldsTimeout::~CheckStartFieldsTimeout()
{

}

void CheckStartFieldsTimeout::timeoutEvent()
{
    if(session) {
        session->check_start_fields();
    }
}

GameStartTimeout::GameStartTimeout(long p_sec, GameSession *p_session)
    : TimeoutHandler(p_sec, 0)
    , session(p_session)
{

}

GameStartTimeout::~GameStartTimeout()
{

}

void GameStartTimeout::timeoutEvent()
{
    if(session) {
        session->start_game();
    }
}

GameTurnTimeout::GameTurnTimeout(long p_sec, GameSession *p_session)
    : TimeoutHandler(p_sec, 0)
    , session(p_session)
{

}

GameTurnTimeout::~GameTurnTimeout()
{

}

void GameTurnTimeout::timeoutEvent()
{
    if(session) {
        session->end_turn();
    }
}

ChangeTurnTimeout::ChangeTurnTimeout(long p_sec, GameSession *p_session)
    : TimeoutHandler(p_sec, 0)
    , session(p_session)
{

}

ChangeTurnTimeout::~ChangeTurnTimeout()
{

}

void ChangeTurnTimeout::timeoutEvent()
{
    if(session) {
        session->change_turn_player();
    }
}

EndGameTimeout::EndGameTimeout(long p_sec, GameSession *p_session)
    : TimeoutHandler(p_sec, 0)
    , session(p_session)
{

}

EndGameTimeout::~EndGameTimeout()
{

}

void EndGameTimeout::timeoutEvent()
{
    if(session) {
        session->end_game();
    }
}
