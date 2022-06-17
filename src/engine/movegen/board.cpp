#include "board.hpp"

#include "utils.hpp"
#include "bitboard.hpp"
#include "magics.hpp"
#include "move.hpp"
#include "tables.hpp"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <string>

// Constructors

Board::Board() { this->setStartingPosition(); }

Board::Board(const Board &board)
{
  memcpy(_pieces, board._pieces, sizeof(_pieces));
  memcpy(_occupancies, board._occupancies, sizeof(_occupancies));

  _to_move = board._to_move;
  _castling_rights = board._castling_rights;
  _en_passant_square = board._en_passant_square;
  _half_move_clock = board._half_move_clock;
  _full_move_number = board._full_move_number;

  _white_on_bottom = board._white_on_bottom;
  memcpy(_square, board._square, sizeof(_square));
}

// Private Methods

void Board::updateOccupancies()
{
  _occupancies[WHITE] = ZERO;
  _occupancies[BLACK] = ZERO;
  _occupancies[BOTH] = ZERO;

  _occupancies[WHITE] |= _pieces[WHITE][PAWN];
  _occupancies[WHITE] |= _pieces[WHITE][KNIGHT];
  _occupancies[WHITE] |= _pieces[WHITE][BISHOP];
  _occupancies[WHITE] |= _pieces[WHITE][ROOK];
  _occupancies[WHITE] |= _pieces[WHITE][QUEEN];
  _occupancies[WHITE] |= _pieces[WHITE][KING];

  _occupancies[BLACK] |= _pieces[BLACK][PAWN];
  _occupancies[BLACK] |= _pieces[BLACK][KNIGHT];
  _occupancies[BLACK] |= _pieces[BLACK][BISHOP];
  _occupancies[BLACK] |= _pieces[BLACK][ROOK];
  _occupancies[BLACK] |= _pieces[BLACK][QUEEN];
  _occupancies[BLACK] |= _pieces[BLACK][KING];

  _occupancies[BOTH] |= _occupancies[WHITE];
  _occupancies[BOTH] |= _occupancies[BLACK];
}

void Board::updateBBFromSquares()
{
  for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
  {
    _pieces[WHITE][piece_type] = ZERO;
    _pieces[BLACK][piece_type] = ZERO;
  }

  for (int sq = A1; sq < N_SQUARES; sq++)
  {
    if (_square[sq].type != EMPTY)
    {
      _pieces[_square[sq].color][_square[sq].type] |= tables::SQUARE_BB[sq];
    }
  }

  this->updateOccupancies();
}

// Public Methods

// Getters

U64 Board::getPieces(int color, int type) const
{
  return _pieces[color][type];
}

U64 Board::getOccupancies(int color) const
{
  return _occupancies[color];
}

int Board::getSideToMove() const
{
  return _to_move;
}

int Board::getOpponent() const
{
  return utils::getOpponent(_to_move);
}

int Board::getCastlingRights() const
{
  return _castling_rights;
}

int Board::getEnPassantSquare() const
{
  return _en_passant_square;
}

int Board::getHalfMoveClock() const
{
  return _half_move_clock;
}

int Board::getFullMoveNumber() const
{
  return _full_move_number;
}

Piece Board::getPieceFromSquare(int sq) const
{
  return _square[sq];
}

bool Board::isSquareAttacked(int sq, int attacker) const
{
  U64 pawns = _pieces[attacker][PAWN];
  if (tables::ATTACKS_PAWN[utils::getOpponent(attacker)][sq] & pawns)
  {
    return true;
  }
  U64 knights = _pieces[attacker][KNIGHT];
  if (tables::ATTACKS_KNIGHT[sq] & knights)
  {
    return true;
  }
  U64 king = _pieces[attacker][KING];
  if (tables::ATTACKS_KING[sq] & king)
  {
    return true;
  }
  U64 bishopsQueens = _pieces[attacker][QUEEN] | _pieces[attacker][BISHOP];
  if (tables::getBishopAttacks(sq, _occupancies[BOTH]) & bishopsQueens)
  {
    return true;
  }
  U64 rooksQueens = _pieces[attacker][QUEEN] | _pieces[attacker][ROOK];
  if (tables::getRookAttacks(sq, _occupancies[BOTH]) & rooksQueens)
  {
    return true;
  }

  return false;
}

