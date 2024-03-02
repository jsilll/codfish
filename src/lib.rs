use strum_macros::EnumCount;

/// Represents the possible castling permissions.
#[repr(u8)]
#[derive(Debug, Clone, Copy)]
pub enum CastlePerm {
    None = 0,
    WhiteKing = 1,
    WhiteQueen = 2,
    BlackKing = 4,
    BlackQueen = 8,
    All = 15,
}

/// Represents the possible colors.
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, EnumCount)]
pub enum Color {
    White = 0,
    Black = 1,
    Both = 2,
}

/// Represents the possible pieces, including an piece.
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, EnumCount)]
pub enum Piece {
    None = 0,
    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6,
}

impl TryFrom<u8> for Piece {
    type Error = ();

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(Piece::None),
            1 => Ok(Piece::Pawn),
            2 => Ok(Piece::Knight),
            3 => Ok(Piece::Bishop),
            4 => Ok(Piece::Rook),
            5 => Ok(Piece::Queen),
            6 => Ok(Piece::King),
            _ => Err(()),
        }
    }
}

/// Represents the possible colored pieces.
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, EnumCount)]
pub enum ColoredPiece {
    WhitePawn = 0,
    WhiteKnight = 1,
    WhiteBishop = 2,
    WhiteRook = 3,
    WhiteQueen = 4,
    WhiteKing = 5,
    BlackPawn = 6,
    BlackKnight = 7,
    BlackBishop = 8,
    BlackRook = 9,
    BlackQueen = 10,
    BlackKing = 11,
}

const PIECE_TO_CHAR: [char; 12] = ['P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'];

impl ColoredPiece {
    pub fn to_char(&self) -> char {
        PIECE_TO_CHAR[*self as usize]
    }
}

/// Represents the possible files.
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, EnumCount)]
pub enum File {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    F = 5,
    G = 6,
    H = 7,
}

/// Represents the possible ranks.
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, EnumCount)]
pub enum Rank {
    R1 = 0,
    R2 = 1,
    R3 = 2,
    R4 = 3,
    R5 = 4,
    R6 = 5,
    R7 = 6,
    R8 = 7,
}

/// Represents the possible squares.
#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, EnumCount)]
pub enum Square {
    A1 = 0,
    B1 = 1,
    C1 = 2,
    D1 = 3,
    E1 = 4,
    F1 = 5,
    G1 = 6,
    H1 = 7,
    A2 = 8,
    B2 = 9,
    C2 = 10,
    D2 = 11,
    E2 = 12,
    F2 = 13,
    G2 = 14,
    H2 = 15,
    A3 = 16,
    B3 = 17,
    C3 = 18,
    D3 = 19,
    E3 = 20,
    F3 = 21,
    G3 = 22,
    H3 = 23,
    A4 = 24,
    B4 = 25,
    C4 = 26,
    D4 = 27,
    E4 = 28,
    F4 = 29,
    G4 = 30,
    H4 = 31,
    A5 = 32,
    B5 = 33,
    C5 = 34,
    D5 = 35,
    E5 = 36,
    F5 = 37,
    G5 = 38,
    H5 = 39,
    A6 = 40,
    B6 = 41,
    C6 = 42,
    D6 = 43,
    E6 = 44,
    F6 = 45,
    G6 = 46,
    H6 = 47,
    A7 = 48,
    B7 = 49,
    C7 = 50,
    D7 = 51,
    E7 = 52,
    F7 = 53,
    G7 = 54,
    H7 = 55,
    A8 = 56,
    B8 = 57,
    C8 = 58,
    D8 = 59,
    E8 = 60,
    F8 = 61,
    G8 = 62,
    H8 = 63,
}

