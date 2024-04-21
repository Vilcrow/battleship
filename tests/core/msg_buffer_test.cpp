#include "CppUTest/TestHarness.h"
#include <unistd.h>
#include "msg_buffer.h"

TEST_GROUP(MsgBufferGroup)
{
    MsgBuffer *buffer = 0;
    void setup()
    {
        buffer = new MsgBuffer;
    }
    void teardown()
    {
        delete buffer;
    }
};

TEST(MsgBufferGroup, EmptyBuffer)
{
    CHECK(buffer->is_empty());
    CHECK_FALSE(buffer->extract());
}

TEST(MsgBufferGroup, ReadMessage)
{
    Message *msg = 0;
    int fd[2];
    pipe(fd);

    char *data = 0;
    int size;

    // The GameState message.
    GameState state = ENEMY_TURN;
    int id_msg_state = 343;
    GameStateMsg msg_state(state);
    msg_state.id = id_msg_state;

    data = msg_state.get_data();
    size = msg_state.get_data_size();
    write(fd[1], data, size);
    delete[] data;

    buffer->read_msg(fd[0]);
    msg = buffer->extract();
    CHECK(msg);
    CHECK_EQUAL(GAME_STATE_MSG, msg->get_type());
    CHECK_EQUAL(id_msg_state, msg->id);
    CHECK(static_cast<GameStateMsg*>(msg)->state == state);
    Message::remove(msg);

    // The invalid message.
    char data_invalid[100];
    write(fd[1], data_invalid, sizeof(data_invalid));
    buffer->read_msg(fd[0]);
    CHECK_FALSE(buffer->extract());

    // The GameMove message.
    Point point(4, 2);
    int id_msg_move = 4311;
    GameMoveMsg msg_move(point);
    msg_move.id = id_msg_move;

    data = msg_move.get_data();
    size = msg_move.get_data_size();
    write(fd[1], data, size);
    delete[] data;

    buffer->read_msg(fd[0]);
    msg = buffer->extract();
    CHECK(msg);
    CHECK_EQUAL(GAME_MOVE_MSG, msg->get_type());
    CHECK_EQUAL(id_msg_move, msg->id);
    CHECK(static_cast<GameMoveMsg*>(msg)->point == point);
    Message::remove(msg);


    CHECK_FALSE(buffer->extract());
    CHECK(buffer->is_empty());

    close(fd[0]);
    close(fd[1]);
}
