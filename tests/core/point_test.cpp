#include "CppUTest/TestHarness.h"
#include "point.h"

TEST_GROUP(PointGroup)
{
    Point *point = 0;
    Point *point_def = 0;
    int x = 143;
    int y = -33;
    void setup()
    {
        point = new Point(x, y);
        point_def = new Point;
    }
    void teardown()
    {
        delete point;
        delete point_def;
    }
};

TEST(PointGroup, DefaultConstructor)
{
    CHECK_EQUAL(0, point_def->x);
    CHECK_EQUAL(0, point_def->y);
}

TEST(PointGroup, ConstructorWithArgs)
{
    CHECK_EQUAL(x, point->x);
    CHECK_EQUAL(y, point->y);
}

TEST(PointGroup, ComparisonOperator)
{
    CHECK(*point == * point);
    CHECK(*point_def == *point_def);
    CHECK_FALSE(*point == *point_def);
    CHECK(*point != *point_def);
    CHECK_FALSE(*point != *point);
    CHECK_FALSE(*point_def != *point_def);
}

TEST(PointGroup, Swap)
{
    int x_def = point_def->x;
    int y_def = point_def->y;
    int x_point = point->x;
    int y_point = point->y;

    CHECK_EQUAL(x_def, point_def->x);
    CHECK_EQUAL(y_def, point_def->y);
    CHECK_EQUAL(x_point, point->x);
    CHECK_EQUAL(y_point, point->y);

    point->swap(*point_def);

    CHECK_EQUAL(x_point, point_def->x);
    CHECK_EQUAL(y_point, point_def->y);
    CHECK_EQUAL(x_def, point->x);
    CHECK_EQUAL(y_def, point->y);
}

TEST(PointGroup, Operators)
{
    Point tmp(34, 3);
    CHECK_FALSE(*point == tmp);
    tmp = *point;
    CHECK(*point == tmp);

    tmp -= *point;
    CHECK(tmp == *point_def);

    tmp += *point;
    CHECK(tmp == *point);
}
