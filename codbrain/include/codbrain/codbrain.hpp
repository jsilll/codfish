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

/// @brief The principal variation.
typedef std::vector<codchess::Move> PrincipalVariation;

struct Result {
    /// @brief The depth of the search.
    Nodes nodes;
    /// @brief The evaluation of the position.
    Evaluation evaluation;
    /// @brief The principal variation.
    PrincipalVariation pv;
};

class Brain final {
  public:
    /// @brief Construct a new Brain object.
    explicit Brain(const Depth depth) noexcept : _depth{depth} {}

    /// @brief Returns the board.
    [[nodiscard]] auto &board() noexcept { return _board; }

    /// @brief Searches the position.
    [[nodiscard]] Result Search() noexcept {
        const auto legal_moves = codchess::movegen::Legal(_board);
        return {0, 0, {*legal_moves.begin()}};
    }

  private:
    /// @brief  The depth of the search.
    [[maybe_unused]] Depth _depth;
    /// @brief The board.
    codchess::Board _board{};
};
}   // namespace codbrain