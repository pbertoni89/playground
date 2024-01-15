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

///
pub fn fact(n: u64) -> u64 {
    match n {
		0 => 1,
		1 => 1,
		_ => n * fact(n - 1)
	}
}

///
pub fn fibo(n: u64) -> u64 {

	/*
	let rv = if n == 0 {
        	0
	} else if n == 1 {
		1
	} else { fib(n - 1) + fib(n - 2) };
	*/

	match n {
		0 => 0,
		1 => 1,
		_ => fibo(n - 1) + fibo(n - 2)
	}

	// this breaks the whole stuff... let b = 21;
}


/// This doc, instead, hides a line of code sample
///
///     # use code::not::tested::Here;
///     apply_sunlight(4);
///
pub fn apply_sunlight(time: u32) {
    println!("applying sunlight {}", time)
}


#[cfg(test)]
mod tests {
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    #[test]
    fn test_fibo() {
        assert_eq!(fibo(5), 5);
    }

    #[test]
    fn test_fact() {
        assert_eq!(fact(5), 120);
    }
}
