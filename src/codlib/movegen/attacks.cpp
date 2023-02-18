#include <codlib/movegen/attacks.hpp>

#include <codlib/bitboard.hpp>
#include <codlib/utils.hpp>

using bitboard::Bitboard;

namespace attacks {

    Bitboard KING_ATTACKS[N_SQUARES];

    Bitboard KNIGHT_ATTACKS[N_SQUARES];

    Bitboard PAWN_ATTACKS[N_COLORS][N_SQUARES];

    /// @brief Bitboard masks for each rook attack
    /// @note The Init() function must be called before using this
    /// @note The function RookAttacks() should be used to get the actual attacks
    auto ROOK_ATTACKS = std::vector<std::vector<Bitboard>>(N_SQUARES, std::vector<Bitboard>(N_ROOK_MAGICS));

    /// @brief Bitboard masks for each bishop attack
    /// @note The Init() function must be called before using this
    /// @note The function BishopAttacks() should be used to get the actual attacks
    auto BISHOP_ATTACKS = std::vector<std::vector<Bitboard>>(N_SQUARES, std::vector<Bitboard>(N_BISHOP_MAGICS));

    /// @brief Returns a bitboard of all the knight attacks.
    /// @param knights A bitboard of knights.
    /// @return A bitboard of all the knight attacks.
    [[nodiscard]] constexpr Bitboard KnightAttacks(const Bitboard knights) noexcept {
        constexpr Bitboard CLEAR_FILE_HG = 0x3f3f3f3f3f3f3f3f;
        constexpr Bitboard CLEAR_FILE_AB = 0xfcfcfcfcfcfcfcfc;

        Bitboard l1 = (knights >> 1) & utils::MASK_CLEAR_FILE[7];
        Bitboard r1 = (knights << 1) & utils::MASK_CLEAR_FILE[0];
        Bitboard h1 = l1 | r1;

        Bitboard l2 = (knights >> 2) & CLEAR_FILE_HG;
        Bitboard r2 = (knights << 2) & CLEAR_FILE_AB;
        Bitboard h2 = l2 | r2;

        return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
    }

    /// @brief Returns a bitboard of all the king attacks.
    /// @param kings A bitboard of kings.
    /// @return A bitboard of all the king attacks.
    [[nodiscard]] constexpr Bitboard KingAttacks(Bitboard kings) noexcept {
        Bitboard attacks = bitboard::ShiftOne<bitboard::Direction::EAST>(kings) |
                           bitboard::ShiftOne<bitboard::Direction::WEST>(kings);
        kings |= attacks;
        attacks |= bitboard::ShiftOne<bitboard::Direction::NORTH>(kings) |
                   bitboard::ShiftOne<bitboard::Direction::SOUTH>(kings);
        return attacks;
    }

    /// @brief Returns a bitboard of all the bishop attacks from a given square with
    /// a given block.
    /// @param sq The square to generate attacks from.
    /// @param block A bitboard of the blocked squares.
    /// @return A bitboard of all the bishop attacks.
    [[nodiscard]] constexpr bitboard::Bitboard
    BishopNoXrayAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
        bitboard::Bitboard attacks = bitboard::ZERO;
        Rank rank = utils::GetRank(sq);
        File file = utils::GetFile(sq);
        for (int r = rank + 1, f = file + 1; r < N_RANKS && f < N_FILES; ++r, ++f) {
            attacks |= (bitboard::ONE << utils::GetSquare((Rank) r, (File) f));
            if ((bitboard::ONE << utils::GetSquare((Rank) r, (File) f)) & block) { break; }
        }

        for (int r = rank + 1, f = file - 1; r < N_RANKS && f >= FILE_A; ++r, --f) {
            attacks |= (bitboard::ONE << utils::GetSquare((Rank) r, (File) f));
            if ((bitboard::ONE << utils::GetSquare((Rank) r, (File) f)) & block) { break; }
        }

        for (int r = rank - 1, f = file + 1; r >= RANK_1 && f < N_FILES; --r, ++f) {
            attacks |= (bitboard::ONE << utils::GetSquare((Rank) r, (File) f));
            if ((bitboard::ONE << utils::GetSquare((Rank) r, (File) f)) & block) { break; }
        }

        for (int r = rank - 1, f = file - 1; r >= RANK_1 && f >= FILE_A; --r, --f) {
            attacks |= (bitboard::ONE << utils::GetSquare((Rank) r, (File) f));
            if ((bitboard::ONE << utils::GetSquare((Rank) r, (File) f)) & block) { break; }
        }

