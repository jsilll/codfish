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
    MoveList generatePseudoLegalMoves(const Board &board);

    /**
     * @brief Generates all the pseudo-legal captures for a given board
     *
     * @param board
     * @return MoveList
     */
    MoveList generatePseudoLegalCaptures(const Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return MoveList
     */
    MoveList generateLegalMoves(const Board &board);

    /**
     * @brief Checks whether a given board as has legal moves
     *
     * @param board
     * @return true
     * @return false
     */
    bool hasLegalMoves(const Board &board); // TODO: performance can be improved (quiescence usage)

} // namespace movegen
