#!/bin/sh

# https://security.stackexchange.com/questions/103034/gnupg-decryption-not-asking-for-passphrase
# invalidate cache
echo -e "default-cache-ttl 1
max-cache-ttl 1" > ~/.gnupg/gpg-agent.conf

rm plain.txt.*
# https://astera.zendesk.com/hc/en-us/community/posts/115000231383-How-to-pass-the-passphrase-into-GPG-for-decryption
# encrypt (symmetric)
echo "enc gui"
gpg --cipher-algo AES-256 -o plain.txt.gpg.gui -c plain.txt
echo "enc cli"
gpg --batch --yes --passphrase pippo -o plain.txt.gpg.cli -c plain.txt
# decrypt (symmetric)
echo "dec gui"
gpg -o plain.txt.gui -d plain.txt.gpg.gui
echo "dec cli"
gpg --batch --yes --passphrase pippo -o plain.txt.cli -d plain.txt.gpg.cli

# Now another user logs in. Without passphrase, he won't see any clear text

ls -l .
