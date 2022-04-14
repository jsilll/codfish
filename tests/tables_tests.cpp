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

TEST_CASE("Test Bishop Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(Tables::getBishopAttacks(A1, 0xa000008) == 0x8040200);
        REQUIRE(Tables::getBishopAttacks(H1, 0xfefdfbf7efdfbfff) == 0x102040810204000);
        REQUIRE(Tables::getBishopAttacks(H1, 0xfefdfbf7efdfbf7f) == 0x102040810204000);
        REQUIRE(Tables::getBishopAttacks(H8, 0x8120000808000202) == 0x40201008000000);
        REQUIRE(Tables::getBishopAttacks(A8, 0x8124000858204282) == 0x2040800000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(Tables::getBishopAttacks(D4, 0x8124000858204282) == 0x8041221400142240);
        REQUIRE(Tables::getBishopAttacks(G6, 0x8124400858204282) == 0xa000a010000000);
    }
}

// Test Rook    getRookAttacks

TEST_CASE("Test Rook Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(Tables::getRookAttacks(A1, 0xa000009) == 0x10101010101010e);
        REQUIRE(Tables::getRookAttacks(H1, 0xfefdfbf7efdfbfff) == 0x8040);
        REQUIRE(Tables::getRookAttacks(H1, 0xfefdfbf7efdfbf7f) == 0x8040);
        REQUIRE(Tables::getRookAttacks(H8, 0x8120000808000202) == 0x7f80808080808080);
        REQUIRE(Tables::getRookAttacks(A8, 0x8324000958a142ba) == 0x201010100000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(Tables::getRookAttacks(D4, 0x8124000858204282) == 0x817080808);
        REQUIRE(Tables::getRookAttacks(F5, 0x8124002858204282) == 0x2020d820200000);
    }
}

// Test Queen   getQueenAttacks