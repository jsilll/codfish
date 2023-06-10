#pragma once

#include <codchess/movegen.hpp>

#include "repl.hpp"

namespace codfish::cli {
/// @brief The commands of the CLI.
repl::Commands commands{
    {"fen",
     [](auto &state, const auto &) {
         std::cout << state.brain.board().GetFen() << '\n';
     }},

    {"display",
     [](auto &state, const auto &) {
         state.brain.board().Display(std::cout, state.ascii,
                                     state.white_on_bottom);
     }},

    {"ascii",
     [](auto &state, const auto &) {
         std::cout << "Display set to "
                   << ((state.ascii = !state.ascii) ? "ASCII" : "Unicode")
                   << ".\n";
     }},

    {"rotate",
     [](auto &state, const auto &) {
         std::cout << "Bottom pieces set to "
                   << ((state.white_on_bottom = !state.white_on_bottom)
                           ? "white"
                           : "black")
                   << ".\n";
     }},

    {"switch",
     [](auto &state, const auto &) {
         std::cout << "Side to play is now "
                   << (state.brain.board().SwitchActive() == codchess::WHITE
                           ? "white"
                           : "black")
                   << ".\n";
     }},

    {"new",
     [](auto &state, const auto &) {
         state.brain.ClearTTable();
         state.brain.ClearHistory();
         state.brain.ClearMoveTables();
         state.brain.board().SetStartingPosition();
     }},

    {"moves",
     [](auto &state, const auto &) {
         std::cout << "Legal moves:\n";
         for (const auto m : codchess::movegen::Legal(state.brain.board())) {
             std::cout << m.ToString() << '\n';
         }
     }},

    {"move",
     [](auto &state, const auto &tokens) {
         if (tokens.size() < 2) {
             std::cerr << "Usage: move <move>\n";
         } else {
             const auto &move = tokens[1];
             for (const auto m :
                  codchess::movegen::Legal(state.brain.board())) {
                 if (m.ToString() == move) {
                     state.brain.board().Make(m);
                     return;
                 }
             }
             std::cerr << "Invalid move.\n";
         }
     }},

    {"go",
     [](auto &state, const auto &) {
         const auto res = state.brain.FindBestMove();
         if (!res.pv.empty()) {
             std::cout << "Best move: " << res.pv[0].ToString() << '\n';
         } else {
             std::cout << "No moves found.\n";
         }
     }},

    {
        "setfen",
        [](auto &state, const auto &tokens) {
            if (tokens.size() < 2) {
                std::cerr << "Usage: setfen <fen>\n";
            } else {
                const auto &fen = tokens[1];
                if (!codchess::utils::ValidFen(fen)) {
                    std::cerr << "Invalid FEN.\n";
                    return;
                }
                state.brain.board().SetFromFen(fen);
            }
        },

    },
};
};   // namespace codfish::cli