#pragma once

#include "../defs.hpp"

class Board;
class Move;

// TODO: move the 64 on the arrays to some kind of constant with a name

class MovePicker
{
private:
    static constexpr int MAX_DEPTH = 64;

    Board &_board;
    int _max_depth;

    int _current_nodes;
    int _current_depth;

    int _killer_moves[2][MAX_DEPTH];
    int _history_moves[N_SIDES][N_PIECES][N_SQUARES];

    struct MoveMoreThanKey
    {
        MovePicker &move_picker;
        inline bool operator()(const Move &move1, const Move &move2)
        {
            return (move_picker.score(move1) > move_picker.score(move2));
        }
    } _move_more_than_key;

    int _pv_length[MAX_DEPTH];
    int _pv_table[MAX_DEPTH][MAX_DEPTH];

public:
    struct SearchResult
    {
        int score;
        int nodes;
        int pv_length;
        int pv[MAX_DEPTH]{};
    };

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

    int search(int depth);
    int negamax(int alpha, int beta, int depth, const Board &board);
    int quiescence(int alpha, int beta, int depth, const Board &board);

    void addToKillerMoves(Move const &move);
    void addToHistoryMoves(Move const &move);
    void addToPrincipalVariation(Move const &move);

public:
    MovePicker(Board &board) : _board(board), _max_depth(6), _current_nodes(0), _move_more_than_key{*this}
    {
    }

    int getDepth() const;

    void setDepth(int depth);

    SearchResult findBestMove();
};