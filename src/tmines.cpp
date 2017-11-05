#include <iostream>

#include "display.hpp"

#include <string>
#include <argp.h>
#include <stdexcept>
#include <curses.h>
#include <iostream>
#include <random>
#include <cstdlib>

struct {
    int width = 8;
    int height = 8;
    int mine_count = -1;
    int seed = -1;
    bool autodiscover_only = false;
} opts;

bool has_only_digits(const std::string s){
  return s.find_first_not_of( "0123456789" ) == std::string::npos;
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    switch (key) {
        case 'x':
        case 'w':
            if (has_only_digits(arg)) {
                opts.width = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;
        
        case 'y':
        case 'h':
            if (has_only_digits(arg)) {
                opts.height = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;

        case 'c':
            if (has_only_digits(arg)) {
                opts.mine_count = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;

        case 's':
            if (has_only_digits(arg)) {
                opts.seed = std::atoi(arg);
            } else {
                argp_failure(state, 1, 0, "Argument must be number");
            }
            break;

        case 'a':
            opts.autodiscover_only = true;
            break;
    }

    return 0;
}

void run() {
    // seed not initialized? get one from hardware
    if (-1 == opts.seed) {
        opts.seed = std::abs((int) std::random_device()());
    }

    if (-1 == opts.mine_count) {
        opts.mine_count = opts.height * opts.width * 0.16;
    }
    
    Display(opts.width, opts.height, opts.mine_count, opts.seed, opts.autodiscover_only);
}

int main(int argc, char** argv) {
    struct argp_option options[] = {
        {"width", 'w', "NUM", 0, "width of the minefield, default: 8", 0},
        {0, 'x', 0, OPTION_ALIAS, 0, 0},
        {"height", 'h', "NUM", 0, "height of the minefield, default: 8", 0},
        {0, 'y', 0, OPTION_ALIAS, 0, 0},
        {"mine-count", 'c', "NUM", 0, "number of mines to be placed, default: 16%", 0},
        {"count", 0, 0, OPTION_ALIAS, 0, 0},
        {"autodiscover-only", 'a', 0, 0, "if enabled: fields can only be opened using autodiscover feature (see man)", 0},
        {"seed", 's', "NUM", 0, "seed for field generation, suitable seed will be chosen by automatically", 0},
        {0, 0, 0, 0, 0, 0}
    };
    struct argp argp = {options, parse_opt, 0, "Play Minesweeper on the terminal.\vControls:\nMovement    WASD, Arrow Keys, vimlike\nFlag Mine   F\nOpen Field  Space\nQuit        Q", 0, 0, 0};

    int argp_state = argp_parse(&argp, argc, argv, 0, 0, 0);

    try{
        run();
    } catch (std::exception& e) {
        endwin();
        std::cerr << "Unfortunately, an error occured:" << std::endl;
        std::cerr << e.what() << std::endl << std::endl;
        std::cerr << "Settings:" << std::endl;
        std::cerr << "  Width, Height: " << opts.width << ", " << opts.height << std::endl;
        std::cerr << "  Mine Count:    " << opts.mine_count << std::endl;
        std::cerr << "  Seed:          " << opts.seed << std::endl;
        std::cerr << "  Args:";
        for (int i = 1; i < argc; i++) {
            std::cerr << " " << argv[i];
        }
        std::cerr << std::endl;
    }

    return argp_state;
}