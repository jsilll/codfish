#include "board.hpp"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <string>

#include "attacks.hpp"
#include "magics.hpp"
#include "move.hpp"
#include "tables.hpp"
#include "utils.hpp"

inline int getOpponent(int to_play) { return to_play ^ 1; }

void Board::print(bool ascii)
{
  int offset = ascii ? 0 : 13;
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
        struct Piece piece = _square[Utils::getSquare(rank, file)];
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
        struct Piece piece = _square[Utils::getSquare(rank, file)];
        std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
      }
      std::cout << '\n';
    }
    std::cout << "    +---+---+---+---+---+---+---+---+\n"
              << "      a   b   c   d   e   f   g   h\n";
  }
  std::cout << std::endl;
}

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

void Board::setStartingPosition() { this->setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1"); }

void Board::updateOccupancies()
{
  _occupancies[WHITE] = ZERO;
  _occupancies[BLACK] = ZERO;
  _occupancies[BOTH] = ZERO;

  for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
  {
    _occupancies[WHITE] |= _pieces[WHITE][piece_type];
  }

  for (int piece_type = PAWN; piece_type < EMPTY; piece_type++)
  {
    _occupancies[BLACK] |= _pieces[BLACK][piece_type];
  }

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
      _pieces[_square[sq].color][_square[sq].type] |= Tables::SQUARE_BB[sq];
    }
  }

  this->updateOccupancies();
}

