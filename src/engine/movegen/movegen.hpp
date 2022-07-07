#pragma once

#include "../defs.hpp"

#include <vector>

class Board;
class Move;

namespace movegen
{
    /**
     * @brief Generates all the pseudo-legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generatePseudoLegalMoves(const Board &board);

    /**
     * @brief Generates all the pseudo-legal captures for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generatePseudoLegalCaptures(const Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generateLegalMoves(const Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generateLegalCaptures(const Board &board);

    /**
     * @brief Checks whether a given board as has legal moves
     *
     * @param board
     * @return bool
     */
    bool hasLegalMoves(const Board &board);

} // namespace movegen
