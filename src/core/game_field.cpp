#include "game_field.h"
#include "const.h"
#include <string.h>

GameField::GameField()
{
    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            field[i][j] = CELL_EMPTY;
        }
    }
}

GameField::GameField(const GameField &other)
{
    fill(other.field);
}

GameField::~GameField()
{

}

void GameField::fill(const char p_field[FLD_SZ][FLD_SZ])
{
    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            field[i][j] = p_field[i][j];
        }
    }
}

bool GameField::contains(const Point &point)
{
    if(point.x < 0 || point.x >= FLD_SZ || point.y < 0 || point.y >= FLD_SZ) {
        return false;
    }
    return true;
}

const GameField& GameField::operator=(const GameField &other)
{
    fill(other.field);
    return *this;
}

GameField GameField::get_for_enemy() const
{
    GameField ret = *this;
    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            if(ret.field[i][j] == CELL_UNHARMED) {
                ret.field[i][j] = CELL_EMPTY;
            }
        }
    }
    return ret;
}

GFCellState GameField::register_shot(const Point &point)
{
    CellState ret = CELL_EMPTY;

    if(!contains(point)) {
        return ret;
    }

    CellState cur_state = get_state(point);
    switch(cur_state) {
    case CELL_EMPTY:
        ret = CELL_MISSED;
        break;
    case CELL_UNHARMED:
        ret = CELL_DAMAGED;
        break;
    case CELL_DAMAGED:
    case CELL_MISSED:
        break;
    }

    if(ret != CELL_EMPTY && ret != cur_state) {
        field[point.y][point.x] = ret;
    }

    if(ret == CELL_DAMAGED && _ship_destroyed(point)) {
        _mark_destroyed_ship_boundary(point);
    }

    return ret;
}

bool GameField::_ship_destroyed(const Point &point) const
{
    CellState pos_state = get_state(point);
    if(pos_state != CELL_DAMAGED) {
        return false;
    }

    Point cur_pos;
    int dir_count = 4;
    const Point dirs[dir_count] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
    for(int i = 0; i < dir_count; i++) {
        cur_pos = point;
        pos_state = get_state(cur_pos);
        do {
            cur_pos += dirs[i];
            if(!contains(cur_pos)) {
                cur_pos -= dirs[i];
                break;
            }
            pos_state = get_state(cur_pos);
        } while(pos_state == CELL_DAMAGED);

        if(pos_state == CELL_UNHARMED) {
            return false;
        }
    }
    return true;
}

void GameField::_mark_destroyed_ship_boundary(const Point &point)
{
    CellState pos_state = get_state(point);
    if(pos_state != CELL_DAMAGED) {
        return;
    }

    int dir_count = 4;
    int around_count = 8;
    const Point dirs[dir_count] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
    const Point around[around_count] = {
        {  1,  0 }, {  1,  1 }, {  1, -1 }, { -1,  0 },
        { -1,  1 }, { -1, -1 }, {  0,  1 }, {  0, -1 }
    };
    Point cur_pos = point;
    int i = 0;
    do {
        if(!contains(cur_pos) || get_state(cur_pos) != CELL_DAMAGED) {
            cur_pos = point;
        }
        else {
            while(get_state(cur_pos) == CELL_DAMAGED) {
                for(int j = 0; j < around_count; j++) {
                    Point around_pos = cur_pos + around[j];
                    if(!contains(around_pos)) {
                        continue;
                    }
                    if(get_state(around_pos) == CELL_EMPTY) {
                        set_point(around_pos, CELL_MISSED);
                    }
                }
                cur_pos += dirs[i];
            }
        }
        cur_pos = point;
        ++i;
        cur_pos += dirs[i];
    } while(i < dir_count);
}

