#include <engine/board.hpp>

#include <engine/utils.hpp>
#include <engine/bitboard.hpp>
#include <engine/move.hpp>
#include <engine/movegen/tables.hpp>
#include <engine/zobrist.hpp>

#include <cstring>
#include <iomanip>
#include <string>

Board::Board() { this->set_starting_position(); }

Board::Board(const Board &board)
{
  memcpy(_pieces, board._pieces, sizeof(_pieces));
  memcpy(_occupancies, board._occupancies, sizeof(_occupancies));

  _to_move = board._to_move;
  _castling_rights = board._castling_rights;
  _en_passant_square = board._en_passant_square;
  _half_move_clock = board._half_move_clock;
  _full_move_number = board._full_move_number;
  _hash_key = board._hash_key;

  _ascii = board._ascii;
  _white_on_bottom = board._white_on_bottom;
  memcpy(_square, board._square, sizeof(_square));
}

void Board::update_occupancies()
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

void Board::update_bb_from_squares()
{
  for (int piece_type = PAWN; piece_type < N_PIECES; piece_type++)
  {
    _pieces[WHITE][piece_type] = ZERO;
    _pieces[BLACK][piece_type] = ZERO;
  }

  for (int sq = A1; sq < N_SQUARES; sq++)
  {
    if (_square[sq].type != EMPTY_PIECE)
    {
      bitboard::set_bit(_pieces[_square[sq].color][_square[sq].type], sq);
    }
  }

  this->update_occupancies();
}

u64 Board::get_pieces(int color, int type) const
{
  return _pieces[color][type];
}

u64 Board::get_occupancies(int color) const
{
  return _occupancies[color];
}

int Board::get_side_to_move() const
{
  return _to_move;
}

int Board::get_opponent() const
{
  return utils::get_opponent(_to_move);
}

int Board::get_castling_rights() const
{
  return _castling_rights;
}

int Board::get_en_passant_square() const
{
  return _en_passant_square;
}

int Board::get_half_move_clock() const
{
  return _half_move_clock;
}

int Board::get_full_move_number() const
{
  return _full_move_number;
}

u64 Board::calculate_hash_key()
{
  return zobrist::generate_hash_key(*this);
}

u64 Board::get_hash_key()
{
  return _hash_key;
}

Board::Piece Board::get_piece_from_square(int sq) const
{
  return _square[sq];
}

bool Board::is_square_attacked(int sq, int attacker) const
{
  u64 pawns = _pieces[attacker][PAWN];
  if (tables::ATTACKS_PAWN[utils::get_opponent(attacker)][sq] & pawns)
  {
    return true;
  }
  u64 knights = _pieces[attacker][KNIGHT];
  if (tables::ATTACKS_KNIGHT[sq] & knights)
  {
    return true;
  }
  u64 king = _pieces[attacker][KING];
  if (tables::ATTACKS_KING[sq] & king)
  {
    return true;
  }
  u64 bishopsQueens = _pieces[attacker][QUEEN] | _pieces[attacker][BISHOP];
  if (tables::get_bishop_attacks(sq, _occupancies[BOTH]) & bishopsQueens)
  {
    return true;
  }
  u64 rooksQueens = _pieces[attacker][QUEEN] | _pieces[attacker][ROOK];
  if (tables::get_rook_attacks(sq, _occupancies[BOTH]) & rooksQueens)
  {
    return true;
  }

  return false;
}

bool Board::is_ascii() const
{
  return _ascii;
}
bool Board::is_white_on_bottom() const
{
  return _white_on_bottom;
}

