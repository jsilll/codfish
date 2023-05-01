#pragma once

#include <array>
#include <vector>

#include <codchess/base.hpp>
#include <codchess/bitboard.hpp>
#include <codchess/board.hpp>
#include <codchess/move.hpp>

#include <codbrain/histtable.hpp>
#include <codbrain/pvtable.hpp>
#include <codbrain/ttable.hpp>

namespace codbrain {
#define MAX_DEPTH 64

/// @brief Class that implements the search algorithm
class MovePicker {
  public:
    /// @brief Search result
    struct SearchResult {
        /// @brief The score of the position
        int score;
        /// @brief The number of nodes searched
        std::uint64_t nodes;
        /// @brief The principal variation
        std::vector<codchess::Move> pv;
    };

    /// @brief Default constructor
    [[nodiscard]] MovePicker(const std::uint32_t depth)
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
    void max_depth(const std::uint32_t depth) {
        CheckDepth(depth);
        _max_depth = depth;
    }

    /// @brief Clears the transposition table
    void ClearTTable() noexcept { _ttable.clear(); }

    /// @brief Clears all known game history
    void ClearHistory() noexcept { _hist_table.Clear(); }

    /// @brief Clears all the move tables
    void ClearMoveTables() noexcept {
        _pv_table.Clear();
        std::memset(_killer_moves, 0, sizeof(_killer_moves));
        std::memset(_history_moves, 0, sizeof(_history_moves));
    }

    /// @brief Adds a board hash to the current known history
    /// @param key The board hash
    void AddToHistory(const codchess::bitboard::Bitboard key) noexcept {
        _hist_table.Push(key);
    }

    /// @brief Searches the current position
    /// @return A search result
    SearchResult FindBestMove() noexcept;

    /// @brief Searches the current position with the given depth
    /// @param depth The depth
    /// @param alpha The alpha value
    /// @param beta The beta value
    /// @return A search result
    /// @note This function correspondes ton one of the loops of FindBestMove()
    SearchResult FindBestMove(int depth, int alpha, int beta) noexcept {
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
    TTable _ttable{};
    /// @brief Principal variation table
    PVTable<MAX_DEPTH> _pv_table{};
    /// @brief History table
    HistoryTable<128> _hist_table{};
    /// @brief Killer moves TODO: encapsulate
    codchess::Move _killer_moves[2][MAX_DEPTH]{};
    /// @brief History moves
    std::uint32_t _history_moves[codchess::N_COLORS][codchess::N_PIECES]
                                [codchess::N_SQUARES]{};

    /// @brief Functor to compare moves
    [[maybe_unused]] struct MoveMoreThanKey {
        const MovePicker &move_picker;
        inline bool operator()(const codchess::Move &move1,
                               const codchess::Move &move2) const noexcept {
            return (move_picker.Score(move1) > move_picker.Score(move2));
        }
    } _move_more_than_key;

    static void CheckDepth(const std::uint32_t depth) {
        if (depth == 0) {
            throw std::invalid_argument(
                "Depth argument must be positive integer.");
        } else if (depth > MAX_DEPTH) {
            throw std::invalid_argument("Depth argument must be less than 64.");
        }
    }

    /// @brief Clears the search counters
    constexpr void ClearCounters() noexcept {
        _current_nodes = 0;
        _current_depth = 0;
    }

    /// @brief Adds a move to the killer moves
    /// @param move The move
    void AddToKillerMoves(codchess::Move move) noexcept {
        if (move != _killer_moves[0][_current_depth]) {
            _killer_moves[1][_current_depth] = _killer_moves[0][_current_depth];
        }
        _killer_moves[0][_current_depth] = move;
    }

    /// @brief Adds a move to the move history
    /// @param move The move
    void AddToHistoryMoves(codchess::Move move) noexcept {
        _history_moves[_board.active()][move.MovedPiece()][move.ToSquare()] +=
            _current_depth;
    }

    /// @brief Scores a given move using heuristics like MVV LVA, Killer and
    /// History
    /// @param move The move to be scored
    /// @return The score
    /// @note This function is used for sorting the list of moves upon
    /// generation in order to cause the most alpha-beta cuts.
    [[nodiscard]] std::int32_t Score(const codchess::Move move) const noexcept {
        static constexpr int MVV_LVA[6][6] = {
            {10105, 10205, 10305, 10405, 10505, 10605},
            {10104, 10204, 10304, 10404, 10504, 10604},
            {10103, 10203, 10303, 10403, 10503, 10603},
            {10102, 10202, 10302, 10402, 10502, 10602},
            {10101, 10201, 10301, 10401, 10501, 10601},
            {10100, 10200, 10300, 10400, 10500, 10600}};

        if (_pv_table.PVMove(_current_depth) == move) {
            return 20000;
        }

        if (move.IsCapture()) {
            return MVV_LVA[move.MovedPiece()][move.CapturedPiece()];
        }

        if (_killer_moves[0][_current_depth] == move) {
            return 9000;
        }

        if (_killer_moves[1][_current_depth] == move) {
            return 8000;
        }

        return _history_moves[_board.active()][move.MovedPiece()]
                             [move.ToSquare()];
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
};
}   // namespace codbrain