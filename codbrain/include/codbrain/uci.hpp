// Original Source Code: https://github.com/acdemiralp/uci

#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/signals2.hpp>

namespace codbrain {
/// @brief Universal Chess Interface.
class Uci final {
  public:
    /// @brief UCI states.
    enum class State {
        /// @brief Checking state.
        Checking,
        /// @brief Ok state.
        Ok,
        /// @brief Error state.
        Error
    };

    /// @brief UCI commands.
    enum class Command {
        /// @brief Search command.
        SearchMoves,
        /// @brief Ponder command.
        Ponder,
        /// @brief White time command.
        WhiteTime,
        /// @brief Black time command.
        BlackTime,
        /// @brief White increment command.
        WhiteIncrement,
        /// @brief Black increment command.
        BlackIncrement,
        /// @brief Moves to go command.
        MovesToGo,
        /// @brief Depth command.
        Depth,
        /// @brief Nodes command.
        Nodes,
        /// @brief Mate command.
        Mate,
        /// @brief Move time command.
        MoveTime,
        /// @brief Infinite command.
        Infinite
    };

    /// @brief UCI information.
    enum class Information {
        /// @brief Depth information.
        Depth,
        /// @brief Selective depth information.
        SelectiveDepth,
        /// @brief Time information.
        Time,
        /// @brief Nodes information.
        Nodes,
        /// @brief Principle variation information.
        PrincipleVariation,
        /// @brief Multi-principle variation information.
        MultiPrincipleVariation,
        /// @brief Score centipawns information.
        ScoreCentipawns,
        /// @brief Score mate information.
        ScoreMate,
        /// @brief Score lower bound information.
        ScoreLowerBound,
        /// @brief Score upper bound information.
        ScoreUpperBound,
        /// @brief Current move information.
        CurrentMove,
        /// @brief Current move number information.
        CurrentMoveNumber,
        /// @brief Hash full information.
        HashFull,
        /// @brief Nodes per second information.
        NodesPerSecond,
        /// @brief Table base hits information.
        TableBaseHits,
        /// @brief CPU load information.
        CpuLoad,
        /// @brief String information.
        String,
        /// @brief Refutation information.
        Refutation,
        /// @brief Current line information.
        CurrentLine
    };

    /// @brief Receive 'uci' command signal.
    boost::signals2::signal<void()> receive_uci;
    /// @brief Receive 'stop' command signal.
    boost::signals2::signal<void()> receive_stop;
    /// @brief Receive 'quit' command signal.
    boost::signals2::signal<void()> receive_quit;
    /// @brief Receive 'isready' command signal.
    boost::signals2::signal<void()> receive_is_ready;
    /// @brief Receive 'ponderhit' command signal.
    boost::signals2::signal<void()> receive_ponder_hit;
    /// @brief Receive 'ucinewgame' command signal.
    boost::signals2::signal<void()> receive_uci_new_game;
    //// @brief Receive 'debug' command signal.
    boost::signals2::signal<void(bool on)> receive_debug;
    /// @brief Receive 'setoption' command signal.
    boost::signals2::signal<void(const std::string &name,
                                 const std::string &value)>
        receive_set_option;
    //// @brief Receive 'register' command signal.
    boost::signals2::signal<void(bool later, const std::string &name,
                                 const std::size_t &code)>
        receive_register;
    //// @brief Receive 'position' command signal.
    boost::signals2::signal<void(const std::string &fen,
                                 const std::vector<std::string> &moves)>
        receive_position;
    //// @brief Receive 'go' command signal.
    boost::signals2::signal<void(
        const std::map<Command, std::string> &parameters)>
        receive_go;

    const std::string start_fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    /// @brief Sends the engine id.
    /// @param name The engine name.
    /// @param author The engine author.
    static void SendId(const std::string &name = "",
                       const std::string &author = "") {
        std::cout << "id" << (!name.empty() ? " name " + name : "")
                  << (!author.empty() ? " author " + author : "") << std::endl;
    }

