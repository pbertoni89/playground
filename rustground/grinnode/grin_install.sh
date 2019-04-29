#!/bin/bash

# Follow https://github.com/mimblewimble/docs/wiki/how-to-run-a-grin-node

curl https://sh.rustup.rs -sSf | sh
source $HOME/.cargo/env
rustup update

cd ~
curl -L https://github.com/mimblewimble/grin/releases/download/v1.0.3/grin-v1.0.3-514864287-linux-amd64.tgz | tar xz
mkdir -p ~/grin.d ; mv grin grin.d/
echo export 'PATH=~/grin.d:$PATH' >> ~/.bashrc
source ~/.bashrc

grin

