#include "cursor.h"
#include "ncurses.h"

Cursor::Cursor()
{

}

Cursor::Cursor(const Point &p_min, const Point &p_max)
    : pos(Point())
    , min(p_min)
    , max(p_max)
{
    if(!_is_valid_boundary(p_min, p_max)) {
        min = max = Point();
    }
    else {
        pos = min;
    }
}

Cursor::~Cursor()
{

}

const Point& Cursor::get_pos() const
{
    return pos;
}

void Cursor::set_pos(const Point &p_pos)
{
    pos = p_pos;
    _check_pos();
}

void Cursor::set_related_pos(const Point &p_pos)
{
    pos = p_pos + min;
    _check_pos();
}

Point Cursor::get_related_pos() const
{
    Point ret = pos;
    ret -= min;
    return ret;
}

void Cursor::move(const Point &offset)
{
    pos += offset;
    _check_pos();
    ::move(pos.y, pos.x);
}

void Cursor::show()
{
    ::move(pos.y, pos.x);
    curs_set(1);
}

void Cursor::hide()
{
    curs_set(0);
}

void Cursor::set_boundary(const Point &p_min, const Point &p_max)
{
    if(_is_valid_boundary(p_min, p_max)) {
        min = p_min;
        max = p_max;
        pos = p_min;
    }
    ::move(pos.y, pos.x);
}

void Cursor::return_to_pos()
{
    ::move(pos.y, pos.x);
}

void Cursor::_check_pos()
{
    if(pos.x < min.x) {
        pos.x = max.x - 1;
    }
    if(pos.y < min.y) {
        pos.y = max.y - 1;
    }
    if(pos.x >= max.x) {
        pos.x = min.x;
    }
    if(pos.y >= max.y) {
        pos.y = min.y;
    }
}

bool Cursor::_is_valid_boundary(const Point &p_min, const Point &p_max)
{
    if(p_min.x >= p_max.x || p_min.y >= p_max.y) {
        return false;
    }
    return true;
}
