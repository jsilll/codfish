use crate::{Piece, Square};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Move(u32);

impl Move {
    #[inline]
    pub fn new(
        from: Square,
        to: Square,
        piece: Piece,
        captured: Piece,
        promoted: Piece,
        is_double_push: bool,
        is_en_passant: bool,
        is_castle: bool,
    ) -> Move {
        let mut mov = 0;
        mov |= from as u32;
        mov |= (to as u32) << 6;
        mov |= (piece as u32) << 12;
        mov |= (captured as u32) << 15;
        mov |= (promoted as u32) << 18;
        mov |= (is_double_push as u32) << 21;
        mov |= (is_en_passant as u32) << 22;
        mov |= (is_castle as u32) << 23;
        Move(mov)
    }

    #[inline]
    pub fn from(&self) -> Square {
        Square::try_from((self.0 & 0b111111) as u8).unwrap()
    }

    #[inline]
    pub fn to(&self) -> Square {
        Square::try_from(((self.0 >> 6) & 0b111111) as u8).unwrap()
    }

    #[inline]
    pub fn piece(&self) -> Piece {
        Piece::try_from(((self.0 >> 12) & 0b111) as u8).unwrap()
    }

    #[inline]
    pub fn captured(&self) -> Piece {
        Piece::try_from(((self.0 >> 15) & 0b111) as u8).unwrap()
    }

    #[inline]
    pub fn promoted(&self) -> Piece {
        Piece::try_from(((self.0 >> 18) & 0b111) as u8).unwrap()
    }

    #[inline]
    pub fn is_double_push(&self) -> bool {
        ((self.0 >> 21) & 0b1) == 1
    }

    #[inline]
    pub fn is_en_passant(&self) -> bool {
        ((self.0 >> 22) & 0b1) == 1
    }

    #[inline]
    pub fn is_castle(&self) -> bool {
        ((self.0 >> 23) & 0b1) == 1
    }

    #[inline]
    pub fn is_capture(&self) -> bool {
        self.captured() != Piece::None
    }

    #[inline]
    pub fn is_promotion(&self) -> bool {
        self.promoted() != Piece::None
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_move() {
        let mov = Move::new(
            Square::A2,
            Square::A4,
            Piece::Pawn,
            Piece::None,
            Piece::None,
            false,
            false,
            false,
        );
        assert_eq!(mov.from(), Square::A2);
        assert_eq!(mov.to(), Square::A4);
        assert_eq!(mov.piece(), Piece::Pawn);
        assert_eq!(mov.captured(), Piece::None);
        assert_eq!(mov.promoted(), Piece::None);
        assert_eq!(mov.is_double_push(), false);
        assert_eq!(mov.is_en_passant(), false);
        assert_eq!(mov.is_castle(), false);
        assert_eq!(mov.is_capture(), false);
        assert_eq!(mov.is_promotion(), false);
    }

    #[test]
    fn test_move_capture() {
        let mov = Move::new(
            Square::A2,
            Square::A4,
            Piece::Pawn,
            Piece::None,
            Piece::None,
            false,
            false,
            false,
        );
        assert_eq!(mov.is_capture(), false);

        let mov = Move::new(
            Square::A2,
            Square::A4,
            Piece::Pawn,
            Piece::Pawn,
            Piece::None,
            false,
            false,
            false,
        );
        assert_eq!(mov.is_capture(), true);
    }

    #[test]
    fn test_move_promotion() {
        let mov = Move::new(
            Square::A7,
            Square::A8,
            Piece::Pawn,
            Piece::None,
            Piece::Queen,
            false,
            false,
            false,
        );
        assert_eq!(mov.is_promotion(), true);
    }

    #[test]
    fn test_move_double_push() {
        let mov = Move::new(
            Square::A2,
            Square::A4,
            Piece::Pawn,
            Piece::None,
            Piece::None,
            true,
            false,
            false,
        );
        assert_eq!(mov.is_double_push(), true);
    }

    #[test]
    fn test_move_en_passant() {
        let mov = Move::new(
            Square::A5,
            Square::B6,
            Piece::Pawn,
            Piece::Pawn,
            Piece::None,
            false,
            true,
            false,
        );
        assert_eq!(mov.is_en_passant(), true);
    }

    #[test]
    fn test_move_castle() {
        let mov = Move::new(
            Square::E1,
            Square::G1,
            Piece::King,
            Piece::None,
            Piece::None,
            false,
            false,
            true,
        );
        assert_eq!(mov.is_castle(), true);
    }
}
