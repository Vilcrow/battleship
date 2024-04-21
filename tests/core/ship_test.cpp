#include "CppUTest/TestHarness.h"
#include "ship.h"

TEST_GROUP(ShipGroup)
{
    Ship *ship;
    void setup()
    {
        ship = new Ship(Point(2, 3), Point(1, 3));
    }
    void teardown()
    {
        delete ship;
    }
};

TEST(ShipGroup, Constructors)
{
    // The default constructor.
    Ship def_ship;
    CHECK_EQUAL(INVALID_SHIP, def_ship.get_type());
    CHECK_EQUAL(0, def_ship.get_length());
    CHECK_EQUAL(false, def_ship.is_vertical());
    CHECK(def_ship.get_head() == Point());
    CHECK(def_ship.get_tail() == Point());
    CHECK(def_ship.get_delta() == Point());
    CHECK_FALSE(def_ship.contains(Point()));

    // The constructor with tail and head arguments.
    Point head(0, 0), tail(0, 4);
    Ship ht_ship(head, tail);
    CHECK_EQUAL(AIRCRAFT_CARRIER_SHIP, ht_ship.get_type());
    CHECK_EQUAL(5, ht_ship.get_length());
    CHECK(ht_ship.is_vertical());
    CHECK(ht_ship.get_head() == head);
    CHECK(ht_ship.get_tail() == tail);
    CHECK(ht_ship.get_delta() == Point(0, 1));
    CHECK(ht_ship.contains(Point(0, 2)));
    CHECK_FALSE(ht_ship.contains(Point(4, 1)));

    // The copy constructor.
    Ship def_copy(def_ship);
    CHECK_EQUAL(INVALID_SHIP, def_copy.get_type());
    CHECK_EQUAL(0, def_copy.get_length());
    CHECK_EQUAL(false, def_copy.is_vertical());
    CHECK(def_copy.get_head() == Point());
    CHECK(def_copy.get_tail() == Point());
    CHECK(def_copy.get_delta() == Point());
    CHECK_FALSE(def_copy.contains(Point()));
    Ship ht_copy(ht_ship);
    CHECK_EQUAL(AIRCRAFT_CARRIER_SHIP, ht_copy.get_type());
    CHECK_EQUAL(5, ht_copy.get_length());
    CHECK(ht_copy.is_vertical());
    CHECK(ht_copy.get_head() == head);
    CHECK(ht_copy.get_tail() == tail);
    CHECK(ht_copy.get_delta() == Point(0, 1));
    CHECK(ht_copy.contains(Point(0, 2)));
    CHECK_FALSE(ht_copy.contains(Point(4, 1)));

    // The assignment operator.
    Ship ht_asgn = ht_ship;
    CHECK_EQUAL(AIRCRAFT_CARRIER_SHIP, ht_asgn.get_type());
    CHECK_EQUAL(5, ht_asgn.get_length());
    CHECK(ht_asgn.is_vertical());
    CHECK(ht_asgn.get_head() == head);
    CHECK(ht_asgn.get_tail() == tail);
    CHECK(ht_asgn.get_delta() == Point(0, 1));
    CHECK(ht_asgn.contains(Point(0, 2)));
    CHECK_FALSE(ht_asgn.contains(Point(4, 1)));
}

TEST(ShipGroup, InvalidArgs)
{
    Point head(0, 0), tail(3, 4);
    Ship ship(head, tail);
    CHECK_EQUAL(INVALID_SHIP, ship.get_type());
    CHECK_EQUAL(0, ship.get_length());
    CHECK_EQUAL(false, ship.is_vertical());
    CHECK(ship.get_head() == head);
    CHECK(ship.get_tail() == tail);
    CHECK(ship.get_delta() == Point());
    CHECK_FALSE(ship.contains(head));
    CHECK_FALSE(ship.contains(tail));
}
