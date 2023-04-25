#include <codchess/movegen/attacks.hpp>

#include <codchess/bitboard.hpp>
#include <codchess/utils.hpp>

namespace codchess::attacks {
/// @brief Stores the attacks for the king
bitboard::Bitboard KING_ATTACKS[N_SQUARES];

/// @brief Stores the attacks for the knight
bitboard::Bitboard KNIGHT_ATTACKS[N_SQUARES];

/// @brief Stores the attacks for the pawns
bitboard::Bitboard PAWN_ATTACKS[N_COLORS][N_SQUARES];

/// @brief Stores each rook attack
/// @note The codchess::Init() function must be called before using this
/// @note The function RookAttacks() should be used to get the actual attacks
auto ROOK_ATTACKS = std::vector<std::vector<bitboard::Bitboard>>(
    N_SQUARES, std::vector<bitboard::Bitboard>(N_ROOK_MAGICS));

/// @brief Stores each bishop attack
/// @note The codchess::Init() function must be called before using this
/// @note The function BishopAttacks() should be used to get the actual attacks
auto BISHOP_ATTACKS = std::vector<std::vector<bitboard::Bitboard>>(
    N_SQUARES, std::vector<bitboard::Bitboard>(N_BISHOP_MAGICS));

/// @brief Returns a bitboard of all the knight attacks.
/// @param knights A bitboard of knights.
/// @return A bitboard of all the knight attacks.
[[nodiscard]] constexpr bitboard::Bitboard
KnightAttacks(const bitboard::Bitboard knights) noexcept {
    constexpr bitboard::Bitboard CLEAR_FILE_HG =
        utils::MASK_CLEAR_FILE[FILE_H] | utils::MASK_CLEAR_FILE[FILE_G];
    constexpr bitboard::Bitboard CLEAR_FILE_AB =
        utils::MASK_CLEAR_FILE[FILE_A] | utils::MASK_CLEAR_FILE[FILE_B];

    bitboard::Bitboard l1 =
        (bitboard::ShiftOne<bitboard::Direction::WEST>(knights)) &
        utils::MASK_CLEAR_FILE[7];
    bitboard::Bitboard r1 =
        (bitboard::ShiftOne<bitboard::Direction::EAST>(knights)) &
        utils::MASK_CLEAR_FILE[0];

    bitboard::Bitboard h1 = l1 | r1;
    bitboard::Bitboard l2 = (knights >> 2) & CLEAR_FILE_HG;
    bitboard::Bitboard r2 = (knights << 2) & CLEAR_FILE_AB;
    bitboard::Bitboard h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

/// @brief Returns a bitboard of all the king attacks.
/// @param kings A bitboard of kings.
/// @return A bitboard of all the king attacks.
[[nodiscard]] constexpr bitboard::Bitboard
KingAttacks(bitboard::Bitboard kings) noexcept {
    bitboard::Bitboard attacks =
        bitboard::ShiftOne<bitboard::Direction::EAST>(kings) |
        bitboard::ShiftOne<bitboard::Direction::WEST>(kings);

    kings |= attacks;
    attacks |= bitboard::ShiftOne<bitboard::Direction::NORTH>(kings) |
               bitboard::ShiftOne<bitboard::Direction::SOUTH>(kings);

    return attacks;
}

/// @brief Returns a bitboard of all the bishop attacks from a given square with
/// @brief a given block.
/// @param sq The square to generate attacks from.
/// @param block A bitboard of the blocked squares.
/// @return A bitboard of all the bishop attacks.
[[nodiscard]] constexpr bitboard::Bitboard
BishopNoXrayAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto rank = utils::GetRank(sq);
    const auto file = utils::GetFile(sq);

    auto attacks{bitboard::ZERO};

    for (int r = rank + 1, f = file + 1; r < N_RANKS && f < N_FILES; ++r, ++f) {
        attacks |= (bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                                      static_cast<File>(f)));
        if ((bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                               static_cast<File>(f))) &
            block) {
            break;
        }
    }

    for (int r = rank + 1, f = file - 1; r < N_RANKS && f >= FILE_A; ++r, --f) {
        attacks |= (bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                                      static_cast<File>(f)));
        if ((bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                               static_cast<File>(f))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1, f = file + 1; r >= RANK_1 && f < N_FILES; --r, ++f) {
        attacks |= (bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                                      static_cast<File>(f)));
        if ((bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                               static_cast<File>(f))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1, f = file - 1; r >= RANK_1 && f >= FILE_A; --r, --f) {
        attacks |= (bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                                      static_cast<File>(f)));
        if ((bitboard::ONE << utils::GetSquare(static_cast<Rank>(r),
                                               static_cast<File>(f))) &
            block) {
            break;
        }
    }

    return attacks;
}

