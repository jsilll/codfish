#pragma once

#include <codlib/board.hpp>

namespace perft {

    /// @brief Perft function for testing move generation
    /// @param board The board to move generation on
    /// @param depth The depth to search to
    /// @return The number of nodes found
    int perft(Board &board, int depth);

}// namespace perft