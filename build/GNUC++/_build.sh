#!/bin/sh
#
# Builds InfoSel++ project targets
#

make -r -R -f infosel.mak $1 $2 $3 $4 $5 2> out/_build.log

if [ -f ../../dist/bin/infosel++.g.exe ]
 then 
	 echo "Program build success !";
 else 	 
       echo "Build failed or no program target, check log file: out/_build.log";
fi

