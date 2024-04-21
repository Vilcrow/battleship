#include "ncurses_interface.h"
#include "ncurses.h"

NcursesInterface::NcursesInterface()
    : loaded(false)
    , has_color(false)
    , timer(0)
{
    loaded = _load();
    if(loaded) {
        _init_curses_mode();
    }
    _move_cursor_to_own_field();
}

NcursesInterface::~NcursesInterface()
{
    endwin();
}

void NcursesInterface::show()
{
    _draw_background();
    _draw_own_field();
    _draw_enemy_field();
    _draw_game_state();
    _draw_server_conn_state();
    _draw_timer();
    refresh();
}

bool NcursesInterface::is_valid() const
{
    return loaded;
}

InputResult NcursesInterface::process_input()
{
    timeout(100);
    int key = getch();

    switch(key) {
    case KEY_UP:
    case KEY_DOWN:
    case KEY_LEFT:
    case KEY_RIGHT:
        return _process_move_input(key);
    case ' ':
        return _process_enter_input();
    case 's':
        return REQ_GAME_SESSION_IN;
    case 'c':
        return REQ_SERVER_CONN_IN;
    case 'q':
        return QUIT_IN;
    case ERR:
        break;
    }
    return INVALID_IN;
}

InputResult NcursesInterface::_process_enter_input()
{
    if(mode == PREPARATION_MODE) {
        Point pos_on_field = cursor.get_related_pos();
        if(!own_field.contains(pos_on_field)) {
            return INVALID_IN;
        }

        set_last_shot(pos_on_field);
        if(own_field.get_state(pos_on_field) == GameField::CELL_EMPTY) {
            own_field.set_point(pos_on_field, GameField::CELL_UNHARMED);
        }
        else {
            own_field.set_point(pos_on_field, GameField::CELL_EMPTY);
        }

        return PREPARE_FIELD_IN;
    }
    else if(mode == OWN_TURN_MODE) {
        Point shot_pos = cursor.get_related_pos();
        GFCellState pos_state = enemy_field.get_state(shot_pos);
        if(pos_state != GameField::CELL_EMPTY) {
            return INVALID_IN;
        }
        set_last_shot(shot_pos);
        mode = WAIT_MODE;
        return SHOT_IN;
    }
    return INVALID_IN;
}

InputResult NcursesInterface::_process_move_input(int key)
{
    if(mode != PREPARATION_MODE && mode != OWN_TURN_MODE) {
        return INVALID_IN;
    }

    switch(key) {
    case KEY_UP:
        cursor.move(Point(0, -1));
        break;
    case KEY_DOWN:
        cursor.move(Point(0, 1));
        break;
    case KEY_LEFT:
        cursor.move(Point(-1, 0));
        break;
    case KEY_RIGHT:
        cursor.move(Point(1, 0));
        break;
    default:
        return INVALID_IN;
    }
    return CURSOR_MOVE_IN;
}

void NcursesInterface::set_game_state(GameState p_state)
{
    state = p_state;
    _draw_game_state();
}

void NcursesInterface::set_server_conn_state(bool connected)
{
    has_sever_conn = connected;
    _draw_server_conn_state();
}

void NcursesInterface::activate_preparation_mode(time_t end_time)
{
    set_mode(PREPARATION_MODE);
    _move_cursor_to_own_field();
    cursor.show();
    time_t cur_time = time(NULL);
    time_t duration = end_time - cur_time;
    timer.set_duration(duration);
    timer.start();
}

void NcursesInterface::stop_preparation_mode()
{
    if(mode != PREPARATION_MODE) {
        return;
    }

    timer.stop();
    set_mode(WAIT_MODE);
    set_last_shot(Point(0, 0));
    cursor.hide();
}

void NcursesInterface::activate_own_turn_mode(time_t end_time)
{
    set_mode(OWN_TURN_MODE);
    _move_cursor_to_enemy_field();
    cursor.set_related_pos(get_last_shot());
    cursor.show();
    time_t cur_time = time(NULL);
    time_t duration = end_time - cur_time;
    timer.set_duration(duration);
    timer.start();
}

