#pragma once

#include "repl.hpp"

namespace codfish::uci {
void Go(repl::State &state, const repl::Tokens &args);

void Position(repl::State &state, repl::Tokens &args);

/// @brief The commands of the UCI.
inline repl::Commands commands{
    {"uci",
     [](auto &out, auto &, const auto &) {
         out << "id name codfish\n";
         out << "id author codfish\n";
         out << "uciok" << std::endl;
     }},

    {"debug",
     [](auto &out, auto &state, const auto &) {
         state.debug = !state.debug;
         out << "debug mode is now " << (state.debug ? "on" : "off")
             << std::endl;
     }},

    {"isready",
     [](auto &out, auto &, const auto &) { out << "readyok" << std::endl; }},

    {"ucinewgame",
     [](auto &, auto &state, const auto &) {
         state.brain.ClearTTable();
         state.brain.ClearHistory();
         state.brain.ClearMoveTables();
         state.brain.board().SetStartingPosition();
     }},

    {"go", [](auto &, auto &state, const auto &args) { Go(state, args); }},

    {"position",
     [](auto &, auto &state, const auto &args) {
         Position(state, const_cast<repl::Tokens &>(args));
     }},
};
}   // namespace codfish::uci