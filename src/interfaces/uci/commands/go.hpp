#pragma once

#include <interfaces/utils.hpp>

#include <engine/movepicker/movepicker.hpp>

#include <thread>
#include <future>

namespace interfaces::uci::commands
{
    /**
     * @brief Handles the UCI 'go' command
     *
     */
    class GoCommand : public interfaces::utils::Command
    {
    public:
        void execute(std::vector<std::string> &args, Board &board);

    private:
        static void search(std::future<void> future, MovePicker &ai, MovePicker::SearchResult &result);
        static void displaySearchIteration(MovePicker::SearchResult result, int depth, std::chrono::duration<double> elapsed);
    };
} // namespace interfaces::uci::commands