std::string Board::get_fen() const
{
  std::string piece_placements;
  std::string active_color;
  std::string castling_rights;
  std::string en_passant;
  std::string half_move_clock;
  std::string full_move_number;

  int empty_squares = 0;
  for (int rank = 7; rank >= 0; rank--)
  {
    for (int file = 0; file < 8; file++)
    {
      int sq = utils::get_square(rank, file);
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
      case EMPTY_PIECE:
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

  active_color = _to_move == WHITE ? "w" : "b";

  char castling_rights_buf[5];
  snprintf(castling_rights_buf,
           5,
           "%s%s%s%s",
           (_castling_rights & CASTLE_KING_WHITE) ? "K" : "",
           (_castling_rights & CASTLE_QUEEN_WHITE) ? "Q" : "",
           (_castling_rights & CASTLE_KING_BLACK) ? "k" : "",
           (_castling_rights & CASTLE_QUEEN_BLACK) ? "q" : "");
  castling_rights = std::string(castling_rights_buf);
  if (castling_rights.empty())
  {
    castling_rights = "-";
  }

  std::string fen = piece_placements +
                    " " +
                    active_color +
                    " " +
                    castling_rights +
                    " " +
                    SQUARE_NAMES[this->get_en_passant_square() == -1 ? 64 : this->get_en_passant_square()] +
                    " " +
                    std::to_string(_half_move_clock) + " " +
                    std::to_string(_full_move_number) + "\n";

  return fen.substr(1, std::string::npos);
}

struct Board::GameState Board::get_state() const
{
  return GameState{_en_passant_square, _castling_rights, _half_move_clock, _hash_key};
}

void Board::set_en_passant_square(int sq)
{
  _en_passant_square = sq;
}

void Board::set_castling_rights(int castling_rights)
{
  _castling_rights = castling_rights;
}

void Board::set_state(GameState state)
{
  _en_passant_square = state.en_passant_square;
  _castling_rights = state.castling_rights;
  _half_move_clock = state.half_move_clock;
  _hash_key = state.hash_key;
}

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
        struct Piece piece = _square[utils::get_square(rank, file)];
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
        struct Piece piece = _square[utils::get_square(rank, file)];
        std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
      }
      std::cout << '\n';
    }
    std::cout << "    +---+---+---+---+---+---+---+---+\n"
              << "      a   b   c   d   e   f   g   h\n";
  }
  std::cout << std::endl;
}

bool Board::toggle_ascii()
{
  return _ascii = !_ascii;
}

bool Board::rotate_display()
{
  return _white_on_bottom = !_white_on_bottom;
}

void Board::clear()
{
  for (int color = WHITE; color < BOTH; color++)
  {
    for (int piece_type = PAWN; piece_type < N_PIECES; piece_type++)
    {
      _pieces[color][piece_type] = ZERO;
    }

    _occupancies[color] = ZERO;
  }
  _occupancies[BOTH] = ZERO;

  _to_move = WHITE;
  _castling_rights = 0;
  _en_passant_square = EMPTY_SQUARE;
  _half_move_clock = 0;
  _full_move_number = 0;
  _hash_key = 0ULL; // replace with original

  _white_on_bottom = true;

  for (int sq = A1; sq < N_SQUARES; sq++)
  {
    _square[sq].type = EMPTY_PIECE;
    _square[sq].color = BLACK;
  }
}

void Board::set_starting_position()
{
  this->set_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1");
}

