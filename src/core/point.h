#ifndef POINT_H
#define POINT_H

/**
 * @brief The class of a point on the plane
 */
class Point {
public:
    int x;
    int y;

    Point();
    Point(int p_x, int p_y);
    Point(const Point &other);
    ~Point();

    void swap(Point &other);

    Point operator+(const Point &other) const;
    Point operator-(const Point &other) const;
    const Point& operator+=(const Point &other);
    const Point& operator-=(const Point &other);
    const Point& operator=(const Point &other);
    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;
};

#endif
