#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#include <codlib/base.hpp>
#include <codlib/move.hpp>

/// @brief Represents a chess board
class Board {
public:
    /// @brief A piece on the board
    struct Piece {
        /// @brief The color of the piece
        Color color;
        /// @brief The type of the piece
        PieceType type;

        friend constexpr bool operator==(const Piece &p1, const Piece &p2) {
            return p1.color == p2.color && p1.type == p2.type;
        }

        friend constexpr bool operator!=(const Piece &p1, const Piece &p2) {
            return p1.color != p2.color || p1.type != p2.type;
        }
    };

    /// @brief A backup of the board state used for undoing moves
    struct StateBackup {
        /// @brief The en passant square
        Square en_passant_square;
        /// @brief The castling rights
        int castling_rights;
        /// @brief The half move clock
        int half_move_clock;
        /// @brief The hash key
        std::uint64_t hash_key;

        friend constexpr bool operator==(const StateBackup &s1, const StateBackup &s2) {
            return s1.castling_rights == s2.castling_rights && s1.en_passant_square == s2.en_passant_square
                   && s1.half_move_clock == s2.half_move_clock;
        }

        friend constexpr bool operator!=(const StateBackup &s1, const StateBackup &s2) {
            return s1.castling_rights != s2.castling_rights || s1.en_passant_square != s2.en_passant_square
                   || s1.half_move_clock != s2.half_move_clock;
        }
    };

private:
    /// @brief Returns the inactive color
    /// @param to_move The color to move
    /// @return The inactive color
    [[nodiscard]] static constexpr auto flip(const Color to_move) noexcept {
        return static_cast<Color>(static_cast<int>(to_move) ^ 1);
    }

public:
    /// @brief Default constructor
    /// @details Sets the board to the starting position
    [[nodiscard]] Board() noexcept { SetStartingPosition(); }

    /// @brief Constructor from FEN
    /// @param piece_placements The piece placements
    /// @param active_color The active color
    /// @param castling_rights The castling rights
    /// @param en_passant The en passant square
    /// @param halfmove_clock The half move clock
    /// @param fullmove_number The full move number
    // TODO: make it receive only a string
    [[nodiscard]] Board(const std::string &piece_placements,
                        const std::string &active_color,
                        const std::string &castling_rights,
                        const std::string &en_passant,
                        const std::string &halfmove_clock,
                        const std::string &fullmove_number) noexcept {
        SetFromFen(piece_placements, active_color, castling_rights, en_passant, halfmove_clock, fullmove_number);
    }

    /// @brief Copy constructor
    /// @param board The board to copy
    [[nodiscard]] Board(const Board &board) noexcept = default;

    // -- Getters --

    /// @brief Returns the current side to move
    /// @return The current side to move
    [[nodiscard]] constexpr auto active() const noexcept { return _active; }

    /// @brief Returns the current inactive color
    /// @return The current inactive color
    [[nodiscard]] constexpr auto inactive() const noexcept { return flip(_active); }

    /// @brief Returns the castling availability
    /// @return The castling rights
    [[nodiscard]] constexpr auto castling_availability() const noexcept { return _castling_availability; }

    /// @brief Returns the half move clock
    /// @return The half move clock
    [[nodiscard]] constexpr auto half_move_clock() const noexcept { return _half_move_clock; }

    /// @brief Returns the full move number
    /// @return The full move number
    [[nodiscard]] constexpr auto full_move_number() const noexcept { return _full_move_number; }

    /// @brief Returns the en passant square
    /// @return The en passant square
    [[nodiscard]] constexpr auto en_passant_square() const noexcept { return _en_passant_square; }

    /// @brief Returns the piece on a square
    /// @param sq The square
    /// @return The piece on the square
    [[nodiscard]] constexpr auto piece(const Square sq) const noexcept { return _piece[sq]; }

    /// @brief Returns the pieces bitboard of a color and type
    /// @param color The color
    /// @param type The type
    [[nodiscard]] constexpr auto pieces(const Color color, const PieceType type) const noexcept {
        return _pieces[color][type];
    }

