#pragma once

#include <random>
#include <vector>

#include <core/codchess.hpp>

#include <brain/history.hpp>

namespace codfish::brain {
/// @brief Initializes the library
inline void
Init() noexcept {
    core::Init();
}

/// @brief The depth of the search.
typedef std::uint8_t Depth;

/// @brief The evaluation of a position.
typedef double Evaluation;

/// @brief The number of nodes searched.
typedef std::uint64_t Nodes;

/// @brief The principal variation.
typedef std::vector<core::Move> PrincipalVariation;

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
    core::Board _board{};
};

/// @brief A simple brain that always plays the first legal move.
class Simple final : public Brain {
  public:
    /// @brief Construct a new Simple object.
    Simple() noexcept : Brain() {}

    /// @brief Searches the position.
    /// @return The search result.
    Result PickMove() noexcept override {
        const auto moves = core::movegen::Legal(_board);
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
        const auto moves = core::movegen::Legal(_board);
        const auto random_move =
            *(moves.begin() + (std::rand() % moves.size()));
        return Result{0, 0, {random_move}};
    }
};

/// @brief A brain that uses the monte carlo tree search algorithm.
class Mcts final : public Brain {
  public:
    /// @brief Construct a new Mcts object.
    Mcts() noexcept : Brain(), _g(_rd()) {}

    /// @brief Searches the position.
    /// @return The search result.
    Result PickMove() noexcept override;

  private:
    /// @brief The history table.
    utils::History _history{};
    /// @brief The random device.
    std::random_device _rd{};
    /// @brief The random number generator.
    std::mt19937 _g;

    /// @brief Make a move and register it in the history table.
    /// @param move The move to make.
    void Make(const core::Move move) noexcept {
        _board.Make(move);
        _history.push(_board.hash());
    }

    /// @brief Unmake a move and unregister it from the history table.
    /// @param move The move to unmake.
    void Unmake(const core::Move move,
                const core::Board::StateBackup &backup) noexcept {
        _board.Unmake(move, backup);
        _history.pop();
    }

    /// @brief The result of a rollout.
    enum class Outcome { Loss, Draw };

    /// @brief The result of a rollout.
    struct RolloutResult {
        /// @brief Whether the rollout was a win.
        Outcome outcome;
        /// @brief The color that was going to move.
        core::Color last;
    };

    /// @brief Rollout the position.
    /// @return The evaluation of the position.
    RolloutResult Rollout() noexcept;
};
}   // namespace codfish::brain