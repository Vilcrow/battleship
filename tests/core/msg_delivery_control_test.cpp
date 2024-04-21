#include "CppUTest/TestHarness.h"
#include "msg_delivery_control.h"

#define MSG_COUNT 5

TEST_GROUP(MsgDeliveryControlGroup)
{
    MsgDeliveryControl *control = 0;
    Message* messages[MSG_COUNT] = { 0, 0, 0, 0, 0 };
    ReceiptMsg* receipts[MSG_COUNT] = { 0, 0, 0, 0, 0 };
    int id[MSG_COUNT] = { 33, 42, 23, 1, 45 };

    void setup()
    {
        for(int i = 0; i < MSG_COUNT; i++) {
            messages[i] = new ServerConnStatusMsg(false);
            messages[i]->id = id[i];
            receipts[i] = new ReceiptMsg(id[i]);
        }

        control = new MsgDeliveryControl;
    }

    void teardown()
    {
        for(int i = 0; i < MSG_COUNT; i++) {
            delete messages[i];
            delete receipts[i];
        }

        delete control;
    }
};

TEST(MsgDeliveryControlGroup, Empty)
{
    CHECK_EQUAL(0, control->get_size());
    CHECK_FALSE((*control)[0]);
}

TEST(MsgDeliveryControlGroup, AddRemoveMethods)
{
    for(int i = 0; i < MSG_COUNT; i++) {
        control->add(messages[i]);
        CHECK_EQUAL(i + 1, control->get_size());
        CHECK(control->under_control(messages[i]));
        CHECK(control->under_control(id[i]));
    }

    for(int i = 0; i < MSG_COUNT; i++) {
        control->remove(messages[i]);
        CHECK_FALSE(control->under_control(id[i]));
        CHECK_EQUAL(MSG_COUNT - 1 - i, control->get_size());
        messages[i] = 0;
    }
}

TEST(MsgDeliveryControlGroup, ProcessCheckMethod)
{
    for(int i = 0; i < MSG_COUNT; i++) {
        control->add(messages[i]);
        CHECK(control->under_control(messages[i]));
    }

    for(int i = 0; i < MSG_COUNT; i++) {
        control->process_check(receipts[i]);
        CHECK_FALSE(control->under_control(id[i]));
        messages[i] = 0;
    }
}
