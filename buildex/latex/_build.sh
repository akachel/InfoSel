#!/bin/sh
#
# Builds InfoSel++ library documentations
#
echo ">>> generating a latex tutorial for the library..."
cd ../../prj/libdocs/tutorial
../../../buildex/latex/infosel-tutorial.sh
cd ../../../buildex/latex
echo ">>> done."
