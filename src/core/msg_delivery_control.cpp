#include "msg_delivery_control.h"
#include "message.h"
#include "tools/write_log.h"
#include <unistd.h>
#include <stdio.h>


MsgDeliveryControl::MsgDeliveryControl(int p_resend, int p_resend_time)
    : last(0)
    , size(0)
    , resend_cnt(p_resend)
    , resend_time(p_resend_time)
{

}

MsgDeliveryControl::~MsgDeliveryControl()
{
    ListItem *tmp = last;
    while(tmp) {
        last = last->next;
        delete tmp;
        tmp = last;
    }
}

void MsgDeliveryControl::add(Message *msg)
{
    if(!msg || under_control(msg)) {
        return;
    }

    ListItem *tmp = new ListItem;
    tmp->msg = msg;
    tmp->last_send_time = time(0);
    tmp->resend_cnt = resend_cnt;
    tmp->next = last;
    last = tmp;
    ++size;
}

bool MsgDeliveryControl::remove(Message *msg)
{
    ListItem *tmp = _find(msg);
    if(tmp) {
        _remove_msg(tmp);
        return true;
    }
    return false;
}

bool MsgDeliveryControl::remove(long long id)
{
    ListItem *tmp = _find(id);
    if(tmp) {
        _remove_msg(tmp);
        return true;
    }
    return false;
}

void MsgDeliveryControl::decrease(const Message *msg)
{
    ListItem *tmp = _find(msg);
    if(tmp) {
        tmp->last_send_time = time(0);
        tmp->resend_cnt -= 1;
    }
}

bool MsgDeliveryControl::under_control(const Message *msg) const
{
    return _find(msg) != 0;
}

bool MsgDeliveryControl::under_control(long long id) const
{
    return _find(id) != 0;
}

void MsgDeliveryControl::resend(List<Message> &ret)
{
    time_t cur_time = time(0);
    ListItem *tmp = last;
    Message *msg = 0;
    while(tmp) {
        if(_need_resend(tmp, cur_time)) {
            msg = tmp->msg;
            ret.put(msg);
        }
        tmp = tmp->next;
    }
    _rm_all_exhausted();
}

bool MsgDeliveryControl::_need_resend(const ListItem *item,
                                      time_t cur_time) const
{
    NULL_PTR_CHECK_RET_ZERO(item)

    if(item->resend_cnt > 0) {
        time_t passed = cur_time - item->last_send_time;
        if(passed > resend_time) {
            return true;
        }
    }
    return false;
}

void MsgDeliveryControl::_rm_all_exhausted()
{
    ListItem *tmp = last;
    while(tmp) {
        if(tmp->resend_cnt <= 0) {
            _remove_msg(tmp);
        }
        tmp = tmp->next;
    }
}

int MsgDeliveryControl::get_size() const
{
    return size;
}

Message* MsgDeliveryControl::operator[](int idx)
{
    if(idx < 0 || idx >= size) {
        return 0;
    }

    Message *ret = 0;
    int i = 0;
    ListItem *tmp = last;
    for(; tmp; tmp = tmp->next) {
        if(i == idx) {
            ret = tmp->msg;
            break;
        }
        ++i;
    }
    return ret;
}

bool MsgDeliveryControl::process_check(const ReceiptMsg *msg)
{
    NULL_PTR_CHECK_RET_ZERO(msg)

    return remove(msg->req_msg_id);
}

ListItem* MsgDeliveryControl::_find(const Message *msg) const
{
    NULL_PTR_CHECK_RET_ZERO(msg)

    ListItem *tmp = last;
    while(tmp) {
        if(tmp->msg == msg) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return 0;
}

ListItem* MsgDeliveryControl::_find(long long id) const
{
    ListItem *tmp = last;
    while(tmp) {
        if(tmp->msg->id == id) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return 0;
}

Message* MsgDeliveryControl::_find_msg(const Message *msg) const
{
    NULL_PTR_CHECK_RET_ZERO(msg)

    ListItem *tmp = _find(msg);
    if(!tmp) {
        return 0;
    }
    return tmp->msg;
}

void MsgDeliveryControl::_remove_msg(ListItem *item)
{
    NULL_PTR_CHECK_RETURN(item)

    delete item->msg;
    if(item == last) {
        last = last->next;
    }
    else if(last->next == item) {
        last->next = item->next;
    }
    else {
        ListItem *prev = last;
        while(prev->next != item) {
            prev = prev->next;
        }
        prev->next = item->next;
    }
    delete item;
    --size;
}
