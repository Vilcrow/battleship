#include "point.h"

Point::Point()
    : x(0)
    , y(0)
{

}

Point::Point(int p_x, int p_y)
    : x(p_x)
    , y(p_y)
{

}

Point::Point(const Point &other)
    : x(other.x)
    , y(other.y)
{

}

Point::~Point()
{

}

void Point::swap(Point &other)
{
    Point tmp = other;
    other = *this;
    *this = tmp;
}

Point Point::operator+(const Point &other) const
{
    return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point &other) const
{
    return Point(x - other.x, y - other.y);
}

const Point& Point::operator+=(const Point &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

const Point& Point::operator-=(const Point &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

const Point& Point::operator=(const Point &other)
{
    x = other.x;
    y = other.y;
    return *this;
}

bool Point::operator==(const Point &other) const
{
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point &other) const
{
    return x != other.x || y != other.y;
}
