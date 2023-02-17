#include "catch2/catch_test_macros.hpp"

#include <codlib/codlib.hpp>
// #include <codlib/movegen/attacks.hpp>

TEST_CASE("Test Pawn Moves") {
//   codlib::init();
//
//   SECTION("Corners Start") {
//     REQUIRE(attacks::mask_white_pawn_any_attacks(Square::A2) == 0x20000);
//     REQUIRE(attacks::mask_white_pawn_any_attacks(Square::H2) == 0x400000);
//     REQUIRE(attacks::mask_black_pawn_any_attacks(Square::A7) == 0x20000000000);
//     REQUIRE(attacks::mask_black_pawn_any_attacks(Square::H7) == 0x400000000000);
//   }
//
//   SECTION("Limits White") {
//     REQUIRE(attacks::mask_white_pawn_any_attacks(A8) == 0x0);
//     REQUIRE(attacks::mask_white_pawn_any_attacks(H8) == 0x0);
//   }
//
//   SECTION("Limits Black") {
//     REQUIRE(attacks::mask_black_pawn_any_attacks(A1) == 0x0);
//     REQUIRE(attacks::mask_black_pawn_any_attacks(H1) == 0x0);
//   }
//
//   SECTION("Middle of the Board") {
//     REQUIRE(attacks::mask_white_pawn_any_attacks(D4) == 0x1400000000);
//     REQUIRE(attacks::mask_black_pawn_any_attacks(E5) == 0x28000000);
//   }
// }
//
// TEST_CASE("Test King Moves") {
//   codlib::init();
//
//   SECTION("Corners") {
//     REQUIRE(attacks::ATTACKS_KING[A1] == 0x302);
//     REQUIRE(attacks::ATTACKS_KING[H1] == 0xc040);
//     REQUIRE(attacks::ATTACKS_KING[A8] == 0x203000000000000);
//     REQUIRE(attacks::ATTACKS_KING[H8] == 0x40c0000000000000);
//   }
//
//   SECTION("Middle of the Board") {
//     REQUIRE(attacks::ATTACKS_KING[D4] == 0x1c141c0000);
//   }
// }
//
// TEST_CASE("Test Knights Moves") {
//   codlib::init();
//
//   SECTION("Corners") {
//     REQUIRE(attacks::ATTACKS_KNIGHT[A1] == 0x20400);
//     REQUIRE(attacks::ATTACKS_KNIGHT[H1] == 0x402000);
//     REQUIRE(attacks::ATTACKS_KNIGHT[A8] == 0x4020000000000);
//     REQUIRE(attacks::ATTACKS_KNIGHT[H8] == 0x20400000000000);
//   }
//
//   SECTION("Middle of the Board") {
//     REQUIRE(attacks::ATTACKS_KNIGHT[D4] == 0x142200221400);
//   }
// }
//
// TEST_CASE("Test Bishop Moves") {
//   codlib::init();
//
//   SECTION("Corners") {
//     REQUIRE(attacks::get_bishop_attacks(A1, 0xa000008) == 0x8040200);
//     REQUIRE(attacks::get_bishop_attacks(H1, 0xfefdfbf7efdfbfff) ==
//             0x102040810204000);
//     REQUIRE(attacks::get_bishop_attacks(H1, 0xfefdfbf7efdfbf7f) ==
//             0x102040810204000);
//     REQUIRE(attacks::get_bishop_attacks(H8, 0x8120000808000202) ==
//             0x40201008000000);
//     REQUIRE(attacks::get_bishop_attacks(A8, 0x8124000858204282) ==
//             0x2040800000000);
//   }
//
//   SECTION("Middle of the Board") {
//     REQUIRE(attacks::get_bishop_attacks(D4, 0x8124000858204282) ==
//             0x8041221400142240);
//     REQUIRE(attacks::get_bishop_attacks(G6, 0x8124400858204282) ==
//             0xa000a010000000);
//   }
// }
//
// TEST_CASE("Test Rook Moves") {
//   codlib::init();
//
//   SECTION("Corners") {
//     REQUIRE(attacks::get_rook_attacks(A1, 0xa000009) == 0x10101010101010e);
//     REQUIRE(attacks::get_rook_attacks(H1, 0xfefdfbf7efdfbfff) == 0x8040);
//     REQUIRE(attacks::get_rook_attacks(H1, 0xfefdfbf7efdfbf7f) == 0x8040);
//     REQUIRE(attacks::get_rook_attacks(H8, 0x8120000808000202) ==
//             0x7f80808080808080);
//     REQUIRE(attacks::get_rook_attacks(A8, 0x8324000958a142ba) ==
//             0x201010100000000);
//   }
//
//   SECTION("Middle of the Board") {
//     REQUIRE(attacks::get_rook_attacks(D4, 0x8124000858204282) == 0x817080808);
//     REQUIRE(attacks::get_rook_attacks(F5, 0x8124002858204282) ==
//             0x2020d820200000);
//   }
// }
//
// TEST_CASE("Test Queen Moves") {
//   codlib::init();
//
//   SECTION("Corners") {
//     REQUIRE(attacks::get_queen_attacks(A1, 0xa000009) == 0x10101010905030e);
//     REQUIRE(attacks::get_queen_attacks(H1, 0xfefdfbf7efdfbfff) ==
//             0x10204081020c040);
//     REQUIRE(attacks::get_queen_attacks(H1, 0xfefdfbf7efdfbf7f) ==
//             0x10204081020c040);
//     REQUIRE(attacks::get_queen_attacks(H8, 0x8120000808000202) ==
//             0x7fc0a09088808080);
//     REQUIRE(attacks::get_queen_attacks(A8, 0x8324000958a142ba) ==
//             0x203050900000000);
//   }
//
//   SECTION("Middle of the Board") {
//     REQUIRE(attacks::get_queen_attacks(D4, 0x812402085a24428a) ==
//             0x8040221c161c2848);
//     REQUIRE(attacks::get_queen_attacks(F6, 0x812432a8da245282) ==
//             0x8870d07088000000);
//   }
}