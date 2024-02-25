use crate::{CastlePerm, Color, Piece, Square};

use strum::EnumCount;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Board {
    /// Whether it's white's turn to move.
    w: bool,

    /// The number of half moves since the last capture or pawn move.
    hmc: u8,

    /// The number of full moves.
    fmc: u8,

    /// The castling permissions.
    cas: u8,

    /// The en passant target square.
    enp: Option<Square>,

    /// The occupancy bitboards.
    occ: [u64; Color::COUNT],

    /// The piece bitboards.
    pie: [u64; Piece::COUNT],

    /// The piece-square table.
    sqr: [Option<Piece>; Square::COUNT],
}

impl Board {
    pub fn from_fen(fen: &str) -> Option<Self> {
        let fields: Vec<&str> = fen.split(' ').collect();
        match fields.len() {
            6 => {
                let mut b = Board {
                    cas: 0,
                    w: fields[1] == "w",
                    occ: [0; Color::COUNT],
                    pie: [0; Piece::COUNT],
                    sqr: [None; Square::COUNT],
                    hmc: fields[4].parse().ok()?,
                    fmc: fields[5].parse().ok()?,
                    enp: match fields[3] {
                        "-" => None,
                        s => Some(Square::from_str(s)?),
                    },
                };
                let mut rank: u8 = 7;
                let mut file: u8 = 0;
                for c in fields[0].chars() {
                    match c {
                        '1'..='8' => file += c as u8 - b'0',
                        'p' => {
                            b.occ[Color::Black as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::BlackPawn as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::BlackPawn);
                            file += 1;
                        }
                        'n' => {
                            b.occ[Color::Black as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::BlackKnight as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::BlackKnight);
                            file += 1;
                        }
                        'b' => {
                            b.occ[Color::Black as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::BlackBishop as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::BlackBishop);
                            file += 1;
                        }
                        'r' => {
                            b.occ[Color::Black as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::BlackRook as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::BlackRook);
                            file += 1;
                        }
                        'q' => {
                            b.occ[Color::Black as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::BlackQueen as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::BlackQueen);
                            file += 1;
                        }
                        'k' => {
                            b.occ[Color::Black as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::BlackKing as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::BlackKing);
                            file += 1;
                        }
                        'P' => {
                            b.occ[Color::White as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::WhitePawn as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::WhitePawn);
                            file += 1;
                        }
                        'N' => {
                            b.occ[Color::White as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::WhiteKnight as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::WhiteKnight);
                            file += 1;
                        }
                        'B' => {
                            b.occ[Color::White as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::WhiteBishop as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::WhiteBishop);
                            file += 1;
                        }
                        'R' => {
                            b.occ[Color::White as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::WhiteRook as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::WhiteRook);
                            file += 1;
                        }
                        'Q' => {
                            b.occ[Color::White as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::WhiteQueen as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::WhiteQueen);
                            file += 1;
                        }
                        'K' => {
                            b.occ[Color::White as usize] |= 1 << (rank * 8 + file);
                            b.pie[Piece::WhiteKing as usize] |= 1 << (rank * 8 + file);
                            b.sqr[(rank * 8 + file) as usize] = Some(Piece::WhiteKing);
                            file += 1;
                        }
                        '/' => {
                            rank -= 1;
                            file = 0;
                        }
                        _ => return None,
                    }
                }
                for c in fields[2].chars() {
                    match c {
                        'K' => b.cas |= CastlePerm::WhiteKing as u8,
                        'Q' => b.cas |= CastlePerm::WhiteQueen as u8,
                        'k' => b.cas |= CastlePerm::BlackKing as u8,
                        'q' => b.cas |= CastlePerm::BlackQueen as u8,
                        '-' => (),
                        _ => return None,
                    }
                }
                b.occ[Color::Both as usize] =
                    b.occ[Color::White as usize] | b.occ[Color::Black as usize];
                Some(b)
            }
            _ => None,
        }
    }