GFCellState GameField::get_state(const Point &point) const
{
    if(!contains(point)) {
        return CELL_EMPTY;
    }

    char cell = field[point.y][point.x];
    switch(cell) {
    case CELL_EMPTY:
    case CELL_UNHARMED:
    case CELL_DAMAGED:
    case CELL_MISSED:
        break;
    default:
        return CELL_EMPTY;
    }

    return static_cast<CellState>(cell);
}

void GameField::set_point(const Point &pos, CellState state)
{
    if(!contains(pos)) {
        return;
    }

    field[pos.y][pos.x] = state;
}

char GameField::get_cell_char(CellState state)
{
    char ret = CELL_EMPTY;
    switch(state) {
    case CELL_EMPTY:
        ret = CHAR_EMPTY;
        break;
    case CELL_UNHARMED:
        ret = CHAR_UNHARMED;
        break;
    case CELL_DAMAGED:
        ret = CHAR_DAMAGED;
        break;
    case CELL_MISSED:
        ret = CHAR_MISSED;
        break;
    }
    return ret;
}

bool GameField::is_empty() const
{
    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            if(field[i][j] != CELL_EMPTY) {
                return false;
            }
        }
    }
    return true;
}

bool GameField::is_valid_start_field() const
{
    if(is_empty() || !_is_valid_field(field)) {
        return false;
    }

    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            if(field[i][j] != CELL_EMPTY && field[i][j] != CELL_UNHARMED) {
                return false;
            }
        }
    }
    return true;
}

bool GameField::all_ships_destroyed() const
{
    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            if(field[i][j] == CELL_UNHARMED) {
                return false;
            }
        }
    }
    return true;
}

