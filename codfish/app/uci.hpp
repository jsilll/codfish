#pragma once

#include <cstddef>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/signals2.hpp>

class uci final {
  public:
    enum class command {
        search_moves,
        ponder,
        white_time,
        black_time,
        white_increment,
        black_increment,
        moves_to_go,
        depth,
        nodes,
        mate,
        move_time,
        infinite
    };
    enum class state { checking, ok, error };
    enum class information {
        depth,
        selective_depth,
        time,
        nodes,
        principle_variation,
        multi_principle_variation,
        score_centipawns,
        score_mate,
        score_lowerbound,
        score_upperbound,
        current_move,
        current_move_number,
        hash_full,
        nodes_per_second,
        table_base_hits,
        cpu_load,
        string,
        refutation,
        current_line
    };

    const std::string start_fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // UI to Engine.
    boost::signals2::signal<void()> receive_uci;
    boost::signals2::signal<void(bool on)> receive_debug;
    boost::signals2::signal<void()> receive_is_ready;
    boost::signals2::signal<void(const std::string &name,
                                 const std::string &value)>
        receive_set_option;
    boost::signals2::signal<void(bool later, const std::string &name,
                                 const std::size_t &code)>
        receive_register;
    boost::signals2::signal<void()> receive_uci_new_game;
    boost::signals2::signal<void(const std::string &fen,
                                 const std::vector<std::string> &moves)>
        receive_position;
    boost::signals2::signal<void(
        const std::map<command, std::string> &parameters)>
        receive_go;
    boost::signals2::signal<void()> receive_stop;
    boost::signals2::signal<void()> receive_ponder_hit;
    boost::signals2::signal<void()> receive_quit;

    // Engine to UI.
    static void send_id(const std::string &name = "",
                        const std::string &author = "") {
        std::cout << "id" << (!name.empty() ? " name " + name : "")
                  << (!author.empty() ? " author " + author : "") << std::endl;
    }
    static void send_uci_ok() { std::cout << "uciok" << std::endl; }
    static void send_ready_ok() { std::cout << "readyok" << std::endl; }
    static void send_best_move([[maybe_unused]] const std::string &move,
                               const bool ponder = false) {
        std::cout << "bestmove" << (ponder ? " ponder" : "") << std::endl;
    }
    static void send_copy_protection(const state state) {
        std::string string = "copyprotection";
        if (state == state::checking)
            string += " checking";
        else if (state == state::ok)
            string += " ok";
        else if (state == state::error)
            string += " error";
        std::cout << string << std::endl;
    }
    static void send_registration(const state state) {
        std::string string = "registration";
        if (state == state::checking)
            string += " checking";
        else if (state == state::ok)
            string += " ok";
        else if (state == state::error)
            string += " error";
        std::cout << string << std::endl;
    }
    static void
    send_information(const std::map<information, std::string> &parameters) {
        std::string string = "info";
        for (auto &parameter : parameters) {
            switch (parameter.first) {
            case information::depth:
                string += " depth ";
                break;
            case information::selective_depth:
                string += " seldepth ";
                break;
            case information::time:
                string += " time ";
                break;
            case information::nodes:
                string += " nodes ";
                break;
            case information::principle_variation:
                string += " pv ";
                break;
            case information::multi_principle_variation:
                string += " multipv ";
                break;
            case information::current_move:
                string += " currmove ";
                break;
            case information::current_move_number:
                string += " currmovenumber ";
                break;
            case information::hash_full:
                string += " hashfull ";
                break;
            case information::nodes_per_second:
                string += " nps ";
                break;
            case information::table_base_hits:
                string += " tbhits ";
                break;
            case information::cpu_load:
                string += " cpuload ";
                break;
            case information::string:
                string += " string ";
                break;
            case information::refutation:
                string += " refutation ";
                break;
            case information::current_line:
                string += " currline ";
                break;
            default:
                break;
            }
            string += parameter.second;
        }
        if (parameters.count(information::score_centipawns) > 0 ||
            parameters.count(information::score_mate) > 0) {
            string += " score";
            if (parameters.count(information::score_centipawns))
                string += " cp " + parameters.at(information::score_centipawns);
            if (parameters.count(information::score_mate))
                string += " mate " + parameters.at(information::score_mate);
            if (parameters.count(information::score_lowerbound))
                string += " lowerbound";
            if (parameters.count(information::score_upperbound))
                string += " upperbound";
        }
        std::cout << string << std::endl;
    }
    static void send_option_check_box(const std::string &name,
                                      const bool initial = true) {
        std::cout << "option name " << name << " type check default "
                  << std::boolalpha << initial << std::endl;
    }
    static void send_option_spin_wheel(const std::string &name,
                                       const std::size_t &initial,
                                       const std::size_t &minimum,
                                       const std::size_t &maximum) {
        std::cout << "option name " << name << " type spin default " << initial
                  << " min " << minimum << " max " << maximum << std::endl;
    }
    static void send_option_combo_box(const std::string &name,
                                      const std::string &initial,
                                      const std::vector<std::string> &values) {
        auto string = "option name " + name + " type combo default " + initial;
        for (auto &value : values)
            string += " var " + value;
        std::cout << string << std::endl;
    }
    static void send_option_button(const std::string &name) {
        std::cout << "option name " << name << " type button" << std::endl;
    }
    static void send_option_string(const std::string &name,
                                   const std::string &initial) {
        std::cout << "option name " << name << " type string default "
                  << initial << std::endl;
    }

