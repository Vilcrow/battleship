#ifndef MESSAGE_H
#define MESSAGE_H

#include "const.h"
#include "game_field.h"
#include <time.h>

/**
 * @brief The base abstract class of message for the server
 * interaction with clients
 */
class Message {
public:
    enum {
        MSG_START = '`',
        NO_REQ_ID = -1,
        NO_ID = 0,
        START_REG_NUM = 1,
    };

    /**
     * @brief Returns size of the message by passed type
     * @param type type of the message
     */
    static int get_type_size(MsgType type);

    /**
     * @brief Returns size of the messages data(fields without the start mark
     * and the type)
     * @param type
     */
    static int get_type_data_size(MsgType type);

    /**
     * @brief Returns the minimum amount of data required to determine
     * the type of message
     */
    static int min_size();

    /**
     * @brief Returns true if the size is less than the minimum amount of data
     * required to determine the type of message
     */
    static bool is_too_small(int size);

    // TODO: there is no need the argument?
    /**
     * @brief Returns the offset size equal to the size of the header data
     * @param p_data the pointer to the message data
     */
    static int get_start_offset(const char *p_data);

    /**
     * @brief Returns the message type by the data and size
     * @param data the pointer to the message data
     * @param size size of the data
     * @return
     */
    static MsgType get_type(const char *data, int size);

    /**
     * @brief Creates the message in the dynamic memory and returns
     * pointer to it if passed data and size are valid, otherwise
     * returns 0
     * @param data the pointer to the message data
     * @param size size of the data
     */
    static Message* create_instance(const char *data, int size);

    /**
     * @brief Creates the message in the dynamic memory with a certain type.
     * Returns the pointer to it if passed data and size are valid, otherwise
     * returns 0
     * This method must be overriden for all message classes
     * @param data the pointer to the message data
     * @param size size of the data
     */
    static Message* create(const char *data, int size);

    /**
     * @brief Returns the id of the message
     * @param data the pointer to the message data
     */
    static long long get_id(const char *data);

    /**
     * @brief Deletes the passed message
     * @param msg pointer to the message
     */
    static void remove(Message *msg);

    Message(MsgType p_type);
    virtual ~Message();

    /**
     * @brief Returns type of the message
     */
    MsgType get_type() const;

    /**
     * @brief Returns size of the messages
     */
    int get_size() const;

    /**
     * @brief Returns size of the message data
     */
    static int get_data_size();

    /**
     * @brief Returns the pointer to array with the message data
     * allocated in dynamic memory
     */
    virtual char* get_data() const = 0;

    // Header data.
    // TODO: The start mark must be removed.
    const unsigned char start_mark;
    MsgType type;
    long long id;
protected:
    /**
     * @brief Copies the header data to passed memory area
     * @param r_data memory area to the copying
     * @return pointer to data after the header data.
     */
    char* set_header_data(char *r_data) const;
};

/**
 * @brief The class of the receipt message.
 * Sent to confirm receipt of the message
 */
class ReceiptMsg : public Message {
public:
    ReceiptMsg(long long p_req_id);
    virtual ~ReceiptMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    long long req_msg_id; /**< ID of request msg */
};

// TODO: Must be removed or reworked?
/**
 * @brief The ErrorMsg class
 */
class ErrorMsg : public Message {
public:
    enum { BUF_SIZE = 512 };

    ErrorMsg(const char *p_msg);
    virtual ~ErrorMsg();

    const char* get_error() const;

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;
private:
    char buf[BUF_SIZE];
};

// TODO: Must be removed or reworked?
/**
 * @brief The InfoMsg class
 */
class InfoMsg : public Message {
public:
    enum { BUF_SIZE = 512 };

    InfoMsg(const char *p_msg);
    virtual ~InfoMsg();

    const char* get_info() const;

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;
private:
    char buf[BUF_SIZE];
};

/**
 * @brief The GameStateMsg class
 */
class GameStateMsg : public Message {
public:
    GameStateMsg(GameState p_state);
    virtual ~GameStateMsg();
    
    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    GameState state;
};

/**
 * @brief The GameSessionReqMsg class
 */
class GameSessionReqMsg : public Message {
public:
    GameSessionReqMsg();
    virtual ~GameSessionReqMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;
};

/**
 * @brief The GameFieldMsg class
 */
class GameFieldMsg : public Message {
public:
    GameFieldMsg(const GameField &p_field, bool p_is_own_field = true);
    virtual ~GameFieldMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    GameField field;
    bool is_own_field;
};

/**
 * @brief The GameMoveMsg class
 */
class GameMoveMsg : public Message {
public:
    GameMoveMsg(const Point &p_point);
    virtual ~GameMoveMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    Point point; /**< The shot coordinates */
};

/**
 * @brief The ServerConnStatusReqMsg class.
 * To exchange messages between the interface and the client
 */
class ServerConnStatusReqMsg : public Message {
public:
    ServerConnStatusReqMsg();
    virtual ~ServerConnStatusReqMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;
};

/**
 * @brief The ServerConnStatusMsg class
 */
class ServerConnStatusMsg : public Message {
public:
    ServerConnStatusMsg(bool p_connected);
    virtual ~ServerConnStatusMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    bool connected;
};

/**
 * @brief The ServerConnReqMsg class.
 * To exchange messages between the interface and the client
 */
class ServerConnReqMsg : public Message {
public:
    ServerConnReqMsg();
    virtual ~ServerConnReqMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;
};

/**
 * @brief The StartFieldReqMsg class
 */
class StartFieldReqMsg : public Message {
public:
    StartFieldReqMsg(time_t p_end_time);
    virtual ~StartFieldReqMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    time_t end_time; /**< Deadline for sending a response */
};

/**
 * @brief The StopPreparationMsg class
 */
class StopPreparationMsg : public Message {
public:
    StopPreparationMsg();
    virtual ~StopPreparationMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;
};

/**
 * @brief The StartTurnMsg class
 */
class StartTurnMsg : public Message {
public:
    StartTurnMsg(time_t p_end_time, bool p_is_own);
    virtual ~StartTurnMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    time_t end_time; /**< Deadline for sending a response */
    bool is_own; /**< Is own or enemy turn? */
};

/**
 * @brief The StopTurnMsg class
 */
class StopTurnMsg : public Message {
public:
    StopTurnMsg(bool p_is_own);
    virtual ~StopTurnMsg();

    static int get_data_size();
    static Message* create(const char *data, int size);
    char* get_data() const;

    // Data.
    bool is_own; /**< Is own or enemy turn? */
};

#endif
