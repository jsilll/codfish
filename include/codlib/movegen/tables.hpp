#pragma once

#include <codlib/constants.hpp>

#include <codlib/movegen/magics.hpp>

namespace tables
{
    /**
     * This arrays should only be indexed with compile time values
     */

    constexpr u64 MASK_RANK[] = {
        0xFF,
        0xFF00,
        0xFF0000,
        0xFF000000,
        0xFF00000000,
        0xFF0000000000,
        0xFF000000000000,
        0xFF00000000000000};

    constexpr u64 MASK_FILE[] = {
        0x0101010101010101,
        0x202020202020202,
        0x404040404040404,
        0x808080808080808,
        0x1010101010101010,
        0x2020202020202020,
        0x4040404040404040,
        0x8080808080808080};

    constexpr u64 MASK_CLEAR_RANK[] = {
        0xFFFFFFFFFFFFFF00,
        0xFFFFFFFFFFFF00FF,
        0xFFFFFFFFFF00FFFF,
        0xFFFFFFFF00FFFFFF,
        0xFFFFFF00FFFFFFFF,
        0xFFFF00FFFFFFFFFF,
        0xFF00FFFFFFFFFFFF,
        0x00FFFFFFFFFFFFFF};

    constexpr u64 MASK_CLEAR_FILE[] = {
        0xFEFEFEFEFEFEFEFE,
        0xFDFDFDFDFDFDFDFD,
        0xFBFBFBFBFBFBFBFB,
        0xF7F7F7F7F7F7F7F7,
        0xEFEFEFEFEFEFEFEF,
        0xDFDFDFDFDFDFDFDF,
        0xBFBFBFBFBFBFBFBF,
        0x7F7F7F7F7F7F7F7F};

    /**
     * @brief Converts a square to its bitboard representation
     *
     * @param sq
     * @return u64
     */
    u64 square_to_bitboard(Square sq);

    /**
     * @brief Gets the relevant bits count for the bishop piece
     *
     * @param sq
     * @return int
     */
    int get_relevant_bits_count_bishop(Square sq);

    /**
     * @brief Gets the relevant bits count for the rook piece
     *
     * @param sq
     * @return int
     */
    int get_relevant_bits_count_rook(Square sq);

    /**
     * @brief Inits all the tables
     *
     */
    void init();

    /**
     * @brief Deallocates all the heap allocated tables
     *
     */
    void teardown();

    /**
     * @brief Gets the pawn attacks
     *
     * @param color
     * @param sq
     * @return u64
     */
    u64 get_pawn_attacks(Color color, Square sq);

    /**
     * @brief Gets the knight attacks
     *
     * @param color
     * @param sq
     * @return u64
     */
    u64 get_knight_attacks(Square sq);

    /**
     * @brief Gets the king attacks
     *
     * @param color
     * @param sq
     * @return u64
     */
    u64 get_king_attacks(Square sq);

    /**
     * @brief Gets the bishop attacks
     *
     * @param sq
     * @param occ
     * @return u64
     */
    u64 get_bishop_attacks(const Square sq, u64 occ);

    /**
     * @brief Gets the rook attacks
     *
     * @param sq
     * @param occ
     * @return u64
     */
    u64 get_rook_attacks(const Square sq, u64 occ);

    /**
     * @brief Gets the queen attacks
     *
     * @param sq
     * @param occ
     * @return u64
     */
    u64 get_queen_attacks(const Square sq, u64 occ);

} // namespace tables