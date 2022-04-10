#include "defs.hpp"

#include <vector>

class Move;
class MoveList;
class Board;

namespace Movegen
{
    /**
     * @brief Generates all the pseudo-legal moves for a given board
     *
     * @param board
     * @return MoveList
     */
    MoveList generatePseudoLegalMoves(const Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return MoveList
     */
    MoveList generateLegalMoves(const Board &board);
} // namespace Movegen
