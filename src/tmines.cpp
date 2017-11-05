#include <iostream>

#include "display.hpp"
#include "config.h"

#include <string>
#include <argp.h>
#include <stdexcept>
#include <curses.h>
#include <iostream>
#include <random>
#include <cstdlib>
#include <fstream>

const char* argp_program_bug_address = TerminateMines_BUG_ADDRESS;
const char* argp_program_version = "version " TerminateMines_VERSION_MAJOR "." TerminateMines_VERSION_MINOR " (commit " TerminateMines_GIT_COMMIT_HASH ")";

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
        std::string err_report = "";
        err_report += "Error Report\n\n";
        err_report += "=====================================\n";
        err_report += "Please send this to \"" + std::string(TerminateMines_BUG_ADDRESS) + "\"\n";
        err_report += "Thank you!\n";
        err_report += "=====================================\n\n";
        err_report += "Message:\n";
        err_report += "  " + std::string(e.what()) + "\n\n";
        err_report += "Settings:\n";
        err_report += "  Width, Height: " + std::to_string(opts.width) + ", " + std::to_string(opts.height) + "\n";
        err_report += "  Mine Count:    " + std::to_string(opts.mine_count) + "\n";
        err_report += "  Seed:          " + std::to_string(opts.seed) + "\n";
        err_report += "  Args:         ";
        for (int i = 1; i < argc; i++) {
            err_report += " " + std::string(argv[i]);
        }
        err_report += "\n\n";

        err_report += "Version\n";
        err_report += "  Version Number: " + std::string(TerminateMines_VERSION_MAJOR) + "." + std::string(TerminateMines_VERSION_MINOR) + "\n";
        err_report += "  Git Commit:     " + std::string(TerminateMines_GIT_COMMIT_HASH) + "\n";

        std::cerr << "Unfourtunately an Error occured!" << std::endl;
        try{
            std::ofstream report_file;
            report_file.open(CRASH_REPORT_FILE);

            if (! report_file.is_open()) {
                throw std::runtime_error("Can't open file \"" + std::string(CRASH_REPORT_FILE) + "\"");
            }
            report_file << err_report;
            
            std::cerr << "A crash report has been written to the file \"" << CRASH_REPORT_FILE <<"\"" << std::endl;
            std::cerr << "To help to fix that error please send an email to \"" << TerminateMines_BUG_ADDRESS << "\" and attach the file \"" << CRASH_REPORT_FILE << "\"." << std::endl;
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

    return argp_state;
}
