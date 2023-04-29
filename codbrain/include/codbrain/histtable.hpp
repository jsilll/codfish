#pragma once

#include <array>

namespace codbrain {
/// @brief Class that implements the history table
class HistoryTable {
  public:
    /// @brief Returns whether the position is a repetition
    /// @param key The key to check
    /// @return Whether the position is a repetition
    [[nodiscard]] bool IsRepetition(const std::uint64_t key) const noexcept {
        for (std::size_t i = 0; i < _size; ++i) {
            if (_hist[i] == key) {
                return true;
            }
        }
        return false;
    }

    /// @brief Pops the last key from the history table
    void Pop() noexcept { --_size; }

    /// @brief Clears the history table 
    void Clear() noexcept { _size = 0; }

    /// @brief Pushes a key to the history table 
    /// @param key The key to push 
    void Push(const std::uint64_t key) noexcept { _hist[_size++] = key; }

  private:
    /// @brief The size of the history table
    std::size_t _size{0};
    /// @brief The history table
    std::array<std::uint64_t, 128> _hist;
};
}   // namespace codbrain