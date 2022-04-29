#include "ai.hpp"

#include "move.hpp"
#include "board.hpp"
#include "eval.hpp"

Move AI::find_best_move()
{
    // calls search(alpha, beta, level)
    return Move(0, 0, 0, 0, 0, 0, 0, 0);
}

int AI::search(int alpha, int beta, int level)
{
    // implements negamax
    // call eval(this->board);
    return 1;
}