/// @brief Returns a bitboard of all the rook attacks from a given square with a
/// @brief given block.
/// @param sq The square to generate attacks from.
/// @param block A bitboard of the blocked squares.
/// @return A bitboard of all the rook attacks.
[[nodiscard]] constexpr bitboard::Bitboard
RookNoXrayAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto rank = utils::GetRank(sq);
    const auto file = utils::GetFile(sq);

    auto attacks{bitboard::ZERO};

    for (int r = rank + 1; r < N_RANKS; ++r) {
        attacks |=
            (bitboard::ONE << utils::GetSquare(static_cast<Rank>(r), file));
        if ((bitboard::ONE << utils::GetSquare(static_cast<Rank>(r), file)) &
            block) {
            break;
        }
    }

    for (int r = rank - 1; r >= RANK_1; --r) {
        attacks |=
            (bitboard::ONE << utils::GetSquare(static_cast<Rank>(r), file));
        if ((bitboard::ONE << utils::GetSquare(static_cast<Rank>(r), file)) &
            block) {
            break;
        }
    }

    for (int f = file + 1; f < N_FILES; ++f) {
        attacks |=
            (bitboard::ONE << utils::GetSquare(rank, static_cast<File>(f)));
        if ((bitboard::ONE << utils::GetSquare(rank, static_cast<File>(f))) &
            block) {
            break;
        }
    }

    for (int f = file - 1; f >= FILE_A; --f) {
        attacks |=
            (bitboard::ONE << utils::GetSquare(rank, static_cast<File>(f)));
        if ((bitboard::ONE << utils::GetSquare(rank, static_cast<File>(f))) &
            block) {
            break;
        }
    }

    return attacks;
}

void
Init() noexcept {
    for (int sq = A1; sq < N_SQUARES; ++sq) {
        PAWN_ATTACKS[WHITE][sq] =
            attacks::PawnAllAttacks<Color::WHITE>(utils::SQUARE_BB[sq]);
        PAWN_ATTACKS[BLACK][sq] =
            attacks::PawnAllAttacks<Color::BLACK>(utils::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; ++sq) {
        KNIGHT_ATTACKS[sq] = attacks::KnightAttacks(utils::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; ++sq) {
        KING_ATTACKS[sq] = attacks::KingAttacks(utils::SQUARE_BB[sq]);
    }

    for (int sq = A1; sq < N_SQUARES; ++sq) {
        int occupancy_indices = 1 << utils::BISHOP_RELEVANT_BITS_COUNT[sq];
        for (int i = 0; i < occupancy_indices; ++i) {
            magics::Magic magic = magics::BISHOP_MAGIC_TABLE[sq];
            bitboard::Bitboard occupancy = bitboard::SetOccupancy(
                i, utils::BISHOP_RELEVANT_BITS_COUNT[sq], magic.mask);
            const auto index =
                static_cast<int>((occupancy * magic.magic) >> magic.shift);
            BISHOP_ATTACKS[sq][index] =
                attacks::BishopNoXrayAttacks((Square) sq, occupancy);
        }
    }

    for (int sq = A1; sq < N_SQUARES; ++sq) {
        int occupancy_indices = 1 << utils::ROOK_RELEVANT_BITS_COUNT[sq];
        for (int i = 0; i < occupancy_indices; ++i) {
            magics::Magic magic = magics::ROOK_MAGIC_TABLE[sq];
            bitboard::Bitboard occupancy = bitboard::SetOccupancy(
                i, utils::ROOK_RELEVANT_BITS_COUNT[sq], magic.mask);
            int index = (int) ((occupancy * magic.magic) >> magic.shift);
            ROOK_ATTACKS[sq][index] =
                attacks::RookNoXrayAttacks((Square) sq, occupancy);
        }
    }
}

bitboard::Bitboard
BishopAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto idx = magics::MagicIndex(block, magics::BISHOP_MAGIC_TABLE[sq]);
    return BISHOP_ATTACKS[sq][idx];
}

bitboard::Bitboard
RookAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto idx = magics::MagicIndex(block, magics::ROOK_MAGIC_TABLE[sq]);
    return ROOK_ATTACKS[sq][idx];
}
}   // namespace codchess::attacks