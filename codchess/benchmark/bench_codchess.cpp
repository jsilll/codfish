#include <chrono>
#include <iostream>

#include <codchess/perft.hpp>

using namespace codchess;

void
PrintResults(const std::string &label, const std::int64_t ms,
             const std::uint64_t nodes) noexcept {
    auto nps = nodes / (static_cast<double>(ms) / 1000.0) / 1000000.0;
    std::cout << label << ": Nodes: " << nodes << " Time: " << ms
              << "ms MNPS: " << nps << "\n";
}

void
BenchPerft(const std::string &label, const std::string &fen,
           std::uint32_t depth) {
    std::cout << "\n" << label << " at depth " << depth << "\n";

    {
        auto board = Board();
        board.FromFen(fen);
        const auto start = std::chrono::high_resolution_clock::now();
        const auto nodes = codchess::perft::PerftCopy(board, depth);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
        PrintResults("codchess::perft::PerftCopy", ms, nodes);
    }

    {
        auto board = Board();
        board.FromFen(fen);
        const auto start = std::chrono::high_resolution_clock::now();
        const auto nodes = codchess::perft::PerftUnmake(board, depth);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
        PrintResults("codchess::perft::PerftUnmake", ms, nodes);
    }
}

int
main() {
    ::codchess::Init();

    BenchPerft("Chess Programming Wiki Board 1",
               "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);

    BenchPerft(
        "Chess Programming Wiki Board 2",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        5);

    BenchPerft("Chess Programming Wiki Board 3",
               "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6);

    BenchPerft(
        "Chess Programming Wiki Board 4",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5);

    BenchPerft("Chess Programming Wiki Board 5",
               "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5);

    BenchPerft("Chess Programming Wiki Board 6",
               "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 "
               "w - - 0 10",
               5);
}
