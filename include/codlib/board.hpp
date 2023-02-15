#pragma once

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
        /// @brief The type of the piece
        PieceType type;
        /// @brief The color of the piece
        Color color;

        friend constexpr bool operator==(const Piece &p1, const Piece &p2) {
            return p1.color == p2.color && p1.type == p2.type;
        }

        friend constexpr bool operator!=(const Piece &p1, const Piece &p2) {
            return p1.color != p2.color || p1.type != p2.type;
        }
    };

    /// @brief The auxiliary information of the board
    struct GameState {
        /// @brief The en passant square
        Square en_passant_square;
        /// @brief The castling rights
        int castling_rights;
        /// @brief The half move clock
        int half_move_clock;
        /// @brief The hash key
        std::uint64_t hash_key;

        friend constexpr bool operator==(const GameState &s1, const GameState &s2) {
            return s1.castling_rights == s2.castling_rights &&
                   s1.en_passant_square == s2.en_passant_square &&
                   s1.half_move_clock == s2.half_move_clock;
        }

        friend constexpr bool operator!=(const GameState &s1, const GameState &s2) {
            return s1.castling_rights != s2.castling_rights ||
                   s1.en_passant_square != s2.en_passant_square ||
                   s1.half_move_clock != s2.half_move_clock;
        }
    };

private:
    /// @brief Returns the opponent color
    /// @param to_move The color to move
    /// @return The opponent color
    [[nodiscard]] static constexpr auto get_opponent(Color to_move) noexcept { return static_cast<Color>((int) to_move ^ 1); }

    /// @brief Returns the square of a rank and file
    /// @param rk The rank
    /// @param fl The file
    /// @return The square
    [[nodiscard]] static constexpr auto get_square(const Rank rk, const File fl) noexcept { return static_cast<Square>(8 * rk + fl); }

