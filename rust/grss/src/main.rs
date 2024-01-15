/*
 * https://rust-lang-nursery.github.io/cli-wg/tutorial/impl-draft.html
 */

use structopt::StructOpt;

#[derive(StructOpt)]
struct Cli {
    // hevaycuerdo
    pattern: String,

    #[structopt(parse(from_os_str))]
    path: std::path::PathBuf,
}

fn my_box() -> Result<(), Box<dyn std::error::Error>> {
    let content = std::fs::read_to_string("test.txt")?;
    println!("file content: {}", content);
    Ok(())
}

fn main() {
    let args = Cli::from_args();  // where does from_args come from? StructOpt I guess
    println!("Pattern: {}", &args.pattern);

    // TODO adopt a BufReader
    // let content = std::fs::read_to_string(&args.path).expect("no file!");
    // no default formatter... println!("Path: {}", &args.path);

    let result = std::fs::read_to_string(&args.path);
    match result {
        Ok(content) => { println!("File content: {}", content.len()); }
        Err(myerr) => { println!("Oh noes: {}", myerr); }
    };

    // previous "result" has been consumed somehow; to twice the demo, let'reinitialize
    let result = std::fs::read_to_string(&args.path);
    let content = match result {
        Ok(content) => { content }  // return last instruction..
        Err(myerr) => { panic!("can't deal with {}", myerr); }
    };

    // or, even simpler, terminate program if some error occurs
    let content = std::fs::read_to_string(&args.path).unwrap();
    // wow! mega concise. But what the fuck fixme
    // let content = std::fs::read_to_string(&args.path)?;

    println!("\n-----------------------------------------\n");
    for line in content.lines() {
        if line.contains(&args.pattern) {
            println!("{}", line)
        }
    }

    // it seems the initial ampersand is required... why bleah
}
