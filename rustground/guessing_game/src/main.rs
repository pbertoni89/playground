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
fn demo_five() -> i32 {
    5
}


fn demo_guess() -> () {
    println!("Guess the number!");

    loop {
        let secr = rand::thread_rng().gen_range(1, 101);

        println!("Please input your guess. Any negative number will stop");
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

        if guess < 0 {
            println!("bye bye");
            break
        }
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


/// TODO richiedi B = num_bytes
/// calcola la serie 2^i con 0<=i<=B e registra per ogni i
/// quanti caratteri ti servono per rappresentare l'elemento
/// in base 10, in base 16, ed il loro rapporto
/// alla fine sarebbe carino disegnare il plot
#[warn(dead_code)]
fn demo_hexa(n_bytes: u32) -> () {
    println!("\nper favore implementami! {} bytes", n_bytes)
}


/// TODO richiedi P = num_players, G = num_games da tastiera
/// World of Tank rating system
/// Ogni partita è giocata da P (pari) giocatori. Vengono giocate G partite in totale da questi.
/// In ogni partita vengono generati due schieramenti casuali di pari dimensioni.
/// La vittoria è assegnata casualmente a uno di questi. // TODO oppure si va a pareggio
/// Ogni giocatore dunque incrementa il conteggio delle sue vittorie
/// Alla fine si stampa la media delle partite vinte da ciascun giocatore
/// calcola media aritmetica della percentuale partite vinte
fn demo_wot(n_players: u32, n_games: u32) -> f64 {
    println!("\nper favore implementami! {} players, {} games", n_players, n_games);
    let n_teams: u32 = 2;

    if n_players % n_teams == 1 {
        println!("odd number of players");  // FIXME bad error checking, use match
        -1.2
    }
    else {
        let players_per_team = n_players / n_teams;
        let mut vu_wins: Vec<u32> = vec![0; n_players as usize];

        for ig in 0 .. n_games {
            //let mut teams = Vec<Vec<u32>>();
            for it in 0 .. n_teams {
                println!("game {}, team {}", ig, it);
                let mut idx_p_team: Vec<u32> = vec![0; players_per_team as usize];
                for it in 0 .. players_per_team {
                    let ip = rand::thread_rng().gen_range(0, n_players);
                    println!("game {}", ig);
                }
                // teams[it] = idx_p_team;
            }
        }
        3.14
    }
}


fn main() {
    demo_five();
    demo_wot(8, 10);
    // demo_guess()
}

