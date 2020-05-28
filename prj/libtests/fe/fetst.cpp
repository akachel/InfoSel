 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////          Library Front-end Test        ////////
     // //////                                        ////////
     // //////          //////////////////////////////////////
     // //////          //////////////////////////////////////
     // //////
     // ////// Copyright (C) 2010
     // //////
     // ////// Design & Programming: Adam Kachel
     // ////// Theoretical Analysis: Jacek Biesiada
     // //////
     // ////// Silesian University of Technology
     // ////// Department of M. & Computer Science
     // //////
// //////////////////////
// //////////////////////
///
/// Sample Algorithms For Front-End Test
/**
 * @file       fetst.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.07.27
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains sample algorithm code for
 *             testing the library front-end interface.
 */

#include "InfoSelAPI.h"
REGPACK(fetst)

namespace _FETST_ /** \copybrief fetst.cpp \copydetails fetst.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

class TestA : public Algorithm {
 private: 
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  TestA(): Algorithm("a") {}
};
REGALGC(TestA)

void TestA::read_(parstream &pars,repfile &rep)
{
 rep.cp().log() << "$$$ do read pars $$$" << endl;
}

void TestA::run_(datfile &file,repfile &rep)
{
 rep.log() << "$$$ do run alg $$$" << endl;
}

class TestB : public Algorithm {
 private: 
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  TestB(): Algorithm("b") {}
};
REGALGC(TestB)

void TestB::read_(parstream &pars,repfile &rep)
{
 rep.cp().log() << "$$$ do read pars $$$" << endl;
}

void TestB::run_(datfile &file,repfile &rep)
{
 rep.log() << "$$$ do run alg $$$" << endl;
}

class TestC : public Algorithm {
 private: 
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  TestC(): Algorithm("c") {}
};
REGALGC(TestC)

void TestC::read_(parstream &pars,repfile &rep)
{
 rep.cp().log() << "$$$ do read pars $$$" << endl;
}

void TestC::run_(datfile &file,repfile &rep)
{
 rep.log() << "$$$ do run alg $$$" << endl;
}

} // end of _FETST_ namespace

