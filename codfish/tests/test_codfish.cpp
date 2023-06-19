#include <catch2/catch_test_macros.hpp>

#include <codbrain/codbrain.hpp>

#include <codfish/uci.hpp>

TEST_CASE("codfish::uci::commands") {
    codbrain::Init();
    using namespace codfish;

    std::string buffer{};
    std::stringstream in{};
    std::stringstream out{};
    auto repl = repl::Repl(in, out, std::move(uci::commands), false);

    SECTION("uci") {
        in << "uci\n";
        repl.RunOnce();
        std::getline(out, buffer);
        REQUIRE(buffer == "id name codfish");
        std::getline(out, buffer);
        REQUIRE(buffer == "id author codfish");
        std::getline(out, buffer);
        REQUIRE(buffer == "uciok");
    }

    SECTION("debug") {
        in << "debug\n";
        repl.RunOnce();
        std::getline(out, buffer);
        REQUIRE(buffer == "debug mode is now on");
        in << "debug\n";
        repl.RunOnce();
        std::getline(out, buffer);
        REQUIRE(buffer == "debug mode is now off");
    }

    SECTION("isready") {
        in << "isready\n";
        repl.RunOnce();
        std::getline(out, buffer);
        REQUIRE(buffer == "readyok");
    }

    SECTION("ucinewgame") {
        in << "ucinewgame\n";
        repl.RunOnce();
        std::getline(out, buffer);
        REQUIRE(buffer == "");
    }
}