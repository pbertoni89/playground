#!/bin/sh

outFn=/tmp/menghia.png
call1="cargo run ${outFn} 600x400 -2.5,1.0 1.0,-1.0"
call2="gwenview ${outFn}"

echo "${call1} && ${call2}"

${call1} && ${call2}
