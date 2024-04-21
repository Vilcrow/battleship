#include "msg_buffer.h"
#include "const.h"
#include "tools/write_log.h"
#include "message.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

MsgBuffer::MsgBuffer()
    : free_pos(0)
    , to_extract_msg(0)
    , is_too_small(true)
    , readed(0)
{
    memset(buf, 0, BUFF_SIZE);
}

MsgBuffer::~MsgBuffer()
{
    if(to_extract_msg) {
        delete[] to_extract_msg;
    }
}

BuffError MsgBuffer::read_msg(int fd)
{
    if(fd < 0) {
        return INVALID_FD;
    }

    int free_space = BUFF_SIZE - free_pos;
    if(free_space <= 0) {
        return FULL_BUFF;
    }

    int res = read(fd, buf+free_pos, free_space);
    if(res == -1) {
        return READ_ERR;
    }
    else if(res == 0) {
        return CONN_BROKEN;
    }

    free_pos += res;
    _gain_msg();
    return BUFF_OK;
}

Message* MsgBuffer::extract()
{
    if(!_ready_to_extract()) {
        return 0;
    }

    return msg_queue.get();
}

bool MsgBuffer::is_empty() const
{
    return free_pos == 0;
}

void MsgBuffer::clear()
{
    free_pos = 0;
}

bool MsgBuffer::_has_start_mark() const
{
    return (!is_empty() && buf[0] == Message::MSG_START);
}

MsgType MsgBuffer::_get_msg_type(const char *data, int size) const
{
    if(!data) {
        data = buf;
    }
    if(size == 0) {
        size = free_pos;
    }

    if(Message::is_too_small(size)) {
        return UNDEFINED_MSG;
    }

    return Message::get_type(data, size);
}

bool MsgBuffer::_ready_to_extract() const
{
    return msg_queue.get_size();
}

void MsgBuffer::_move_msg_to_start()
{
    if(is_empty()) {
        return;
    }

    int idx = 0;
    while(true) {
        while((buf[idx] != Message::MSG_START) && (idx < free_pos)) {
            ++idx;
        }

        if(idx == free_pos) { // The start mark doesn't exist.
            free_pos = 0;
            return;
        }

        if(idx != 0) {
            int size = free_pos - idx;
            memmove(buf, buf+idx, size);
            free_pos -= idx;
            idx = 0;
        }
        MsgType type = _get_msg_type();
        if(type == UNDEFINED_MSG) {
            return;
        }
        else if(type == INVALID_MSG) {
            int offset = Message::min_size();
            memmove(buf, buf+offset, free_pos-offset);
            free_pos -= offset;
            idx = 0;
            continue;
        }
        else {
            break;
        }
    }
}

BuffError MsgBuffer::_gain_msg()
{
    is_too_small = false;

    int to_write = 0;

    // Readed message without the start mark.
    // Try find next message.
    if(!to_extract_msg && !_has_start_mark()) {
        _move_msg_to_start();
        if(is_empty()) {
            return EMPTY_BUFF;
        }
    }
    // Couldn't finish reading the message.
    // Remove already readed message.
    else if(to_extract_msg && _has_start_mark()) {
        delete[] to_extract_msg;
        to_extract_msg = 0;
        readed = 0;
    }

    // Try gain messages from the buffer.
    while(!is_empty()) {
        if(!to_extract_msg) {
            to_write = _new_reading();
        }
        else {
            to_write = _finishing_reading();
        }

        if(!to_write) {
            continue;
        }

        // Extract message from buffer.
        memcpy(to_extract_msg+readed, buf, to_write);
        // Remove the extracted message from buffer.
        memmove(buf, buf+to_write, free_pos-to_write);
        // Update positions.
        free_pos -= to_write;
        readed += to_write;

        Message *tmp = Message::create_instance(to_extract_msg, readed);
        // The message has been read in full.
        if(tmp) {
            msg_queue.put(tmp);
            delete[] to_extract_msg;
            to_extract_msg = 0;
            readed = 0;
        }
        else {
            break;
        }
    }
    return BUFF_OK;
}

int MsgBuffer::_start_mark_pos(const char *data, int start) const
{
    if(!data) {
        data = buf;
    }
    if(start < 0) {
        start = 0;
    }

    int pos = start;
    while(pos < free_pos) {
        if(buf[pos] == Message::MSG_START) {
            return pos;
        }
        ++pos;
    }
    return -1;
}

int MsgBuffer::_new_reading()
{
    int size = -1;
    while(size == -1 && !is_empty()) {
        if(!_has_start_mark()) {
            _move_msg_to_start();
            continue;
        }
        MsgType type = _get_msg_type();
        if(type == UNDEFINED_MSG) {
            int to_write = 0;
            int next_start_pos = _start_mark_pos(buf, 1);
            if(next_start_pos == -1) {
                to_write = free_pos;
            }
            else {
                to_write = next_start_pos;
            }
            size = to_write;
            is_too_small = true;
            break;
        }
        else if(type == INVALID_MSG) {
            _move_msg_to_start();
            continue;
        }
        else {
            size = Message::get_type_data_size(type);
        }
    }
    // Empty buffer.
    if(size == -1) {
        return 0;
    }
    
    to_extract_msg = new char[size];
    return (free_pos > size) ? size : free_pos;
}

int MsgBuffer::_finishing_reading()
{
    while(!is_empty()) {
        int remains = 0;
        int next_start_pos = _start_mark_pos();
        if(next_start_pos == -1) {
            remains = free_pos;
        }
        else {
            remains = next_start_pos;
        }
        if(is_too_small) {
            // Temporary buffer.
            int tmp_size = readed + remains;
            char *tmp = new char[tmp_size];
            memcpy(tmp, to_extract_msg, readed);
            memcpy(tmp+readed, buf, remains);

            // Trying to determine the type again.
            MsgType type = _get_msg_type(tmp, tmp_size);
            if(type == UNDEFINED_MSG) {
                delete[] to_extract_msg;
                to_extract_msg = tmp;
                readed = tmp_size;
                _move_msg_to_start();
                return 0;
            }
            else if(type == INVALID_MSG) {
                delete[] tmp;
                delete[] to_extract_msg;
                to_extract_msg = 0;
                readed = 0;
                is_too_small = false;
                _move_msg_to_start();
                return 0;
            }
            else {
                int msg_size = Message::get_type_data_size(type);
                char *new_extract = new char[msg_size];
                memcpy(new_extract, to_extract_msg, readed);
                delete[] tmp;
                delete[] to_extract_msg;
                to_extract_msg = new_extract;
                return remains;
            }
        }
        else {
            int size = ((Message*)to_extract_msg)->get_size();
            if((readed+remains) > size) {
                remains = size;
            }
            return remains;
        }
    }
    return 0;
}
