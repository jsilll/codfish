#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#include <codchess/base.hpp>
#include <codchess/move.hpp>
#include <codchess/utils.hpp>

namespace codchess {
/// @brief Represents a chess board
class Board {
  public:
    /// @brief A piece on the board
    struct Piece {
        /// @brief The color of the piece
        Color color;
        /// @brief The type of the piece
        PieceType type;

        /// @brief Equality operator
        /// @param p1 Piece 1
        /// @param p2 Piece 2
        /// @return True if the pieces are equal, false otherwise
        friend constexpr bool operator==(const Piece &p1,
                                         const Piece &p2) noexcept {
            return p1.color == p2.color and p1.type == p2.type;
        }

        /// @brief Inequality operator
        /// @param p1 Piece 1
        /// @param p2 Piece 2
        /// @return True if the pieces are not equal, false otherwise
        friend constexpr bool operator!=(const Piece &p1,
                                         const Piece &p2) noexcept {
            return p1.color != p2.color or p1.type != p2.type;
        }
    };

    /// @brief A backup of the board state used for undoing moves
    struct StateBackup {
        /// @brief The en passant square
        Square en_passant_square;
        /// @brief The castling_availability rights
        int castling_availability;
        /// @brief The half move clock
        int half_move_clock;
        /// @brief The full move number
        int full_move_number;
        /// @brief The hash key
        std::uint64_t hash;

        /// @brief Equality operator
        /// @param s1 State backup 1
        /// @param s2 State backup 2
        /// @return True if the state backups are equal, false otherwise
        friend constexpr bool operator==(const StateBackup &s1,
                                         const StateBackup &s2) noexcept {
            return s1.en_passant_square == s2.en_passant_square &&
                   s1.castling_availability == s2.castling_availability &&
                   s1.half_move_clock == s2.half_move_clock &&
                   s1.full_move_number == s2.full_move_number &&
                   s1.hash == s2.hash;
        }

        /// @brief Inequality operator
        /// @param s1 State backup 1
        /// @param s2 State backup 2
        /// @return True if the state backups are not equal, false otherwise
        friend constexpr bool operator!=(const StateBackup &s1,
                                         const StateBackup &s2) noexcept {
            return !(s1 == s2);
        }
    };

    /// @brief Default constructor
    /// @details Sets the board to the starting position
    [[nodiscard]] Board() noexcept { SetStartingPosition(); }

    /// @brief Constructor from FEN
    /// @note The fen string is assumed to be valid
    [[nodiscard]] explicit Board(const std::string &fen) noexcept {
        SetFromFen(fen);
    }

    /// @brief Copy constructor
    /// @param board The board to copy
    [[nodiscard]] Board(const Board &board) noexcept = default;

    /// @brief Returns the current side to move
    /// @return The current side to move
    [[nodiscard]] constexpr auto active() const noexcept { return _active; }

    /// @brief Returns the current inactive color
    /// @return The current inactive color
    [[nodiscard]] constexpr auto inactive() const noexcept {
        return utils::FlipColor(_active);
    }

    /// @brief Returns the castling_availability availability
    /// @return The castling_availability rights
    [[nodiscard]] constexpr auto castling_availability() const noexcept {
        return _castling_availability;
    }

    /// @brief Returns the half move clock
    /// @return The half move clock
    [[nodiscard]] constexpr auto half_move_clock() const noexcept {
        return _half_move_clock;
    }

    /// @brief Returns the full move number
    /// @return The full move number
    [[nodiscard]] constexpr auto full_move_number() const noexcept {
        return _full_move_number;
    }

    /// @brief Returns the en passant square
    /// @return The en passant square
    [[nodiscard]] constexpr auto en_passant_square() const noexcept {
        return _en_passant_square;
    }

    /// @brief Returns the piece on a square
    /// @param sq The square
    /// @return The piece on the square
    [[nodiscard]] constexpr auto piece(const Square sq) const noexcept {
        return _piece[sq];
    }

    /// @brief Returns the pieces bitboard of a color and type
    /// @param color The color
    /// @param type The type
    [[nodiscard]] constexpr auto pieces(const Color color,
                                        const PieceType type) const noexcept {
        return _pieces[color][type];
    }

    /// @brief Returns the occupancy of a color
    /// @param color The color
    /// @return The occupancy of the color
    [[nodiscard]] constexpr auto occupancies(const Color color) const noexcept {
        return _occupancies[color];
    }

    /// @brief Returns the hash key of the board
    /// @return The hash key of the board
    [[maybe_unused]] [[nodiscard]] constexpr auto hash() const noexcept {
        return _hash;
    }

