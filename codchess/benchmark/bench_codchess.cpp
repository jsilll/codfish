#include <chrono>
#include <iostream>

#include <codchess/perft.hpp>

void
BenchPerft(std::string &label, std::string &fen, std::uint32_t depth) {
    using namespace codchess;

    std::cout << "----------------------------------------\n";
    std::cout << label << " at depth " << depth << "\n";

    auto board = Board();
    board.FromFen(fen);

    auto start = std::chrono::high_resolution_clock::now();
    auto nodes = codchess::perft::Perft(board, depth);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    std::cout << "Nodes: " << nodes << " Time: " << duration_ms << "ms\n";

    auto nps_mill = nodes / (static_cast<double>(duration_ms) / 1000.0) / 1000000.0;
    std::cout << "MNPS: " << nps_mill << "\n";
}

#define BENCH_PERFT(label, fen, depth)                                         \
    {                                                                          \
        std::string _fen = fen;                                                \
        std::string _label = label;                                            \
        std::uint32_t _depth = depth;                                          \
        BenchPerft(_label, _fen, _depth);                                      \
    }

int
main() {
    codchess::Init();

    BENCH_PERFT("Starting Position",
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);

    BENCH_PERFT(
        "KiwiPete",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        5);

    BENCH_PERFT("Chess Programming Wiki Board 3",
                "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6);

    BENCH_PERFT(
        "Chess Programming Wiki Board 4",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);

    BENCH_PERFT("Chess Programming Wiki Board 5",
                "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5);

    BENCH_PERFT("Chess Programming Wiki Board 6",
                "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 "
                "w - - 0 10",
                5);
}
