#!/bin/sh

call1="cargo run menghia.png 600x400 -2.5,1.0 1.0,-1.0"
call2="gwenview menghia.png"

echo "$call1 && $call2"
$call1 && $call2