void NcursesInterface::stop_own_turn_mode()
{
    set_mode(WAIT_MODE);
    cursor.hide();
    timer.stop();
}

void NcursesInterface::activate_enemy_turn_mode(time_t end_time)
{
    set_mode(ENEMY_TURN_MODE);
    cursor.hide();
    time_t cur_time = time(NULL);
    time_t duration = end_time - cur_time;
    timer.set_duration(duration);
    timer.start();
}

void NcursesInterface::stop_enemy_turn_mode()
{
    set_mode(WAIT_MODE);
    timer.stop();
}

void NcursesInterface::_draw_server_conn_state()
{
    _clear_server_state();
    move(SERVER_STATE_Y, SERVER_STATE_X);
    const char *status;
    ColorPair col_pair;
    if(has_sever_conn) {
        status = ON_TXT;
        col_pair = OK_COL_PAIR;
    }
    else {
        status = OFF_TXT;
        col_pair = ERROR_COL_PAIR;
    }
    _on_color(col_pair);
    addstr(status);
    _off_color(col_pair);
    cursor.return_to_pos();
}

void NcursesInterface::_draw_game_state()
{
    _clear_game_state();
    move(GAME_STATUS_Y, GAME_STATUS_X);
    const char *str = "";
    switch(state) {
    case NO_VACANT_GAME:
        str = "No game";
        break;
    case WAIT_PLAYER:
        str = "Wait opponent";
        break;
    case WAIT_PREPARATION:
        str = "Wait preparation";
        break;
    case PREPARATION:
        str = "Preparation";
        break;
    case WAIT_START:
        str = "Wait start";
        break;
    case GAME_TURN:
        str = "In game";
        break;
    case PROCESS_TURN:
        str = "Process turn";
        break;
    case OWN_TURN:
        str = "Your turn";
        break;
    case ENEMY_TURN:
        str = "Enemies turn";
        break;
    case WIN_GAME:
        str = "You win!";
        break;
    case LOSE_GAME:
        str = "You lose";
        break;
    }
    addstr(str);
    cursor.return_to_pos();
}

void NcursesInterface::_draw_timer()
{
    if(!timer.is_active()) {
        return;
    }

    time_t sec = timer.remain();
    if(sec == 0) {
        return;
    }

    _clear_timer();

    int min = sec / 60;
    sec -= min * 60;
    int str_len = 6;
    char buf[str_len];
    snprintf(buf, str_len, "%02d:%02ld", min, sec);
    ::move(TIMER_Y, TIMER_X);
    addstr(buf);
    cursor.return_to_pos();
}

bool NcursesInterface::_load()
{
    FILE *interface = fopen(INTERFACE_PATH, "r");
    if(!interface) {
        return false;
    }

    int y, x, c;
    for(y = 0; y < HEIGHT; y++) {
        for(x = 0; x < WIDTH; x++) {
            c = fgetc(interface);
            if(c == '\n') {
                c = fgetc(interface);
            }
            if(c == EOF) {
                return false;
            }
            background_img[x][y] = c;
        }
    }
    fclose(interface);
    return true;
}

void NcursesInterface::_init_curses_mode()
{
    initscr();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    cursor.hide();

    if(has_colors()) {
        start_color();
        _init_color_pairs();
        has_color = true;
    }
}

