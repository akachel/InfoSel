 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////     Algorithm Programming Interface    ////////
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
/// Defintions Of Algorithm Programmer Elements
/**
 * @file       alprg.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2009.03.20
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains defintions of selection algorithm related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dfile.h"
#include "albas.h"
#include "altls.h"
#include "alprg.h"

namespace InfoSelAPI {
 
namespace Algorithms {

/////////////////////////////
////
//// Algorithm class definition
////

Algorithm::Algorithm(const string &symbol):
 AlgorithmBase(symbol)
{}
  
Algorithm::~Algorithm()
{}

bool Algorithm::completed(const Data::set &auxset) const
{ 
 int nauxsels=auxset.card();
 return completed(nauxsels); 
}

bool Algorithm::completed(const Data::file &file) const
{ 
 int nfinsels=abs(file.finset().card()-fincard());
 if (completed(nfinsels)) return true;
 else {
  int ninisels=abs(file.iniset().card()-inicard());
  if (completed(ninisels)) return true;
  else return false;
 } 
}

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

