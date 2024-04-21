#include "CppUTest/TestHarness.h"
#include <stdlib.h>
#include "game_field.h"

#define FSIZE GameField::FLD_SZ

static const int FIELD_CNT = 3;

static const char* valid_fields[FIELD_CNT] =
            { "fields/valid_1"
            , "fields/valid_2"
            , "fields/valid_3"
            };

static const char* invalid_fields[FIELD_CNT] =
            { "fields/invalid_1"
            , "fields/invalid_2"
            , "fields/invalid_3"
            };

static void fill_field(const char *name, GameField &field)
{
    FILE *file = fopen(name, "r");
    CHECK_TEXT(file, "Can't open the field file");
    if(!file) {
        return;
    }

    char arr[FSIZE][FSIZE];
    int c;
    for(int i = 0; i < FSIZE; ++i) {
        for(int j = 0; j < FSIZE; ++j) {
            c = fgetc(file);
            arr[i][j] = (c == GameField::CHAR_EMPTY) ? GameField::CELL_EMPTY
                                                     : GameField::CELL_UNHARMED;
            if(j == FSIZE - 1) {
                while((c = fgetc(file)) != '\n') {
                    ;
                }
            }
        }
    }
    field.fill(arr);
    fclose(file);
}

TEST_GROUP(GameFieldGroup)
{
    GameField *field = 0;
    GameField *field_cp = 0;

    void setup()
    {
        field = new GameField;
        field_cp = new GameField(*field);
    }

    void teardown()
    {
        delete field;
        delete field_cp;
    }
};

TEST(GameFieldGroup, DefaultConstructor)
{
    CHECK_FALSE(field->is_valid_start_field());
    CHECK(field->all_ships_destroyed());
    CHECK(field->is_empty());
}

TEST(GameFieldGroup, CopyConstructor)
{
    CHECK_FALSE(field_cp->is_valid_start_field());
    CHECK(field_cp->all_ships_destroyed());
    CHECK(field_cp->is_empty());
}

TEST(GameFieldGroup, ValidFields)
{
    CHECK(field->is_empty());
    CHECK_FALSE(field->is_valid_start_field());
    for(int i = 0; i < FIELD_CNT; i++) {
        fill_field(valid_fields[i], *field);
        CHECK_TEXT(field->is_valid_start_field(), valid_fields[i]);
        CHECK_FALSE(field->all_ships_destroyed());
        CHECK_FALSE(field->is_empty());
    }
}

TEST(GameFieldGroup, InvalidFields)
{
    CHECK(field->is_empty());
    CHECK_FALSE(field->is_valid_start_field());
    for(int i = 0; i < FIELD_CNT; i++) {
        fill_field(invalid_fields[i], *field);
        CHECK_FALSE_TEXT(field->is_valid_start_field(), invalid_fields[i]);
        CHECK_FALSE(field->all_ships_destroyed());
        CHECK_FALSE(field->is_empty());
    }
}

TEST(GameFieldGroup, GetForEnemy)
{
    CHECK(field->is_empty());
    fill_field(valid_fields[0], *field);
    CHECK_FALSE(field->is_empty());
    GameField for_enemy = field->get_for_enemy();
    CHECK(for_enemy.is_empty());
}
