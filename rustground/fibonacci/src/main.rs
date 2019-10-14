fn fib(n: u32) -> u32 {

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
		_ => fib(n - 1) + fib(n - 2)
	}

	// this breaks the whole stuff... let b = 21;
}


fn main() {

	let mut n = String::new();
}