std::string Board::getFen() const
{
  std::string piece_placements;
  std::string active_color;
  std::string castling_rights;
  std::string en_passant;
  std::string half_move_clock;
  std::string full_move_number;

  // Piece Placements
  int empty_squares = 0;
  for (int rank = 7; rank >= 0; rank--)
  {
    for (int file = 0; file < 8; file++)
    {
      int sq = utils::getSquare(rank, file);
      if (file == 0)
      {
        if (empty_squares)
        {
          piece_placements += std::to_string(empty_squares);
          empty_squares = 0;
        }
        piece_placements += '/';
      }
      switch (_square[sq].type)
      {
      case EMPTY:
        empty_squares++;
        break;
      default:
        if (empty_squares)
        {
          piece_placements += std::to_string(empty_squares);
          empty_squares = 0;
        }
        piece_placements += PIECE_REPR[_square[sq].type + (6 * _square[sq].color)];
        break;
      }
    }
    if (empty_squares)
    {
      piece_placements += std::to_string(empty_squares);
      empty_squares = 0;
    }
  }

  // Active Color
  active_color = _to_move == WHITE ? "w" : "b";

  // Castling Rights
  char castling_rights_buf[5];
  snprintf(castling_rights_buf,
           5,
           "%s%s%s%s",
           (_castling_rights & CASTLE_KING_WHITE) ? "K" : "",
           (_castling_rights & CASTLE_QUEEN_WHITE) ? "Q" : "",
           (_castling_rights & CASTLE_KING_BLACK) ? "k" : "",
           (_castling_rights & CASTLE_QUEEN_BLACK) ? "q" : "");
  castling_rights = std::string(castling_rights_buf);
  if (castling_rights == "")
  {
    castling_rights = "-";
  }

  // En Passant Square
  std::string fen = piece_placements + " " + active_color + " " + castling_rights + " " + SQUARE_NAMES[this->getEnPassantSquare() == -1 ? 64 : this->getEnPassantSquare()] + " " + std::to_string(_half_move_clock) + " " + // Halfmove Clock
                    std::to_string(_full_move_number) + "\n";                                                                                                                                                               // Fullmove Number

  return fen.substr(1, std::string::npos);
}

// Display

void Board::display() const
{
  int offset = _ascii ? 0 : 13;
  std::cout << '\n';
  if (!_white_on_bottom)
  {
    std::cout << "      h   g   f   e   d   c   b   a\n";
    for (int rank = 0; rank < 8; rank++)
    {
      std::cout << "    +---+---+---+---+---+---+---+---+\n"
                << "    |";
      for (int file = 7; file >= 0; file--)
      {
        struct Piece piece = _square[utils::getSquare(rank, file)];
        std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
      }
      std::cout << std::setw(3) << rank + 1 << "\n";
    }
    std::cout << "    +---+---+---+---+---+---+---+---+\n";
  }
  else
  {
    for (int rank = 7; rank >= 0; rank--)
    {
      std::cout << "    +---+---+---+---+---+---+---+---+\n"
                << std::setw(3) << rank + 1 << " |";

      for (int file = 0; file < 8; file++)
      {
        struct Piece piece = _square[utils::getSquare(rank, file)];
        std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
      }
      std::cout << '\n';
    }
    std::cout << "    +---+---+---+---+---+---+---+---+\n"
              << "      a   b   c   d   e   f   g   h\n";
  }
  std::cout << std::endl;
}

bool Board::toggleAscii()
{
  return _ascii = !_ascii;
}

bool Board::rotateDisplay()
{
  return _white_on_bottom = !_white_on_bottom;
}

// Modifiers

void Board::clear()
{
  for (int color = WHITE; color < BOTH; color++)
  {
    for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
    {
      _pieces[color][piece_type] = ZERO;
    }

    _occupancies[color] = ZERO;
  }
  _occupancies[BOTH] = ZERO;

  _to_move = WHITE;
  _castling_rights = 0;
  _en_passant_square = -1;
  _half_move_clock = 0;
  _full_move_number = 0;

  _white_on_bottom = true;

  for (int sq = A1; sq < N_SQUARES; sq++)
  {
    _square[sq].type = EMPTY; // needs to have this particular values set for correct printing
    _square[sq].color = BLACK;
  }
}

void Board::setStartingPosition()
{
  this->setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1");
}

