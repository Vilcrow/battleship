#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "point.h"
#include "ship.h"

class GameField {
public:
    enum {
        FLD_SZ = 10,
        SHIP_COUNT = 7
    };

    enum CellState {
        CELL_EMPTY = 0,
        CELL_UNHARMED,
        CELL_DAMAGED,
        CELL_MISSED
    };

    enum CellChar {
        CHAR_EMPTY = ' ',
        CHAR_UNHARMED = 'o',
        CHAR_DAMAGED = 'x',
        CHAR_MISSED = '.'
    };

    /**
     * @brief Constructor
     */
    GameField();

    /**
     * @brief Constructs a copy of other
     */
    GameField(const GameField &other);

    /**
     * @brief Destructor
     */
    ~GameField();

    /**
     * @brief Fills the field with the contents of the passed array
     * @param p_field
     */
    void fill(const char p_field[FLD_SZ][FLD_SZ]);

    /**
     * @brief Returns true if the field contains the point,
     * otherwise returns false
     * @param point
     */
    static bool contains(const Point &point);

    /**
     * @brief operator =
     * @param other
     * @return
     */
    const GameField& operator=(const GameField &other);

    /**
     * @brief Returns copy of the game field without unharmed cells
     */
    GameField get_for_enemy() const;

    /**
     * @brief Processes the shot
     * @param point the shot coordinates
     * @return result of the shot
     */
    CellState register_shot(const Point &point);

    /**
     * @brief Returns the state of the cell on point
     * @param point coordinates
     */
    CellState get_state(const Point &point) const;

    /**
     * @brief Sets the state of the cell on pos
     * @param pos position of the cell
     * @param state the specified state
     */
    void set_point(const Point &pos, CellState state);

    /**
     * @brief Returns the char of the cell state for drawing
     * @param state state of the cell
     */
    static char get_cell_char(CellState state);

    /**
     * @brief Returns true if the field is empty, otherwise returns false
     */
    bool is_empty() const;

    /**
     * @brief Returns true if the field is valid for starting,
     * otherwise returns false
     */
    bool is_valid_start_field() const;

    /**
     * @brief Returns true if all ships on the field are destroyed,
     * otherwise returns false
     */
    bool all_ships_destroyed() const;
private:
    char field[FLD_SZ][FLD_SZ];

    static bool _is_valid_field(const char p_field[FLD_SZ][FLD_SZ]);
    static bool _is_cell_values_valid(const char p_field[FLD_SZ][FLD_SZ]);
    static void _remove_ship(const Ship& ship, char r_field[FLD_SZ][FLD_SZ]);

    bool _ship_destroyed(const Point &point) const;
    void _mark_destroyed_ship_boundary(const Point &point);
};

typedef GameField::CellState GFCellState;

#endif