void Board::set_from_fen(const std::string &piece_placements,
                         const std::string &active_color,
                         const std::string &castling_rights,
                         const std::string &en_passant,
                         const std::string &half_move_clock,
                         const std::string &full_move_number)
{
  this->clear();

  int file = 0, rank = 7;
  for (const char &c : piece_placements)
  {
    switch (c)
    {
    case 'p':
      _square[utils::get_square(rank, file)].type = PAWN;
      _square[utils::get_square(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'n':
      _square[utils::get_square(rank, file)].type = KNIGHT;
      _square[utils::get_square(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'b':
      _square[utils::get_square(rank, file)].type = BISHOP;
      _square[utils::get_square(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'r':
      _square[utils::get_square(rank, file)].type = ROOK;
      _square[utils::get_square(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'q':
      _square[utils::get_square(rank, file)].type = QUEEN;
      _square[utils::get_square(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'k':
      _square[utils::get_square(rank, file)].type = KING;
      _square[utils::get_square(rank, file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'P':
      _square[utils::get_square(rank, file)].type = PAWN;
      _square[utils::get_square(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'N':
      _square[utils::get_square(rank, file)].type = KNIGHT;
      _square[utils::get_square(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'B':
      _square[utils::get_square(rank, file)].type = BISHOP;
      _square[utils::get_square(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'R':
      _square[utils::get_square(rank, file)].type = ROOK;
      _square[utils::get_square(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'Q':
      _square[utils::get_square(rank, file)].type = QUEEN;
      _square[utils::get_square(rank, file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'K':
      _square[utils::get_square(rank, file)].type = KING;
      _square[utils::get_square(rank, file)].color = WHITE;
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

  if (active_color == "w")
  {
    _to_move = WHITE;
  }
  else
  {
    _to_move = BLACK;
  }

  for (const char &c : castling_rights)
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

  if (en_passant != "-")
  {
    int en_passant_file = en_passant[0] - 'a';
    int en_passant_rank = en_passant[1] - '1';
    _en_passant_square = utils::get_square(en_passant_rank, en_passant_file);
  }
  else
  {
    _en_passant_square = EMPTY_SQUARE;
  }

  _half_move_clock = std::stoi(half_move_clock);

  _full_move_number = std::stoi(full_move_number);

  this->update_bb_from_squares();

  _hash_key = this->calculate_hash_key();
}

int Board::switch_side_to_move()
{
  return _to_move = this->get_opponent();
}

void Board::make_move(const Move move)
{
  // clang-format off
  static const int castling_rights[64] = {
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

  int from_square = move.get_from_square();
  int to_square = move.get_to_square();
  int piece = move.get_piece();
  int captured_piece = move.get_captured_piece();
  int promoted_piece = move.get_promoted_piece();
  bool is_capture = move.is_capture();
  bool is_promotion = move.is_promotion();
  bool is_double_push = move.is_double_push();
  bool is_en_passant = move.is_en_passant();
  bool is_castle = move.is_castle();

  int pawn_push_en_passant_offset = _to_move == WHITE ? -8 : 8;

  bitboard::pop_bit(_pieces[_to_move][piece], from_square);
  _square[from_square].type = EMPTY_PIECE;
  _square[from_square].color = BLACK;

  // remove from hash key moved piece
  _hash_key ^= zobrist::piece_keys[_to_move][piece][from_square];

  if (is_en_passant)
  {
    int captured_piece_square = to_square + pawn_push_en_passant_offset;
    _square[captured_piece_square].type = EMPTY_PIECE;
    _square[captured_piece_square].color = BLACK;
    bitboard::pop_bit(_pieces[this->get_opponent()][PAWN], captured_piece_square);

    // remove from hash key captured pawn
    _hash_key ^= zobrist::piece_keys[this->get_opponent()][PAWN][captured_piece_square];
  }
  else if (is_capture)
  {
    bitboard::pop_bit(_pieces[this->get_opponent()][captured_piece], to_square);

    // remove from hash key captured piece
    _hash_key ^= zobrist::piece_keys[this->get_opponent()][captured_piece][to_square];
  }

  if (is_promotion)
  {
    _square[to_square].type = promoted_piece;
    bitboard::set_bit(_pieces[_to_move][promoted_piece], to_square);

    // add to hash key promoted piece
    _hash_key ^= zobrist::piece_keys[_to_move][promoted_piece][to_square];
  }
  else
  {
    _square[to_square].type = piece;
    bitboard::set_bit(_pieces[_to_move][piece], to_square);

    // add to hash key moved piece
    _hash_key ^= zobrist::piece_keys[_to_move][piece][to_square];
  }

  _square[to_square].color = _to_move;

  if (is_castle)
  {
    int rook_from_square, rook_to_square;
    if (to_square - from_square > 0)
    {
      rook_from_square = _to_move == WHITE ? H1 : H8;
      rook_to_square = _to_move == WHITE ? F1 : F8;
    }
    else
    {
      rook_from_square = _to_move == WHITE ? A1 : A8;
      rook_to_square = _to_move == WHITE ? D1 : D8;
    }

    _square[rook_from_square].type = EMPTY_PIECE;
    _square[rook_from_square].color = BLACK;
    _square[rook_to_square].type = ROOK;
    _square[rook_to_square].color = _to_move;

    bitboard::pop_bit(_pieces[_to_move][ROOK], rook_from_square);
    // remove from hash key rook
    _hash_key ^= zobrist::piece_keys[_to_move][ROOK][rook_from_square];
    bitboard::set_bit(_pieces[_to_move][ROOK], rook_to_square);
    // add to hash key rook
    _hash_key ^= zobrist::piece_keys[_to_move][ROOK][rook_to_square];
  }

  // remove from hash key castling rights and en passant
  if (_en_passant_square != EMPTY_SQUARE)
  {
    _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
  }
  _hash_key ^= zobrist::castle_keys[_castling_rights];

  _en_passant_square = is_double_push ? to_square + pawn_push_en_passant_offset : -1;
  _castling_rights &= castling_rights[from_square];
  _castling_rights &= castling_rights[to_square];

  // update hash key with castling rights and en passant
  if (_en_passant_square != EMPTY_SQUARE)
  {
    _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
  }
  _hash_key ^= zobrist::castle_keys[_castling_rights];

  if (piece == PAWN || (is_capture))
  {
    _half_move_clock = 0;
  }
  else
  {
    _half_move_clock++;
  }

  if (_to_move == BLACK)
  {
    _full_move_number++;
  }

  // update from hash key side to move
  _hash_key ^= zobrist::side_key[BLACK];
  this->switch_side_to_move();
  // _hash_key ^= zobrist::side_key[_to_move];

  this->update_occupancies();
}

void Board::unmake_move(const Move move, const GameState state)
{
  this->switch_side_to_move();

  int from_square = move.get_from_square();
  int to_square = move.get_to_square();
  int piece = move.get_piece();
  int captured_piece = move.get_captured_piece();
  int promoted_piece = move.get_promoted_piece();
  bool is_capture = move.is_capture();
  bool is_promotion = move.is_promotion();
  bool is_en_passant = move.is_en_passant();
  bool is_castle = move.is_castle();

  _square[from_square].type = piece;
  _square[from_square].color = _to_move;
  bitboard::set_bit(_pieces[_to_move][piece], from_square);

  bitboard::pop_bit(_pieces[_to_move][piece], to_square);

  if (is_en_passant)
  {
    int captured_piece_square = _to_move == WHITE ? to_square - 8 : to_square + 8;

    _square[captured_piece_square].type = PAWN;
    _square[captured_piece_square].color = this->get_opponent();
    bitboard::set_bit(_pieces[this->get_opponent()][PAWN], captured_piece_square);

    _square[to_square].type = EMPTY_PIECE;
    _square[to_square].color = BLACK;
  }
  else if (is_capture)
  {
    _square[to_square].type = captured_piece;
    _square[to_square].color = this->get_opponent();
    bitboard::set_bit(_pieces[this->get_opponent()][captured_piece], to_square);
  }
  else
  {
    _square[to_square].type = EMPTY_PIECE;
    _square[to_square].color = BLACK;
  }

  if (is_promotion)
  {
    bitboard::pop_bit(_pieces[_to_move][promoted_piece], to_square);
  }

  if (is_castle)
  {
    int rook_from_square, rook_to_square;
    if (to_square - from_square > 0)
    {
      rook_from_square = _to_move == WHITE ? H1 : H8;
      rook_to_square = _to_move == WHITE ? F1 : F8;
    }
    else
    {
      rook_from_square = _to_move == WHITE ? A1 : A8;
      rook_to_square = _to_move == WHITE ? D1 : D8;
    }

    _square[rook_to_square].type = EMPTY_PIECE;
    _square[rook_to_square].color = BLACK;
    bitboard::pop_bit(_pieces[_to_move][ROOK], rook_to_square);

    _square[rook_from_square].type = ROOK;
    _square[rook_from_square].color = _to_move;
    bitboard::set_bit(_pieces[_to_move][ROOK], rook_from_square);
  }

  if (_to_move == BLACK)
  {
    _full_move_number--;
  }

  _en_passant_square = state.en_passant_square;
  _castling_rights = state.castling_rights;
  _half_move_clock = state.half_move_clock;
  _hash_key = state.hash_key;

  this->update_occupancies();
}