#ifndef SHIP_H
#define SHIP_H

#include "point.h"

enum ShipType {
    SUBMARINE_SHIP,        /**< #     */
    DESTROYER_SHIP,        /**< ##    */
    CRUISER_SHIP,          /**< ###   */
    BATTLESHIP_SHIP,       /**< ####  */
    AIRCRAFT_CARRIER_SHIP, /**< ##### */
    INVALID_SHIP
};

/**
 * @brief The ship class
 */
class Ship {
public:
    /**
     * @brief Returns the length of the passed type ship
     */
    static int ship_length(ShipType type);

    /**
     * @brief Returns the count of the ship on a game field
     */
    static int ship_count(ShipType type);

    /**
     * @brief Returns the type of the ship by length
     */
    static ShipType ship_type(int len);

    Ship();
    Ship(const Point& p_head, const Point& p_tail);
    Ship(const Ship &other);
    ~Ship();

    /**
     * @brief Returns the ship type
     */
    ShipType get_type() const;

    /**
     * @brief Returns the ship length
     * @return
     */
    int get_length() const;

    /**
     * @brief Returns true if the ship is valid and vertical,
     * otherwise returns false
     */
    bool is_vertical() const;

    /**
     * @brief Returns the head coordinates
     */
    Point get_head() const;

    /**
     * @brief Returns the tail coordinates
     */
    Point get_tail() const;

    /**
     * @brief Returns the ship direction vector
     */
    Point get_delta() const;

    /**
     * @brief Returns true if the ship contains the passed point
     */
    bool contains(const Point &point) const;

    /**
     * @brief The assignment operator
     */
    const Ship& operator=(const Ship &other);
private:
    ShipType type;
    Point head;
    Point tail;
};

#endif
