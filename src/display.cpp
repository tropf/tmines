#include "display.hpp"

#include "controller.hpp"

#include <iostream>
#include <curses.h>
#include <string>
#include <algorithm>
#include <exception>
#include <iostream>

#include <chrono>

void Display::renderBoard() {
    auto mfield = controller.getMinefield();
    for (int x = 0; x < mfield.getXDimension(); x++) {
        for (int y = 0; y < mfield.getYDimension(); y++) {
            if (state[x][y] != last_state[x][y]) {
                int color;
                char to_print;
                std::tie(color, to_print) = state[x][y];

                color_set(color, 0);

                int x_to_print, y_to_print;
                std::tie(x_to_print, y_to_print) = getConsolePosition(x, y);
                mvaddch(y_to_print, x_to_print, to_print);

                last_state[x][y] = state[x][y];
            }
        }
    }
}

void Display::calculateStates() {
    auto mfield = controller.getMinefield();
    
    for (int x = 0; x < mfield.getXDimension(); x++) {
        for (int y = 0; y < mfield.getYDimension(); y++) {
            char to_print;
            int color;
            if (mfield.isOpen(x, y)) {
                auto sourrounding_mine_count = mfield.getSorroundingMineCount(x, y);
                color = sourrounding_mine_count;
                to_print = std::to_string(sourrounding_mine_count).c_str()[0];
                if ('0' == to_print) {
                    to_print = ' ';
                }

                if (mfield.isMine(x, y)) {
                    color = 11;
                    to_print = 'X';
                }
            } else if (mfield.isGameLost() && mfield.isMine(x, y) && !mfield.isFlagged(x, y)) {
                // mark only unflagged mines
                color = 11;
                to_print = 'X';
            } else if (mfield.isFlagged(x, y)) {
                color = 0;
                to_print = '?';
                if (mfield.isGameEnded() && !mfield.isMine(x, y)) {
                    // incorrect mine -> make red
                    color = 11;
                }
            } else {
                color = 10;
                to_print = '*';
            }

            auto tuple_to_print = std::make_tuple(color, to_print);
            if (state[x][y] != tuple_to_print) {
                state[x][y] = tuple_to_print;
            }
        }
    }
}

