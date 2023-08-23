#include "attacks.hpp"

namespace cod::chess::attacks {
/// @brief Stores the attacks for the king
/// @note The cod::chess::Init() function must be called before using this
bitboard::Bitboard kKingAttacks[static_cast<std::size_t>(Square::Total)];

/// @brief Stores the attacks for the knight
/// @note The cod::chess::Init() function must be called before using this
bitboard::Bitboard kKnightAttacks[static_cast<std::size_t>(Square::Total)];

/// @brief Stores the attacks for the pawns
/// @note The cod::chess::Init() function must be called before using this
bitboard::Bitboard kPawnAttacks[static_cast<std::size_t>(Color::Total)]
                               [static_cast<std::size_t>(Square::Total)];

/// @brief Stores each rook attack
/// @note The cod::chess::Init() function must be called before using this
/// @note The function RookAttacks() should be used to get the actual attacks
auto kRookAttacks = std::vector<std::vector<bitboard::Bitboard>>(
    static_cast<std::size_t>(Square::Total),
    std::vector<bitboard::Bitboard>(kRookMagics));

/// @brief Stores each bishop attack
/// @note The cod::chess::Init() function must be called before using this
/// @note The function BishopAttacks() should be used to get the actual attacks
auto kBishopAttacks = std::vector<std::vector<bitboard::Bitboard>>(
    static_cast<std::size_t>(Square::Total),
    std::vector<bitboard::Bitboard>(kBishopMagics));

/// @brief Returns a bitboard of all the knight attacks.
/// @param knights A bitboard of knights.
/// @return A bitboard of all the knight attacks.
[[nodiscard]] constexpr bitboard::Bitboard
KnightAttacks(const bitboard::Bitboard knights) noexcept {
    constexpr auto CLEAR_FILE_HG =
        utils::kMaskNotFile[static_cast<std::size_t>(File::FH)] &
        utils::kMaskNotFile[static_cast<std::size_t>(File::FG)];
    constexpr auto CLEAR_FILE_AB =
        utils::kMaskNotFile[static_cast<std::size_t>(File::FA)] &
        utils::kMaskNotFile[static_cast<std::size_t>(File::FB)];

    const auto l1 = (bitboard::ShiftOne<bitboard::Direction::West>(knights)) &
                    utils::kMaskNotFile[7];
    const auto r1 = (bitboard::ShiftOne<bitboard::Direction::East>(knights)) &
                    utils::kMaskNotFile[0];

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
    const auto attacks = bitboard::ShiftOne<bitboard::Direction::East>(kings) |
                         bitboard::ShiftOne<bitboard::Direction::West>(kings);

    kings |= attacks;

    return attacks | bitboard::ShiftOne<bitboard::Direction::North>(kings) |
           bitboard::ShiftOne<bitboard::Direction::South>(kings);
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

    auto attacks{bitboard::kZero};

    for (int r = rank + 1, f = file + 1; r < static_cast<int>(Rank::Total) and
                                         f < static_cast<int>(File::Total);
         ++r, ++f) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int r = rank + 1, f = file - 1;
         r < static_cast<int>(Rank::Total) and f >= static_cast<int>(File::FA);
         ++r, --f) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1, f = file + 1;
         r >= static_cast<int>(Rank::R1) and f < static_cast<int>(File::Total);
         --r, ++f) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1, f = file - 1;
         r >= static_cast<int>(Rank::R1) and f >= static_cast<int>(File::FA);
         --r, --f) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(f))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
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

    auto attacks{bitboard::kZero};

    for (int r = rank + 1; r < static_cast<int>(Rank::Total); ++r) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(file))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(file)))) &
            block) {
            break;
        }
    }

    for (int r = rank - 1; r >= static_cast<int>(Rank::R1); --r) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(r), static_cast<File>(file))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(r), static_cast<File>(file)))) &
            block) {
            break;
        }
    }

    for (int f = file + 1; f < static_cast<int>(File::Total); ++f) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(rank), static_cast<File>(f))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
                 static_cast<Rank>(rank), static_cast<File>(f)))) &
            block) {
            break;
        }
    }

    for (int f = file - 1; f >= static_cast<int>(File::FA); --f) {
        attacks |= (bitboard::kOne << static_cast<int>(utils::GetSquare(
                        static_cast<Rank>(rank), static_cast<File>(f))));
        if ((bitboard::kOne << static_cast<int>(utils::GetSquare(
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
        kPawnAttacks[static_cast<std::size_t>(Color::White)][sq] =
            attacks::PawnAllAttacks<Color::White>(utils::kSquareToBitboard[sq]);
        kPawnAttacks[static_cast<std::size_t>(Color::Black)][sq] =
            attacks::PawnAllAttacks<Color::Black>(utils::kSquareToBitboard[sq]);
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        kKnightAttacks[sq] =
            attacks::KnightAttacks(utils::kSquareToBitboard[sq]);
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        kKingAttacks[sq] = attacks::KingAttacks(utils::kSquareToBitboard[sq]);
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        const int occupancy_indices = 1 << utils::kBishopRelevantBits[sq];
        for (int i = 0; i < occupancy_indices; ++i) {
            const auto magic = magics::kBishopTable[sq];
            const auto occupancy = bitboard::SetOccupancy(
                i, utils::kBishopRelevantBits[sq], magic.mask);
            const auto index =
                static_cast<int>((occupancy * magic.magic) >> magic.shift);
            kBishopAttacks[sq][index] =
                attacks::BishopNoXrayAttacks((Square) sq, occupancy);
        }
    }

    for (int sq = static_cast<int>(Square::A1);
         sq < static_cast<int>(Square::Total); ++sq) {
        const int occupancy_indices = 1 << utils::kRookRelevantBits[sq];
        for (int i = 0; i < occupancy_indices; ++i) {
            const auto magic = magics::kRookTable[sq];
            const auto occupancy = bitboard::SetOccupancy(
                i, utils::kRookRelevantBits[sq], magic.mask);
            const auto index =
                static_cast<int>((occupancy * magic.magic) >> magic.shift);
            kRookAttacks[sq][index] =
                attacks::RookNoXrayAttacks((Square) sq, occupancy);
        }
    }
}

bitboard::Bitboard
BishopAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto idx = magics::MagicIndex(
        block, magics::kBishopTable[static_cast<std::size_t>(sq)]);
    return kBishopAttacks[static_cast<std::size_t>(sq)][idx];
}

bitboard::Bitboard
RookAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
    const auto idx = magics::MagicIndex(
        block, magics::kRookTable[static_cast<std::size_t>(sq)]);
    return kRookAttacks[static_cast<std::size_t>(sq)][idx];
}
}   // namespace cod::chess::attacks