void Board::setFromFen(std::string piece_placements, std::string active_color, std::string castling_rights, std::string en_passant, std::string half_move_clock, std::string full_move_number)
{
  this->clear();

  // Piece Placements Parsing
  int file = 0, rank = 7;
  for (auto &c : piece_placements)
  {
    switch (c)
    {
    case 'p':
      _square[Utils::getSquare(rank, file)].type = PAWN;
      _square[Utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'n':
      _square[Utils::getSquare(rank, file)].type = KNIGHT;
      _square[Utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'b':
      _square[Utils::getSquare(rank, file)].type = BISHOP;
      _square[Utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'r':
      _square[Utils::getSquare(rank, file)].type = ROOK;
      _square[Utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'q':
      _square[Utils::getSquare(rank, file)].type = QUEEN;
      _square[Utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'k':
      _square[Utils::getSquare(rank, file)].type = KING;
      _square[Utils::getSquare(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'P':
      _square[Utils::getSquare(rank, file)].type = PAWN;
      _square[Utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'N':
      _square[Utils::getSquare(rank, file)].type = KNIGHT;
      _square[Utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'B':
      _square[Utils::getSquare(rank, file)].type = BISHOP;
      _square[Utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'R':
      _square[Utils::getSquare(rank, file)].type = ROOK;
      _square[Utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'Q':
      _square[Utils::getSquare(rank, file)].type = QUEEN;
      _square[Utils::getSquare(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'K':
      _square[Utils::getSquare(rank, file)].type = KING;
      _square[Utils::getSquare(rank, file)].color = WHITE;
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
  for (auto &c : castling_rights)
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
    _en_passant_square = Utils::getSquare(en_passant_rank, en_passant_file);
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
      int sq = Utils::getSquare(rank, file);
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
  snprintf(castling_rights_buf, 5, "%s%s%s%s", (_castling_rights & CASTLE_KING_WHITE) ? "K" : "", (_castling_rights & CASTLE_QUEEN_WHITE) ? "Q" : "", (_castling_rights & CASTLE_KING_BLACK) ? "k" : "", (_castling_rights & CASTLE_QUEEN_BLACK) ? "q" : "");
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

int Board::getCastlingRights() const { return _castling_rights; }

int Board::getEnPassantSquare() const { return _en_passant_square; }

int Board::getHalfMoveClock() const { return _half_move_clock; }

int Board::getFullMoveNumber() const { return _full_move_number; }

U64 Board::getOccupiedSquares() const { return _occupancies[BOTH]; }

int Board::getSideToMove() const { return _to_move; }

int Board::switchSideToMove() { return _to_move = getOpponent(_to_move); }

bool Board::rotate() { return _white_on_bottom = !_white_on_bottom; }

bool Board::isSquareAttacked(const int sq, const int attacker_side) const
{
  if (Tables::getRookAttacks(sq, _occupancies[BOTH]) & (_pieces[attacker_side][ROOK] | _pieces[attacker_side][QUEEN]))
  {
    return true;
  }
  else if (Tables::getBishopAttacks(sq, _occupancies[BOTH]) & (_pieces[attacker_side][BISHOP] | _pieces[attacker_side][QUEEN]))
  {
    return true;
  }
  else if (Tables::ATTACKS_KNIGHT[sq] & _pieces[attacker_side][KNIGHT])
  {
    return true;
  }
  else if (Tables::ATTACKS_PAWN[getOpponent(attacker_side)][sq] & _pieces[attacker_side][PAWN])
  {
    return true;
  }
  else if (Tables::ATTACKS_KING[sq] & _pieces[attacker_side][KING])
  {
    return true;
  }
  return false;
}

// TODO: improve correctness
// TODO: improve performance 10M NPS -> ~ 20 NPS
std::vector<Move> Board::getPseudoLegalMoves() const
{
  std::vector<Move> moves_vec;

  int opponent = getOpponent(_to_move);
  int castle_b_sq, castle_c_sq, castle_d_sq, castle_e_sq, castle_f_sq, castle_g_sq;
  int castle_king_mask, castle_queen_mask;
  int pawn_double_push_offset, pawn_single_push_offset;
  U64 pawn_double_pushes, pawn_single_pushes;

  if (_to_move == WHITE)
  {
    castle_b_sq = B1;
    castle_c_sq = C1;
    castle_d_sq = D1;
    castle_e_sq = E1;
    castle_f_sq = F1;
    castle_g_sq = G1;
    castle_king_mask = CASTLE_KING_WHITE;
    castle_queen_mask = CASTLE_QUEEN_WHITE;

    pawn_double_push_offset = -16;
    pawn_single_push_offset = pawn_double_push_offset / 2;
    pawn_double_pushes = Attacks::maskWhitePawnDoublePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH]);
    pawn_single_pushes = Attacks::maskWhitePawnSinglePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH]);
  }
  else
  {
    castle_b_sq = Utils::mirrorRank(B1);
    castle_c_sq = Utils::mirrorRank(C1);
    castle_d_sq = Utils::mirrorRank(D1);
    castle_e_sq = Utils::mirrorRank(E1);
    castle_f_sq = Utils::mirrorRank(F1);
    castle_g_sq = Utils::mirrorRank(G1);
    castle_king_mask = CASTLE_KING_BLACK;
    castle_queen_mask = CASTLE_QUEEN_BLACK;

    pawn_double_push_offset = 16;
    pawn_single_push_offset = pawn_double_push_offset / 2;
    pawn_double_pushes = Attacks::maskBlackPawnDoublePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH]);
    pawn_single_pushes = Attacks::maskBlackPawnSinglePushes(_pieces[_to_move][PAWN], ~_occupancies[BOTH]);
  }

  // Castling Moves
  if (!this->isSquareAttacked(castle_e_sq, opponent))
  {
    if ((_castling_rights & castle_king_mask) && !Utils::getBit(_occupancies[BOTH], castle_f_sq) && !Utils::getBit(_occupancies[BOTH], castle_g_sq))
    {
      // TODO: remove this last verification of attack
      // because we are already going to check if king is in check after the
      // move
      if (!this->isSquareAttacked(castle_f_sq, opponent && !this->isSquareAttacked(castle_g_sq, opponent)))
      {
        moves_vec.push_back(Move(castle_e_sq, castle_g_sq, KING, 0, false, false, false, true));
      }
    }
    if ((_castling_rights & castle_queen_mask) && !Utils::getBit(_occupancies[BOTH], castle_d_sq) && !Utils::getBit(_occupancies[BOTH], castle_c_sq) && !Utils::getBit(_occupancies[BOTH], castle_b_sq))
    {
      if (!this->isSquareAttacked(castle_d_sq, opponent) && !this->isSquareAttacked(castle_c_sq, opponent))
      {
        moves_vec.push_back(Move(castle_e_sq, castle_c_sq, KING, 0, false, false, false, true));
      }
    }
  }

  // Pawn Double Pushes
  while (pawn_double_pushes)
  {
    int to_square = Utils::bitScan(pawn_double_pushes);
    int from_square = to_square + pawn_double_push_offset;
    moves_vec.push_back(Move(from_square, to_square, PAWN, 0, false, true, false, false));
    Utils::popLastBit(pawn_double_pushes);
  }

  // Pawn Single Pushes With Promotion
  U64 pawn_single_pushes_promo = pawn_single_pushes & (Tables::MASK_RANK[0] | Tables::MASK_RANK[7]);
  while (pawn_single_pushes_promo)
  {
    int to_square = Utils::bitScan(pawn_single_pushes_promo);
    int from_square = to_square + pawn_single_push_offset;
    moves_vec.push_back(Move(from_square, to_square, PAWN, KNIGHT, false, false, false, false));
    moves_vec.push_back(Move(from_square, to_square, PAWN, BISHOP, false, false, false, false));
    moves_vec.push_back(Move(from_square, to_square, PAWN, ROOK, false, false, false, false));
    moves_vec.push_back(Move(from_square, to_square, PAWN, QUEEN, false, false, false, false));
    Utils::popLastBit(pawn_single_pushes_promo);
  }

  // Pawn Single Pushes No Promotion
  U64 pawn_single_pushes_no_promo = pawn_single_pushes & Tables::MASK_CLEAR_RANK[0] & Tables::MASK_CLEAR_RANK[7];
  while (pawn_single_pushes_no_promo)
  {
    int to_square = Utils::bitScan(pawn_single_pushes_no_promo);
    int from_square = to_square + pawn_single_push_offset;
    moves_vec.push_back(Move(from_square, to_square, PAWN, 0, false, false, false, false));
    Utils::popLastBit(pawn_single_pushes_no_promo);
  }

  // Pawn Captures With Promotion
  U64 pawns_can_capture_with_promo = _pieces[_to_move][PAWN] & Tables::MASK_RANK[6 - (5 * _to_move)];
  while (pawns_can_capture_with_promo)
  {
    int from_square = Utils::bitScan(pawns_can_capture_with_promo);
    U64 pawn_captures_promo = Tables::ATTACKS_PAWN[_to_move][from_square] & _occupancies[opponent];
    while (pawn_captures_promo)
    {
      int to_square = Utils::bitScan(pawn_captures_promo);
      moves_vec.push_back(Move(from_square, to_square, PAWN, KNIGHT, true, false, false, false));
      moves_vec.push_back(Move(from_square, to_square, PAWN, BISHOP, true, false, false, false));
      moves_vec.push_back(Move(from_square, to_square, PAWN, ROOK, true, false, false, false));
      moves_vec.push_back(Move(from_square, to_square, PAWN, QUEEN, true, false, false, false));
      Utils::popLastBit(pawn_captures_promo);
    }
    Utils::popLastBit(pawns_can_capture_with_promo);
  }

  // Pawns Captures No Promotion
  U64 pawns_can_capture_no_promo = _pieces[_to_move][PAWN] & Tables::MASK_CLEAR_RANK[6 - (5 * _to_move)];
  while (pawns_can_capture_no_promo)
  {
    int from_square = Utils::bitScan(pawns_can_capture_no_promo);
    U64 pawn_captures_no_promo = Tables::ATTACKS_PAWN[_to_move][from_square] & _occupancies[opponent];
    while (pawn_captures_no_promo)
    {
      int to_square = Utils::bitScan(pawn_captures_no_promo);
      moves_vec.push_back(Move(from_square, to_square, PAWN, 0, true, false, false, false));
      Utils::popLastBit(pawn_captures_no_promo);
    }
    Utils::popLastBit(pawns_can_capture_no_promo);
  }

  // En-Passant Captur
  if (_en_passant_square != -1)
  {
    U64 pawns_can_en_passant = Tables::ATTACKS_PAWN[opponent][_en_passant_square] & _pieces[_to_move][PAWN];
    while (pawns_can_en_passant)
    {
      int from_square = Utils::bitScan(pawns_can_en_passant);
      moves_vec.push_back(Move(from_square, _en_passant_square, PAWN, 0, true, false, true, false));
      Utils::popLastBit(pawns_can_en_passant);
    }
  }

  // Knight Moves
  U64 knights = _pieces[_to_move][KNIGHT];
  while (knights)
  {
    int from_square = Utils::bitScan(knights);
    U64 moves = Tables::ATTACKS_KNIGHT[from_square] & ~_occupancies[_to_move];
    while (moves)
    {
      int to_square = Utils::bitScan(moves);
      moves_vec.push_back(Move(from_square, to_square, KNIGHT, 0, Utils::getBit(_occupancies[opponent], to_square), false, false, false));
      Utils::popLastBit(moves);
    }
    Utils::popLastBit(knights);
  }

  // Bishop Moves
  U64 bishops = _pieces[_to_move][BISHOP];
  while (bishops)
  {
    int from_square = Utils::bitScan(bishops);
    U64 moves = Tables::getBishopAttacks(from_square, _occupancies[BOTH]) & ~_occupancies[_to_move];
    while (moves)
    {
      int to_square = Utils::bitScan(moves);
      moves_vec.push_back(Move(from_square, to_square, BISHOP, 0, Utils::getBit(_occupancies[opponent], to_square), false, false, false));
      Utils::popLastBit(moves);
    }
    Utils::popLastBit(bishops);
  }

  // Rook Moves
  U64 rooks = _pieces[_to_move][ROOK];
  while (rooks)
  {
    int from_square = Utils::bitScan(rooks);
    U64 moves = Tables::getRookAttacks(from_square, _occupancies[BOTH]) & ~_occupancies[_to_move];
    while (moves)
    {
      int to_square = Utils::bitScan(moves);
      moves_vec.push_back(Move(from_square, to_square, ROOK, 0, Utils::getBit(_occupancies[opponent], to_square), false, false, false));
      Utils::popLastBit(moves);
    }
    Utils::popLastBit(rooks);
  }

  // Queen Moves
  U64 queens = _pieces[_to_move][QUEEN];
  while (queens)
  {
    int from_square = Utils::bitScan(queens);
    U64 moves = Tables::getQueenAttacks(from_square, _occupancies[BOTH]) & ~_occupancies[_to_move];
    while (moves)
    {
      int to_square = Utils::bitScan(moves);
      moves_vec.push_back(Move(from_square, to_square, QUEEN, 0, Utils::getBit(_occupancies[opponent], to_square), false, false, false));
      Utils::popLastBit(moves);
    }
    Utils::popLastBit(queens);
  }

  // King Moves
  U64 king = _pieces[_to_move][KING];
  int from_square = Utils::bitScan(king);
  U64 moves = Tables::ATTACKS_KING[from_square] & ~_occupancies[_to_move];
  while (moves)
  {
    int to_square = Utils::bitScan(moves);
    moves_vec.push_back(Move(from_square, to_square, KING, 0, Utils::getBit(_occupancies[opponent], to_square), false, false, false));
    Utils::popLastBit(moves);
  }

  return moves_vec;
}

std::vector<std::string> Board::getLegalMovesUCI()
{
  std::vector<std::string> moves_uci;
  for (Move move : this->getPseudoLegalMoves())
  {
    Board board_copy = *this;
    if (board_copy.makeMoveFromUCI(move.getUCI()))
    {
      moves_uci.push_back(move.getUCI());
    }
  }
  return moves_uci;
}

bool Board::makeMove(Move move)
{
  // clang-format off
  constexpr int castling_rights[64] = {
      14, 15, 15, 15, 12, 15, 15, 13,
      15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15,
      11, 15, 15, 15, 3, 15, 15, 7
  };
  // clang-format on

  Board board_backup = *this;

  int from_square = move.getFromSquare();
  int to_square = move.getToSquare();
  int piece = move.getPiece();
  int promoted_piece = move.getPromotedPiece();
  bool is_capture = move.isCapture();
  bool is_double_push = move.isDoublePush();
  bool is_en_passant = move.isEnPassant();
  bool is_castle = move.isCastle();

  Utils::popBit(_pieces[_to_move][piece], from_square);
  _square[from_square].type = EMPTY;
  _square[from_square].color = BLACK;

  if (is_en_passant)
  {
    int captured_piece_square = to_square + 8 * (((_to_move + 1) * 2) - 3);
    _square[captured_piece_square].type = EMPTY;
    _square[captured_piece_square].color = BLACK;
    Utils::popBit(_pieces[getOpponent(_to_move)][PAWN], captured_piece_square);
  }
  else if (is_capture)

  {
    int captured_piece_type = _square[to_square].type;
    Utils::popBit(_pieces[getOpponent(_to_move)][captured_piece_type], to_square);
  }

  if (promoted_piece)
  {
    _square[to_square].type = promoted_piece;
    Utils::setBit(_pieces[_to_move][promoted_piece], to_square);
  }
  else
  {
    _square[to_square].type = piece;
    Utils::setBit(_pieces[_to_move][piece], to_square);
  }
  _square[to_square].color = _to_move;

  if (is_castle) // move corresponding rook piece
  {
    int rook_from_square, rook_to_square;
    if (to_square - from_square > 0)
    {
      // King Side Caslting
      rook_from_square = _to_move == WHITE ? H1 : Utils::mirrorRank(H1);
      rook_to_square = _to_move == WHITE ? F1 : Utils::mirrorRank(F1);
    }
    else
    {
      // Queen Side Castling
      rook_from_square = _to_move == WHITE ? A1 : Utils::mirrorRank(A1);
      rook_to_square = _to_move == WHITE ? D1 : Utils::mirrorRank(D1);
    }
    _square[rook_from_square].type = EMPTY;
    _square[rook_from_square].color = BLACK;
    _square[rook_to_square].type = ROOK;
    _square[rook_to_square].color = _to_move;
    Utils::popBit(_pieces[_to_move][ROOK], rook_from_square);
    Utils::setBit(_pieces[_to_move][ROOK], rook_to_square);
  } // TODO: updating castling rights

  _en_passant_square = is_double_push ? to_square + 8 * (((_to_move + 1) * 2) - 3) : -1;
  _castling_rights &= castling_rights[from_square];
  _castling_rights &= castling_rights[to_square];
  _to_move = getOpponent(_to_move);
  this->updateOccupancies();

  if (this->isSquareAttacked(Utils::bitScan(_pieces[getOpponent(_to_move)][KING]), _to_move))
  {
    *this = board_backup;
    return false;
  }

  return true;
}

bool Board::makeMoveFromUCI(std::string move)
{
  for (auto move_candidate : this->getPseudoLegalMoves())
  {
    if (move_candidate.getUCI() == move && this->makeMove(move_candidate))
    {
      return true;
    }
  }
  return false;
}