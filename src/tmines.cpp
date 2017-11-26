#include "display.hpp"
#include "config.h"
#include "iodevice.hpp"
#include "iodevice_curses.cpp"

#include <string>
#include <argp.h>
#include <stdexcept>
#include <curses.h>
#include <iostream>
#include <random>
#include <cstdlib>
#include <fstream>
#include <sstream>

const char* argp_program_bug_address = TerminateMines_BUG_ADDRESS;
const char* argp_program_version = "version " TerminateMines_VERSION_MAJOR "." TerminateMines_VERSION_MINOR " (commit " TerminateMines_GIT_COMMIT_HASH ")";

struct {
    int width = -1;
    int height = -1;
    int mine_count = -1;
    int seed = -1;
    int mine_density = -1;
    bool autodiscover_only = false;
    bool fullscreen = false;
} opts;

bool has_only_digits(const std::string s){
  return s.find_first_not_of( "0123456789" ) == std::string::npos;
}

static int parse_opt(int key, char* arg, struct argp_state* state) {
    // check for incompatibilities
    if ((-1 != opts.height || -1 != opts.width) && opts.fullscreen) {
        argp_error(state, "can't set window size (height & width) explicitly and use fullscreen option at the same time");
    }

    if (-1 != opts.mine_count && -1 != opts.mine_density) {
        argp_error(state, "can't specify mine count explicitly and set mine density at the same time");
    }

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

        case 'd':
            if (has_only_digits(arg)) {
                opts.mine_density = std::atoi(arg);
                if (0 > opts.mine_density || opts.mine_density > 100) {
                    argp_failure(state, 1, 0, "mine density must be between 0 and 100 (percent)");
                }
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

        case 'f':
            opts.fullscreen = true;
            break;

        case 'a':
            opts.autodiscover_only = true;
            break;
    }

    return 0;
}

int get_minecount_for_size(int width = -1, int height = -1) {
    if (-1 == width) {
        width = opts.width;
    }
    if (-1 == height) {
        height = opts.height;
    }

    if (-1 == opts.mine_density) {
        opts.mine_density = 16;
    }

    if (-1 != opts.mine_count) {
        return opts.mine_count;
    } else {
        return (width * height * opts.mine_density) / 100;
    }
}

void run() {
    // seed not initialized? get one from hardware
    if (-1 == opts.seed) {
        opts.seed = std::abs((int) std::random_device()());
    }

    if (-1 == opts.height) {
        opts.height = 10;
    }

    if (-1 == opts.width) {
        opts.width = 10;
    }

    if (opts.fullscreen) {
        // initalize LINES and COLS vars
        initscr();
        endwin();

        int max_mine_count;
        std::tie(opts.width, opts.height, max_mine_count) = Display::getMaximumFieldsize(COLS, LINES);

        if (get_minecount_for_size(opts.width, opts.height) > max_mine_count) {
            opts.mine_count = get_minecount_for_size(opts.width, opts.height);
        }
    }

    if (-1 == opts.mine_count) {
        opts.mine_count = get_minecount_for_size(opts.width, opts.height);
    }

    std::shared_ptr<IODevice> iodevice_ptr = std::make_shared<IODeviceCurses>(IODeviceCurses());
    Display(iodevice_ptr, opts.width, opts.height, opts.mine_count, opts.seed, opts.autodiscover_only);
}

