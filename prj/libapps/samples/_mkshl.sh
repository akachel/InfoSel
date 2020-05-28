#!/bin/sh
#
# Makes an user application of the InfoSel++ library as a program with the programmatic shell user interface,
# the target program is linked with the main algorithm packages assembly and a given user algorithm package
#
PACKNAME=$1
PROGFILE=$1.g.exe
CXX=g++
CXXFLAGS="-w -O2 -ffast-math -I../include"
LD=gcc
LDFLAGS="-lstdc++ -lm"
$CXX $CXXFLAGS -c ../src/$PACKNAME.cpp -o out/$PACKNAME.o
$CXX $CXXFLAGS -c ../src/_packuse.cxx -o out/_packuse.o
$CXX $CXXFLAGS -c ../src/shlmain.cpp -o out/shlmain.o
$LD out/shlmain.o out/_packuse.o out/$PACKNAME.o ../lib/libiselpacks.a ../lib/libinfosel++.a $LDFLAGS -o ../bin/$PROGFILE
chmod a+x ../bin/$PROGFILE

