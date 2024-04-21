#ifndef MSG_BUFFER_H
#define MSG_BUFFER_H

#include "list.h"
#include "message.h"

// TODO: The mechanism of messages reading must be reworked
// with the removal the start mark from the messages.
/**
 * @brief The buffer for reading the messages from fd
 */
class MsgBuffer {
public:
    explicit MsgBuffer();
    ~MsgBuffer();

    /**
     * @brief Reads the message from fd
     * @param fd the file descriptor
     * @return result of the reading
     */
    BuffError read_msg(int fd);

    /**
     * @brief Extracts the readed message if it exists
     * @return the pointer to extracted message or null pointer if no message
     */
    Message* extract();

    /**
     * @brief Returns true if the buffer is empty
     */
    bool is_empty() const;

    /**
     * @brief Clears the buffer
     */
    void clear();
private:
    enum {
        BUFF_SIZE = 4096
    };

    char buf[BUFF_SIZE];
    int free_pos;
    char *to_extract_msg;

    // The message is too small to determine the size.
    bool is_too_small;

    // Already readed to 'to_extract_msg' message size.
    int readed;

    // Extracted messages.
    List<Message> msg_queue;

    bool _has_start_mark() const;
    MsgType _get_msg_type(const char *data = 0, int size = 0) const;

    bool _ready_to_extract() const;
    void _move_msg_to_start();

    BuffError _gain_msg();

    // Returns -1 if doesn't exist.
    int _start_mark_pos(const char *data = 0, int start = 0) const;

    // Methods return the size of message to writing.
    int _new_reading();
    int _finishing_reading();
};

#endif