void Board::setFromFen(std::string piece_placements, std::string active_color, std::string castling_rights, std::string en_passant, std::string half_move_clock, std::string full_move_number)
{
  this->clear();

  // Piece Placements Parsing
  int file = 0, rank = 7;
  for (char const &c : piece_placements)
  {
    switch (c)
    {
    case 'p':
      _square[utils::getSquare(rank, file)].type = PAWN;
      _square[utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'n':
      _square[utils::getSquare(rank, file)].type = KNIGHT;
      _square[utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'b':
      _square[utils::getSquare(rank, file)].type = BISHOP;
      _square[utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'r':
      _square[utils::getSquare(rank, file)].type = ROOK;
      _square[utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'q':
      _square[utils::getSquare(rank, file)].type = QUEEN;
      _square[utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'k':
      _square[utils::getSquare(rank, file)].type = KING;
      _square[utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'P':
      _square[utils::getSquare(rank, file)].type = PAWN;
      _square[utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'N':
      _square[utils::getSquare(rank, file)].type = KNIGHT;
      _square[utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'B':
      _square[utils::getSquare(rank, file)].type = BISHOP;
      _square[utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'R':
      _square[utils::getSquare(rank, file)].type = ROOK;
      _square[utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'Q':
      _square[utils::getSquare(rank, file)].type = QUEEN;
      _square[utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'K':
      _square[utils::getSquare(rank, file)].type = KING;
      _square[utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case '/':
      rank--;
      file = 0;
      break;
    default:
      file += (c - '0');
      break;
    }
  }

  // Active Color Parsing
  if (active_color == "w")
  {
    _to_move = WHITE;
  }
  else
  {
    _to_move = BLACK;
  }

  // Castling Rights Parsing
  for (char const &c : castling_rights)
  {
    switch (c)
    {
    case 'Q':
      _castling_rights += CASTLE_QUEEN_WHITE;
      break;
    case 'K':
      _castling_rights += CASTLE_KING_WHITE;
      break;
    case 'q':
      _castling_rights += CASTLE_QUEEN_BLACK;
      break;
    case 'k':
      _castling_rights += CASTLE_KING_BLACK;
      break;
    default:
      break;
    }
  }

  // En Passant Square Parsing
  if (en_passant != "-")
  {
    int en_passant_file = en_passant[0] - 'a';
    int en_passant_rank = en_passant[1] - '1';
    _en_passant_square = utils::getSquare(en_passant_rank, en_passant_file);
  }
  else
  {
    _en_passant_square = -1;
  }

  // Halfmove Clock Parsing
  _half_move_clock = std::stoi(half_move_clock);

  // Fullmove Number Parsing
  _full_move_number = std::stoi(full_move_number);

  this->updateBBFromSquares();
}

int Board::switchSideToMove()
{
  return _to_move = this->getOpponent();
}

void Board::makeMove(Move move)
{
  // clang-format off
  constexpr int castling_rights[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11,
  };
  // clang-format on

  int from_square = move.getFromSquare();
  int to_square = move.getToSquare();
  int piece = move.getPiece();
  int promoted_piece = move.getPromotedPiece();
  bool is_capture = move.isCapture();
  bool is_double_push = move.isDoublePush();
  bool is_en_passant = move.isEnPassant();
  bool is_castle = move.isCastle();

  bitboard::popBit(_pieces[_to_move][piece], from_square);
  _square[from_square].type = EMPTY;
  _square[from_square].color = BLACK;

  if (is_en_passant)
  {
    int captured_piece_square = to_square + 8 * (((_to_move + 1) * 2) - 3);
    _square[captured_piece_square].type = EMPTY;
    _square[captured_piece_square].color = BLACK;
    bitboard::popBit(_pieces[this->getOpponent()][PAWN], captured_piece_square);
  }
  else if (is_capture)
  {
    int captured_piece_type = _square[to_square].type;
    bitboard::popBit(_pieces[this->getOpponent()][captured_piece_type], to_square);
  }

  if (promoted_piece)
  {
    _square[to_square].type = promoted_piece;
    bitboard::setBit(_pieces[_to_move][promoted_piece], to_square);
  }
  else
  {
    _square[to_square].type = piece;
    bitboard::setBit(_pieces[_to_move][piece], to_square);
  }
  _square[to_square].color = _to_move;

  if (is_castle) // move corresponding rook piece
  {
    int rook_from_square, rook_to_square;
    if (to_square - from_square > 0)
    {
      // King Side Caslting
      rook_from_square = _to_move == WHITE ? H1 : H8;
      rook_to_square = _to_move == WHITE ? F1 : F8;
    }
    else
    {
      // Queen Side Castling
      rook_from_square = _to_move == WHITE ? A1 : A8;
      rook_to_square = _to_move == WHITE ? D1 : D8;
    }
    _square[rook_from_square].type = EMPTY;
    _square[rook_from_square].color = BLACK;
    _square[rook_to_square].type = ROOK;
    _square[rook_to_square].color = _to_move;
    bitboard::popBit(_pieces[_to_move][ROOK], rook_from_square);
    bitboard::setBit(_pieces[_to_move][ROOK], rook_to_square);
  }

  _en_passant_square = is_double_push ? to_square + 8 * (((_to_move + 1) * 2) - 3) : -1;
  _castling_rights &= castling_rights[from_square];
  _castling_rights &= castling_rights[to_square];

  this->switchSideToMove();
  this->updateOccupancies();
}