void NcursesInterface::_init_color_pairs()
{
    init_pair(ERROR_COL_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(OK_COL_PAIR, COLOR_GREEN, COLOR_BLACK);
}

void NcursesInterface::_on_color(ColorPair cp)
{
    attron(COLOR_PAIR(cp));
}

void NcursesInterface::_off_color(ColorPair cp)
{
    attroff(COLOR_PAIR(cp));
}

void NcursesInterface::_clear_all()
{
    _clear_own_field();
    _clear_enemy_field();
    _clear_server_state();
    _clear_game_state();
    _clear_user_msg();
    _clear_timer();
}

void NcursesInterface::_clear_own_field()
{
    for(int i = 0; i < FIELD_SIZE; i++) {
        move(OWN_FIELD_START_Y + i, OWN_FIELD_START_X);
        for(int j = 0; j < FIELD_SIZE; j++) {
            addch(' ');
        }
    }
    refresh();
}

void NcursesInterface::_clear_enemy_field()
{
    for(int i = 0; i < FIELD_SIZE; i++) {
        move(ENEMY_FIELD_START_Y + i, ENEMY_FIELD_START_X);
        for(int j = 0; j < FIELD_SIZE; j++) {
            addch(' ');
        }
    }
    refresh();
}

void NcursesInterface::_clear_server_state()
{
    move(SERVER_STATE_Y, SERVER_STATE_X);
    for(int i = 0; i < FIELD_SIZE; i++) {
        addch(' ');
    }
    refresh();
}

void NcursesInterface::_clear_game_state()
{
    move(GAME_STATUS_Y, GAME_STATUS_X);
    for(int i = 0; i < GAME_STATUS_WIDTH; i++) {
        addch(' ');
    }
    refresh();
}

void NcursesInterface::_clear_user_msg()
{
    move(USER_MSG_Y, USER_MSG_X);
    for(int i = 0; i < USER_MSG_WIDTH; i++) {
        addch(' ');
    }
    refresh();
}

void NcursesInterface::_clear_timer()
{
    move(TIMER_Y, TIMER_X);
    for(int i = 0; i < TIMER_WIDTH; i++) {
        addch(' ');
    }
    refresh();
}

void NcursesInterface::_draw_background()
{
    int y, x;
    for(y = 0; y < HEIGHT; y++) {
        for(x = 0; x < WIDTH; x++) {
            mvaddch(y, x, background_img[x][y]);
        }
    }
}

void NcursesInterface::_draw_own_field()
{
    Point pos;
    Point offset(OWN_FIELD_START_X, OWN_FIELD_START_Y);
    for(int i = 0; i < FIELD_SIZE; i++) {
        pos = Point(OWN_FIELD_START_X, OWN_FIELD_START_Y+i);
        for(int j = 0; j < FIELD_SIZE; j++) {
            Point pos_on_field = pos - offset;
            GameField::CellState state = own_field.get_state(pos_on_field);
            char ch = own_field.get_cell_char(state);
            mvaddch(pos.y, pos.x, ch);
            pos += Point(1, 0);
        }
    }
    cursor.return_to_pos();
}

void NcursesInterface::_draw_enemy_field()
{
    Point pos;
    Point offset(ENEMY_FIELD_START_X, ENEMY_FIELD_START_Y);
    for(int i = 0; i < FIELD_SIZE; i++) {
        pos = Point(ENEMY_FIELD_START_X, ENEMY_FIELD_START_Y+i);
        for(int j = 0; j < FIELD_SIZE; j++) {
            Point pos_on_field = pos - offset;
            GameField::CellState state = enemy_field.get_state(pos_on_field);
            char ch = enemy_field.get_cell_char(state);
            mvaddch(pos.y, pos.x, ch);
            pos += Point(1, 0);
        }
    }
    cursor.return_to_pos();
}

void NcursesInterface::_move_cursor_to_own_field()
{
    Point cursor_min(OWN_FIELD_START_X, OWN_FIELD_START_Y);
    Point cursor_max(OWN_FIELD_START_X+FIELD_SIZE,
                     OWN_FIELD_START_Y+FIELD_SIZE);
    cursor.set_boundary(cursor_min, cursor_max);
}

void NcursesInterface::_move_cursor_to_enemy_field()
{
    Point cursor_min(ENEMY_FIELD_START_X, ENEMY_FIELD_START_Y);
    Point cursor_max(ENEMY_FIELD_START_X+FIELD_SIZE,
                     ENEMY_FIELD_START_Y+FIELD_SIZE);
    cursor.set_boundary(cursor_min, cursor_max);
}