    pub fn to_fen(&self) -> String {
        let mut fen = String::new();
        for rank in (0..8).rev() {
            let mut empty = 0;
            for file in 0..8 {
                let sq = rank * 8 + file;
                match self.sqr[sq] {
                    None => empty += 1,
                    Some(p) => match empty {
                        0 => fen.push(p.to_char()),
                        _ => {
                            fen.push((empty + b'0') as char);
                            fen.push(p.to_char());
                            empty = 0;
                        }
                    },
                }
            }
            if empty > 0 {
                fen.push((empty + b'0') as char);
            }
            if rank > 0 {
                fen.push('/');
            }
        }
        fen.push(' ');
        fen.push(match self.w {
            true => 'w',
            false => 'b',
        });
        fen.push(' ');
        match self.cas {
            0 => fen.push('-'),
            c => {
                if c & CastlePerm::WhiteKing as u8 != 0 {
                    fen.push('K');
                }
                if c & CastlePerm::WhiteQueen as u8 != 0 {
                    fen.push('Q');
                }
                if c & CastlePerm::BlackKing as u8 != 0 {
                    fen.push('k');
                }
                if c & CastlePerm::BlackQueen as u8 != 0 {
                    fen.push('q');
                }
            }
        }
        fen.push(' ');
        match self.enp {
            None => fen.push('-'),
            Some(sq) => fen.push_str(&sq.to_str()),
        }
        fen.push(' ');
        fen.push_str(&self.hmc.to_string());
        fen.push(' ');
        fen.push_str(&self.fmc.to_string());
        fen
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn from_fen() {
        let b =
            Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").unwrap();
        assert_eq!(b.w, true);
        assert_eq!(b.hmc, 0);
        assert_eq!(b.fmc, 1);
        assert_eq!(b.enp, None);
        assert_eq!(b.cas, CastlePerm::All as u8);
        assert_eq!(b.occ[Color::White as usize], 0x000000000000FFFF);
        assert_eq!(b.occ[Color::Black as usize], 0xFFFF000000000000);
        assert_eq!(b.occ[Color::Both as usize], 0xFFFF00000000FFFF);
        assert_eq!(b.pie[Piece::WhitePawn as usize], 0x000000000000FF00);
        assert_eq!(b.pie[Piece::WhiteKnight as usize], 0x0000000000000042);
        assert_eq!(b.pie[Piece::WhiteBishop as usize], 0x0000000000000024);
        assert_eq!(b.pie[Piece::WhiteRook as usize], 0x0000000000000081);
        assert_eq!(b.pie[Piece::WhiteQueen as usize], 0x0000000000000008);
        assert_eq!(b.pie[Piece::WhiteKing as usize], 0x0000000000000010);
        assert_eq!(b.pie[Piece::BlackPawn as usize], 0x00FF000000000000);
        assert_eq!(b.pie[Piece::BlackKnight as usize], 0x4200000000000000);
        assert_eq!(b.pie[Piece::BlackBishop as usize], 0x2400000000000000);
        assert_eq!(b.pie[Piece::BlackRook as usize], 0x8100000000000000);
        assert_eq!(b.pie[Piece::BlackQueen as usize], 0x0800000000000000);
        assert_eq!(b.pie[Piece::BlackKing as usize], 0x1000000000000000);
        assert_eq!(b.sqr[Square::A1 as usize], Some(Piece::WhiteRook));
        assert_eq!(b.sqr[Square::B1 as usize], Some(Piece::WhiteKnight));
        assert_eq!(b.sqr[Square::C1 as usize], Some(Piece::WhiteBishop));
        assert_eq!(b.sqr[Square::D1 as usize], Some(Piece::WhiteQueen));
        assert_eq!(b.sqr[Square::E1 as usize], Some(Piece::WhiteKing));
        assert_eq!(b.sqr[Square::F1 as usize], Some(Piece::WhiteBishop));
        assert_eq!(b.sqr[Square::G1 as usize], Some(Piece::WhiteKnight));
        assert_eq!(b.sqr[Square::H1 as usize], Some(Piece::WhiteRook));
        assert_eq!(b.sqr[Square::A2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::B2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::C2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::D2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::E2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::F2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::G2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::H2 as usize], Some(Piece::WhitePawn));
        assert_eq!(b.sqr[Square::A3 as usize], None);
        assert_eq!(b.sqr[Square::B3 as usize], None);
        assert_eq!(b.sqr[Square::C3 as usize], None);
        assert_eq!(b.sqr[Square::D3 as usize], None);
        assert_eq!(b.sqr[Square::E3 as usize], None);
        assert_eq!(b.sqr[Square::F3 as usize], None);
        assert_eq!(b.sqr[Square::G3 as usize], None);
        assert_eq!(b.sqr[Square::H3 as usize], None);
        assert_eq!(b.sqr[Square::A4 as usize], None);
        assert_eq!(b.sqr[Square::B4 as usize], None);
        assert_eq!(b.sqr[Square::C4 as usize], None);
        assert_eq!(b.sqr[Square::D4 as usize], None);
        assert_eq!(b.sqr[Square::E4 as usize], None);
        assert_eq!(b.sqr[Square::F4 as usize], None);
        assert_eq!(b.sqr[Square::G4 as usize], None);
        assert_eq!(b.sqr[Square::H4 as usize], None);
        assert_eq!(b.sqr[Square::A5 as usize], None);
        assert_eq!(b.sqr[Square::B5 as usize], None);
        assert_eq!(b.sqr[Square::C5 as usize], None);
        assert_eq!(b.sqr[Square::D5 as usize], None);
        assert_eq!(b.sqr[Square::E5 as usize], None);
        assert_eq!(b.sqr[Square::F5 as usize], None);
        assert_eq!(b.sqr[Square::G5 as usize], None);
        assert_eq!(b.sqr[Square::H5 as usize], None);
        assert_eq!(b.sqr[Square::A6 as usize], None);
        assert_eq!(b.sqr[Square::B6 as usize], None);
        assert_eq!(b.sqr[Square::C6 as usize], None);
        assert_eq!(b.sqr[Square::D6 as usize], None);
        assert_eq!(b.sqr[Square::E6 as usize], None);
        assert_eq!(b.sqr[Square::F6 as usize], None);
        assert_eq!(b.sqr[Square::G6 as usize], None);
        assert_eq!(b.sqr[Square::H6 as usize], None);
        assert_eq!(b.sqr[Square::A7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::B7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::C7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::D7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::E7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::F7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::G7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::H7 as usize], Some(Piece::BlackPawn));
        assert_eq!(b.sqr[Square::A8 as usize], Some(Piece::BlackRook));
        assert_eq!(b.sqr[Square::B8 as usize], Some(Piece::BlackKnight));
        assert_eq!(b.sqr[Square::C8 as usize], Some(Piece::BlackBishop));
        assert_eq!(b.sqr[Square::D8 as usize], Some(Piece::BlackQueen));
        assert_eq!(b.sqr[Square::E8 as usize], Some(Piece::BlackKing));
        assert_eq!(b.sqr[Square::F8 as usize], Some(Piece::BlackBishop));
        assert_eq!(b.sqr[Square::G8 as usize], Some(Piece::BlackKnight));
        assert_eq!(b.sqr[Square::H8 as usize], Some(Piece::BlackRook));
    }

    #[test]
    fn to_fen() {
        let b =
            Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1").unwrap();
        assert_eq!(
            b.to_fen(),
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        );
    }
}
