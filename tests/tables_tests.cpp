#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/defs.hpp"
#include "../src/magics.hpp"
#include "../src/tables.hpp"
#include "../src/board.hpp"

// Test Pawn
// Test King

TEST_CASE("Test Knights Moves")
{
    Magics::init();
    Tables::init();

    SECTION("Corners")
    {
        REQUIRE(Tables::ATTACKS_KNIGHT[A1] == 0x20400);
        REQUIRE(Tables::ATTACKS_KNIGHT[H1] == 0x402000);
        REQUIRE(Tables::ATTACKS_KNIGHT[A8] == 0x4020000000000);
        REQUIRE(Tables::ATTACKS_KNIGHT[H8] == 0x20400000000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(Tables::ATTACKS_KNIGHT[D4] == 0x142200221400);
    }
}

// Test Bishop
// Test Rook
// Test Queen