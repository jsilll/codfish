#pragma once

#include "repl.hpp"

namespace codfish::uci {
void Go(repl::State &state, const repl::Tokens &args);

void Position(repl::State &state, repl::Tokens &args);

/// @brief The commands of the UCI.
inline repl::Commands commands{
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

    {"go",
     [](auto &state, const auto &args) {
        Go(state, args);
     }},

    {"position",
     [](auto &state, const auto &args) {
        Position(state, const_cast<repl::Tokens&>(args));
     }},
};
}   // namespace codfish::uci