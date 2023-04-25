#pragma once

#include <codchess/base.hpp>
#include <codchess/board.hpp>
#include <codchess/move.hpp>

#include <codchess/movepicker/histtable.hpp>
#include <codchess/movepicker/pvtable.hpp>
#include <codchess/movepicker/ttable.hpp>

#include <vector>

class MovePicker
{
private:
  static const int DEFAULT_MAX_DEPTH = 64;

  Board &_board;

  int _max_depth;
  int _current_nodes;
  int _current_depth;

  int _history_moves[N_SIDES][N_PIECES][N_SQUARES]{};
  Move _killer_moves[2][DEFAULT_MAX_DEPTH]{};

  PVTable _pv_table;
  HistoryTable _hist_table;
  TTable _tt;

  struct MoveMoreThanKey
  {
    MovePicker &move_picker;
    inline bool operator()(const Move &move1, const Move &move2) const
    {
      return (move_picker.score(move1) > move_picker.score(move2));
    }
  } _move_more_than_key;

  /**
   * @brief Evaluates a given move using a few heuristics:
   * - MVV LVA
   * - Killer Heuristic
   * - History Heuristic
   *
   * It is used for sorting the list of moves upon
   * generation, ini order to cause the most alpha-beta cuts.
   *
   * @param move
   * @return move score
   */
  int score(const Move move);

  int search(int depth, int alpha, int beta);
  int negamax(int alpha, int beta, int depth);
  int quiescence(int alpha, int beta);

  void add_to_killer_moves(const Move move);
  void add_to_history_moves(const Move move);

  void clear_search_counters();

public:
  struct SearchResult
  {
    int score{};
    int nodes{};
    std::vector<Move> pv;
  };

  explicit MovePicker(Board &board) : _board(board), _max_depth(6), _current_nodes(0), _move_more_than_key{ *this } {}

  [[nodiscard]] int get_max_depth() const;

  /**
   * @brief Set the max depth for the search
   *
   * @param depth
   */
  void set_max_depth(int depth);

  /**
   * @brief Adds a board hash to the current known history
   *
   * @param key
   */
  void add_to_history(u64 key);

  /**
   * @brief Clears all known game history
   *
   */
  void clear_history();

  /**
   * @brief Clears all the move tables
   *
   */
  void clear_move_tables();

  /**
   * @brief Clears the transposition table
   *
   */
  void clear_transposition_table();

  /**
   * @brief Searches the current position with max_depth
   *
   * @return SearchResult
   */
  SearchResult find_best_move();

  /**
   * @brief Searches the current position with the given depth
   *
   * It can also be used to implement iterative deepening outside
   * of the ai class (for uci prints, for example)
   *
   * @param depth
   * @return SearchResult
   */
  SearchResult find_best_move(int depth, int alpha, int beta);
};