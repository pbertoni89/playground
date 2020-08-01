use std::ops::Range;

/// Return true iff two ranges overlap.
///
///     assert_eq!(ranges::overlap(0..7, 3..10, true);
///
/// If either range is empty, they don't count
///
pub fn overlap(r1: Range<usize>, r2: Range<usize>) -> bool {
    r1.start < r1.end && r2.start < r2.end &&
      r1.start < r2.end && r2.start < r1.end
}


/// This doc, instead, hides a line of code sample
///
///     # use code::not::tested::Here;
///     apply_sunlight(4);
///
pub fn apply_sunlight(time: u32) {
    println!("applying sunlight {}", time)
}
