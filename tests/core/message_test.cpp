#include "CppUTest/TestHarness.h"
#include "const.h"
#include "message.h"


///////////////////////////////////////////////////////////// GameStateMsg BEGIN

TEST_GROUP(GameStateMsgGroup)
{
    int id = 421;
    GameState state = GAME_TURN;
    MsgType type = GAME_STATE_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    GameStateMsg *msg = 0;
    char *data = 0;

    GameStateMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new GameStateMsg(state);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<GameStateMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(GameStateMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(GameStateMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(state, msg->state);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(state, msg_by_data->state);
}

/////////////////////////////////////////////////////////////// GameStateMsg END


//////////////////////////////////////////////////////// GameSessionReqMsg BEGIN

TEST_GROUP(GameSessionReqMsgGroup)
{
    int id = 4351;
    MsgType type = GAME_SESSION_REQ_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    GameSessionReqMsg *msg = 0;
    char *data = 0;

    GameSessionReqMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new GameSessionReqMsg();
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<GameSessionReqMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(GameSessionReqMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(GameSessionReqMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
}

////////////////////////////////////////////////////////// GameSessionReqMsg END


/////////////////////////////////////////////////////////////// ReceiptMsg BEGIN

TEST_GROUP(ReceiptMsgGroup)
{
    int id = Message::NO_REQ_ID;
    int req_id = 432;
    MsgType type = RECEIPT_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    ReceiptMsg *msg = 0;
    char *data = 0;

    ReceiptMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new ReceiptMsg(req_id);
        data = msg->get_data();

        msg_by_data = static_cast<ReceiptMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(ReceiptMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(ReceiptMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(req_id, msg->req_msg_id);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(req_id, msg_by_data->req_msg_id);
}

///////////////////////////////////////////////////////////////// ReceiptMsg END


///////////////////////////////////////////////////////////// GameFieldMsg BEGIN

TEST_GROUP(GameFieldMsgGroup)
{
    int id = 331;
    GameField field;
    bool is_own = false;
    MsgType type = GAME_FIELD_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    GameFieldMsg *msg = 0;
    char *data = 0;

    GameFieldMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new GameFieldMsg(field, is_own);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<GameFieldMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(GameFieldMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(GameFieldMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(is_own, msg->is_own_field);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(is_own, msg_by_data->is_own_field);
}

/////////////////////////////////////////////////////////////// GameFieldMsg END


///////////////////////////////////////////////////////// StartFieldReqMsg BEGIN

TEST_GROUP(StartFieldReqMsgGroup)
{
    int id = 43451;
    time_t end_time = 43221;
    MsgType type = START_FIELD_REQ_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    StartFieldReqMsg *msg = 0;
    char *data = 0;

    StartFieldReqMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new StartFieldReqMsg(end_time);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<StartFieldReqMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(StartFieldReqMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(StartFieldReqMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(end_time, msg->end_time);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(end_time, msg_by_data->end_time);
}

/////////////////////////////////////////////////////////// StartFieldReqMsg END


/////////////////////////////////////////////////////// StopPreparationMsg BEGIN

TEST_GROUP(StopPreparationMsgGroup)
{
    int id = 9654;
    MsgType type = STOP_PREPARATION_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    StopPreparationMsg *msg = 0;
    char *data = 0;

    StopPreparationMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new StopPreparationMsg();
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<StopPreparationMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(StopPreparationMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(StopPreparationMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
}

///////////////////////////////////////////////////////// StopPreparationMsg END


///////////////////////////////////////////////////////////// StartTurnMsg BEGIN

TEST_GROUP(StartTurnMsgGroup)
{
    int id = 34351;
    time_t end_time = 95224;
    MsgType type = START_TURN_MSG;
    bool is_own = true;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    StartTurnMsg *msg = 0;
    char *data = 0;

    StartTurnMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new StartTurnMsg(end_time, is_own);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<StartTurnMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(StartTurnMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(StartTurnMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(end_time, msg->end_time);
    CHECK_EQUAL(is_own, msg->is_own);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(end_time, msg_by_data->end_time);
    CHECK_EQUAL(is_own, msg->is_own);
}

/////////////////////////////////////////////////////////////// StartTurnMsg END


////////////////////////////////////////////////////////////// StopTurnMsg BEGIN

TEST_GROUP(StopTurnMsgGroup)
{
    int id = 44555;
    MsgType type = STOP_TURN_MSG;
    bool is_own = true;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    StopTurnMsg *msg = 0;
    char *data = 0;

    StopTurnMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new StopTurnMsg(is_own);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<StopTurnMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(StopTurnMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(StopTurnMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(is_own, msg->is_own);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(is_own, msg->is_own);
}

//////////////////////////////////////////////////////////////// StopTurnMsg END


////////////////////////////////////////////////////////////// GameMoveMsg BEGIN

TEST_GROUP(GameMoveMsgGroup)
{
    int id = 14455;
    MsgType type = GAME_MOVE_MSG;
    Point point = Point(4, 8);

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    GameMoveMsg *msg = 0;
    char *data = 0;

    GameMoveMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new GameMoveMsg(point);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<GameMoveMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(GameMoveMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(GameMoveMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(point.x, msg->point.x);
    CHECK_EQUAL(point.y, msg->point.y);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(point.x, msg->point.x);
    CHECK_EQUAL(point.y, msg->point.y);
}

//////////////////////////////////////////////////////////////// GameMoveMsg END


/////////////////////////////////////////////////// ServerConnStatusReqMsg BEGIN

TEST_GROUP(ServerConnStatusReqMsgGroup)
{
    int id = 23551;
    MsgType type = SERVER_CONN_STATUS_REQ_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    ServerConnStatusReqMsg *msg = 0;
    char *data = 0;

    ServerConnStatusReqMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new ServerConnStatusReqMsg();
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<ServerConnStatusReqMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(ServerConnStatusReqMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(ServerConnStatusReqMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
}

///////////////////////////////////////////////////// ServerConnStatusReqMsg END


////////////////////////////////////////////////////// ServerConnStatusMsg BEGIN

TEST_GROUP(ServerConnStatusMsgGroup)
{
    int id = 1421;
    bool connected = true;
    MsgType type = SERVER_CONN_STATUS_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    ServerConnStatusMsg *msg = 0;
    char *data = 0;

    ServerConnStatusMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new ServerConnStatusMsg(connected);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<ServerConnStatusMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(ServerConnStatusMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(ServerConnStatusMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    CHECK_EQUAL(connected, msg->connected);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    CHECK_EQUAL(connected, msg_by_data->connected);
}

//////////////////////////////////////////////////////// ServerConnStatusMsg END


///////////////////////////////////////////////////////// ServerConnReqMsg BEGIN

TEST_GROUP(ServerConnReqMsgGroup)
{
    int id = 35159;
    MsgType type = SERVER_CONN_REQ_MSG;

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    ServerConnReqMsg *msg = 0;
    char *data = 0;

    ServerConnReqMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new ServerConnReqMsg();
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<ServerConnReqMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(ServerConnReqMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(ServerConnReqMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
}

/////////////////////////////////////////////////////////// ServerConnReqMsg END


///////////////////////////////////////////////////////////////// ErrorMsg BEGIN

TEST_GROUP(ErrorMsgGroup)
{
    int id = 1421;
    MsgType type = ERROR_MSG;
    const char *msg_txt = "Hello, World! It is error!";

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    ErrorMsg *msg = 0;
    char *data = 0;

    ErrorMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new ErrorMsg(msg_txt);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<ErrorMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(ErrorMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(ErrorMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    STRCMP_EQUAL(msg_txt, msg->get_error());

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    STRCMP_EQUAL(msg_txt, msg_by_data->get_error());
}

/////////////////////////////////////////////////////////////////// ErrorMsg END


////////////////////////////////////////////////////////////////// InfoMsg BEGIN

TEST_GROUP(InfoMsgGroup)
{
    int id = 1421;
    MsgType type = INFO_MSG;
    const char *msg_txt = "Hello, World! It is info message.";

    int msg_sz = Message::get_type_size(type);
    int data_sz = Message::get_type_data_size(type);

    InfoMsg *msg = 0;
    char *data = 0;

    InfoMsg *msg_by_data = 0;
    char *data_other = 0;

    void setup()
    {
        msg = new InfoMsg(msg_txt);
        CHECK_EQUAL(Message::NO_ID, msg->id);
        msg->id = id;
        data = msg->get_data();

        msg_by_data = static_cast<InfoMsg*>(Message::create_instance(data, data_sz));
        CHECK(msg_by_data);
        data_other = msg_by_data->get_data();
    }
    void teardown()
    {
        Message::remove(msg);
        Message::remove(msg_by_data);

        delete[] data;
        delete[] data_other;
    }
};

TEST(InfoMsgGroup, StaticMethods)
{
    CHECK_FALSE(Message::is_too_small(msg_sz));
    CHECK_FALSE(Message::is_too_small(data_sz));

    CHECK(msg_sz >= Message::min_size());
    CHECK(data_sz >= Message::min_size());

    CHECK_EQUAL(id, Message::get_id(data));

    CHECK_EQUAL(type, Message::get_type(data, data_sz));

    MEMCMP_EQUAL(data, data_other, data_sz);
}

TEST(InfoMsgGroup, GetData)
{
    CHECK_EQUAL(type, msg->get_type());
    CHECK_EQUAL(id, msg->id);
    STRCMP_EQUAL(msg_txt, msg->get_info());

    CHECK_EQUAL(type, msg_by_data->get_type());
    CHECK_EQUAL(id, msg_by_data->id);
    STRCMP_EQUAL(msg_txt, msg_by_data->get_info());
}

//////////////////////////////////////////////////////////////////// InfoMsg END
