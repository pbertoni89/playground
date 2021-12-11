
extern crate num;        // kinda include header / import package
extern crate image;
extern crate crossbeam;  // I can postpone this declaration but it's common sense

use num::Complex;        // kinda using something as / import something from module

use std::str::FromStr;   // trait for parse_pair definition

use image::ColorType;
use image::png::PNGEncoder;
use std::fs::File;

use std::io::Write;      // trait for writeln! macro


/// La mia prima funzioneeee!!! Forse inutile???
fn parse_single<T: FromStr>(s: &str) -> Option<T> {
    match T::from_str(&s) {
        Ok(v) => Some(v),
        _ => None
    }
}


///
/// "Type Parameter" ~~~ "Function Templates" in C++
///
fn parse_pair<T: FromStr>(s: &str, separator: char) -> Option<(T, T)> {

    // What we will find here? str::find return value, which is either None or Some(stuff) which we call index
    match s.find(separator) {
        None => None,
        Some(idx) => {
            match(T::from_str(&s[..idx]), T::from_str(&s[idx + 1 ..])) {
                (Ok(l), Ok(r)) => Some((l, r)),
                _ => None                             // kinda default: inside switches
            }
        }
    }
}

#[test]
fn test_parse_pair() {
    assert_eq!(parse_pair::<i32>("", ','), None);
    assert_eq!(parse_pair::<u64>("5@6", '@'), Some((5, 6)));
    assert_eq!(parse_pair::<u64>("4_7_8", '_'), None);
    assert_eq!(parse_pair::<u64>("4_7°8", '_'), None);
}


///
/// Too much `Some` around? Well they prevent a lot of other checks
///
fn parse_complex(s: &str) -> Option<Complex<f64>> {
    match parse_pair(s, ',') {
        Some((re, im)) => Some(Complex{ re, im }),  // choosing names equal to formal parameter enables this shorthand
        // Some((l, r)) => Some(Complex{ re: l, im: r }),  // choosing other names won't
        None => None
    }
}

#[test]
fn test_parse_complex() {
    assert_eq!(parse_complex("0.5,1.5"),
               Some(Complex{ re: 0.5, im: 1.5}));
}


///
/// Define our main iteration function
///
fn escape_time(c: Complex<f64>, limit: u32) -> Option<u32> {

    let mut z = Complex { re: 0.0, im:0.0};

    for i in 0..limit {
        z = z * z + c;
        if z.norm_sqr() > 4.0 {
            return Some(i)
        }
    }
    None
}

#[test]
fn test_escape_time() {
    assert_eq!(escape_time(Complex{ re: 0.1, im: 0.1}, 8), None);
    assert_eq!(escape_time(Complex{ re: 1.1, im: 0.1}, 8), Some(1));
    // {:?} helps when dealing with Option
    println!("Some eg 2 is {:?}", escape_time(Complex{ re: 1.1, im: 0.1}, 8));
}


///
/// bounds: image size
/// pixel: coordinates
/// tl: coordinates for top-left point
/// br: coordinates for bottom-right point
///
fn pixel_to_point(bounds: (usize, usize), pixel: (usize, usize), tl: Complex<f64>, br: Complex<f64>) -> Complex<f64> {

    let (w, h) = (br.re - tl.re, tl.im - br.im);

    // all parentheses here are not mandatory; castings have the highest priority !!!
    Complex{re: tl.re +  pixel.0 as f64  * (w / (bounds.0 as f64)),
        im: tl.im - (pixel.1 as f64) * (h / (bounds.1 as f64))}
}


#[test]
fn test_pixel_to_point() {
    let bn = (100, 100);
    let tl = Complex{re: -1.,  im: 1.0};
    let br = Complex{re: 1.0, im: -1.};
    assert_eq!(pixel_to_point(bn, (0, 0), tl, br),     Complex{re: -1.0, im:  1.0});
    assert_eq!(pixel_to_point(bn, (25, 75), tl, br),   Complex{re: -0.5, im: -0.5});
    assert_eq!(pixel_to_point(bn, (50, 50), tl, br),   Complex{re: -0.0, im: -0.0});
    assert_eq!(pixel_to_point(bn, (100, 100), tl, br), Complex{re:  1.0, im: -1.0});
}


