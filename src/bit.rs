/// Bitboard operations

/// Gets the bit at the given position
/// 
/// # Arguments
/// 
/// * `bb` - The bitboard
/// * `idx` - The index of the bit
#[inline]
pub fn get(bb: u64, idx: u8) -> bool {
    (bb >> idx as u64) & 1 == 1
}

/// Sets the bit at the given position
///
/// # Arguments
///
/// * `bb` - The bitboard
/// * `idx` - The index of the bit
#[inline]
pub fn set(bb: &mut u64, idx: u8) {
    *bb |= 1 << idx as u64;
}

/// Clears the bit at the given position
///
/// # Arguments

/// * `bb` - The bitboard
/// * `idx` - The index of the bit
#[inline]
pub fn clear(bb: &mut u64, idx: u8) {
    *bb &= !(1 << idx as u64);
}

/// Clears the least significant bit
///
/// # Arguments
///
/// * `bb` - The bitboard
#[inline]
pub fn clear_last(bb: &mut u64) {
    *bb &= *bb - 1;
}

/// Counts the number of bits set to 1
///
/// # Arguments
///
/// * `bb` - The bitboard
#[inline]
pub fn count(mut bb: u64) -> u8 {
    let mut count = 0;
    while bb != 0 {
        count += 1;
        bb &= bb - 1;
    }
    count
}

/// Gets the least significant bit index
///
/// # Arguments
///
/// * `bb` - The bitboard
#[inline]
pub fn scan(bb: u64) -> u8 {
    match bb == 0 {
        true => 0,
        false => count((bb & !bb.wrapping_sub(1)).wrapping_sub(1)),
    }
}
