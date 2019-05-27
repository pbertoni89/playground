use std::thread;

fn demo_playground() {
    let condition = true;
    let number = if condition {
        5
    } else {
        6 // "six" -> error!!! static check fails
    };
    println!("number is {}", number)
}

/**
 * https://github.com/nrc/r4cppp/blob/master/control%20flow.md
 */
fn demo_match(x: i32) {
    let msg = match x {
        0 | 1 | 10 => "one of zero, one, or ten",
        y if y < 20 => "less than 20, but not zero, one, or ten",
        y if y == 200 => "200 (but this is not very stylish)",
        _ => "something else",
    };
    println!("x is {}", msg);
}

/**
 * An example compiler that turns 0s into 1s, and 1s into 0s.
 */
fn demo_compiler() {
    let input = "1 0 1 A 1 0 1 3";

    // iterate over every character `c` in input
    let output: String = input
        .chars()
        .map(
            |c| {
                if c == '1' {
                    '0'
                } else if c == '0' {
                    '1'
                } else {
                    c
                }
            }, // if not 0 or 1, leave it alone
        )
        .collect();

    println!("I: {}", input); // 1 0 1 A 1 0 1 3
    println!("O: {}", output); // 0 1 0 A 0 1 0 3
}

fn demo_threads() {
    let handles: Vec<_> = (0..10)
        .map(|_| {
            thread::spawn(|| {
                let mut x = 0;
                let my_range = 0..5_000_000;
                for _ in my_range {
                    x += 1
                }
                x
            })
        })
        .collect();

    for h in handles {
        println!(
            "Thread finished with count={}",
            h.join().map_err(|_| "Could not join a thread!").unwrap()
        );
    }
    println!("done!\n");
}

fn main() {
    println!("Hello, world!");

    demo_compiler();
    demo_playground();
    demo_threads();
    demo_match(19);
}
