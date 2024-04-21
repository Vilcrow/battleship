#include "ship.h"

int Ship::ship_length(ShipType type)
{
    switch(type) {
    case SUBMARINE_SHIP:
        return 1;
    case DESTROYER_SHIP:
        return 2;
    case CRUISER_SHIP:
        return 3;
    case BATTLESHIP_SHIP:
        return 4;
    case AIRCRAFT_CARRIER_SHIP:
        return 5;
    case INVALID_SHIP:
        return 0;
    }
    return 0;
}

int Ship::ship_count(ShipType type)
{
    switch(type) {
    case SUBMARINE_SHIP:
        return 2;
    case DESTROYER_SHIP:
        return 2;
    case CRUISER_SHIP:
        return 1;
    case BATTLESHIP_SHIP:
        return 1;
    case AIRCRAFT_CARRIER_SHIP:
        return 1;
    case INVALID_SHIP:
        return 0;
    }
    return 0;
}

ShipType Ship::ship_type(int len)
{
    switch(len) {
    case 1:
        return SUBMARINE_SHIP;
    case 2:
        return DESTROYER_SHIP;
    case 3:
        return CRUISER_SHIP;
    case 4:
        return BATTLESHIP_SHIP;
    case 5:
        return AIRCRAFT_CARRIER_SHIP;
    default:
        return INVALID_SHIP;
    }
}

Ship::Ship()
    : type(INVALID_SHIP)
    , head()
    , tail()
{

}

Ship::Ship(const Point& p_head, const Point& p_tail)
    : type(INVALID_SHIP)
    , head(p_head)
    , tail(p_tail)
{
    if(head == tail) {
        type = SUBMARINE_SHIP;
        return;
    }

    // Vertical.
    if(head.x == tail.x) {
        int len = tail.y - head.y + 1;
        if(len < 0) {
            head.swap(tail);
            len *= -1;
        }
        type = ship_type(len);
    }
    // Horizontal.
    else if(head.y == tail.y) {
        int len = tail.x - head.x + 1;
        if(len < 0) {
            head.swap(tail);
            len *= -1;
        }
        type = ship_type(len);
    }
    // Invalid arguments.
    else {
        return;
    }
}

Ship::Ship(const Ship &other)
    : type(other.type)
    , head(other.head)
    , tail(other.tail)
{

}

Ship::~Ship()
{

}

ShipType Ship::get_type() const
{
    return type;
}

int Ship::get_length() const
{
    return ship_length(type);
}

bool Ship::is_vertical() const
{
    if(type == INVALID_SHIP) {
        return false;
    }

    return (head.x == tail.x);
}

Point Ship::get_head() const
{
    return head;
}

Point Ship::get_tail() const
{
    return tail;
}

Point Ship::get_delta() const
{
    Point ret;
    if(get_type() == INVALID_SHIP) {
        ret = Point(0, 0);
    }
    else {
        ret = is_vertical() ? Point(0, 1) : Point(1, 0);
    }
    return ret;
}

bool Ship::contains(const Point &point) const
{
    if(get_type() == INVALID_SHIP) {
        return false;
    }
    int len = get_length();
    Point tmp = head;
    Point delta = get_delta();
    for(int i = 0; i < len; i++) {
        if(point == tmp) {
            return true;
        }
        tmp += delta;
    }
    return false;
}

const Ship& Ship::operator=(const Ship &other)
{
    type = other.type;
    head = other.head;
    tail = other.tail;
    return *this;
}
