#include "attacks.hpp"

namespace cod::chess::attacks {
/// @brief Stores the attacks for the king
bitboard::Bitboard KING_ATTACKS[static_cast<std::size_t>(Square::Total)];

/// @brief Stores the attacks for the knight
bitboard::Bitboard KNIGHT_ATTACKS[static_cast<std::size_t>(Square::Total)];

/// @brief Stores the attacks for the pawns
bitboard::Bitboard PAWN_ATTACKS[static_cast<std::size_t>(Color::Total)]
                               [static_cast<std::size_t>(Square::Total)];

/// @brief Stores each rook attack
/// @note The codchess::Init() function must be called before using this
/// @note The function RookAttacks() should be used to get the actual attacks
auto ROOK_ATTACKS = std::vector<std::vector<bitboard::Bitboard>>(
    static_cast<std::size_t>(Square::Total),
    std::vector<bitboard::Bitboard>(N_ROOK_MAGICS));

/// @brief Stores each bishop attack
/// @note The codchess::Init() function must be called before using this
/// @note The function BishopAttacks() should be used to get the actual attacks
auto BISHOP_ATTACKS = std::vector<std::vector<bitboard::Bitboard>>(
    static_cast<std::size_t>(Square::Total),
    std::vector<bitboard::Bitboard>(N_BISHOP_MAGICS));

/// @brief Returns a bitboard of all the knight attacks.
/// @param knights A bitboard of knights.
/// @return A bitboard of all the knight attacks.
[[nodiscard]] constexpr bitboard::Bitboard
KnightAttacks(const bitboard::Bitboard knights) noexcept {
    constexpr auto CLEAR_FILE_HG =
        utils::MASK_NOT_FILE[static_cast<std::size_t>(File::FH)] &
        utils::MASK_NOT_FILE[static_cast<std::size_t>(File::FG)];
    constexpr auto CLEAR_FILE_AB =
        utils::MASK_NOT_FILE[static_cast<std::size_t>(File::FA)] &
        utils::MASK_NOT_FILE[static_cast<std::size_t>(File::FB)];

    const auto l1 = (bitboard::ShiftOne<bitboard::Direction::WEST>(knights)) &
                    utils::MASK_NOT_FILE[7];
    const auto r1 = (bitboard::ShiftOne<bitboard::Direction::EAST>(knights)) &
                    utils::MASK_NOT_FILE[0];

    const auto h1 = l1 | r1;
    const auto l2 = (knights >> 2) & CLEAR_FILE_HG;
    const auto r2 = (knights << 2) & CLEAR_FILE_AB;
    const auto h2 = l2 | r2;

    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

/// @brief Returns a bitboard of all the king attacks.
/// @param kings A bitboard of kings.
/// @return A bitboard of all the king attacks.
[[nodiscard]] constexpr bitboard::Bitboard
KingAttacks(bitboard::Bitboard kings) noexcept {
    const auto attacks = bitboard::ShiftOne<bitboard::Direction::EAST>(kings) |
                         bitboard::ShiftOne<bitboard::Direction::WEST>(kings);

    kings |= attacks;

    return attacks | bitboard::ShiftOne<bitboard::Direction::NORTH>(kings) |
           bitboard::ShiftOne<bitboard::Direction::SOUTH>(kings);
}

/// @brief Returns a bitboard of all the bishop attacks from a given square with
/// @brief a given block.
/// @param sq The square to generate attacks from.
/// @param block A bitboard of the blocked squares.
/// @return A bitboard of all the bishop attacks.
[[nodiscard]] constexpr bitboard::Bitboard
BishopNoXrayAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto rank = static_cast<int>(utils::GetRank(sq));
    const auto file = static_cast<int>(utils::GetFile(sq));

    auto attacks{bitboard::ZERO};

    for (int r = rank + 1, f = file + 1; r < static_cast<int>(Rank::Total) and
                                         f < static_cast<int>(File::Total);
         ++r, ++f) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int r = rank + 1, f = file - 1; r < static_cast<int>(Rank::Total) and
                                         f >= static_cast<int>(File::FA);
         ++r, --f) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1, f = file + 1; r >= static_cast<int>(Rank::R1) and
                                         f < static_cast<int>(File::Total);
         --r, ++f) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1, f = file - 1; r >= static_cast<int>(Rank::R1) and
                                         f >= static_cast<int>(File::FA);
         --r, --f) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(f)))) &
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
    const auto rank = static_cast<int>(utils::GetRank(sq));
    const auto file = static_cast<int>(utils::GetFile(sq));

    auto attacks{bitboard::ZERO};

    for (int r = rank + 1; r < static_cast<int>(Rank::Total); ++r) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(file))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(file)))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1; r >= static_cast<int>(Rank::R1); --r) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(file))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(file)))) &
            block) {
            break;
        }
    }

    for (int f = file + 1; f < static_cast<int>(File::Total); ++f) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(rank), static_cast<File>(f))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(rank), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int f = file - 1; f >= static_cast<int>(File::FA); --f) {
        attacks |= (bitboard::ONE << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(rank), static_cast<File>(f))));
        if ((bitboard::ONE << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(rank), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    return attacks;
}

void
Init() noexcept {
    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        PAWN_ATTACKS[static_cast<std::size_t>(Color::White)][sq] =
            attacks::PawnAllAttacks<Color::White>(utils::SQUARE_TO_BB[sq]);
        PAWN_ATTACKS[static_cast<std::size_t>(Color::Black)][sq] =
            attacks::PawnAllAttacks<Color::Black>(utils::SQUARE_TO_BB[sq]);
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        KNIGHT_ATTACKS[sq] = attacks::KnightAttacks(utils::SQUARE_TO_BB[sq]);
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        KING_ATTACKS[sq] = attacks::KingAttacks(utils::SQUARE_TO_BB[sq]);
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        const int occupancy_indices = 1
                                      << utils::BISHOP_RELEVANT_BITS_COUNT[sq];
        for (int i = 0; i < occupancy_indices; ++i) {
            const auto magic = magics::BISHOP_MAGIC_TABLE[sq];
            const auto occupancy = bitboard::SetOccupancy(
                i, utils::BISHOP_RELEVANT_BITS_COUNT[sq], magic.mask);
            const auto index =
                static_cast<int>((occupancy * magic.magic) >> magic.shift);
            BISHOP_ATTACKS[sq][index] =
                attacks::BishopNoXrayAttacks((Square) sq, occupancy);
        }
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        const int occupancy_indices = 1 << utils::ROOK_RELEVANT_BITS_COUNT[sq];
        for (int i = 0; i < occupancy_indices; ++i) {
            const auto magic = magics::ROOK_MAGIC_TABLE[sq];
            const auto occupancy = bitboard::SetOccupancy(
                i, utils::ROOK_RELEVANT_BITS_COUNT[sq], magic.mask);
            const auto index =
                static_cast<int>((occupancy * magic.magic) >> magic.shift);
            ROOK_ATTACKS[sq][index] =
                attacks::RookNoXrayAttacks((Square) sq, occupancy);
        }
    }
}

bitboard::Bitboard
BishopAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto idx = magics::MagicIndex(
        block, magics::BISHOP_MAGIC_TABLE[static_cast<std::size_t>(sq)]);
    return BISHOP_ATTACKS[static_cast<std::size_t>(sq)][idx];
}

bitboard::Bitboard
RookAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto idx = magics::MagicIndex(
        block, magics::ROOK_MAGIC_TABLE[static_cast<std::size_t>(sq)]);
    return ROOK_ATTACKS[static_cast<std::size_t>(sq)][idx];
}
}   // namespace cod::chess::attacks