    /// @brief Send the 'uciok' response.
    static void SendUciOk() { std::cout << "uciok" << std::endl; }

    /// @brief Sends the 'readyok' response.
    static void SendReadyOk() { std::cout << "readyok" << std::endl; }

    /// @brief Sends the 'bestmove' response.
    /// @param move
    /// @param ponder
    static void SendBestMove(const std::string &move,
                             const bool ponder = false) {
        std::cout << "bestmove " << move << (ponder ? " ponder" : "")
                  << std::endl;
    }

    /// @brief Sends the copy protection state.
    /// @param state The state.
    static void SendCopyProtection(const State state) {
        std::string string = "copyprotection";
        if (state == State::Checking)
            string += " checking";
        else if (state == State::Ok)
            string += " ok";
        else if (state == State::Error)
            string += " error";
        std::cout << string << std::endl;
    }

    /// @brief Sends the registration state.
    /// @param state The state.
    static void SendRegistration(const State state) {
        std::string string = "registration";
        if (state == State::Checking)
            string += " checking";
        else if (state == State::Ok)
            string += " ok";
        else if (state == State::Error)
            string += " error";
        std::cout << string << std::endl;
    }

    /// @brief Sends the information.
    /// @param parameters The parameters.
    static void
    SendInformation(const std::map<Information, std::string> &parameters) {
        std::string string = "info";
        for (auto &parameter : parameters) {
            switch (parameter.first) {
            case Information::Depth:
                string += " depth ";
                break;
            case Information::SelectiveDepth:
                string += " seldepth ";
                break;
            case Information::Time:
                string += " time ";
                break;
            case Information::Nodes:
                string += " nodes ";
                break;
            case Information::PrincipleVariation:
                string += " pv ";
                break;
            case Information::MultiPrincipleVariation:
                string += " multipv ";
                break;
            case Information::CurrentMove:
                string += " currmove ";
                break;
            case Information::CurrentMoveNumber:
                string += " currmovenumber ";
                break;
            case Information::HashFull:
                string += " hashfull ";
                break;
            case Information::NodesPerSecond:
                string += " nps ";
                break;
            case Information::TableBaseHits:
                string += " tbhits ";
                break;
            case Information::CpuLoad:
                string += " cpuload ";
                break;
            case Information::String:
                string += " string ";
                break;
            case Information::Refutation:
                string += " refutation ";
                break;
            case Information::CurrentLine:
                string += " currline ";
                break;
            default:
                break;
            }
            string += parameter.second;
        }
        if (parameters.count(Information::ScoreCentipawns) > 0 ||
            parameters.count(Information::ScoreMate) > 0) {
            string += " score";
            if (parameters.count(Information::ScoreCentipawns))
                string += " cp " + parameters.at(Information::ScoreCentipawns);
            if (parameters.count(Information::ScoreMate))
                string += " mate " + parameters.at(Information::ScoreMate);
            if (parameters.count(Information::ScoreLowerBound))
                string += " lowerbound";
            if (parameters.count(Information::ScoreUpperBound))
                string += " upperbound";
        }
        std::cout << string << std::endl;
    }

    /// @brief Sends a check box option.
    /// @param name The name.
    /// @param initial The initial value.
    static void SendOptionCheckBox(const std::string &name,
                                   const bool initial = true) {
        std::cout << "option name " << name << " type check default "
                  << std::boolalpha << initial << std::endl;
    }

    /// @brief Sends a spin wheel option.
    /// @param name The name.
    /// @param initial The initial value.
    static void SendOptionSpinWheel(const std::string &name,
                                    const std::size_t &initial,
                                    const std::size_t &minimum,
                                    const std::size_t &maximum) {
        std::cout << "option name " << name << " type spin default " << initial
                  << " min " << minimum << " max " << maximum << std::endl;
    }

