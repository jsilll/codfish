#include "defs.hpp"

#include <vector>

class Board;
class Move;
class MoveList;

namespace movegen
{
    /**
     * @brief Generates all the pseudo-legal moves for a given board
     *
     * @param board
     * @return MoveList
     */
    template <PieceColor ToMove>
    MoveList generatePseudoLegalMoves(const Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return MoveList
     */
    MoveList generateLegalMoves(const Board &board);
} // namespace movegen
