#include <brain/codbrain.hpp>

#include <uci/uci.hpp>

namespace codfish::brain {
Brain::Brain() noexcept {
    uci::receive_uci.connect([&]() {
        uci::SendId("Codfish", "Codfish");
        uci::SendOptionUciLimitStrength(false);
        uci::SendUciOk();
    });

    // TODO: receive_stop

    // TODO: receive_quit

    uci::receive_is_ready.connect([&]() { uci::SendReadyOk(); });

    // TODO: receive_ponder_hit

    // TODO: receive_uci_new_game

    // TODO: receive_debug

    // TODO: receive_set_option

    // TODO: receive_register

    uci::receive_position.connect([&](const std::string &fen,
                                      const std::vector<std::string> &moves) {
        if (core::utils::ValidFen(fen) == false) {
            uci::SendErrorInvalidFen();
        } else {
            _board.FromFen(fen);
            for (const auto &move : moves) {
                const auto legal = core::movegen::Legal(_board);
                const auto it = std::find_if(legal.begin(), legal.end(),
                                             [&](const core::Move &m) {
                                                 return m.ToString() == move;
                                             });
                if (it == legal.end()) {
                    uci::SendErrorIllegalMove();
                    return;
                } else {
                    _board.Make(*it);
                }
            }
        }
    });

    uci::receive_go.connect([&](const std::map<uci::Command, std::string> &) {
        const auto result = PickMove();
        uci::SendBestMove(result.pv[0].ToString());
    });
}

void
Brain::Launch() noexcept {
    uci::Launch();
}
}   // namespace codfish::brain