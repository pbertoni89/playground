/*
    acts like #include, or import
    This also does the equivalent of calling `use rand`,
    so now we can call anything in the rand crate by placing rand:: before it.
*/
extern crate rand;

use std::io;

/*
    The Rng trait defines methods that random number generators implement,
    and this trait must be in scope for us to use those methods.
*/
use rand::Rng;

use std::cmp::Ordering;

/* Return value of the function is synonymous with the value of the final expression in the
   block of the body of a function. You can return early from a function by using the return
   keyword and specifying a value, but most functions return the last expression implicitly.
 */
fn five() -> i32 {
    5
}


fn main() {
    println!("Guess the number!");

    loop {
        let secr = rand::thread_rng().gen_range(1, 101);

        println!("Please input your guess.");
        let mut guess = String::new();

        io::stdin().read_line(&mut guess)
            .expect("Failed to read line");

        /* Shadow `guess` and provide type hint
           guess.trim().parse().expect("provide number")
           Switching from an expect call to a match expression is how you generally move
           from crashing on an error to handling the error
        */
        let guess: i32 = match guess.trim().parse() {
            Ok(num) => num,
            /* Expressions do not include ending semicolons.
               If you add a semicolon to the end of an expression, you turn it into a statement,
               which will then not return a value.
            */
            Err(_) => continue
        };

        println!("You guessed: {}; secret number is {}", guess, secr);

        // cmp() returns an Ordering enum
        match guess.cmp(&secr) {
            Ordering::Less => println!("too small"),
            Ordering::Equal => {
                println!("correct");
                break;
            },
            Ordering::Greater => println!("too big")
        }
    }
}

