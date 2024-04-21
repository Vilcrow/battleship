#ifndef CURSOR_H
#define CURSOR_H

#include "point.h"

/**
 * @brief The Cursor class
 */
class Cursor {
public:
    /**
     * @brief Cursor
     */
    Cursor();

    /**
     * @brief Cursor
     * @param p_min
     * @param p_max
     */
    Cursor(const Point &p_min, const Point &p_max);

    /*
     * @brief Destructor
     */
    ~Cursor();

    /**
     * @brief get_pos
     * @return
     */
    const Point& get_pos() const;

    /**
     * @brief set_pos
     * @param p_pos
     */
    void set_pos(const Point &p_pos);

    /**
     * @brief set_related_pos
     * @param p_pos
     */
    void set_related_pos(const Point &p_pos);

    // TODO: Rename?
    /**
     * @brief get_related_pos
     * @return
     */
    Point get_related_pos() const;

    /**
     * @brief move
     * @param offset
     */
    void move(const Point &offset);

    /**
     * @brief show
     */
    void show();

    /**
     * @brief hide
     */
    void hide();

    /**
     * @brief set_boundary
     * @param p_min
     * @param p_max
     */
    void set_boundary(const Point &p_min, const Point &p_max);

    /**
     * @brief return_to_pos
     */
    void return_to_pos();
private:
    Point pos;

    // Left up corner.
    Point min;
    // Right down corner.
    Point max;

    void _check_pos();
    static bool _is_valid_boundary(const Point &p_min, const Point &p_max);
};

#endif
