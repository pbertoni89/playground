/// FIXME What is this crate supposed to do?

extern crate pbrlib;

use std::thread;
use std::net::{TcpListener, TcpStream, Shutdown};
use std::io::{Read, Write};


///
///
fn serve_request(msg: String) -> String {

    let itparts = msg.split(":");
    let parts = itparts.collect::<Vec<&str>>();

    //println!("having {} parts", parts.len());
    //for part in &parts {            // if not borrowing, this is a consumption !
    //    println!("part {}", part);
    //}

    let req_fun = parts[0];
    let req_arg = parts[1].parse::<u64>();
    let req_val = match req_arg {
        Ok(v) => v,
        Err(e) => panic!(e)
    };
    println!("asking {}({})", req_fun, req_val);

    let resp_val = match req_fun {
        "fibo" => pbrlib::fibo(req_val),
        "fact" => pbrlib::fact(req_val),
        _ => 0
    };

    println!("computed {}", resp_val);
    let resp = format!("{}:{}", req_fun, resp_val);
    resp
}


fn handle_client(mut stream: TcpStream) {

    let mut data = [0 as u8; 50]; // using 50 byte buffer

    while match stream.read(&mut data) {
        Ok(0) => {
            // no data received. better sleep here... seems already handled somewhere else (necessarily inside read())
            true
        },
        Ok(size) => {
            // println!("read {} bytes", size);
            let msg = String::from_utf8(data[0..size].to_vec()).unwrap();
            // great but now has `data` been consumed ? It seems not
            // why that unwrap? because from_utf8 returns a `std::result::Result`

            let resp = serve_request(msg);

            // stream.write(&data[0..size]).unwrap();
            // // let echo = msg.as_bytes();
            // // stream.write(msg.as_bytes()).unwrap();
            stream.write(resp.as_bytes()).unwrap();
            true
        },
        Err(_) => {
            println!("An error occurred, terminating connection with {}", stream.peer_addr().unwrap());
            stream.shutdown(Shutdown::Both).unwrap();
            false
        }
    } {}  // this is the body of while cycle
}


/// accept connections and process them, spawning a new thread for each one
fn main() {

    let listener = TcpListener::bind("0.0.0.0:3333").unwrap();
    println!("Server listening on port 3333");

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                thread::spawn(move|| {
                    let peer_addr = stream.peer_addr().unwrap();
                    println!("New connection: {}", peer_addr);
                    handle_client(stream);
                    println!("Closed connection: {}", peer_addr);
                });
            }
            Err(e) => {
                println!("Error: {}", e);  // conn failed
            }
        }
    }

    drop(listener);  // close the socket server
}
