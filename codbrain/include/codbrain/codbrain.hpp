#pragma once

#include <codchess/codchess.hpp>

namespace codbrain {
/// @brief Initializes the library
void
Init() noexcept {
    codchess::Init();
}

/// @brief The depth of the search.
typedef std::uint8_t Depth;

/// @brief The number of nodes searched.
typedef std::uint64_t Nodes;

/// @brief The evaluation of a position.
typedef std::int16_t Evaluation;

struct Result {
    Nodes nodes;
    Evaluation evaluation;
    std::vector<codchess::Move> pv;
};

class Brain final {
  public:
    /// @brief Construct a new Brain object.
    explicit Brain(const Depth depth) noexcept : _depth{depth} {}

    /// @brief Returns the board.
    [[nodiscard]] auto &board() noexcept { return _board; }

    /// @brief Returns the best move.
    [[nodiscard]] Result Search() noexcept {
        const auto legal_moves = codchess::movegen::Legal(_board);
        return {0, 0, {*legal_moves.begin()}};
    }

  private:
    Depth _depth;
    /// @brief The board.
    codchess::Board _board{};
};
}   // namespace codbrain