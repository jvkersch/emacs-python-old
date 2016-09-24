#!/bin/sh

# Script to build a very basic emacs version with modules support

set -e

EMACS_INSTALL_DIR="$HOME/emacs"

echo "looking in $EMACS_INSTALL_DIR"

mkdir -p "$EMACS_INSTALL_DIR"
if [ -f "$EMACS_INSTALL_DIR/bin/emacs" ]; then
    echo "it exists"
    exit 0
else
    echo "it doesn't exist"
fi

wget http://ftp.gnu.org/gnu/emacs/emacs-25.1.tar.gz
tar xzf emacs-25.1.tar.gz
cd emacs-25.1
./configure \
    --with-modules \
    --prefix="$EMACS_INSTALL_DIR" \
    --with-jpeg=no \
    --with-gif=no \
    --with-tiff=no \
    --with-gnutls=no \
    --with-x=no \
    --with-sound=no \
    --without-xml2
make
make install
