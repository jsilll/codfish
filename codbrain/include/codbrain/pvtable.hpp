#pragma once

#include <codchess/move.hpp>

#include <array>
#include <vector>

namespace codbrain {
class PVTable {
  public:
    std::vector<codchess::Move> get_pv() const noexcept {
        return {_pv[0].begin(), _pv[0].end()};
    }

    std::vector<codchess::Move>
    get_pv_from_depth(int start_depth) const noexcept {
        return {_pv[start_depth].begin(), _pv[start_depth].end()};
    }

    constexpr auto get_pv_move(int depth) const { return _pv[depth][depth]; }

    void clear() { memset(_pv, 0, sizeof(_pv)); }

    void set_length(int depth) { _length[depth] = depth; }

    void add(codchess::Move const move, int depth) {
        // Write Principal Variation Move
        _pv[depth][depth] = move;

        // Copy moves from deeper depth into current depths line
        memcpy(&_pv[depth][depth + 1], &_pv[depth + 1][depth + 1],
               (unsigned long) _length[depth + 1] * sizeof(int));

        // Adjust Principal Variation Length
        _length[depth] = _length[depth + 1];
    }

    void add_pv_from_depth(std::vector<codchess::Move> moves,
                           int starting_depth) {
        int last_depth = starting_depth + (int) moves.size() - 1;

        // Update PV with moves from starting_depth to last depth (reverse
        // order)
        for (int i = (int) moves.size() - 1; i >= 0; i--) {
            add(moves[(std::vector<Move>::size_type) i], last_depth--);
        }

        // Update the Higher Depths
        for (int current_depth = starting_depth - 1; current_depth >= 0;
             current_depth--) {
            memcpy(&_pv[current_depth][current_depth + 1],
                   &_pv[current_depth + 1][current_depth + 1],
                   (unsigned long) _length[current_depth + 1] * sizeof(int));
        }
    }

  private:
    // note: 64 is the max depth

    class PVList {
        /// @brief Returns an iterator to the beginning of the list.
        /// @return An iterator to the beginning of the list.
        [[nodiscard]] auto begin() const noexcept { return moves.begin(); }

        /// @brief Returns an iterator to the end of the list.
        /// @return An iterator to the end of the list.
        [[nodiscard]] auto end() const noexcept { return moves.begin() + size; }

        [[nodiscard]] constexpr auto operator[](const int idx) const noexcept {
            return moves[idx];
        }

        [[nodiscard]] constexpr auto &operator[](const int idx) noexcept {
            return moves[idx];
        }

        /// @brief Returns whehter the list is empty.
        /// @return True if the list is empty, false otherwise.
        [[nodiscard]] bool Empty() const noexcept { return size == 0; }

        /// @brief Inserts a move into the list.
        /// @param move The move to insert.
        void Push(codchess::Move move) noexcept { moves[size++] = move; }

      private:
        std::size_t size{0};
        std::array<codchess::Move, 64> moves{};
    };

    std::array<PVList, 64> _pv{};
};
}   // namespace codbrain