const SQUARE_FROM_U8: [Square; 64] = [
    Square::A1,
    Square::B1,
    Square::C1,
    Square::D1,
    Square::E1,
    Square::F1,
    Square::G1,
    Square::H1,
    Square::A2,
    Square::B2,
    Square::C2,
    Square::D2,
    Square::E2,
    Square::F2,
    Square::G2,
    Square::H2,
    Square::A3,
    Square::B3,
    Square::C3,
    Square::D3,
    Square::E3,
    Square::F3,
    Square::G3,
    Square::H3,
    Square::A4,
    Square::B4,
    Square::C4,
    Square::D4,
    Square::E4,
    Square::F4,
    Square::G4,
    Square::H4,
    Square::A5,
    Square::B5,
    Square::C5,
    Square::D5,
    Square::E5,
    Square::F5,
    Square::G5,
    Square::H5,
    Square::A6,
    Square::B6,
    Square::C6,
    Square::D6,
    Square::E6,
    Square::F6,
    Square::G6,
    Square::H6,
    Square::A7,
    Square::B7,
    Square::C7,
    Square::D7,
    Square::E7,
    Square::F7,
    Square::G7,
    Square::H7,
    Square::A8,
    Square::B8,
    Square::C8,
    Square::D8,
    Square::E8,
    Square::F8,
    Square::G8,
    Square::H8,
];

impl TryFrom<u8> for Square {
    type Error = ();

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        if value < 64 {
            Ok(SQUARE_FROM_U8[value as usize])
        } else {
            Err(())
        }
    }
}

const SQUARE_FROM_STR: phf::Map<&str, Square> = phf::phf_map! {
    "a1" => Square::A1,
    "b1" => Square::B1,
    "c1" => Square::C1,
    "d1" => Square::D1,
    "e1" => Square::E1,
    "f1" => Square::F1,
    "g1" => Square::G1,
    "h1" => Square::H1,
    "a2" => Square::A2,
    "b2" => Square::B2,
    "c2" => Square::C2,
    "d2" => Square::D2,
    "e2" => Square::E2,
    "f2" => Square::F2,
    "g2" => Square::G2,
    "h2" => Square::H2,
    "a3" => Square::A3,
    "b3" => Square::B3,
    "c3" => Square::C3,
    "d3" => Square::D3,
    "e3" => Square::E3,
    "f3" => Square::F3,
    "g3" => Square::G3,
    "h3" => Square::H3,
    "a4" => Square::A4,
    "b4" => Square::B4,
    "c4" => Square::C4,
    "d4" => Square::D4,
    "e4" => Square::E4,
    "f4" => Square::F4,
    "g4" => Square::G4,
    "h4" => Square::H4,
    "a5" => Square::A5,
    "b5" => Square::B5,
    "c5" => Square::C5,
    "d5" => Square::D5,
    "e5" => Square::E5,
    "f5" => Square::F5,
    "g5" => Square::G5,
    "h5" => Square::H5,
    "a6" => Square::A6,
    "b6" => Square::B6,
    "c6" => Square::C6,
    "d6" => Square::D6,
    "e6" => Square::E6,
    "f6" => Square::F6,
    "g6" => Square::G6,
    "h6" => Square::H6,
    "a7" => Square::A7,
    "b7" => Square::B7,
    "c7" => Square::C7,
    "d7" => Square::D7,
    "e7" => Square::E7,
    "f7" => Square::F7,
    "g7" => Square::G7,
    "h7" => Square::H7,
    "a8" => Square::A8,
    "b8" => Square::B8,
    "c8" => Square::C8,
    "d8" => Square::D8,
    "e8" => Square::E8,
    "f8" => Square::F8,
    "g8" => Square::G8,
    "h8" => Square::H8,
};

const SQUARE_TO_STR: [&str; 64] = [
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
];

impl Square {
    pub fn from_str(s: &str) -> Option<Self> {
        SQUARE_FROM_STR.get(s).copied() 
    }

    pub fn to_str(&self) -> &'static str {
        SQUARE_TO_STR[*self as usize]
    }
}

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

pub mod bit;

pub mod mov;

pub mod board;
