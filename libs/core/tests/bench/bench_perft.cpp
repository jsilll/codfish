#include <chrono>
#include <iostream>

#include <core/perft.hpp>

using namespace codfish;

using core::Board;

class PerftBench {
  public:
    void Register(const std::string &label, const std::string &fen,
                  const std::uint32_t depth) {
        if (!core::utils::ValidFen(fen)) {
            throw std::runtime_error("Invalid FEN: " + fen);
        } else {
            instances_.push_back({label, fen, depth});
        }
    }

    void Run() noexcept {
        for (const auto &instance : instances_) {
            const auto res =
                RunInstance(instance.label, instance.fen, instance.depth);
            results_.push_back(res);
        }
        PrintFinalStats();
    }

  private:
    struct BenchInstance {
        std::string label;
        std::string fen;
        std::uint32_t depth;
    };

    std::vector<BenchInstance> instances_;

    void PrintInstanceResult(const std::string &label, const std::int64_t ms,
                             const std::uint64_t nodes) noexcept {
        auto nps = nodes / (static_cast<double>(ms) / 1000.0) / 1000000.0;
        std::cout << label << ": Nodes: " << nodes << " Time: " << ms
                  << "ms MNPS: " << nps << "\n";
    }

    struct InstanceResult {
        std::int64_t copy_ms;
        std::uint64_t copy_nodes;
        std::int64_t unmake_ms;
        std::uint64_t unmake_nodes;
    };

    std::vector<InstanceResult> results_;

    InstanceResult RunInstance(const std::string &label, const std::string &fen,
                               const std::uint32_t depth) noexcept {
        InstanceResult res{};

        std::cout << "\n" << label << " at depth " << depth << "\n";

        {
            auto board = core::Board();
            board.FromFen(fen);
            const auto start = std::chrono::high_resolution_clock::now();
            const auto nodes = core::perft::PerftCopy(board, depth);
            const auto end = std::chrono::high_resolution_clock::now();
            const auto ms =
                std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                      start)
                    .count();
            res.copy_ms = ms;
            res.copy_nodes = nodes;
            PrintInstanceResult("cod::core::perft::PerftCopy", ms, nodes);
        }

        {
            auto board = Board();
            board.FromFen(fen);
            const auto start = std::chrono::high_resolution_clock::now();
            const auto nodes = core::perft::PerftUnmake(board, depth);
            const auto end = std::chrono::high_resolution_clock::now();
            const auto ms =
                std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                      start)
                    .count();
            res.unmake_ms = ms;
            res.unmake_nodes = nodes;
            PrintInstanceResult("cod::core::perft::PerftUnmake", ms, nodes);
        }

        return res;
    }

    void PrintFinalStats() noexcept {
        std::int64_t copy_ms{0};
        std::uint64_t copy_nodes{0};
        std::int64_t unmake_ms{0};
        std::uint64_t unmake_nodes{0};
        for (const auto &res : results_) {
            copy_ms += res.copy_ms;
            copy_nodes += res.copy_nodes;
            unmake_ms += res.unmake_ms;
            unmake_nodes += res.unmake_nodes;
        }
        copy_ms /= results_.size();
        copy_nodes /= results_.size();
        unmake_ms /= results_.size();
        unmake_nodes /= results_.size();

        const auto copy_nps =
            copy_nodes / (static_cast<double>(copy_ms) / 1000.0) / 1000000.0;
        const auto unmake_nps = unmake_nodes /
                                (static_cast<double>(unmake_ms) / 1000.0) /
                                1000000.0;

        std::cout << "\n"
                  << "Final stats:\n"
                  << "cod::core::perft::PerftCopy: Average MNPS: " << copy_nps
                  << "\n"
                  << "cod::core::perft::PerftUnmake: Average MNPS: "
                  << unmake_nps << "\n";
    }
};

int
main() {
    core::Init();

    PerftBench bench;

    try {
        // clang-format off
        bench.Register(
            "Chess Programming Wiki Board 1",
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 
            6);
        bench.Register(
            "Chess Programming Wiki Board 2",
            "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 
            5);
        bench.Register(
            "Chess Programming Wiki Board 3",
            "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 
            6);
        bench.Register(
            "Chess Programming Wiki Board 4",
            "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 
            5);
        bench.Register(
            "Chess Programming Wiki Board 5",
            "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 
            5);
        bench.Register(
            "Chess Programming Wiki Board 6",
            "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 
            5);
        // clang-format on
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    bench.Run();

    return EXIT_SUCCESS;
}
