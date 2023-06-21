#include <chrono>
#include <iostream>

#include <codchess/perft.hpp>

void
_TimedPerft(std::string &label, std::string &fen, std::uint32_t depth) {
    using namespace codchess;

    auto board = Board();
    board.FromFen(fen);

    std::cout << label << "\n";
    std::cout << "Running perft to depth " << depth << "...\n";

    auto start = std::chrono::high_resolution_clock::now();
    auto nodes = codchess::perft::Perft(board, depth);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Depth: " << depth << " Nodes: " << nodes
              << " Time: " << duration << "ms\n\n";
}

#define TimedPerft(label, fen, depth)                                          \
    {                                                                          \
        std::string _label = label;                                            \
        std::string _fen = fen;                                                \
        std::uint32_t _depth = depth;                                          \
        _TimedPerft(_label, _fen, _depth);                                     \
    }

int
main() {
    codchess::Init();

    TimedPerft("Starting Position",
               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);

    TimedPerft(
        "KiwiPete",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        5);

    TimedPerft("Chess Programming Wiki Board 3",
               "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6);

    TimedPerft(
        "Chess Programming Wiki Board 4",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);

    TimedPerft("Chess Programming Wiki Board 5",
               "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5);

    TimedPerft("Chess Programming Wiki Board 6",
               "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 "
               "w - - 0 10",
               5);
}