    /// @brief Returns the current game state
    /// @return The current game state
    [[nodiscard]] constexpr StateBackup GetStateBackup() const noexcept {
        return {_en_passant_square, _castling_availability, _half_move_clock,
                _full_move_number, _hash};
    }

    /// @brief Returns the FEN string of the board
    /// @return The FEN string of the board
    [[maybe_unused]] [[nodiscard]] std::string GetFen() const noexcept;

    /// @brief Returns whether a square is attacked by a color
    /// @param sq The square
    /// @param attacker The color
    /// @return Whether the square is attacked by the color
    [[nodiscard]] bool IsSquareAttacked(Square sq,
                                        Color attacker) const noexcept;

    /// @brief Sets the current en passant square
    /// @param sq The en passant square
    [[maybe_unused]] void en_passant_square(const Square sq) noexcept {
        _en_passant_square = sq;
    }

    /// @brief Sets the current castling_availability rights
    /// @param castling_availability The castling_availability rights
    [[maybe_unused]] void
    castling_availability(const int castling_availability) noexcept {
        _castling_availability = castling_availability;
    }

    constexpr void SetStateBackup(const StateBackup &sb) noexcept {
        _hash = sb.hash;
        _half_move_clock = sb.half_move_clock;
        _full_move_number = sb.full_move_number;
        _en_passant_square = sb.en_passant_square;
        _castling_availability = sb.castling_availability;
    }

    /// @brief Switches the side to move
    /// @return The new side to move
    constexpr auto SwitchActive() noexcept {
        return _active = utils::FlipColor(_active);
    }

    /// @brief Sets the board to the starting position
    void SetStartingPosition() noexcept {
        SetFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR, w KQkq - 0 1");
    }

    /// @brief Sets the board from a FEN string
    /// @param fen The FEN string
    /// @note The fen string is assumed to be valid
    void SetFromFen(const std::string &fen) noexcept;

    /// @brief Makes a move on the board
    /// @param move The move
    /// @note This function does not check if the move is legal
    void Make(Move move) noexcept;

    /// @brief Unmakes a move on the board
    /// @param move The move
    /// @param backup The previous game state
    void Unmake(Move move, const StateBackup &backup) noexcept;

    /// @brief Displays the board
    /// @param os The output stream
    /// @param ascii Whether to display the board in ASCII
    /// @param color Whether to display the board with white on bottom
    void Display(std::ostream &os, bool ascii = true,
                 bool white_on_bottom = true) const noexcept;

  private:
    /// @brief Represents a FEN string
    struct Fen {
        /// @brief The position field
        std::string position{};
        /// @brief The active color field
        char active{};
        /// @brief The castling_availability rights field
        std::string castling_availability{};
        /// @brief The en passant field
        std::string en_passant_square{};
        /// @brief The half move clock field
        int half_move_clock{};
        /// @brief The full move number field
        int full_move_number{};

        /// @note Constructs a FEN object from a FEN string
        /// @param fen The FEN string
        /// @note The fen string is assumed to be valid
        [[nodiscard]] explicit Fen(const std::string &fen) noexcept {
            std::stringstream ss(fen);
            std::getline(ss, position, ' ');
            ss >> active;
            ss >> castling_availability;
            ss >> en_passant_square;
            ss >> half_move_clock;
            ss >> full_move_number;
        }
    };

    /// @brief The squares representation of the board
    /// @note This should be synced with the bitboards
    Piece _piece[N_SQUARES]{};
    /// @brief The occupancy bitboards
    /// @note This should be synced with the bitboards
    std::uint64_t _occupancies[N_COLORS + 1]{};
    /// @brief The bitboards for the pieces
    /// @note This should be synced with the arrays above
    std::uint64_t _pieces[N_COLORS][N_PIECES]{};

    /// @brief The side to move
    Color _active{};
    /// @brief The current castling_availability rights
    int _castling_availability{};
    /// @brief The half move clock
    int _half_move_clock{};
    /// @brief The full move number
    int _full_move_number{};
    /// @brief The en passant square
    Square _en_passant_square{};

    /// @brief The hash key of the board
    std::uint64_t _hash{};

    /// @brief Updates the bitboards from the piece array
    /// @note This function is called when setting the board from a FEN
    /// @note string
    void UpdateBitboards() noexcept;

    /// @brief Updates the occupancy bitboards from the pieces bitboards
    /// @note This function is called after a move is made
    void UpdateOccupancies() noexcept;
};
}   // namespace codchess