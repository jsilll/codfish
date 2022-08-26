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

void Board::update_bitboards_from_squares()
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
      bitboard::set_bit(_pieces[_square[sq].color][_square[sq].type], (Square)sq);
    }
  }

  this->update_occupancies();
}

u64 Board::get_pieces(Color color, PieceType type) const
{
  return _pieces[color][type];
}

u64 Board::get_occupancies(Color color) const
{
  return _occupancies[color];
}

Color Board::get_side_to_move() const
{
  return _to_move;
}

Color Board::get_opponent() const
{
  return utils::get_opponent(_to_move);
}

int Board::get_castling_rights() const
{
  return _castling_rights;
}

Square Board::get_en_passant_square() const
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

u64 Board::get_hash_key() const
{
  return _hash_key;
}

Board::Piece Board::get_piece_from_square(Square sq) const
{
  return _square[sq];
}

bool Board::is_square_attacked(Square sq, Color attacker) const
{
  u64 pawns = _pieces[attacker][PAWN];
  if (tables::get_pawn_attacks(utils::get_opponent(attacker), sq) & pawns)
  {
    return true;
  }
  u64 knights = _pieces[attacker][KNIGHT];
  if (tables::get_knight_attacks(sq) & knights)
  {
    return true;
  }
  u64 king = _pieces[attacker][KING];
  if (tables::get_king_attacks(sq) & king)
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
  for (int rank = RANK_8; rank >= RANK_1; rank--)
  {
    for (int file = FILE_A; file < N_FILES; file++)
    {
      Square sq = utils::get_square((Rank)rank, (File)file);
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

void Board::set_en_passant_square(Square sq)
{
  // Remove from hash key en passant square
  if (_en_passant_square != EMPTY_SQUARE)
  {
    _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
  }

  _en_passant_square = sq;

  // Update hash key with new en passant square
  if (_en_passant_square != EMPTY_SQUARE)
  {
    _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
  }
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
    for (int rank = RANK_1; rank < RANK_8; rank++)
    {
      std::cout << "    +---+---+---+---+---+---+---+---+\n"
                << "    |";
      for (int file = FILE_H; file >= FILE_A; file--)
      {
        struct Piece piece = _square[utils::get_square((Rank)rank, (File)file)];
        std::cout << " " << PIECE_REPR[piece.type + offset + (6 * piece.color)] << " |";
      }
      std::cout << std::setw(3) << rank + 1 << "\n";
    }
    std::cout << "    +---+---+---+---+---+---+---+---+\n";
  }
  else
  {
    for (int rank = RANK_8; rank >= RANK_1; rank--)
    {
      std::cout << "    +---+---+---+---+---+---+---+---+\n"
                << std::setw(3) << rank + 1 << " |";

      for (int file = 0; file < 8; file++)
      {
        struct Piece piece = _square[utils::get_square((Rank)rank, (File)file)];
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
  _hash_key = ZERO;

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

  int file = FILE_A, rank = RANK_8;
  for (const char &c : piece_placements)
  {
    switch (c)
    {
    case 'p':
      _square[utils::get_square((Rank)rank, (File)file)].type = PAWN;
      _square[utils::get_square((Rank)rank, (File)file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'n':
      _square[utils::get_square((Rank)rank, (File)file)].type = KNIGHT;
      _square[utils::get_square((Rank)rank, (File)file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'b':
      _square[utils::get_square((Rank)rank, (File)file)].type = BISHOP;
      _square[utils::get_square((Rank)rank, (File)file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'r':
      _square[utils::get_square((Rank)rank, (File)file)].type = ROOK;
      _square[utils::get_square((Rank)rank, (File)file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'q':
      _square[utils::get_square((Rank)rank, (File)file)].type = QUEEN;
      _square[utils::get_square((Rank)rank, (File)file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'k':
      _square[utils::get_square((Rank)rank, (File)file)].type = KING;
      _square[utils::get_square((Rank)rank, (File)file)].color = BLACK;
      file = (file + 1) % 8;
      break;
    case 'P':
      _square[utils::get_square((Rank)rank, (File)file)].type = PAWN;
      _square[utils::get_square((Rank)rank, (File)file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'N':
      _square[utils::get_square((Rank)rank, (File)file)].type = KNIGHT;
      _square[utils::get_square((Rank)rank, (File)file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'B':
      _square[utils::get_square((Rank)rank, (File)file)].type = BISHOP;
      _square[utils::get_square((Rank)rank, (File)file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'R':
      _square[utils::get_square((Rank)rank, (File)file)].type = ROOK;
      _square[utils::get_square((Rank)rank, (File)file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'Q':
      _square[utils::get_square((Rank)rank, (File)file)].type = QUEEN;
      _square[utils::get_square((Rank)rank, (File)file)].color = WHITE;
      file = (file + 1) % 8;
      break;
    case 'K':
      _square[utils::get_square((Rank)rank, (File)file)].type = KING;
      _square[utils::get_square((Rank)rank, (File)file)].color = WHITE;
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
    _en_passant_square = utils::get_square((Rank)en_passant_rank, (File)en_passant_file);
  }
  else
  {
    _en_passant_square = EMPTY_SQUARE;
  }

  _half_move_clock = std::stoi(half_move_clock);

  _full_move_number = std::stoi(full_move_number);

  this->update_bitboards_from_squares();

  _hash_key = zobrist::generate_hash_key(*this);
}

int Board::switch_side_to_move()
{
  return _to_move = this->get_opponent();
}

void Board::make(const Move move)
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

  Square from_square = move.get_from_square();
  Square to_square = move.get_to_square();
  PieceType piece_type = move.get_piece_type();
  PieceType captured_piece = move.get_captured_piece_type();
  PieceType promoted_piece = move.get_promoted_piece_type();
  bool is_capture = move.is_capture();
  bool is_promotion = move.is_promotion();
  bool is_double_push = move.is_double_push();
  bool is_en_passant = move.is_en_passant();
  bool is_castle = move.is_castle();

  int pawn_push_en_passant_offset = _to_move == WHITE ? -8 : 8;

  bitboard::pop_bit(_pieces[_to_move][piece_type], (Square)from_square);
  _square[from_square].type = EMPTY_PIECE;
  _square[from_square].color = BLACK;

  // Remove from hash key moved piece
  _hash_key ^= zobrist::piece_keys[_to_move][piece_type][from_square];

  if (is_en_passant)
  {
    Square captured_piece_square = (Square)(to_square + pawn_push_en_passant_offset);
    _square[captured_piece_square].type = EMPTY_PIECE;
    _square[captured_piece_square].color = BLACK;
    bitboard::pop_bit(_pieces[this->get_opponent()][PAWN], captured_piece_square);

    // Remove from hash key captured pawn
    _hash_key ^= zobrist::piece_keys[this->get_opponent()][PAWN][captured_piece_square];
  }
  else if (is_capture)
  {
    bitboard::pop_bit(_pieces[this->get_opponent()][captured_piece], to_square);

    // Remove from hash key captured piece
    _hash_key ^= zobrist::piece_keys[this->get_opponent()][captured_piece][to_square];
  }

  if (is_promotion)
  {
    _square[to_square].type = promoted_piece;
    bitboard::set_bit(_pieces[_to_move][promoted_piece], to_square);

    // Update hash key with promoted piece
    _hash_key ^= zobrist::piece_keys[_to_move][promoted_piece][to_square];
  }
  else
  {
    _square[to_square].type = piece_type;
    bitboard::set_bit(_pieces[_to_move][piece_type], to_square);

    // Update hash key with moved piece
    _hash_key ^= zobrist::piece_keys[_to_move][piece_type][to_square];
  }

  _square[to_square].color = _to_move;

  if (is_castle)
  {
    Square rook_from_square, rook_to_square;
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

    // Remove from hash key rook
    _hash_key ^= zobrist::piece_keys[_to_move][ROOK][rook_from_square];

    bitboard::set_bit(_pieces[_to_move][ROOK], rook_to_square);

    // Update hash key with rook
    _hash_key ^= zobrist::piece_keys[_to_move][ROOK][rook_to_square];
  }

  // Remove from hash key en passant square
  if (_en_passant_square != EMPTY_SQUARE)
  {
    _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
  }

  // Remove from hash key castling rights
  _hash_key ^= zobrist::castle_keys[_castling_rights];

  _en_passant_square = is_double_push ? (Square)(to_square + pawn_push_en_passant_offset) : EMPTY_SQUARE;
  _castling_rights &= castling_rights[from_square];
  _castling_rights &= castling_rights[to_square];

  // Update hash key with en passant square
  if (_en_passant_square != EMPTY_SQUARE)
  {
    _hash_key ^= zobrist::en_passant_keys[_en_passant_square];
  }

  // Update hash key with castling rights
  _hash_key ^= zobrist::castle_keys[_castling_rights];

  if (piece_type == PAWN || (is_capture))
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

  // Remove (and Update) from hash key side to move
  // This works because zobrist::side_key[WHITE] = 0
  // So XOR side[BLACK] + XOR side[WHITE] = XOR side[WHITE] + side[BLACK] = XOR side[BLACK]
  _hash_key ^= zobrist::side_key[BLACK];

  // Update from hash key new side to move
  this->switch_side_to_move();

  this->update_occupancies();
}

void Board::unmake(const Move move, const GameState state)
{
  this->switch_side_to_move();

  Square from_square = move.get_from_square();
  Square to_square = move.get_to_square();
  PieceType piece_type = move.get_piece_type();
  PieceType captured_piece = move.get_captured_piece_type();
  PieceType promoted_piece = move.get_promoted_piece_type();
  bool is_capture = move.is_capture();
  bool is_promotion = move.is_promotion();
  bool is_en_passant = move.is_en_passant();
  bool is_castle = move.is_castle();

  _square[from_square].type = piece_type;
  _square[from_square].color = _to_move;
  bitboard::set_bit(_pieces[_to_move][piece_type], from_square);

  bitboard::pop_bit(_pieces[_to_move][piece_type], to_square);

  if (is_en_passant)
  {
    Square captured_piece_square = _to_move == WHITE ? (Square)(to_square - 8) : (Square)(to_square + 8);

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
    Square rook_from_square, rook_to_square;
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