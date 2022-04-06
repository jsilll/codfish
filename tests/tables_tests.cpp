#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/defs.hpp"
#include "../src/magics.hpp"
#include "../src/tables.hpp"
#include "../src/board.hpp"

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

TEST_CASE("Test Pawn Moves")
{
    setup();

    SECTION("Corners Start")
    {
        REQUIRE(Tables::ATTACKS_PAWN[WHITE][A2] == 0x20000);
        REQUIRE(Tables::ATTACKS_PAWN[WHITE][H2] == 0x400000);
        REQUIRE(Tables::ATTACKS_PAWN[BLACK][A7] == 0x20000000000);
        REQUIRE(Tables::ATTACKS_PAWN[BLACK][H7] == 0x400000000000);
    }

    SECTION("Limits White")
    {
        REQUIRE(Tables::ATTACKS_PAWN[WHITE][A8] == 0x0);
        REQUIRE(Tables::ATTACKS_PAWN[WHITE][H8] == 0x0);
    }

    SECTION("Limits Black")
    {
        REQUIRE(Tables::ATTACKS_PAWN[BLACK][A1] == 0x0);
        REQUIRE(Tables::ATTACKS_PAWN[BLACK][H1] == 0x0);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(Tables::ATTACKS_PAWN[WHITE][D4] == 0x1400000000);
        REQUIRE(Tables::ATTACKS_PAWN[BLACK][E5] == 0x28000000);
    }
}

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

// Test Knights

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