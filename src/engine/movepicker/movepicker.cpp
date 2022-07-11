#include <engine/movepicker/movepicker.hpp>

#include <engine/movepicker/eval.hpp>

#include <engine/movegen/bitboard.hpp>
#include <engine/movegen/board.hpp>
#include <engine/movegen/move.hpp>
#include <engine/movegen/movegen.hpp>

#include <algorithm>
#include <climits>
#include <cstring>

#define MIN_EVAL (INT_MIN + 1)

int MovePicker::score(const Move &move)
{
    // clang-format off
    static const int MVV_LVA[6][6] = {
        {10105, 10205, 10305, 10405, 10505, 10605}, 
        {10104, 10204, 10304, 10404, 10504, 10604},
        {10103, 10203, 10303, 10403, 10503, 10603},
        {10102, 10202, 10302, 10402, 10502, 10602},
        {10101, 10201, 10301, 10401, 10501, 10601},
        {10100, 10200, 10300, 10400, 10500, 10600}
    };
    // clang-format on

    if (_pv_table[0][_current_depth] == move.getEncoded())
    {
        return 20000;
    }

    if (move.isCapture())
    {
        return MVV_LVA[move.getPiece()][move.getCapturedPiece()];
    }

    if (_killer_moves[0][_current_depth] == move.getEncoded())
    {
        return 9000;
    }

    if (_killer_moves[1][_current_depth] == move.getEncoded())
    {
        return 8000;
    }

    // TODO: can be improved after adding unmake move function
    return _history_moves[_current_depth % 2 ? !_board.getSideToMove() : _board.getSideToMove()][move.getPiece()][move.getToSquare()];
}

int MovePicker::search(int depth)
{
    int alpha = MIN_EVAL;

    Move best_move = Move();
    std::vector<Move> moves = movegen::generatePseudoLegalMoves(_board);
    std::sort(moves.begin(), moves.end(), _move_more_than_key);
    for (const Move &move : moves)
    {
        Board backup = _board;
        backup.makeMove(move);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        int attacker_side = backup.getSideToMove();
        if (!backup.isSquareAttacked(king_sq, attacker_side))
        {
            _current_depth++;
            int score = -negamax(MIN_EVAL, -alpha, depth - 1, backup);
            _current_depth--;
            if (score > alpha)
            {
                // History Move Heuristic
                if (!move.isCapture())
                {
                    this->addToHistoryMoves(move);
                }

                alpha = score;
                best_move = move;
            }
        }
    }

    this->addToPrincipalVariation(best_move);

    return alpha;
}

int MovePicker::negamax(int alpha, int beta, int depth, const Board &board)
{
    _current_nodes++;

    // Terminal Node
    if (board.getHalfMoveClock() == 100)
    {
        // Draw
        return 0;
    }

    // Forced Terminal Node
    if (depth == 0)
    {
        _pv_length[_current_depth] = _current_depth;
        return quiescence(alpha, beta, -1, board);
    }

    bool first_node = true;
    bool has_legal_moves = false;

    Move best_move = Move();
    std::vector<Move> moves = movegen::generatePseudoLegalMoves(board);
    std::sort(moves.begin(), moves.end(), _move_more_than_key);
    for (const Move &move : moves)
    {
        Board backup = board;
        backup.makeMove(move);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        if (!backup.isSquareAttacked(king_sq, backup.getSideToMove()))
        {
            int score;
            has_legal_moves = true;

            if (first_node)
            {
                first_node = false;

                _current_depth++;
                score = -negamax(-beta, -alpha, depth - 1, backup);
                _current_depth--;
            }
            else
            {
                // Perform a Null Window Search
                _current_depth++;
                score = -negamax(-alpha - 1, -alpha, depth - 1, backup);
                _current_depth--;

                // If this move failed to prove to be bad,
                // re-search with normal bounds
                if ((score > alpha) && (score < beta))
                {
                    _current_depth++;
                    score = -negamax(-beta, -alpha, depth - 1, backup);
                    _current_depth--;
                }
            }

            if (score >= beta)
            {
                // Killer Move Heuristic
                if (!move.isCapture())
                {
                    this->addToKillerMoves(move);
                }

                return beta;
            }
            if (score > alpha)
            {
                // History Move Heuristic
                if (!move.isCapture())
                {
                    this->addToHistoryMoves(move);
                }

                alpha = score;
                best_move = move;
            }
        }
    }

    // Terminal Node
    if (!has_legal_moves)
    {
        // Check Mate
        int king_sq = bitboard::bitScanForward(board.getPieces(board.getSideToMove(), KING));
        if (board.isSquareAttacked(king_sq, board.getOpponent()))
        {
            return MIN_EVAL + _current_depth;
        }

        // Stale Mate
        return 0;
    }

    this->addToPrincipalVariation(best_move);

    return alpha;
}

