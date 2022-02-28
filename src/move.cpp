#include "move.hpp"

std::ostream &operator<<(std::ostream &os, const Move &m)
{
    os << SQUARE_NAMES[m.getFromSquare()] << SQUARE_NAMES[m.getToSquare()];
    if (m.getPromotedPiece())
    {
        os << PIECE_REPR[m.getPromotedPiece() + 6];
    }
    os << "\n";
    return os;
}

Move::Move(
    int source_square,
    int target_square,
    int piece,
    int promoted_piece,
    bool capture,
    bool double_push,
    bool en_passant,
    bool castle)
{
    _move_encoded = source_square |
                    (target_square << 6) |
                    (piece << 12) |
                    (promoted_piece << 16) |
                    (capture << 20) |
                    (double_push << 21) |
                    (en_passant << 22) |
                    (castle << 23);
}

int Move::getFromSquare() const
{
    return (_move_encoded & 0x3f);
}

int Move::getToSquare() const
{
    return ((_move_encoded & 0xfc0) >> 6);
}

int Move::getPiece() const
{
    return ((_move_encoded & 0xf000) >> 12);
}

int Move::getPromotedPiece() const
{
    return ((_move_encoded & 0xf0000) >> 16);
}

bool Move::isCapture() const
{
    return (_move_encoded & 0x100000);
}

bool Move::isDoublePush() const
{
    return (_move_encoded & 0x200000);
}

bool Move::isEnPassant() const
{
    return (_move_encoded & 0x400000);
}

bool Move::isCastle() const
{
    return (_move_encoded & 0x800000);
}
