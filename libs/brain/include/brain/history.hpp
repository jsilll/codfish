#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace codfish::brain::utils {
/// @brief The history table class
class History {
  public:
    /// @brief Constructs a new history table object
    History() noexcept { _hist_table.reserve(256); }

    /// @brief Checks if the given key is a repetition
    /// @param key The key to check
    /// @return True if the key is a repetition, false otherwise
    bool IsRepetition(const std::uint64_t key) const noexcept {
        return std::find(_hist_table.begin(), _hist_table.end(), key) !=
               _hist_table.end();
    }

    /// @brief Clears the history table
    void clear() { _hist_table.clear(); }

    /// @brief Pops the last key from the history table
    void pop() noexcept { _hist_table.pop_back(); }

    /// @brief Pushes a new key to the history table
    /// @param key The key to push
    void push(std::uint64_t key) noexcept { _hist_table.push_back(key); }

  private:
    /// @brief The history table
    std::vector<std::uint64_t> _hist_table;
};
}   // namespace codfish::brain::utils