    /// @brief Sends a combo box option.
    /// @param name The name.
    /// @param initial The initial value.
    /// @param values The values.
    static void SendOptionComboBox(const std::string &name,
                                   const std::string &initial,
                                   const std::vector<std::string> &values) {
        auto string = "option name " + name + " type combo default " + initial;
        for (auto &value : values)
            string += " var " + value;
        std::cout << string << std::endl;
    }

    /// @brief Sends an button option.
    /// @param name The name.
    static void SendOptionButton(const std::string &name) {
        std::cout << "option name " << name << " type button" << std::endl;
    }

    /// @brief Sends a string option.
    /// @param name The name.
    static void SendOptionString(const std::string &name,
                                 const std::string &initial) {
        std::cout << "option name " << name << " type string default "
                  << initial << std::endl;
    }

    /// @brief Sends the hash option.
    /// @param initial The initial value.
    /// @param minimum The minimum value.
    /// @param maximum The maximum value.
    static void send_option_hash(const std::size_t &initial,
                                 const std::size_t &minimum,
                                 const std::size_t &maximum) {
        SendOptionSpinWheel("Hash", initial, minimum, maximum);
    }

    /// @brief Sends the nalimov path option.
    /// @param initial The initial value.
    static void SendOptionNalimovPath(const std::string &initial) {
        SendOptionString("NalimovPath", initial);
    }

    /// @brief Sends the nalimov cache option.
    /// @param initial The initial value.
    /// @param minimum The minimum value.
    /// @param maximum The maximum value.
    static void SendOptionNalimovCache(const std::size_t &initial,
                                       const std::size_t &minimum,
                                       const std::size_t &maximum) {
        SendOptionSpinWheel("NalimovCache", initial, minimum, maximum);
    }

    /// @brief Sends the ponder option.
    /// @param initial The initial value.
    static void SendOptionPonder(const bool initial = true) {
        SendOptionCheckBox("Ponder", initial);
    }

    /// @brief Sends the own book option.
    /// @param initial The initial value.
    static void SendOptionOwnBook(const bool initial = true) {
        SendOptionCheckBox("OwnBook", initial);
    }

    /// @brief Sends the multi-principle variation option.
    /// @param initial The initial value.
    /// @param minimum The minimum value.
    /// @param maximum The maximum value.
    static void SendOptionMultiPrincipleVariation(const std::size_t &initial,
                                                  const std::size_t &minimum,
                                                  const std::size_t &maximum) {
        SendOptionSpinWheel("MultiPV", initial, minimum, maximum);
    }

    /// @brief Sends the UCI_ShowCurrLine option.
    /// @param initial The initial value.
    static void SendOptionUciShowCurrentLine(const bool initial = true) {
        SendOptionCheckBox("UCI_ShowCurrLine", initial);
    }

    /// @brief Sends the UCI_ShowRefutations option.
    /// @param initial The initial value.
    static void SendOptionUciShowRefutations(const bool initial = true) {
        SendOptionCheckBox("UCI_ShowRefutations", initial);
    }

    /// @brief Sends the UCI_LimitStrength option.
    /// @param initial The initial value.
    static void SendOptionUciLimitStrength(const bool initial = true) {
        SendOptionCheckBox("UCI_LimitStrength", initial);
    }

    /// @brief Sends the UCI_Elo option.
    /// @param initial The initial value.
    /// @param minimum The minimum value.
    /// @param maximum The maximum value.
    static void SendOptionUciElo(const std::size_t &initial,
                                 const std::size_t &minimum,
                                 const std::size_t &maximum) {
        SendOptionSpinWheel("UCI_Elo", initial, minimum, maximum);
    }

    /// @brief Sends the UCI_AnalyseMode option.
    /// @param initial The initial value.
    static void SendOptionUciAnalyseMode(bool const initial = true) {
        SendOptionCheckBox("UCI_AnalyseMode", initial);
    }

    /// @brief Sends the UCI_Opponent option.
    /// @param initial The initial value.
    static void SendOptionUciOpponent(const std::string &initial) {
        SendOptionString("UCI_Opponent", initial);
    }

