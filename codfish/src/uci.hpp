#pragma once

#include "repl.hpp"
#include "time.hpp"

namespace codfish::uci {
/// @brief The commands of the UCI.
repl::Commands commands{
    {"uci",
     [](auto &, const auto &) {
         std::cout << "id name Codfish\n";
         std::cout << "id name Codfish\n";
         std::cout << "uciok" << std::endl;
     }},

    {"isready",
     [](auto &, const auto &) { std::cout << "readyok" << std::endl; }},

    {"ucinewgame",
     [](auto &state, const auto &) {
         state.brain.ClearTTable();
         state.brain.ClearHistory();
         state.brain.ClearMoveTables();
         state.brain.board().SetStartingPosition();
     }},

    {"position",
     [](auto &, const auto &) {
         std::cout << "TODO: implement position command\n";
         // positionCommand.execute(args);
     }},

    {"go",
     [](auto &, const auto &) {
         // std::cout << "TODO: implement go command\n";
         // goCommand.execute(args);
         // play first move
     }},

    {"stop",
     [](auto &, const auto &) {
         std::cout << "TODO: implement stop command\n";
         // stopCommand.execute(args);
     }},
};
}   // namespace codfish::uci