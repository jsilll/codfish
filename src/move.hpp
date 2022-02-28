#include "defs.hpp"

#include <iostream>
#include <string>

class Move
{
private:
    int _move_encoded;

public:
    Move(
        int source_square,
        int target_square,
        int piece,
        int promoted_piece,
        bool capture,
        bool double_push,
        bool en_passant,
        bool castle);

    int getFromSquare() const;
    int getToSquare() const;
    int getPiece() const;
    int getPromotedPiece() const;
    bool isCapture() const;
    bool isDoublePush() const;
    bool isEnPassant() const;
    bool isCastle() const;

    std::string getUCI();
};