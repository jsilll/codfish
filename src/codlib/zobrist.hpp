#pragma once

#include "bitboard.hpp"
#include "codlib/base.hpp"
#include "codlib/board.hpp"

namespace zobrist {
    /// @brief The number of castling keys
    constexpr std::size_t N_CASTLING_KEYS = 16;

    /// @brief The keys for the side to move
    static u64 side_key[BOTH]{};

    /// @brief The keys for the en passant squares
    static u64 en_passant_keys[N_SQUARES];

    /// @brief The keys for the castling rights
    static u64 castle_keys[N_CASTLING_KEYS];

    /// @brief The keys for the pieces
    static u64 piece_keys[N_SIDES][N_PIECES][N_SQUARES];

    /// @brief Generates a random number
    /// @return The random number
    u64 generate_random_number_u64() noexcept {
        u64 n1 = (static_cast<u64>(std::rand()));
        u64 n2 = (static_cast<u64>(std::rand()));
        return n1 | (n2 << 32);
    }

    /// @brief Initializes the zobrist keys
    void init() noexcept {
        for (auto &en_passant_key: en_passant_keys) {
            en_passant_key = generate_random_number_u64();
        }

        for (auto &castle_key: castle_keys) {
            castle_key = generate_random_number_u64();
        }

        for (int piece = PAWN; piece < N_PIECES; piece++) {
            for (int square = A1; square < N_SQUARES; square++) {
                piece_keys[WHITE][piece][square] = generate_random_number_u64();
                piece_keys[BLACK][piece][square] = generate_random_number_u64();
            }
        }

        side_key[1] = generate_random_number_u64();
    }

    /// @brief Generates the hash key for a board
    /// @param board The board
    /// @return The hash key
    u64 generate_hash_key(const Board &board) noexcept {
        u64 final_key = bitboard::kZERO;
        for (int piece = PAWN; piece < N_PIECES; piece++) {
            for (int side = WHITE; side < BOTH; side++) {
                u64 bitboard = board.get_pieces((Color) side, (PieceType) piece);
                while (bitboard) {
                    Square sq = bitboard::bit_scan_forward(bitboard);
                    final_key ^= piece_keys[side][piece][sq];
                    bitboard::pop_bit(bitboard, sq);
                }
            }
        }

        if (board.get_en_passant_square() != EMPTY_SQUARE) {
            final_key ^= en_passant_keys[board.get_en_passant_square()];
        }

        final_key ^= castle_keys[board.get_castling_rights()];
        final_key ^= side_key[board.get_side_to_move()];

        return final_key;
    }
} // namespace zobrist