///
/// Where `pixels` is a buffer (technically a slice)
///
fn render(pixels: &mut [u8], bounds: (usize, usize), tl: Complex<f64>, br: Complex<f64>) {

    // assert!(pixels.len() == bounds.0 * bounds.1);
    assert_eq!(pixels.len(), bounds.0 * bounds.1);

    for row in 0..bounds.1 {
        if row % 100 == 0 {
            println!("generating row {}", row)
        }
        for col in 0..bounds.0 {
            let pt = pixel_to_point(bounds, (col, row), tl, br);
            pixels[row * bounds.0 + col] = match escape_time(pt, 255) {
                None => 0,
                Some(nit) => 255 - nit as u8
                // Some(nit) => nit as u8
            }
        }
    }
}


///
/// Something new here: if some error occuers, we should not panick and return a Result which is not Ok(s), but Err(e)
/// Since success has no special value, it needs the void `()`
/// Instead, error will ship a `std::io::Error` (same as both File::create and PNGEncoder::new calls)
///
/// `?` is just syntactic sugar to avoid proliferation of match cond { Ok(s) => { s } Err(e) => { return Err(e) } } ...
///
/// One could write `Result<()>` instead of `Result<(), std::io::Error>` with a `use std::io::Result` declaration..
/// I'd avoid namespaces collisions for the moment
///
fn write_image(filename: &str, pixels: & [u8], bounds: (usize, usize))
               -> Result<(), std::io::Error>
{
    let output = File::create(filename)?;

    // if `new` is the default ctor, what about `create`? Kinda factory?
    let encoder = PNGEncoder::new(output);

    encoder.encode(&pixels, bounds.0 as u32, bounds.1 as u32, ColorType::Gray(8))?;
    Ok(())
}


///
/// Now the fun begins. First, a nonconcurrent version for simplicity
/// TODO move serial version here, respecting reference blah blah blah
///
fn monothread(pixels: &mut [u8], bounds: (usize, usize), tl: Complex<f64>, br: Complex<f64>) {
    render(pixels, bounds, tl, br);
}


fn multithread(pixels: &mut [u8], bounds: (usize, usize), tl: Complex<f64>, br: Complex<f64>, threads: usize) {
    let rows_per_band = bounds.1 / threads + 1;
    {
        let bands: Vec<&mut [u8]> = pixels.chunks_mut(rows_per_band * bounds.0).collect();
        crossbeam::scope(|spawner| {
            for (i, band) in bands.into_iter().enumerate() {
                let top = rows_per_band * i;
                // per errore avevo battuto `band` che è un'altra variabile, producendo
                // error[E0382]: borrow of moved value: `bands`
                let height = band.len() / bounds.0;
                let band_bounds = (bounds.0, height);
                let band_tl = pixel_to_point(bounds, (0, top), tl, br);
                let band_br = pixel_to_point(bounds, (bounds.0, top + height), tl, br);

                spawner.spawn(move |_| {
                    render(band, band_bounds, band_tl, band_br);
                });
            }
        }).unwrap();
    }
}


/// Here we are
fn main() {

    let args: Vec<String> = std::env::args().collect();  // without type annotation .len() will fail
    let _strict = false;
    if args.len() != 6 {
        // unwrap è la via "pulita" per non preoccuparsi se la chiamata precedente abbia fallito o no
        // se è tolta, avrai "warning: unused `Result` that must be used"
        // praticamente dichiari che non ti interessa di consultare il risultato...
        writeln!(std::io::stderr(), "usage:   mandelbrot FILE PIXELS TL BR THREADS").unwrap();
        writeln!(std::io::stderr(), "example: {} mandel.png 600x400 -1.20,0.35 -1,0.2 1", args[0]).unwrap();
        std::process::exit(1);
    }
    let filename = &args[1];  // otherwise a move occurs
    let bounds = parse_pair::<usize>(&args[2], 'x').expect("error parsing image dimensions");
    let tl = parse_complex(&args[3]).expect("error parsing top-left");
    let br = parse_complex(&args[4]).expect("error parsing bottom-right");
    let threads = parse_single::<usize>(&args[5]).expect("error parsing threads");
    let mut pixels = vec![0; bounds.0 * bounds.1];

    if threads > 1 {
        multithread(&mut pixels, bounds, tl, br, threads)
    }
    else {
        monothread(&mut pixels, bounds, tl, br);
    }

    write_image(filename, &pixels, bounds).expect("error writing image file")
}
