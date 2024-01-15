extern crate iron;
#[macro_use]
extern crate mime;

use iron::prelude::*;
use iron::status;

fn main() {
    println!("Hello, world!");
    Iron::new(get_form).http("localhost:3000").unwrap();
}

fn get_form(_request: &mut Request) -> IronResult<Response> {
    let mut response = Response::new();
    response.set_mut(status::Ok);
}
