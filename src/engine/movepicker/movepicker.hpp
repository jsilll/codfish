#pragma once

#include "../defs.hpp"

class Board;
class Move;

// TODO: move the 64 on the arrays to some kind of constant with a name

class MovePicker
{
private:
    Board &_board;
    int _max_depth;

    int _current_nodes;
    int _current_depth;

    int _killer_moves[2][64];
    int _history_moves[N_SIDES][N_PIECES][N_SQUARES];

    struct MoveMoreThanKey
    {
        MovePicker &move_picker;
        inline bool operator()(const Move &move1, const Move &move2)
        {
            return (move_picker.score(move1) > move_picker.score(move2));
        }
    } _move_more_than_key;

    int _pv_length[64];
    int _pv_table[64][64];

private:
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
    int score(const Move &move) const;
    int negamax(int alpha, int beta, int depth, const Board &board);
    int quiescence(int alpha, int beta, int depth, const Board &board);

public:
    struct SearchResult
    {
        int score;
        int nodes;
        int pv_length;
        int pv[64]{};
    };

public:
    MovePicker(Board &board) : _board(board), _max_depth(1), _current_nodes(0), _move_more_than_key{*this}
    {
    }

    int getDepth() const;

    void setDepth(int depth);

    SearchResult findBestMove();
};