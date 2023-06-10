#pragma once

#include <cstdint>

namespace codbrain {
/// @brief Class that implements the killer table
/// @tparam S The size of the killer table
template <std::size_t S> class KillerTable {
  public:
    /// @brief Returns the first killer move for the given depth
    /// @param depth The depth 
    /// @return The move 
    constexpr auto First(const std::uint32_t depth) const noexcept {
        return _killer_moves[0][depth];
    }

    /// @brief Returns the second killer move for the given depth
    /// @param depth The depth 
    /// @return The move 
    constexpr auto Second(const std::uint32_t depth) const noexcept {
        return _killer_moves[1][depth];
    }

    /// @brief Pushes a move into the killer table
    /// @param move The move
    /// @param depth The depth
    constexpr void Push(codchess::Move move, const std::uint32_t depth) noexcept {
        if (move != _killer_moves[0][depth]) {
            _killer_moves[1][depth] = _killer_moves[0][depth];
        }
        _killer_moves[0][depth] = move;
    }

    /// @brief Clears the killer table
    constexpr void Clear() noexcept {
        std::memset(_killer_moves, 0, sizeof(_killer_moves));
    }

  private:
    static constexpr auto NUM_KILLER_MOVES = 2;
    /// @brief Killer moves
    codchess::Move _killer_moves[NUM_KILLER_MOVES][S]{};
};
}   // namespace codbrain