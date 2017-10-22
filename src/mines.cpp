#include <iostream>

#include "display.hpp"

#include <string>
#include <argp.h>
#include <stdexcept>
#include <curses.h>
#include <iostream>
#include <random>

struct {
    int width = 8;
    int height = 8;
    int mine_count = -1;
    int seed = -1;
} opts;

bool has_only_digits(const std::string s){
  return s.find_first_not_of( "0123456789" ) == std::string::npos;
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    switch (key) {
        case 'x':
        case 'w':
            if (has_only_digits(arg) && "0" != std::string(arg)) {
                opts.width = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;
        
        case 'y':
        case 'h':
            if (has_only_digits(arg) && "0" != std::string(arg)) {
                opts.height = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;

        case 'c':
            if (has_only_digits(arg) && "0" != std::string(arg)) {
                opts.mine_count = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;

        case 's':
            if (has_only_digits(arg) && "0" != std::string(arg)) {
                opts.seed = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;
    }

    return 0;
}

void run() {
    // seed not initialized? get one from hardware
    if (-1 == opts.seed) {
        opts.seed = std::random_device()();
    }

    if (-1 == opts.mine_count) {
        opts.mine_count = opts.height * opts.width * 0.16;
    }
    
    try {
        Display(opts.width, opts.height, opts.mine_count, opts.seed);
    } catch (std::exception& e) {
        endwin();
        std::cout << "Unfortunately, an error occured:" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    struct argp_option options[] = {
        {"width", 'w', "NUM", 0, "width of the minefield, default: 8", 0},
        {0, 'x', 0, OPTION_ALIAS, 0, 0},
        {"height", 'h', "NUM", 0, "height of the minefield, default: 8", 0},
        {0, 'y', 0, OPTION_ALIAS, 0, 0},
        {"mine-count", 'c', "NUM", 0, "number of mines to be placed, default: 16%", 0},
        {"count", 0, 0, OPTION_ALIAS, 0, 0},
        {"seed", 's', "NUM", 0, "seed for field generation, suitable seed will be chosen by automatically", 0},
        {0, 0, 0, 0, 0, 0}
    };
    struct argp argp = {options, parse_opt, 0, "Play Minesweeper on the terminal.\vControls:\nMovement    WASD, Arrow Keys, vimlike\nFlag Mine   F\nOpen Field  Space\nQuit        Q", 0, 0, 0};

    int argp_state = argp_parse(&argp, argc, argv, 0, 0, 0);

    run();

    return argp_state;
}
