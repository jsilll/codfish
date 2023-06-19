#pragma once

#include <codchess/movegen.hpp>

#include "repl.hpp"

namespace codfish::cli {
/// @brief The commands of the CLI.
repl::Commands commands{
    {"fen", [](auto &out, auto &state,
               const auto &) { out << state.brain.board().GetFen() << '\n'; }},

    {"display",
     [](auto &out, auto &state, const auto &) {
         state.brain.board().Display(out, state.ascii, state.white_on_bottom);
     }},

    {"ascii",
     [](auto &out, auto &state, const auto &) {
         out << "Display set to "
             << ((state.ascii = !state.ascii) ? "ASCII" : "Unicode") << ".\n";
     }},

    {"rotate",
     [](auto &out, auto &state, const auto &) {
         out << "Bottom pieces set to "
             << ((state.white_on_bottom = !state.white_on_bottom) ? "white"
                                                                  : "black")
             << ".\n";
     }},

    {"switch",
     [](auto &out, auto &state, const auto &) {
         out << "Side to play is now "
             << (state.brain.board().SwitchActive() == codchess::WHITE
                     ? "white"
                     : "black")
             << ".\n";
     }},

    {"new",
     [](auto &, auto &state, const auto &) {
         state.brain.ClearTTable();
         state.brain.ClearHistory();
         state.brain.ClearMoveTables();
         state.brain.board().SetStartingPosition();
     }},

    {"moves",
     [](auto &out, auto &state, const auto &) {
         out << "Legal moves:\n";
         for (const auto m : codchess::movegen::Legal(state.brain.board())) {
             out << m.ToString() << '\n';
         }
     }},

    {"move",
     [](auto &out, auto &state, const auto &tokens) {
         if (tokens.size() < 2) {
             out << "Usage: move <move>\n";
         } else {
             const auto &move = tokens[1];
             for (const auto m :
                  codchess::movegen::Legal(state.brain.board())) {
                 if (m.ToString() == move) {
                     state.brain.board().Make(m);
                     return;
                 }
             }
             out << "Invalid move.\n";
         }
     }},

    {"go",
     [](auto &out, auto &state, const auto &) {
         const auto res = state.brain.FindBestMove();
         if (!res.pv.empty()) {
             out << "Best move: " << res.pv[0].ToString() << '\n';
         } else {
             out << "No moves found.\n";
         }
     }},

    {
        "setfen",
        [](auto &out, auto &state, const auto &tokens) {
            if (tokens.size() < 2) {
                out << "Usage: setfen <fen>\n";
            } else {
                const auto &fen = tokens[1];
                if (!codchess::utils::ValidFen(fen)) {
                    out << "Invalid FEN.\n";
                    return;
                }
                state.brain.board().SetFromFen(fen);
            }
        },

    },
};
};   // namespace codfish::cli