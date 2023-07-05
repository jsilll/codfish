#pragma once

#include <vector>

#include <codchess/codchess.hpp>

namespace codbrain {
/// @brief Initializes the library
inline void
Init() noexcept {
    codchess::Init();
}

/// @brief The depth of the search.
typedef std::uint8_t Depth;

/// @brief The evaluation of a position.
typedef double Evaluation;

/// @brief The number of nodes searched.
typedef std::uint64_t Nodes;

/// @brief The principal variation.
typedef std::vector<codchess::Move> PrincipalVariation;

struct Result {
    /// @brief The depth of the search.
    Nodes nodes;
    /// @brief The evaluation of the position.
    Evaluation evaluation;
    /// @brief The principal variation.
    PrincipalVariation pv;
};

class Brain {
  public:
    /// @brief Construct a new Brain object.
    Brain() noexcept;
    virtual ~Brain() noexcept {}

    /// @brief Launches the UCI.
    void Launch() noexcept;

    /// @brief Returns the board.
    [[nodiscard]] auto &board() noexcept { return _board; }

    /// @brief Searches the position.
    /// @return The search result.
    virtual Result PickMove() noexcept = 0;

  protected:
    /// @brief The board.
    codchess::Board _board{};
};

/// @brief A simple brain that always plays the first legal move.
class Simple final : public Brain {
  public:
    /// @brief Construct a new Simple object.
    Simple() noexcept : Brain() {}

    /// @brief Searches the position.
    /// @return The search result.
    Result PickMove() noexcept override {
        const auto moves = codchess::movegen::Legal(_board);
        return Result{0, 0, {*moves.begin()}};
    }
};

/// @brief A brain that picks a random legal move.
class Random final : public Brain {
  public:
    /// @brief Construct a new Random object.
    Random() noexcept : Brain() {}

    /// @brief Searches the position.
    /// @return The search result.
    Result PickMove() noexcept override {
        const auto moves = codchess::movegen::Legal(_board);
        const auto random_move =
            *(moves.begin() + (std::rand() % moves.size()));
        return Result{0, 0, {random_move}};
    }
};

/// @brief A brain that uses the monte carlo tree search algorithm.
class Mcts final : public Brain {
  public:
    /// @brief Construct a new Mcts object.
    Mcts() noexcept : Brain() {}

    /// @brief Searches the position.
    /// @return The search result.
    Result PickMove() noexcept override;
};
}   // namespace codbrain