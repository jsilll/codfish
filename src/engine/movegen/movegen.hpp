#pragma once

#include <engine/constants.hpp>
#include <engine/board.hpp>
#include <engine/move.hpp>

#include <vector>

namespace movegen
{
    /**
     * @brief Generates all the pseudo-legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generate_pseudo_legal_moves(const Board &board);

    /**
     * @brief Generates all the pseudo-legal captures for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generate_pseudo_legal_captures(const Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generateLegalMoves(Board &board);

    /**
     * @brief Generates all the legal moves for a given board
     *
     * @param board
     * @return std::vector<Move>
     */
    std::vector<Move> generate_legal_captures(Board &board);

    /**
     * @brief Checks whether a given board as has legal moves
     *
     * @param board
     * @return bool
     */
    bool has_legal_moves(Board &board);

} // namespace movegen
