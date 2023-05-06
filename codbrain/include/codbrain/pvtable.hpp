#pragma once

#include <vector>

#include <codchess/move.hpp>

namespace codbrain {
/// @brief Class that implements the principal variation table
/// @tparam S The size of the table
template <std::size_t S> class PVTable {
  public:
    /// @brief Returns the principal variation move at the given depth
    /// @param depth The depth
    /// @return The principal variation move
    [[nodiscard]] constexpr auto
    PVMove(const std::size_t depth) const noexcept {
        { return _table[depth][depth]; }
    }

    /// @brief Returns the principal variation at the given depth
    /// @param start_depth The starting depth
    /// @return The principal variation
    [[nodiscard]] constexpr std::vector<codchess::Move>
    PV(const std::size_t start_depth) const noexcept {
        return {&_table[start_depth][0],
                &_table[start_depth][0] + _size[start_depth]};
    }

    /// @brief Returns the principal variation at the given depth
    /// @return The principal variation
    [[nodiscard]] constexpr std::vector<codchess::Move> PV() const noexcept {
        return {&_table[0][0], &_table[0][0] + _size[0]};
    }

    /// @brief Clears the principal variation table
    constexpr void Clear() noexcept { std::memset(_table, 0, sizeof(_table)); }

    /// @brief Sets the depth of the principal variation
    /// @param depth The depth
    constexpr void SetDepth(const std::size_t depth) noexcept {
        _size[depth] = depth;
    }

    /// @brief Adds a move to the principal variation table
    /// @param move The move to add
    /// @param depth The depth
    constexpr void Add(const codchess::Move move,
                       const std::size_t depth) noexcept {
        _table[depth][depth] = move;
        memcpy(&_table[depth][depth + 1], &_table[depth + 1][depth + 1],
               _size[depth + 1] * sizeof(codchess::Move));
        _size[depth] = _size[depth + 1];
    }

    /// @brief Adds a list of moves to the principal variation table
    /// @param moves The moves to add
    /// @param starting_depth The starting depth
    constexpr void Add(const std::vector<codchess::Move> &moves,
                       const std::size_t starting_depth) noexcept {
        auto last_depth = starting_depth + moves.size() - 1;
        for (auto i = moves.size() - 1; i >= 0; --i) {
            Add(moves[i], last_depth--);
        }
        // TODO: Is this necessary?
        // for (auto current_depth = starting_depth - 1; current_depth >= 0;
        //      --current_depth) {
        //     memcpy(&_table[current_depth][current_depth + 1],
        //            &_table[current_depth + 1][current_depth + 1],
        //            _size[current_depth + 1] * sizeof(codchess::Move));
        // }
    }

  private:
    /// @brief Stores the size of the principal variation at each depth
    std::size_t _size[S]{};
    /// @brief The principal variation table
    codchess::Move _table[S][S]{};
};
}   // namespace codbrain