std::tuple<int, int> Display::getConsolePosition(int x, int y) {
    return std::make_tuple(1 + x * 2, y);
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int Display::getMaxTextWidth() {
    std::vector<int> all_lengths = {0};
    
    all_lengths.push_back(msgs.won.length());
    all_lengths.push_back(msgs.lost.length());
    all_lengths.push_back(msgs.running.length());

    auto max_remaining_mines = msgs.remaining_mines;
    replace(max_remaining_mines, "%mine_count%", std::to_string(controller.getMinefield().getMineCount()));

    all_lengths.push_back(max_remaining_mines.length());

    auto result = std::max_element(all_lengths.begin(), all_lengths.end());
    auto max_index = std::distance(all_lengths.begin(), result);
    
    return all_lengths[max_index];
}

void Display::renderStatusline() {
    std::string remaining_mines;
    std::string game_state;
    short color_to_use = 0;

    std::string remaining_mines_number_only;

    auto mfield = controller.getMinefield();
    if (mfield.isGameWon() || mfield.getFlagCount() > mfield.getMineCount()) {
        remaining_mines_number_only = "0";
    } else {
        remaining_mines_number_only = std::to_string(mfield.getMineCount() - mfield.getFlagCount());
    }

    remaining_mines = msgs.remaining_mines;
    replace(remaining_mines, "%mine_count%", remaining_mines_number_only);

    if (mfield.isGameRunning()) {
        color_to_use = 12;
        game_state = msgs.running;
    } else {
        if (mfield.isGameWon()) {
            color_to_use = 13;
            game_state = msgs.won;
        } else {
            color_to_use = 11;
            game_state = msgs.lost;
        }
    }

    int x, y;
    std::tie(x, y) = getConsolePosition(0, mfield.getYDimension() + 1);

    color_set(0, 0);
    mvaddstr(y, x, std::string(getMaxTextWidth(), ' ').c_str());

    color_set(color_to_use, 0);
    mvaddstr(y, x, game_state.c_str());

    std::tie(x, y) = getConsolePosition(0, mfield.getYDimension() + 2);
    color_set(0, 0);
    mvaddstr(y, x, std::string(getMaxTextWidth(), ' ').c_str());
    color_set(12, 0);
    mvaddstr(y, x, remaining_mines.c_str());
}

int Display::getKey() {
    return getch();
}

void Display::handleKey(int key) {
    if ('q' == key || 'Q' == key) {
        pressed_keys.push_back('q');
        exit = true;
    } else if (' ' == key || KEY_ENTER == key) {
        pressed_keys.push_back(' ');
        controller.click();
    } else if ('w' == key || KEY_UP == key || 'W' == key || 'k' == key || 'K' == key) {
        pressed_keys.push_back('k');
        controller.moveUp();
    } else if ('a' == key || KEY_LEFT == key || 'A' == key || 'h' == key || 'H' == key) {
        pressed_keys.push_back('h');
        controller.moveLeft();
    } else if ('s' == key || KEY_DOWN == key || 'S' == key || 'j' == key || 'J' == key) {
        pressed_keys.push_back('j');
        controller.moveDown();
    } else if ('d' == key || KEY_RIGHT == key || 'D' == key || 'l' == key || 'L' == key) {
        pressed_keys.push_back('l');
        controller.moveRight();
    } else if ('f' == key || 'F' == key) {
        pressed_keys.push_back('f');
        controller.tooggleFlag();
    } else if (KEY_RESIZE == key) {
        checkWindowSize();
    }
}

void Display::updateCursor() {
    int x, y;
    std::tie(x, y) = getConsolePosition(controller.getX(), controller.getY());
    move(y, x);

    if (controller.getMinefield().isGameEnded()) {
        curs_set(0);
    } else {
        curs_set(1);
    }
}

void Display::checkWindowSize() {
    int field_width, field_height;
    std::tie(field_width, field_height) = getConsolePosition(controller.getMinefield().getXDimension() - 1, controller.getMinefield().getYDimension() - 1);

    int max_text_width = getMaxTextWidth();

    int text_height;
    text_height = std::get<1>(getConsolePosition(controller.getMinefield().getXDimension() - 1, controller.getMinefield().getYDimension() + 2));

    int required_width = field_width;
    int required_height = field_height;

    if (max_text_width > required_width) {
        required_width = max_text_width;
    }

    if (text_height > required_height) {
        required_height = text_height;
    }

    // the output until now are coordinates to write
    // conver to amount of cols/rows
    required_width++;
    required_height++;

    if (LINES < required_height || COLS < required_width) {
        throw std::runtime_error(std::to_string(required_width) + "x" + std::to_string(required_height) + " terminal required to display this minefield (Current: " + std::to_string(COLS) + "x" + std::to_string(LINES) + ")");
    }
}

void Display::run() {
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_CYAN, COLOR_BLACK);
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    init_pair(11, COLOR_WHITE, COLOR_RED);
    init_pair(12, COLOR_YELLOW, COLOR_BLACK);
    init_pair(13, COLOR_GREEN, COLOR_BLACK);

    bkgd(COLOR_PAIR(10));

    checkWindowSize();

    unsigned long calc_time = 0;
    unsigned long draw_time = 0;
    unsigned long handle_time = 0;

    while(!exit) {
        // rendering process
        curs_set(0);
        auto time_before_calc = std::chrono::system_clock::now();
        calculateStates();
        auto time_before_render = std::chrono::system_clock::now();
        renderBoard();
        auto time_after_render = std::chrono::system_clock::now();
        renderStatusline();
        updateCursor();
        auto time_before_handle = std::chrono::system_clock::now();
        handleKey(getKey());
        auto time_after_handle = std::chrono::system_clock::now();

        calc_time += std::chrono::duration_cast<std::chrono::nanoseconds>(time_before_render - time_before_calc).count();
        draw_time += std::chrono::duration_cast<std::chrono::nanoseconds>(time_after_render - time_before_render).count();
        handle_time += std::chrono::duration_cast<std::chrono::nanoseconds>(time_after_handle - time_before_handle).count();
    }

    throw std::runtime_error("Calc Time:\t" + std::to_string(calc_time) + " ns\nRender Time:\t" + std::to_string(draw_time) + " ns\nHandle Time:\t" + std::to_string(handle_time) + " ns");
}

Display::Display(int width, int height, int mine_count, int seed, bool autodiscover_only) {
    controller = Controller(width, height, mine_count, seed, autodiscover_only);
    exit = false;

    // init state vars
    for (int x = 0; x < width; x++) {
        std::vector<std::tuple<int, char>> column;
        for (int y = 0; y < height; y++) {
            column.push_back(std::make_tuple(0, '*'));
        }
        state.push_back(column);
    }
    
    last_state = state;

    initscr();
    noecho();
    start_color();
    keypad(stdscr, TRUE);
    cbreak();

    try {
        run();
    } catch (std::runtime_error& e) {
        std::string msg = e.what();
        std::string keys_msg = "";
        keys_msg = "Keypresses for error: ";
        for (auto key : pressed_keys) {
            keys_msg += std::to_string((int) key) + ", ";
        }

        throw std::runtime_error(msg + "\n\n" + keys_msg);
    }

    endwin();
}

