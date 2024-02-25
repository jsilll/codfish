// use strum_macros::EnumCount;

// #[repr(u8)]
// #[derive(Debug, Clone, Copy)]
// pub enum CastlePerm {
//     None = 0,
//     WhiteKing = 1,
//     WhiteQueen = 2,
//     BlackKing = 4,
//     BlackQueen = 8,
//     All = 15,
// }

// #[repr(u8)]
// #[derive(Debug, Clone, Copy, EnumCount)]
// pub enum Piece {
//     Pawn = 0,
//     Knight = 1,
//     Bishop = 2,
//     Rook = 3,
//     Queen = 4,
//     King = 5,
// }

// #[repr(u8)]
// #[derive(Debug, Clone, Copy, EnumCount)]
// pub enum File {
//     A = 0,
//     B = 1,
//     C = 2,
//     D = 3,
//     E = 4,
//     F = 5,
//     G = 6,
//     H = 7,
// }

// #[repr(u8)]
// #[derive(Debug, Clone, Copy, EnumCount)]
// pub enum Rank {
//     R1 = 0,
//     R2 = 1,
//     R3 = 2,
//     R4 = 3,
//     R5 = 4,
//     R6 = 5,
//     R7 = 6,
//     R8 = 7,
// }

// #[repr(i8)]
// #[derive(Debug, Clone, Copy, EnumCount)]
// pub enum Direction {
//     North = 8,
//     South = -8,
//     East = 1,
//     West = -1,
//     NorthEast = 9,
//     NorthWest = 7,
//     SouthEast = -7,
//     SouthWest = -9,
// }

// pub fn get_bit(bb: u64, sq: Square) -> bool {
//     (bb >> sq as u64) & 1 == 1
// }

// pub fn count_bits(mut bb: u64) -> u8 {
//     let mut count = 0;
//     while bb != 0 {
//         count += 1;
//         bb &= bb - 1;
//     }
//     count
// }

// pub fn scan_forward(bb: u64) -> Option<u8> {
//     match bb == 0 {
//         true => None,
//         false => Some(count_bits((bb & !bb.wrapping_sub(1)).wrapping_sub(1))),
//     }
// }

// pub fn set_bit(bb: &mut u64, sq: Square) {
//     *bb |= 1 << sq as u64;
// }

// pub fn clear_bit(bb: &mut u64, sq: Square) {
//     *bb &= !(1 << sq as u64);
// }

// pub fn clear_last_bit(bb: &mut u64) {
//     *bb &= *bb - 1;
// }

// pub fn shift_north_one(bb: u64) -> u64 {
//     bb << (Direction::North as u64)
// }

// pub fn shift_south_one(bb: u64) -> u64 {
//     bb >> (Direction::South as u64)
// }

// pub fn shift_east_one(bb: u64) -> u64 {
//     (bb << (Direction::East as u64)) & !File::A as u64
// }