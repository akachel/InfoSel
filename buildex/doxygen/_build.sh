#!/bin/sh
#
# Builds InfoSel++ project references
#
echo ">>> generating a doxygen reference for the main part of the library..."
doxygen infosel-library.dox
echo ">>> generating a doxygen reference for the standard algorithm packages..."
doxygen infosel-libpacks.dox
echo ">>> done."
