#include "commands.hpp"

#include <iostream>

void cli::HelpCommand::execute([[maybe_unused]] std::vector<std::string> &args)
{
    std::cout
        << "ascii                 Toggles between ascii and utf-8 board representation\n"
        << "captures              Shows all pseudo legal captures\n"
        << "cc                    Plays computer-to-computer [TODO]\n"
        << "display               Displays board \n"
        << "dperft (n)            Divided perft\n"
        << "eval                  Shows static evaluation of this position\n"
        << "exit                  Exits program\n"
        << "getfen                Prints current position to in fen string format \n"
        << "go                    Computer plays his best move [TODO]\n"
        << "help                  Shows this help \n"
        << "info                  Displays variables (for testing purposes)\n"
        << "magics                Generates magic numbers for the bishop and rook pieces\n"
        << "move (move)           Plays a move (in uci format)\n"
        << "moves                 Shows all legal moves\n"
        << "new                   Starts new game\n"
        << "perf                  Benchmarks a number of key functions [TODO]\n"
        << "perft n               Calculates raw number of nodes from here, depth n\n"
        << "plmoves               Shows all pseudo legal moves\n"
        << "rotate                Rotates board \n"
        << "sd (n)                Sets the search depth to n [TODO]\n"
        << "setfen (fen)          Reads fen string position and modifies board accordingly\n"
        << "switch                Switches the next side to move\n"
        << "undo                  Takes back last move [TODO]\n"
        << std::endl;
}