int main(int argc, char** argv) {
    struct argp_option options[] = {
        {0, 0, 0, 0, "Field Size", 10},
        {"width", 'w', "WIDTH", 0, "width of the minefield, default: 10", 10},
        {0, 'x', 0, OPTION_ALIAS, 0, 10},
        {"height", 'h', "HEIGHT", 0, "height of the minefield, default: 10", 10},
        {0, 'y', 0, OPTION_ALIAS, 0, 10},
        {"fullscreen", 'f', 0, 0, "use full screen size", 10},
        {"", 0, 0, OPTION_DOC, "note: use either fullscreen or specify size explicitly (not both)", 10},

        {0, 0, 0, 0, "Mines", 20},
        {"mine-count", 'c', "NUM", 0, "number of mines to be placed", 20},
        {"count", 0, 0, OPTION_ALIAS | OPTION_HIDDEN, 0, 20},
        {"mine-density", 'd', "PERCENTAGE", 0, "density of mines to be placed, default: 16%. Specify as number between 0 and 100 (for percentages), omit '\%' charater", 20},
        {"density", 0, 0, OPTION_ALIAS | OPTION_HIDDEN, 0, 20},
        {"", 0, 0, OPTION_DOC, "note: use either mine density or specify mine count explicitly (not both)", 20},

        {0, 0, 0, 0, "Other", 30},
        {"autodiscover-only", 'a', 0, 0, "if enabled: fields can only be opened using autodiscover feature (see man)", 30},
        {"seed", 's', "SEED", 0, "seed for field generation, suitable seed will be chosen by automatically", 30},
        {0, 0, 0, 0, 0, 0}
    };
    struct argp argp = {options, parse_opt, 0, "Play Minesweeper on the terminal.\vControls:\nMovement    WASD, Arrow Keys, vimlike (HJKL)\nFlag Mine   F\nOpen Field  Space\nQuit        Q", 0, 0, 0};

    int argp_state = argp_parse(&argp, argc, argv, 0, 0, 0);

    try{
        run();
    } catch (std::exception& e) {
        std::string err_report = "";
        err_report += "Error Report\n\n";
        err_report += "=====================================\n";
        err_report += "Please send this to \"" + std::string(TerminateMines_BUG_ADDRESS) + "\"\n";
        err_report += "Thank you!\n";
        err_report += "=====================================\n\n";
        err_report += "Message:\n";
        err_report += "  " + std::string(e.what()) + "\n\n";
        err_report += "Settings:\n";
        err_report += "  Width, Height:     " + std::to_string(opts.width) + ", " + std::to_string(opts.height) + "\n";
        err_report += "  Mine Count:        " + std::to_string(opts.mine_count) + "\n";
        err_report += "  Seed:              " + std::to_string(opts.seed) + "\n";
        err_report += "  Autodiscover only: ";
        if (opts.autodiscover_only) {
            err_report += "enabled\n";
        } else {
            err_report += "disabled\n";
        }
        err_report += "  Fullscreen:        ";
        if (opts.fullscreen) {
            err_report += "enabled\n";
        } else {
            err_report += "disabled\n";
        }
        err_report += "  Args:             ";
        for (int i = 1; i < argc; i++) {
            err_report += " " + std::string(argv[i]);
        }
        err_report += "\n\n";

        err_report += "Terminal:\n";
        err_report += "  Lines: " + std::to_string(LINES) + "\n";
        err_report += "  Cols:  " + std::to_string(COLS) + "\n";

        err_report += "\n";

        err_report += "Version\n";
        err_report += "  Version Number: " + std::string(TerminateMines_VERSION_MAJOR) + "." + std::string(TerminateMines_VERSION_MINOR) + "\n";
        err_report += "  Git Commit:     " + std::string(TerminateMines_GIT_COMMIT_HASH) + "\n";

        // end window here, as COLS ans ROWS are read during creation of error report
        endwin();

        std::cerr << "Unfourtunately an Error occured:" << std::endl;
        // only print first line:
        std::istringstream err_msg_stream(e.what());
        std::string firstline;
        std::getline(err_msg_stream, firstline);
        std::cerr << "  " << firstline << std::endl << std::endl;

        try{
            std::ofstream report_file;
            report_file.open(CRASH_REPORT_FILE);

            if (! report_file.good()) {
                throw std::runtime_error("Can't open file \"" + std::string(CRASH_REPORT_FILE) + "\"");
            }
            report_file << err_report;
            
            std::cerr << "A crash report has been written to the file \"" << CRASH_REPORT_FILE <<"\"" << std::endl;
            std::cerr << "If you think that error should not be popping up, please send an email to \"" << TerminateMines_BUG_ADDRESS << "\" and attach the file \"" << CRASH_REPORT_FILE << "\"." << std::endl;
            std::cerr << "Thank you!" << std::endl;
        } catch (std::exception& inner_error) {
            std::cerr << "The error report could not be written to a file due to:" << std::endl;
            std::cerr << "  " << inner_error.what() << std::endl << std::endl;
            std::cerr << "========================" << std::endl;
            std::cerr << err_report << std::endl;
            std::cerr << "========================" << std::endl;
            std::cerr << "Please send this crash report to \"" << TerminateMines_BUG_ADDRESS << "\"." << std::endl;
            std::cerr << "Thank you!" << std::endl;
        }
    }

    auto pos = Display::getConsolePosition(1, 1);

    return argp_state;
}
