#pragma once

#include <vector>

#include <codchess/move.hpp>

namespace codbrain {
/// @brief Class that implements the transposition table
/// @tparam S The size of the table
class TTable {
  public:
    /// @brief The output of a read
    struct TTOutput {
        /// @brief Whether the entry was found
        bool found{false};
        /// @brief The score
        std::int32_t score{0};
        /// @brief The moves
        std::vector<codchess::Move> moves{};
    };

    /// @brief The hash flag for a beta cutoff
    static constexpr std::int32_t HASH_FLAG_BETA{2};
    /// @brief The hash flag for an alpha cutoff
    static constexpr std::int32_t HASH_FLAG_ALPHA{1};
    /// @brief The hash flag for a score
    static constexpr std::int32_t HASH_FLAG_SCORE{0};

    /// @brief Constructor
    /// @param size The size of the table
    [[nodiscard]] constexpr explicit TTable(const std::size_t size) noexcept
        : _table(size) {}

    /// @brief Clear the table
    constexpr void Clear() noexcept { _table.clear(); }

    /// @brief Sets the entry for the given key
    /// @param key The key to hash
    /// @param depth The depth of the search
    /// @param flag The flag
    /// @param score The score
    /// @param moves The moves
    constexpr void Set(const std::uint64_t key, const std::int32_t depth,
                       const std::int32_t flag, const std::int32_t score,
                       std::vector<codchess::Move> &&moves) noexcept {
        _table[key % _table.size()] = {flag, depth, score, key,
                                       std::move(moves)};
    }

    /// @brief Returns the entry for the given key
    /// @param key The key to hash
    /// @param alpha The alpha
    /// @param beta The beta
    /// @param depth The depth
    /// @return The entry
    constexpr auto Read(const std::uint64_t key, const std::int32_t alpha,
                        const std::int32_t beta,
                        const std::int32_t depth) const noexcept {
        const auto entry = _table[key % _table.size()];

        auto result = TTOutput{};
        if (entry.key == key and entry.depth >= depth) {
            result.found = true;
            result.moves = entry.moves;

            if (entry.flag == HASH_FLAG_SCORE) {
                result.score = entry.score;
            }

            if (entry.flag == HASH_FLAG_ALPHA and entry.score <= alpha) {
                result.score = alpha;
            }

            if (entry.flag == HASH_FLAG_BETA and entry.score > beta) {
                result.score = beta;
            }
        }

        return result;
    }

  private:
    /// @brief The transposition table entry
    struct TTEntry {
        /// @brief The flag
        std::int32_t flag;
        /// @brief The depth
        std::int32_t depth;
        /// @brief The score
        std::int32_t score;
        /// @brief The hash key
        std::uint64_t key;
        /// @brief The moves
        std::vector<codchess::Move> moves;
    };

    /// @brief The size of the table
    std::vector<TTEntry> _table;
};
}   // namespace codbrain