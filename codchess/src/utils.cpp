#include <codchess/utils.hpp>

#include <regex>

namespace cod::chess::utils {
std::string
SquareToString(const Square sq) noexcept {
    /// @brief The names of the squares aligned with the Square enum
    /// @note The names are compliant with the UCI format
    static const std::string
        SQUARE[static_cast<std::size_t>(Square::N_SQUARES) + 1] = {
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2",
            "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3",
            "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5",
            "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6",
            "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7",
            "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "-"};

    return SQUARE[static_cast<std::size_t>(sq)];
}

std::string
PieceToString(const Piece piece, const Color color, const bool ascii) noexcept {
    /// @brief The representation of the pieces aligned with the Piece enum
    /// @note The first 13 elements are the ascii representation of the pieces
    /// and
    /// @note the last 13 are the unicode representation
    static const std::string PIECE[] = {
        // White pieces in ascii format
        "P",
        "N",
        "B",
        "R",
        "Q",
        "K",

        // Black pieces in ascii format
        "p",
        "n",
        "b",
        "r",
        "q",
        "k",

        // No piece
        " ",

        // White pieces in unicode format
        "♙",
        "♘",
        "♗",
        "♖",
        "♕",
        "♔",

        /// Black pieces in unicode format
        "♟︎",
        "♞",
        "♝",
        "♜",
        "♛",
        "♚",

        // No piece
        " ",
    };

    return PIECE[static_cast<int>(piece) + (color == Color::WHITE ? 0 : 6) +
                 (ascii ? 0 : 13)];
}

bool
ValidFen(const std::string &fen) noexcept {
    static const std::regex frgx(
        R"((([pnbrqkPNBRQK1-8]{1,8})\/?){8} (b|w) (-|K?Q?k?q?) (-|[a-h][3-6]) \d+ \d+)");
    return std::regex_match(fen, frgx);
}
}   // namespace cod::chess::utils