#pragma once

#include <array>
#include <vector>

#include <codchess/base.hpp>
#include <codchess/bitboard.hpp>
#include <codchess/board.hpp>
#include <codchess/move.hpp>

#include <codbrain/eval.hpp>
#include <codbrain/histmoves.hpp>
#include <codbrain/histtable.hpp>
#include <codbrain/killertable.hpp>
#include <codbrain/pvtable.hpp>
#include <codbrain/ttable.hpp>

namespace codbrain {
/// @brief Search result
struct Result {
    /// @brief The score of the position
    int score;
    /// @brief The number of nodes searched
    std::uint64_t nodes;
    /// @brief The principal variation
    std::vector<codchess::Move> pv;
};

/// @brief Class that implements the search algorithm
class Brain {
  public:
    /// @brief Maximum depth for the search
    static constexpr auto MAX_DEPTH = 64;

    /// @brief Default constructor
    [[nodiscard]] Brain(const std::uint32_t depth)
        : _max_depth{depth}, _move_more_than_key{*this} {
        CheckDepth(depth);
    }

    /// @brief Returns a reference to the board
    /// @return The board
    [[nodiscard]] constexpr auto &board() noexcept { return _board; }

    /// @brief Returns the current max depth for the search
    /// @return The max depth
    [[nodiscard]] constexpr auto max_depth() const noexcept {
        return _max_depth;
    }

    /// @brief Sets the max depth for the search
    /// @param depth The max depth
    constexpr void max_depth(const std::uint32_t depth) {
        CheckDepth(depth);
        _max_depth = depth;
    }

    /// @brief Clears the transposition table
    constexpr void ClearTTable() noexcept { _ttable.Clear(); }

    /// @brief Clears all known game history
    constexpr void ClearHistory() noexcept { _hist_table.Clear(); }

    /// @brief Clears all the move tables
    constexpr void ClearMoveTables() noexcept {
        _pv_table.Clear();
        _hist_moves.Clear();
        _killer_table.Clear();
    }

    /// @brief Adds a board hash to the current known history
    /// @param key The board hash
    constexpr void
    AddToHistory(const codchess::bitboard::Bitboard key) noexcept {
        _hist_table.Push(key);
    }

    /// @brief Searches the current position
    /// @return A search result
    Result FindBestMove() noexcept;

    /// @brief Searches the current position with the given depth
    /// @param depth The depth
    /// @param alpha The alpha value
    /// @param beta The beta value
    /// @return A search result
    /// @note This function corresponds to one of the loops of FindBestMove()
    Result FindBestMove(int depth, int alpha, int beta) noexcept {
        ClearCounters();
        const auto score = Search(depth, alpha, beta);
        return {score, _current_nodes, _pv_table.PV()};
    }

  private:
    /// @brief Board to be searched
    codchess::Board _board{};
    /// @brief Max depth for the search
    std::uint32_t _max_depth;
    /// @brief Current number of nodes searched
    std::uint64_t _current_nodes{0};
    /// @brief Current depth of the search
    std::uint32_t _current_depth{0};

    /// @brief Transposition table
    TTable _ttable{0x400000};
    /// @brief History moves
    HistMoves _hist_moves{};
    /// @brief History table
    HistTable<128> _hist_table{};
    /// @brief Principal variation table
    PVTable<MAX_DEPTH> _pv_table{};
    /// @brief Killer Table
    KillerTable<MAX_DEPTH> _killer_table{};

    /// @brief Functor to compare moves
    [[maybe_unused]] struct MoveMoreThanKey {
        const Brain &move_picker;
    } _move_more_than_key;

    /// @brief Clears the search counters
    constexpr void ClearCounters() noexcept {
        _current_nodes = 0;
        _current_depth = 0;
    }

    /// @brief Performs a quiescence search on the current position
    /// @param alpha The alpha value
    /// @param beta The beta value
    /// @return The score
    std::int32_t Quiescence(int alpha, int beta) noexcept;

    /// @brief Performs a search on the current position
    /// @param depth The depth
    /// @param alpha The alpha value
    /// @param beta The beta value
    /// @return The score
    std::int32_t Search(int alpha, int beta, int depth) noexcept;

    /// @brief Performs a negamax search on the current position
    /// @param alpha The alpha value
    /// @param beta The beta value
    /// @param depth The depth
    /// @return The score
    std::int32_t Negamax(int alpha, int beta, int depth) noexcept;

    /// @brief Scores a given move using heuristics like MVV LVA, Killer and
    /// History
    /// @param move The move to be scored
    /// @return The score
    /// @note This function is used for sorting the list of moves upon
    /// generation in order to cause the most alpha-beta cuts.
    [[nodiscard]] std::int32_t Score(const codchess::Move move) const noexcept;

    /// @brief Returns whether a move can be pruned by LMR
    /// @param move The move
    /// @return Whether the move can be pruned
    /// @note Move can't be a capture nor a promotion for LMR to happen
    static constexpr bool CanLMR(const codchess::Move move) noexcept {
        // TODO: Missing check moves and in check moves
        return !move.IsCapture() and !move.IsPromotion();
    }

    /// @brief Checks the depth argument
    /// @param depth The depth
    constexpr static void CheckDepth(const std::uint32_t depth) {
        if (depth == 0) {
            throw std::invalid_argument(
                "Depth argument must be positive integer.");
        } else if (depth > MAX_DEPTH) {
            throw std::invalid_argument(
                "Depth argument must be less or equal than " +
                std::to_string(MAX_DEPTH) + ".");
        }
    }
};
}   // namespace codbrain