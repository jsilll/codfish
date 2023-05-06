#pragma once

#include <cstdint>

namespace codbrain {
/// @brief Class that implements the history table
/// @tparam S The size of the table
template <std::size_t S> class HistTable {
  public:
    /// @brief Returns whether the position is a repetition
    /// @param key The key to check
    /// @return Whether the position is a repetition
    [[nodiscard]] constexpr bool
    IsRepetition(const std::uint64_t key) const noexcept {
        for (std::size_t i = 0; i < _size; ++i) {
            if (_hist[i] == key) {
                return true;
            }
        }

        return false;
    }

    /// @brief Pops the last key from the history table
    constexpr void Pop() noexcept { --_size; }

    /// @brief Clears the history table
    constexpr void Clear() noexcept { _size = 0; }

    /// @brief Pushes a key to the history table
    /// @param key The key to push
    constexpr void Push(const std::uint64_t key) noexcept {
        _hist[_size++] = key;
    }

  private:
    /// @brief The size of the history table
    std::size_t _size{0};
    /// @brief The history table
    std::uint64_t _hist[S]{};
};
}   // namespace codbrain