    /// @brief Sends the UCI_EngineAbout option.
    /// @param initial The initial value.
    static void SendOptionUciAbout(const std::string &initial) {
        SendOptionString("UCI_EngineAbout", initial);
    }

    /// @brief Sends the UCI_SetPositionValue option.
    /// @param initial The initial value.
    static void SendOptionUciSetPositionCentipawns(const std::string &initial) {
        SendOptionString("UCI_SetPositionValue", initial);
    }

    /// @brief Sends an illegal move error.
    static void SendErrorIllegalMove() {
        std::cout << "Illegal move" << std::endl;
    }

    /// @brief Sends an invalid fen error.
    static void SendErrorInvalidFen() {
        std::cout << "Invalid fen" << std::endl;
    }

    /// @brief Launches the UCI loop.
    void Launch() {
        std::string line{};
        bool running{true};
        while (running && getline(std::cin, line)) {
            std::string token{};
            std::istringstream iss(line);
            iss >> std::skipws >> token;
            if (token == "uci") {
                receive_uci();
            } else if (token == "debug") {
                iss >> token;
                receive_debug(token == "on");
            } else if (token == "isready") {
                receive_is_ready();
            } else if (token == "setoption") {
                std::string name{}, value{};
                iss >> token;
                while (iss >> token && token != "value") {
                    name += std::string(" ", name.empty() ? 0 : 1) + token;
                }
                while (iss >> token) {
                    value += std::string(" ", value.empty() ? 0 : 1) + token;
                }
                receive_set_option(name, value);
            } else if (token == "register") {
                bool later{false};
                std::string name{};
                std::size_t code{0};
                iss >> token;
                if (token == "later") {
                    later = true;
                }
                if (token == "name") {
                    while (iss >> token && token != "code") {
                        name += std::string(" ", name.empty() ? 0 : 1) + token;
                    }
                }
                if (token == "code") {
                    iss >> code;
                }
                receive_register(later, name, code);
            } else if (token == "ucinewgame") {
                receive_uci_new_game();
            } else if (token == "position") {
                std::string fen{};
                std::vector<std::string> moves{};
                iss >> token;
                if (token == "startpos") {
                    fen = start_fen;
                    iss >> token;
                } else if (token == "fen") {
                    while (iss >> token && token != "moves") {
                        fen += token + " ";
                    }
                } else {
                    continue;
                }
                while (iss >> token) {
                    moves.push_back(token);
                }
                receive_position(fen, moves);
            } else if (token == "go") {
                std::map<Command, std::string> commands{};
                while (iss >> token)
                    if (token == "searchmoves")
                        while (iss >> token) {
                            commands[Command::SearchMoves] +=
                                std::string(
                                    " ", commands[Command::SearchMoves].empty()
                                             ? 0
                                             : 1) +
                                token;
                        }
                    else if (token == "ponder") {
                        commands[Command::Ponder];
                    } else if (token == "wtime") {
                        iss >> commands[Command::WhiteTime];
                    } else if (token == "btime") {
                        iss >> commands[Command::BlackTime];
                    } else if (token == "winc") {
                        iss >> commands[Command::WhiteIncrement];
                    } else if (token == "binc") {
                        iss >> commands[Command::BlackIncrement];
                    } else if (token == "movestogo") {
                        iss >> commands[Command::MovesToGo];
                    } else if (token == "depth") {
                        iss >> commands[Command::Depth];
                    } else if (token == "nodes") {
                        iss >> commands[Command::Nodes];
                    } else if (token == "mate") {
                        iss >> commands[Command::Mate];
                    } else if (token == "move_time") {
                        iss >> commands[Command::MoveTime];
                    } else if (token == "infinite") {
                        commands[Command::Infinite];
                    }
                receive_go(commands);
            } else if (token == "stop") {
                receive_stop();
            } else if (token == "ponderhit") {
                receive_ponder_hit();
            } else if (token == "quit") {
                receive_quit();
                running = false;
            } else {
                std::cout << "Unrecognized command: " << line << std::endl;
            }
        }
    }
};
}   // namespace codbrain