        return attacks;
    }

    /// @brief Returns a bitboard of all the rook attacks from a given square with a
    /// given block.
    /// @param sq The square to generate attacks from.
    /// @param block A bitboard of the blocked squares.
    /// @return A bitboard of all the rook attacks.
    [[nodiscard]] constexpr bitboard::Bitboard
    RookNoXrayAttacks(const Square sq, const bitboard::Bitboard block) noexcept {
        bitboard::Bitboard attacks = bitboard::ZERO;
        Rank rank = utils::GetRank(sq);
        File file = utils::GetFile(sq);
        for (int r = rank + 1; r < N_RANKS; ++r) {
            attacks |= (bitboard::ONE << utils::GetSquare((Rank) r, file));
            if ((bitboard::ONE << utils::GetSquare((Rank) r, file)) & block) { break; }
        }

        for (int r = rank - 1; r >= RANK_1; --r) {
            attacks |= (bitboard::ONE << utils::GetSquare((Rank) r, file));
            if ((bitboard::ONE << utils::GetSquare((Rank) r, file)) & block) { break; }
        }

        for (int f = file + 1; f < N_FILES; ++f) {
            attacks |= (bitboard::ONE << utils::GetSquare(rank, (File) f));
            if ((bitboard::ONE << utils::GetSquare(rank, (File) f)) & block) { break; }
        }

        for (int f = file - 1; f >= FILE_A; --f) {
            attacks |= (bitboard::ONE << utils::GetSquare(rank, (File) f));
            if ((bitboard::ONE << utils::GetSquare(rank, (File) f)) & block) { break; }
        }

        return attacks;
    }

    void Init() noexcept {
        // Initializing Leaper Piece Attack Tables
        for (int sq = A1; sq < N_SQUARES; ++sq) {
            PAWN_ATTACKS[WHITE][sq] = attacks::PawnAllAttacks<Color::WHITE>(utils::SQUARE_BB[sq]);
            PAWN_ATTACKS[BLACK][sq] = attacks::PawnAllAttacks<Color::BLACK>(utils::SQUARE_BB[sq]);
        }

        for (int sq = A1; sq < N_SQUARES; ++sq) {
            KNIGHT_ATTACKS[sq] = attacks::KnightAttacks(utils::SQUARE_BB[sq]);
        }

        for (int sq = A1; sq < N_SQUARES; ++sq) { KING_ATTACKS[sq] = attacks::KingAttacks(utils::SQUARE_BB[sq]); }

        // Initialize Slider Piece Attack Tables
        for (int sq = A1; sq < N_SQUARES; ++sq) {
            int occupancy_indices = 1 << utils::BISHOP_RELEVANT_BITS_COUNT[sq];
            for (int i = 0; i < occupancy_indices; ++i) {
                magics::Magic magic = magics::BISHOP_MAGIC_TABLE[sq];
                Bitboard occupancy = bitboard::SetOccupancy(i, utils::BISHOP_RELEVANT_BITS_COUNT[sq], magic.mask);
                int index = static_cast<int>((occupancy * magic.magic) >> magic.shift);
                BISHOP_ATTACKS[sq][index] = attacks::BishopNoXrayAttacks((Square) sq, occupancy);
            }
        }

        for (int sq = A1; sq < N_SQUARES; ++sq) {
            int occupancy_indices = 1 << utils::ROOK_RELEVANT_BITS_COUNT[sq];
            for (int i = 0; i < occupancy_indices; ++i) {
                magics::Magic magic = magics::ROOK_MAGIC_TABLE[sq];
                Bitboard occupancy = bitboard::SetOccupancy(i, utils::ROOK_RELEVANT_BITS_COUNT[sq], magic.mask);
                int index = (int) ((occupancy * magic.magic) >> magic.shift);
                ROOK_ATTACKS[sq][index] = attacks::RookNoXrayAttacks((Square) sq, occupancy);
            }
        }
    }

    Bitboard BishopAttacks(const Square sq, const Bitboard block) noexcept {
        const auto idx = magics::MagicIndex(block, magics::BISHOP_MAGIC_TABLE[sq]);
        return BISHOP_ATTACKS[sq][idx];
    }

    Bitboard RookAttacks(const Square sq, const Bitboard block) noexcept {
        const auto idx = magics::MagicIndex(block, magics::ROOK_MAGIC_TABLE[sq]);
        return ROOK_ATTACKS[sq][idx];
    }

}// namespace attacks