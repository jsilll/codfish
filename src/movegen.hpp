#include "defs.hpp"

#include <vector>

class Move;
class Board;

namespace Movegen
{
    /**
     * @brief Generates all the pseudo-legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generatePseudoLegalMoves(const Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generateLegalMoves(const Board &board);
} // namespace Movegen
