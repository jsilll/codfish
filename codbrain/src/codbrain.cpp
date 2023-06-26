#include <codbrain/codbrain.hpp>

namespace codbrain {
Brain::Brain() noexcept {
    _uci.receive_uci.connect([&]() {
        _uci.SendId("Codfish", "Codfish");
        _uci.SendOptionUciLimitStrength(false);
        _uci.SendUciOk();
    });

    _uci.receive_is_ready.connect([&]() { _uci.SendReadyOk(); });

    _uci.receive_position.connect([&](const std::string &fen,
                                      const std::vector<std::string> &moves) {
        if (codchess::utils::ValidFen(fen) == false) {
            _uci.SendErrorInvalidFen();
        } else {
            _board.FromFen(fen);
            for (const auto &move : moves) {
                const auto legal = codchess::movegen::Legal(_board);
                const auto it = std::find_if(legal.begin(), legal.end(),
                                             [&](const codchess::Move &m) {
                                                 return m.ToString() == move;
                                             });
                if (it == legal.end()) {
                    _uci.SendErrorIllegalMove();
                    return;
                } else {
                    _board.Make(*it);
                }
            }
        }
    });

    _uci.receive_go.connect([&](const std::map<Uci::Command, std::string> &) {
        const auto result = PickMove();
        _uci.SendBestMove(result.pv[0].ToString());
    });
}
}   // namespace codbrain