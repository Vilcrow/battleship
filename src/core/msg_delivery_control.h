#ifndef MSG_DELIVERY_CONTROL_H
#define MSG_DELIVERY_CONTROL_H

#include <time.h>
#include "list.h"
#include "message.h"

class ReceiptMsg;

struct ListItem {
    Message *msg;
    time_t last_send_time;
    int resend_cnt; /**< The remaining number of attempts to send */
    ListItem *next;
};

/**
 * @brief Class for control messages delivery. For control must be passed
 * a pointer to a message created in dynamic memory
 */
class MsgDeliveryControl {
public:
    enum {
        DEFAULT_RESEND_CNT = 3,
        DEFAULT_RESEND_TIME = 10
    };

    explicit MsgDeliveryControl(int p_resend = DEFAULT_RESEND_CNT,
                                int p_resend_time = DEFAULT_RESEND_TIME);
    ~MsgDeliveryControl();

    /**
     * @brief Adds the message undert delivery control
     * if it isn't under control
     * @param msg a pointer to the message
     */
    void add(Message *msg);

    /**
     * @brief Removes the message from delivery control
     * @param msg a pointer to the message
     * @return true if the message was removed
     */
    bool remove(Message *msg);

    /**
     * @brief This method overloads remove()
     * @param id number of the message
     * @return
     */
    bool remove(long long id);

    /**
     * @brief Decreases the counter of the message sending
     * @param msg a pointer to the message
     */
    void decrease(const Message *msg);

    /**
     * @brief Returns true if the message under delivery control
     */
    bool under_control(const Message *msg) const;

    /**
     * @brief This method overloads under_control()
     * @param id number of the message
     */
    bool under_control(long long id) const;

    /**
     * @brief Messages that need to be resend are place in the passed list
     * @param ret the list to which the messages will be added for resending
     */
    void resend(List<Message> &ret);

    /**
     * @brief Returns the count of messages under delivery control
     */
    int get_size() const;

    /**
     * @brief operator []
     * @param idx
     * @return
     */
    Message* operator[](int idx);

    /**
     * @brief Removes a message that the receipt was sent to
     * @param msg the receipt message
     * @return
     */
    bool process_check(const ReceiptMsg *msg);
private:
    ListItem *last;
    int size;

    int resend_cnt;
    int resend_time;

    ListItem* _find(const Message *msg) const;
    ListItem* _find(long long id) const;
    Message* _find_msg(const Message *msg) const;

    void _remove_msg(ListItem *item);
    bool _need_resend(const ListItem *item, time_t cur_time) const;
    void _rm_all_exhausted();
};

#endif