    // Base options.
    static void send_option_hash(const std::size_t &initial,
                                 const std::size_t &minimum,
                                 const std::size_t &maximum) {
        send_option_spin_wheel("Hash", initial, minimum, maximum);
    }
    static void send_option_nalimov_path(const std::string &initial) {
        send_option_string("NalimovPath", initial);
    }
    static void send_option_nalimov_cache(const std::size_t &initial,
                                          const std::size_t &minimum,
                                          const std::size_t &maximum) {
        send_option_spin_wheel("NalimovCache", initial, minimum, maximum);
    }
    static void send_option_ponder(const bool initial = true) {
        send_option_check_box("Ponder", initial);
    }
    static void send_option_own_book(const bool initial = true) {
        send_option_check_box("OwnBook", initial);
    }
    static void
    send_option_multi_principle_variation(const std::size_t &initial,
                                          const std::size_t &minimum,
                                          const std::size_t &maximum) {
        send_option_spin_wheel("MultiPV", initial, minimum, maximum);
    }
    static void send_option_uci_show_current_line(const bool initial = true) {
        send_option_check_box("UCI_ShowCurrLine", initial);
    }
    static void send_option_uci_show_refutations(const bool initial = true) {
        send_option_check_box("UCI_ShowRefutations", initial);
    }
    static void send_option_uci_limit_strength(const bool initial = true) {
        send_option_check_box("UCI_LimitStrength", initial);
    }
    static void send_option_uci_elo(const std::size_t &initial,
                                    const std::size_t &minimum,
                                    const std::size_t &maximum) {
        send_option_spin_wheel("UCI_Elo", initial, minimum, maximum);
    }
    static void send_option_uci_analyse_mode(bool const initial = true) {
        send_option_check_box("UCI_AnalyseMode", initial);
    }
    static void send_option_uci_opponent(const std::string &initial) {
        send_option_string("UCI_Opponent", initial);
    }
    static void send_option_uci_about(const std::string &initial) {
        send_option_string("UCI_EngineAbout", initial);
    }
    static void
    send_option_uci_set_position_centipawns(const std::string &initial) {
        send_option_string("UCI_SetPositionValue", initial);
    }

    // Start console IO.
    void launch() {
        std::string line;
        auto running = true;
        while (running && getline(std::cin, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> std::skipws >> token;
            if (token == "uci") {
                receive_uci();
            } else if (token == "debug") {
                iss >> token;
                receive_debug(token == "on");
            } else if (token == "isready") {
                receive_is_ready();
            } else if (token == "setoption") {
                std::string name, value;
                iss >> token;
                while (iss >> token && token != "value")
                    name += std::string(" ", name.empty() ? 0 : 1) + token;
                while (iss >> token)
                    value += std::string(" ", value.empty() ? 0 : 1) + token;
                receive_set_option(name, value);
            } else if (token == "register") {
                auto later = false;
                std::string name;
                std::size_t code = 0;
                iss >> token;
                if (token == "later")
                    later = true;
                if (token == "name")
                    while (iss >> token && token != "code")
                        name += std::string(" ", name.empty() ? 0 : 1) + token;
                if (token == "code")
                    iss >> code;
                receive_register(later, name, code);
            } else if (token == "ucinewgame") {
                receive_uci_new_game();
            } else if (token == "position") {
                std::string fen;
                std::vector<std::string> moves;
                iss >> token;
                if (token == "startpos") {
                    fen = start_fen;
                    iss >> token;
                } else if (token == "fen")
                    while (iss >> token && token != "moves")
                        fen += token + " ";
                else
                    continue;
                while (iss >> token)
                    moves.push_back(token);
                receive_position(fen, moves);
            } else if (token == "go") {
                std::map<command, std::string> commands;
                while (iss >> token)
                    if (token == "searchmoves")
                        while (iss >> token)
                            commands[command::search_moves] +=
                                std::string(
                                    " ", commands[command::search_moves].empty()
                                             ? 0
                                             : 1) +
                                token;
                    else if (token == "ponder")
                        commands[command::ponder];
                    else if (token == "wtime")
                        iss >> commands[command::white_time];
                    else if (token == "btime")
                        iss >> commands[command::black_time];
                    else if (token == "winc")
                        iss >> commands[command::white_increment];
                    else if (token == "binc")
                        iss >> commands[command::black_increment];
                    else if (token == "movestogo")
                        iss >> commands[command::moves_to_go];
                    else if (token == "depth")
                        iss >> commands[command::depth];
                    else if (token == "nodes")
                        iss >> commands[command::nodes];
                    else if (token == "mate")
                        iss >> commands[command::mate];
                    else if (token == "move_time")
                        iss >> commands[command::move_time];
                    else if (token == "infinite")
                        commands[command::infinite];
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
