#pragma once

#include <vector>

#include <codchess/codchess.hpp>

#include <codbrain/uci.hpp>

namespace codbrain {
/// @brief Initializes the library
inline void
Init() noexcept {
    codchess::Init();
}

/// @brief The depth of the search.
typedef std::uint8_t Depth;

/// @brief The number of nodes searched.
typedef std::uint64_t Nodes;

/// @brief The evaluation of a position.
typedef std::int16_t Evaluation;

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

    /// @brief Searches the position.
    virtual Result PickMove() noexcept = 0;

    /// @brief Launches the UCI.
    void Launch() noexcept { _uci.Launch(); }

    /// @brief Returns the board.
    [[nodiscard]] auto &board() noexcept { return _board; }

  protected:
    /// @brief The UCI.
    Uci _uci{};
    /// @brief The board.
    codchess::Board _board{};
};

/// @brief A simple brain that always plays the first legal move.
class SimpleBrain final : public Brain {
  public:
    /// @brief Construct a new SimpleBrain object.
    SimpleBrain() noexcept : Brain() {}

    /// @brief Searches the position.
    Result PickMove() noexcept override {
        const auto moves = codchess::movegen::Legal(_board);
        return Result{0, 0, {*moves.begin()}};
    }
};

/// @brief A brain that picks a random legal move.
class RandomBrain final : public Brain {
  public:
    /// @brief Construct a new RandomBrain object.
    RandomBrain() noexcept : Brain() {}

    /// @brief Searches the position.
    Result PickMove() noexcept override {
        const auto moves = codchess::movegen::Legal(_board);
        const auto random_move = *(moves.begin() + (std::rand() % moves.size()));
        return Result{0, 0, {random_move}};
    }
};
}   // namespace codbrain