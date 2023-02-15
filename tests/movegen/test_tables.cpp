#define CATCH_CONFIG_MAIN

#include "../catch.hpp"

#include <codlib/base.hpp>
#include <codlib/movegen/magics.hpp>
#include <codlib/movegen/tables.hpp>

void setup()
{
    magics::init();
    tables::init();
}

// Test Pawn

TEST_CASE("Test Pawn Moves")
{
    setup();

    SECTION("Corners Start")
    {
        REQUIRE(tables::get_pawn_attacks(WHITE, A2) == 0x20000);
        REQUIRE(tables::get_pawn_attacks(WHITE, H2) == 0x400000);
        REQUIRE(tables::get_pawn_attacks(BLACK, A7) == 0x20000000000);
        REQUIRE(tables::get_pawn_attacks(BLACK, H7) == 0x400000000000);
    }

    SECTION("Limits White")
    {
        REQUIRE(tables::get_pawn_attacks(WHITE, A8) == 0x0);
        REQUIRE(tables::get_pawn_attacks(WHITE, H8) == 0x0);
    }

    SECTION("Limits Black")
    {
        REQUIRE(tables::get_pawn_attacks(BLACK, A1) == 0x0);
        REQUIRE(tables::get_pawn_attacks(BLACK, H1) == 0x0);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(tables::get_pawn_attacks(WHITE, D4) == 0x1400000000);
        REQUIRE(tables::get_pawn_attacks(BLACK, E5) == 0x28000000);
    }
}

// Test King

TEST_CASE("Test King Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(tables::get_king_attacks(A1) == 0x302);
        REQUIRE(tables::get_king_attacks(H1) == 0xc040);
        REQUIRE(tables::get_king_attacks(A8) == 0x203000000000000);
        REQUIRE(tables::get_king_attacks(H8) == 0x40c0000000000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(tables::get_king_attacks(D4) == 0x1c141c0000);
    }
}

// Test Knights

TEST_CASE("Test Knights Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(tables::get_knight_attacks(A1) == 0x20400);
        REQUIRE(tables::get_knight_attacks(H1) == 0x402000);
        REQUIRE(tables::get_knight_attacks(A8) == 0x4020000000000);
        REQUIRE(tables::get_knight_attacks(H8) == 0x20400000000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(tables::get_knight_attacks(D4) == 0x142200221400);
    }
}

// Test Bishop

TEST_CASE("Test Bishop Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(tables::get_bishop_attacks(A1, 0xa000008) == 0x8040200);
        REQUIRE(tables::get_bishop_attacks(H1, 0xfefdfbf7efdfbfff) == 0x102040810204000);
        REQUIRE(tables::get_bishop_attacks(H1, 0xfefdfbf7efdfbf7f) == 0x102040810204000);
        REQUIRE(tables::get_bishop_attacks(H8, 0x8120000808000202) == 0x40201008000000);
        REQUIRE(tables::get_bishop_attacks(A8, 0x8124000858204282) == 0x2040800000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(tables::get_bishop_attacks(D4, 0x8124000858204282) == 0x8041221400142240);
        REQUIRE(tables::get_bishop_attacks(G6, 0x8124400858204282) == 0xa000a010000000);
    }
}

// Test Rook    get_rook_attacks

TEST_CASE("Test Rook Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(tables::get_rook_attacks(A1, 0xa000009) == 0x10101010101010e);
        REQUIRE(tables::get_rook_attacks(H1, 0xfefdfbf7efdfbfff) == 0x8040);
        REQUIRE(tables::get_rook_attacks(H1, 0xfefdfbf7efdfbf7f) == 0x8040);
        REQUIRE(tables::get_rook_attacks(H8, 0x8120000808000202) == 0x7f80808080808080);
        REQUIRE(tables::get_rook_attacks(A8, 0x8324000958a142ba) == 0x201010100000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(tables::get_rook_attacks(D4, 0x8124000858204282) == 0x817080808);
        REQUIRE(tables::get_rook_attacks(F5, 0x8124002858204282) == 0x2020d820200000);
    }
}

// Test Queen

TEST_CASE("Test Queen Moves")
{
    setup();

    SECTION("Corners")
    {
        REQUIRE(tables::get_queen_attacks(A1, 0xa000009) == 0x10101010905030e);
        REQUIRE(tables::get_queen_attacks(H1, 0xfefdfbf7efdfbfff) == 0x10204081020c040);
        REQUIRE(tables::get_queen_attacks(H1, 0xfefdfbf7efdfbf7f) == 0x10204081020c040);
        REQUIRE(tables::get_queen_attacks(H8, 0x8120000808000202) == 0x7fc0a09088808080);
        REQUIRE(tables::get_queen_attacks(A8, 0x8324000958a142ba) == 0x203050900000000);
    }

    SECTION("Middle of the Board")
    {
        REQUIRE(tables::get_queen_attacks(D4, 0x812402085a24428a) == 0x8040221c161c2848);
        REQUIRE(tables::get_queen_attacks(F6, 0x812432a8da245282) == 0x8870d07088000000);
    }
}
