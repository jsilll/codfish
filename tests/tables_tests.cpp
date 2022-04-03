#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/magics.hpp"
#include "../src/tables.hpp"

void setup()
{
    static bool init = false;

    if (!init)
    {
        Magics::init();
        Tables::init();
        init = true;
    }
}

// Test Pawn
// Test King

TEST_CASE("Test King Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(Tables::ATTACKS_KING[A1] == 0x302);
        REQUIRE(Tables::ATTACKS_KING[H1] == 0xc040);
        REQUIRE(Tables::ATTACKS_KING[A8] == 0x203000000000000);
        REQUIRE(Tables::ATTACKS_KING[H8] == 0x40c0000000000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(Tables::ATTACKS_KING[D4] == 0x1c141c0000);
    }
}

TEST_CASE("Test Knights Moves")
{
    setup();

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