#include "CppUTest/TestHarness.h"
#include "list.h"

#define ITEMS_CNT 10000

TEST_GROUP(ListGroup)
{
    int* values[ITEMS_CNT];
    List<int> *lst = 0;
    void setup()
    {
        for(int i = 0; i < ITEMS_CNT; i++) {
            values[i] = new int(rand() % 20000);
        }
        lst = new List<int>();
    }

    void teardown()
    {
        delete lst;
    }
};

TEST(ListGroup, PutAndGet)
{
    CHECK(lst->is_empty());
    for(int i = 0; i < ITEMS_CNT; i++) {
        lst->put(values[i]);
        CHECK_FALSE(lst->is_empty());
    }
    CHECK_EQUAL(ITEMS_CNT, lst->get_size());

    // For the items destruction.
    List<int> tmp;
    CHECK(tmp.is_empty());

    for(int i = 0; i < ITEMS_CNT; i++) {
        int *item = lst->get();
        POINTERS_EQUAL(values[i], item);
        tmp.put(item);
    }
    CHECK(lst->is_empty());
}

TEST(ListGroup, Contains)
{
    for(int i = 0; i < ITEMS_CNT; i++) {
        lst->put(values[i]);
    }
    for(int i = 0; i < ITEMS_CNT; i++) {
        CHECK(lst->contains(values[i]));
    }

    for(int i = 0; i < ITEMS_CNT; i++) {
        POINTERS_EQUAL(values[i], (*lst)[i]);
    }
}

TEST(ListGroup, Swap)
{
    for(int i = 0; i < ITEMS_CNT; i++) {
        lst->put(values[i]);
    }
    List<int> tmp;
    tmp.swap(*lst);

    for(int i = 0; i < ITEMS_CNT; i++) {
        POINTERS_EQUAL(values[i], tmp[i]);
    }
}

TEST(ListGroup, ClearMethod)
{
    CHECK(lst->is_empty());
    for(int i = 0; i < ITEMS_CNT; i++) {
        lst->put(values[i]);
    }
    CHECK_FALSE(lst->is_empty());

    lst->clear();
    CHECK(lst->is_empty());

    for(int i = 0; i < ITEMS_CNT; i++) {
        CHECK_FALSE(lst->contains(values[i]));
    }

    CHECK_EQUAL(0, lst->get_size());
}

TEST(ListGroup, RemoveMethod)
{
    CHECK(lst->is_empty());
    for(int i = 0; i < ITEMS_CNT; i++) {
        lst->put(values[i]);
    }
    CHECK_FALSE(lst->is_empty());

    for(int i = 0; i < ITEMS_CNT; i++) {
        lst->remove(values[i]);
        CHECK_FALSE(lst->contains(values[i]));
    }
    CHECK(lst->is_empty());
    CHECK_EQUAL(0, lst->get_size());
}
