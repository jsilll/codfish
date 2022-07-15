#include <interfaces/uci/timectl.hpp>

namespace interfaces::uci::timectl
{
    long get_time_budget_ms(long time, int moves_played)
    {
        // this can be made much more complex in the future
        return time / (40 - moves_played);
    }
} // namespace interfaces::uci::timectl
