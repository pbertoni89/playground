#!/bin/sh

name=custom-bt

g++ ${name}.cpp -rdynamic -ggdb -ldl -o ${name}

ulimit -c unlimited

./${name}

echo "program "${name}" returned with "${?}

~/xspectra-dev/xspyctra/xspyctrad.py -t ${name}