public:
    /// @brief Default constructor
    /// @details Sets the board to the starting position
    [[nodiscard]] Board() noexcept { set_starting_position(); }

    /// @brief Copy constructor
    /// @param board The board to copy
    [[nodiscard]] Board(const Board &board) noexcept = default;

    // -- Getters --

    /// @brief Returns the current side to move
    /// @return The current side to move
    [[nodiscard]] constexpr auto get_side_to_move() const noexcept { return _to_move; }

    /// @brief Returns the current opponent color
    /// @return The current opponent color
    [[nodiscard]] constexpr auto get_opponent() const noexcept { return get_opponent(_to_move); }

    /// @brief Returns the castling rights
    /// @return The castling rights
    [[nodiscard]] constexpr auto get_castling_rights() const noexcept { return _castling_rights; }

    /// @brief Returns the half move clock
    /// @return The half move clock
    [[nodiscard]] constexpr auto get_half_move_clock() const noexcept { return _half_move_clock; }

    /// @brief Returns whether the white pieces are on the bottom
    /// @return Whether the white pieces are on the bottom
    [[nodiscard]] constexpr auto is_white_on_bottom() const noexcept { return _white_on_bottom; }

    /// @brief Returns the full move number
    /// @return The full move number
    [[nodiscard]] constexpr auto get_full_move_number() const noexcept { return _full_move_number; }

    /// @brief Returns the en passant square
    /// @return The en passant square
    [[nodiscard]] constexpr auto get_en_passant_square() const noexcept { return _en_passant_square; }

    /// @brief Returns the current game state
    /// @return The current game state
    [[nodiscard]] constexpr auto get_state() const noexcept {
        // TODO: Board should have a GameState field, instead of having to create a new one every time
        return GameState{_en_passant_square,
                         _castling_rights,
                         _half_move_clock,
                         _hash_key};
    }

    /// @brief Returns the piece on a square
    /// @param sq The square
    /// @return The piece on the square
    [[nodiscard]] constexpr auto get_piece_from_square(Square sq) const noexcept { return _square[sq]; }

    /// @brief Returns whether a square is attacked by a color
    /// @param sq The square
    /// @param attacker The color
    /// @return Whether the square is attacked by the color
    [[nodiscard]] bool is_square_attacked(Square sq, Color attacker) const noexcept;

    /// @brief Returns the pieces bitboard of a color and type
    /// @param color The color
    /// @param type The type
    [[nodiscard]] constexpr auto get_pieces(Color color, PieceType type) const noexcept { return _pieces[color][type]; }

    /// @brief Returns the occupancy of a color
    /// @param color The color
    /// @return The occupancy of the color
    [[nodiscard]] constexpr auto get_occupancies(Color color) const noexcept { return _occupancies[color]; }

    /// @brief Returns the hash key of the board
    /// @return The hash key of the board
    [[maybe_unused]] [[nodiscard]] constexpr auto get_hash_key() const noexcept { return _hash_key; }

    /// @brief Returns the FEN string of the board
    /// @return The FEN string of the board
    [[maybe_unused]] [[nodiscard]] std::string get_fen() const noexcept;

    /// @brief Returns whether the board is in ASCII mode
    /// @return Whether the board is in ASCII mode
    [[nodiscard]] constexpr auto is_ascii() const noexcept { return _ascii; }

    // -- Setters --

    /// @brief Sets the board to the starting position
    void set_starting_position() noexcept {
        set_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1");
    }

    /// @brief Switches the side to move
    /// @return The new side to move
    constexpr auto switch_side_to_move() noexcept { return _to_move = get_opponent(); }

    /// @brief Sets the current en passant square
    /// @param sq The en passant square
    [[maybe_unused]] void set_en_passant_square(Square sq) noexcept { _en_passant_square = sq; }

    /// @brief Sets the current castling rights
    /// @param castling_rights The castling rights
    [[maybe_unused]] void set_castling_rights(int castling_rights) noexcept { _castling_rights = castling_rights; }

    /// @brief Sets the current game state
    /// @param state The game state
    [[maybe_unused]] void set_state(const GameState &state) noexcept {
        // TODO: Board should have a GameState field, instead of having to unpack it every time
        _hash_key = state.hash_key;
        _castling_rights = state.castling_rights;
        _half_move_clock = state.half_move_clock;
        _en_passant_square = state.en_passant_square;
    }

    /// @brief Toggles the ASCII mode
    /// @return Whether the board is in ASCII mode
    [[maybe_unused]] constexpr auto toggle_ascii() noexcept { return _ascii = !_ascii; }

    /// @brief Rotates the board
    /// @return Whether white is on the bottom
    [[maybe_unused]] constexpr auto rotate_display() noexcept { return _white_on_bottom = !_white_on_bottom; }

    /// @brief Sets the board from a FEN string
    /// @param piece_placements The piece placements
    /// @param active_color The active color
    /// @param castling_rights The castling rights
    /// @param en_passant The en passant square
    /// @param halfmove_clock The half move clock
    /// @param fullmove_number The full move number
    void set_from_fen(const std::string &piece_placements,
                      const std::string &active_color,
                      const std::string &castling_rights,
                      const std::string &en_passant,
                      const std::string &halfmove_clock,
                      const std::string &fullmove_number) noexcept;

    /// @brief Makes a move on the board
    /// @param move The move
    /// @note This function does not check if the move is legal
    void make(Move move) noexcept;

    /// @brief Unmakes a move on the board
    /// @param move The move
    /// @param info_board The previous game state
    void unmake(Move move, GameState info_board) noexcept;

    // -- Overloaded Operators --

    /// @brief Outputs the string representation of the board
    /// @param os The output stream
    /// @param board The board
    /// @return The output stream
    friend std::ostream &operator<<(std::ostream &os, const Board &board) noexcept;

    /// @brief Checks if two boards are equal
    /// @param b1 The first board
    /// @param b2 The second board
    /// @return Whether the boards are equal
    friend constexpr bool operator==(const Board &b1, const Board &b2) noexcept;

private:
    /// @brief Whether the board is in ASCII mode
    bool _ascii{};
    /// @brief The side to move
    Color _to_move{};
    /// @brief The current castling rights
    int _castling_rights{};
    /// @brief The half move clock
    int _half_move_clock{};
    /// @brief The full move number
    int _full_move_number{};
    /// @brief Whether the white pieces are on the bottom
    bool _white_on_bottom{};
    /// @brief The hash key of the board
    std::uint64_t _hash_key{};
    /// @brief The squares representation of the board
    /// @details This is always maintains the same state as the bitboards
    Piece _square[N_SQUARES]{};
    /// @brief The en passant square
    Square _en_passant_square{};
    /// @brief The occupancy bitboards
    std::uint64_t _occupancies[N_SIDES + 1]{};
    /// @brief The bitboards for the pieces
    std::uint64_t _pieces[N_SIDES][N_PIECES]{};

    /// @brief Updates the occupancy bitboards
    /// @details This function is called after a move is made
    void update_occupancies() noexcept;

    /// @brief Updates the bitboards from the squares
    /// @details This function is called when setting the board from a FEN string
    void update_bitboards_from_squares() noexcept;
};