    /// @brief Returns the occupancy of a color
    /// @param color The color
    /// @return The occupancy of the color
    [[nodiscard]] constexpr auto occupancies(const Color color) const noexcept { return _occupancies[color]; }

    /// @brief Returns the hash key of the board
    /// @return The hash key of the board
    [[maybe_unused]] [[nodiscard]] constexpr auto hash_key() const noexcept { return _hash_key; }

    // -- Getters that require expensive computations (i.e. don't directly correspond to getting a single member variable) --

    /// @brief Returns the current game state
    /// @return The current game state
    [[nodiscard]] constexpr StateBackup GetStateBackup() const noexcept {
        return {_en_passant_square, _castling_availability, _half_move_clock, _hash_key};
    }

    /// @brief Returns the FEN string of the board
    /// @return The FEN string of the board
    [[maybe_unused]] [[nodiscard]] std::string GetFen() const noexcept;

    /// @brief Returns whether a square is attacked by a color
    /// @param sq The square
    /// @param attacker The color
    /// @return Whether the square is attacked by the color
    [[nodiscard]] bool IsSquareAttacked(Square sq, Color attacker) const noexcept;

    // -- Setters --

    /// @brief Switches the side to move
    /// @return The new side to move
    constexpr auto switch_active() noexcept { return _active = flip(_active); }

    /// @brief Sets the current en passant square
    /// @param sq The en passant square
    [[maybe_unused]] void en_passant_square(const Square sq) noexcept { _en_passant_square = sq; }

    /// @brief Sets the current castling rights
    /// @param castling_availability The castling rights
    [[maybe_unused]] void
    castling_availability(const int castling_availability) noexcept { _castling_availability = castling_availability; }

    // -- Setters that require expensive computations (i.e. don't directly correspond to setting a single member variable) --

    /// @brief Sets the current game state
    /// @param state The game state
    [[maybe_unused]] void SetStateFromBackup(const StateBackup &state) noexcept {
        _hash_key = state.hash_key;
        _half_move_clock = state.half_move_clock;
        _en_passant_square = state.en_passant_square;
        _castling_availability = state.castling_rights;
    }

    /// @brief Sets the board to the starting position
    void SetStartingPosition() noexcept {
        SetFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1");
    }

    /// @brief Sets the board from a FEN string
    /// @param piece_placements The piece placements
    /// @param active_color The active color
    /// @param castling_rights The castling rights
    /// @param en_passant The en passant square
    /// @param halfmove_clock The half move clock
    /// @param fullmove_number The full move number
    void SetFromFen(const std::string &piece_placements,
                    const std::string &active_color,
                    const std::string &castling_rights,
                    const std::string &en_passant,
                    const std::string &halfmove_clock,
                    const std::string &fullmove_number) noexcept;

    // -- Chess Logic --

    /// @brief Makes a move on the board
    /// @param move The move
    /// @note This function does not check if the move is legal
    void Make(Move move) noexcept;

    /// @brief Unmakes a move on the board
    /// @param move The move
    /// @param info_board The previous game state
    void Unmake(Move move, StateBackup info_board) noexcept;

private:
    /// @brief The side to move
    Color _active{};
    /// @brief The current castling rights
    int _castling_availability{};
    /// @brief The half move clock
    int _half_move_clock{};
    /// @brief The full move number
    int _full_move_number{};
    /// @brief The hash key of the board
    std::uint64_t _hash_key{};
    /// @brief The en passant square
    Square _en_passant_square{};
    /// @brief The squares representation of the board
    /// @details This is always maintains the same state as the bitboards
    std::array<Piece, N_SQUARES> _piece{};
    /// @brief The occupancy bitboards
    std::array<std::uint64_t, N_SIDES + 1> _occupancies{};
    /// @brief The bitboards for the pieces
    std::array<std::array<std::uint64_t, N_SIDES>, N_PIECES> _pieces{};

    /// @brief Updates the occupancy bitboards
    /// @details This function is called after a move is made
    void update_occupancies() noexcept;

    /// @brief Updates the bitboards from the squares
    /// @details This function is called when setting the board from a FEN string
    void update_bitboards_from_squares() noexcept;
};