int MovePicker::quiescence(int alpha, int beta, int depth, const Board &board)
{
    _current_nodes++;

    if (board.getHalfMoveClock() == 100)
    {
        return 0;
    }

    if (!movegen::hasLegalMoves(board))
    {
        int king_sq = bitboard::bitScanForward(board.getPieces(board.getSideToMove(), KING));
        if (board.isSquareAttacked(king_sq, board.getOpponent()))
        {
            return MIN_EVAL + _current_depth;
        }

        return 0;
    }

    int stand_pat = eval::eval(board);

    if (stand_pat >= beta)
    {
        return beta;
    }

    if (stand_pat > alpha)
    {
        alpha = stand_pat;
    }

    std::vector<Move> captures = movegen::generatePseudoLegalCaptures(board);
    std::sort(captures.begin(), captures.end(), _move_more_than_key);
    for (const Move &capture : captures)
    {
        Board backup = board;
        backup.makeMove(capture);
        int king_sq = bitboard::bitScanForward(backup.getPieces(backup.getOpponent(), KING));
        if (!backup.isSquareAttacked(king_sq, backup.getSideToMove()))
        {
            _current_depth++;
            int score = -quiescence(-beta, -alpha, depth - 1, backup);
            _current_depth--;
            if (score >= beta)
            {
                return beta;
            }
            if (score > alpha)
            {
                alpha = score;
            }
        }
    }

    return alpha;
}

void MovePicker::addToKillerMoves(Move const &move)
{
    // Killer Move Heuristic
    if (move.getEncoded() != _killer_moves[0][_current_depth])
    {
        _killer_moves[1][_current_depth] = _killer_moves[0][_current_depth];
    }
    _killer_moves[0][_current_depth] = move.getEncoded();
}

void MovePicker::addToHistoryMoves(Move const &move)
{
    // History Move Heuristic
    _history_moves[_board.getSideToMove()][move.getPiece()][move.getToSquare()] += _current_depth;
}

void MovePicker::addToPrincipalVariation(Move const &move)
{
    // Write Principal Variation Move
    _pv_table[_current_depth][_current_depth] = move.getEncoded();

    // Copy moves from deeper depth into current depths line
    memcpy(&_pv_table[_current_depth][_current_depth + 1], &_pv_table[_current_depth + 1][_current_depth + 1], (unsigned long)_pv_length[_current_depth + 1] * sizeof(int));

    // Adjust Principal Variation Length
    _pv_length[_current_depth] = _pv_length[_current_depth + 1];
}

void MovePicker::clearSearchCounters()
{
    _current_nodes = 0;
    _current_depth = 0;
}

// Public Methods

int MovePicker::getMaxDepth() const
{
    return _max_depth;
}

void MovePicker::setMaxDepth(int depth)
{
    if (depth <= 0)
    {
        throw std::invalid_argument("Depth argument must be positive integer.");
    }

    _max_depth = depth;
}

MovePicker::SearchResult MovePicker::findBestMove()
{
    this->clearTables();

    // Iterative Deepening
    int alpha;
    for (int depth = 1; depth <= _max_depth; depth++)
    {
        this->clearSearchCounters();
        alpha = search(depth);
    }

    auto res = SearchResult{alpha, _current_nodes, _pv_length[0]};
    memcpy(&res.pv, &_pv_table[0], (unsigned long)_pv_length[0] * sizeof(int));
    return res;
}

/**
 * @brief This function corresponds to one of
 * the loops of findBestMove()
 *
 * @param depth
 * @return MovePicker::SearchResult
 */
MovePicker::SearchResult MovePicker::findBestMove(int depth)
{
    this->clearSearchCounters();
    int alpha = search(depth);
    SearchResult res = SearchResult{alpha, _current_nodes, _pv_length[0]};
    memcpy(&res.pv, &_pv_table[0], (unsigned long)_pv_length[0] * sizeof(int));
    return res;
}

void MovePicker::clearTables()
{
    memset(_history_moves, 0, sizeof(_history_moves));
    memset(_killer_moves, 0, sizeof(_killer_moves));
    memset(_killer_moves, 0, sizeof(_killer_moves));
    memset(_pv_table, 0, sizeof(_pv_table));
}