bool GameField::_is_valid_field(const char p_field[FLD_SZ][FLD_SZ])
{
    // Check values of cells.
    if(!_is_cell_values_valid(p_field)) {
        return false;
    }

    char tmp[FLD_SZ][FLD_SZ];
    memcpy(tmp, p_field, sizeof(tmp));
    Ship ship_arr[SHIP_COUNT];
    int ship_count = 0;
    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            if(tmp[i][j] == CELL_EMPTY) {
                continue;
            }
            Point head(j, i);
            while(tmp[i][j] != CELL_EMPTY && j < FLD_SZ) {
                ++j;
            }
            --j;
            Point tail(j, i);
            Ship ship(head, tail);
            ShipType s_type = ship.get_type();
            // Vertical ship.
            if(s_type != INVALID_SHIP && s_type != SUBMARINE_SHIP) {
                ;
            }
            else if(s_type == INVALID_SHIP) {
                return false;
            }
            // Maybe it is horizontal ship.
            else if(s_type == SUBMARINE_SHIP) {
                int tmp_i = i;
                while(tmp[tmp_i][j] != CELL_EMPTY && tmp_i < FLD_SZ) {
                    ++tmp_i;
                }
                --tmp_i;
                if(tmp_i != i) {
                    tail = { j, tmp_i };
                    ship = Ship(head, tail);
                    if(ship.get_type() == INVALID_SHIP) {
                        return false;
                    }
                }
            }
            // Found excess ship.
            if(ship_count == SHIP_COUNT) {
                return false;
            }
            ship_arr[ship_count++] = ship;
            _remove_ship(ship, tmp);
        }
    }

    if(ship_count != SHIP_COUNT) {
        return false;
    }

    int submarine_cnt = Ship::ship_count(SUBMARINE_SHIP);
    int destroyer_cnt = Ship::ship_count(DESTROYER_SHIP);
    int cruiser_cnt = Ship::ship_count(CRUISER_SHIP);
    int battleship_cnt = Ship::ship_count(BATTLESHIP_SHIP);
    int aircraft_carrier_cnt = Ship::ship_count(AIRCRAFT_CARRIER_SHIP);

    for(int i = 0; i < ship_count; i++) {
        ShipType type = ship_arr[i].get_type();
        switch(type) {
        case SUBMARINE_SHIP:
            --submarine_cnt;
            break;
        case DESTROYER_SHIP:
            --destroyer_cnt;
            break;
        case CRUISER_SHIP:
            --cruiser_cnt;
            break;
        case BATTLESHIP_SHIP:
            --battleship_cnt;
            break;
        case AIRCRAFT_CARRIER_SHIP:
            --aircraft_carrier_cnt;
            break;
        case INVALID_SHIP:
            return false;
        }
    }

    if(submarine_cnt || destroyer_cnt || cruiser_cnt || battleship_cnt ||
       aircraft_carrier_cnt) {
        return false;
    }

    int end_count = 3;
    int flank_count = 2;
    Point h_delta_h[end_count] = { {-1, -1}, {-1, 0}, {-1, 1} };
    Point t_delta_h[end_count] = { { 1, -1}, { 1, 0}, { 1, 1} };
    Point b_delta_h[flank_count] = { {0, -1}, {0, 1} };
    Point h_delta_v[end_count] = { {-1, -1}, {0, -1}, {1, -1} };
    Point t_delta_v[end_count] = { {-1, 1}, {0, 1}, {1, 1} };
    Point b_delta_v[flank_count] = { {-1, 0}, {1, 0} };

    for(int i = 0; i < ship_count; i++) {
        Ship& ship = ship_arr[i];
        int len = ship.get_length();
        bool is_vertical = ship.is_vertical();
        Point delta = ship.get_delta();

        Point tmp = ship.get_head();
        Point nbr;
        for(int j = 0; j < len; j++) {
            if(tmp == ship.get_head()) {
                for(int k = 0; k < end_count; k++) {
                    if(is_vertical) {
                        nbr = tmp + h_delta_v[k];
                    }
                    else {
                        nbr = tmp + h_delta_h[k];
                    }
                    if(nbr.x < 0 || nbr.x >= FLD_SZ ||
                       nbr.y < 0 || nbr.y >= FLD_SZ) {
                        continue;
                    }
                    if(p_field[nbr.y][nbr.x]) {
                        return false;
                    }
                }
            }
            if(tmp == ship.get_tail()) {
                for(int k = 0; k < end_count; k++) {
                    if(is_vertical) {
                        nbr = tmp + t_delta_v[k];
                    }
                    else {
                        nbr = tmp + t_delta_h[k];
                    }
                    if(nbr.x < 0 || nbr.x >= FLD_SZ ||
                       nbr.y < 0 || nbr.y >= FLD_SZ) {
                        continue;
                    }
                    if(p_field[nbr.y][nbr.x]) {
                        return false;
                    }
                }
            }
            for(int k = 0; k < flank_count; k++) {
                if(is_vertical) {
                    nbr = tmp + b_delta_v[k];
                }
                else {
                    nbr = tmp + b_delta_h[k];
                }
                if(nbr.x < 0 || nbr.x >= FLD_SZ ||
                   nbr.y < 0 || nbr.y >= FLD_SZ) {
                    continue;
                }
                if(p_field[nbr.y][nbr.x]) {
                    return false;
                }
            }
            tmp += delta;
        }
    }
    return true;
}

bool GameField::_is_cell_values_valid(const char p_field[FLD_SZ][FLD_SZ])
{
    for(int i = 0; i < FLD_SZ; i++) {
        for(int j = 0; j < FLD_SZ; j++) {
            if(p_field[i][j] < CELL_EMPTY || p_field[i][j] > CELL_MISSED) {
                return false;
            }
        }
    }
    return true;
}

void GameField::_remove_ship(const Ship& ship, char r_field[FLD_SZ][FLD_SZ])
{
    if(ship.get_type() == INVALID_SHIP) {
        return;
    }

    int len = ship.get_length();
    Point tmp = ship.get_head();
    Point delta = ship.get_delta();
    for(int i = 0; i < len; i++) {
        if(contains(tmp)) {
            r_field[tmp.y][tmp.x] = CELL_EMPTY;
        }
        tmp += delta;
    }
}
