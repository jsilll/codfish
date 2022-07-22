#pragma once

#include <engine/constants.hpp>
#include <engine/board.hpp>
#include <engine/move.hpp>

#include <engine/movepicker/pvtable.hpp>

#include <vector>

class MovePicker
{
private:
    static const int DEFAULT_MAX_DEPTH = 64;

    Board &_board;

    int _max_depth;
    int _current_nodes;
    int _current_depth;

    int _history_moves[N_SIDES][N_PIECES][N_SQUARES]{};
    Move _killer_moves[2][DEFAULT_MAX_DEPTH]{};

    PVTable _pv_table;

    struct MoveMoreThanKey
    {
        MovePicker &move_picker;
        inline bool operator()(const Move &move1, const Move &move2) const
        {
            return (move_picker.score(move1) > move_picker.score(move2));
        }
    } _move_more_than_key;

    /**
     * @brief Evaluates a given move using a few heuristics:
     * - MVV LVA
     * - Killer Heuristic
     * - History Heuristic
     *
     * It is used for sorting the list of moves upon
     * generation, ini order to cause the most alpha-beta cuts.
     *
     * @param move
     * @return move score
     */
    int score(const Move move);

    int search(int depth, int alpha, int beta);
    int negamax(int alpha, int beta, int depth);
    int quiescence(int alpha, int beta);

    void add_to_killer_moves(const Move move);
    void add_to_history_moves(const Move move);

    void clear_search_counters();

public:
    struct SearchResult
    {
        int score{};
        int nodes{};
        std::vector<Move> pv;
    };

    explicit MovePicker(Board &board) : _board(board), _max_depth(6), _current_nodes(0), _move_more_than_key{*this} {}

    [[nodiscard]] int get_max_depth() const;

    void set_max_depth(int depth);

    /**
     * @brief Clears all the tables
     *
     */
    void clear_tables();

    /**
     * @brief Searches the current position with max_depth
     *
     * @return SearchResult
     */
    SearchResult find_best_move();

    /**
     * @brief Searches the current position with the given depth
     *
     * It can also be used to implement iterative deepening outside
     * of the ai class (for uci prints, for example)
     *
     * @param depth
     * @return SearchResult
     */
    SearchResult find_best_move(int depth, int alpha, int beta);
};