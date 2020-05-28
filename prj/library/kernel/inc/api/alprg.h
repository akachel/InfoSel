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
/// Algorithm Programmer Elements
/**
 * @file       alprg.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.08.04
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains elements for supporting
 *             programmers who wants to implement any studied feature 
 *             selection algorithms.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef ALPRG_H_
#define ALPRG_H_

/////////////////////////////
///
/// Macro-Directive for Registering 
/// Algorithm Packages 
///
/// To register at the library kernel
/// this directive must be applied at 
/// the beginning of any package with 
/// algorithm implementation classes.
///

#ifndef REGPACK
#define REGPACK(name) \
 extern "C" int infosel_##name##_package_=0; \
 static const char *infosel_package_qualification_=#name;
#endif

/////////////////////////////
///
/// Macro-Directive for Registering 
/// Algorithm Classes
///
/// It must be applied after declaration
/// of any class with studied algorithm 
/// implementation. 
//!
//! \note Each registered class must be 
//! first declared inside a package which
//! is already itself registered.
//!

#ifndef REGALGC
#define REGALGC(name) \
 static InfoSelAPI::Algorithms::algs_collection_register_<name> infosel_##name##_algclass_(infosel_package_qualification_);
#endif
 
namespace InfoSelAPI {
 
namespace Algorithms {

using namespace Tools;

/////////////////////////////
///
/// Base of All Algorithm Classes 
///
/// This direct base of all algorithm
/// implementation classes. It provides 
/// an extra support for programming 
/// algorithms.
///

class Algorithm : public AlgorithmBase {
 public:
/// create algorithm instance:

  Algorithm(const string &symbol);

/// destroy algorithm instance:

  ~Algorithm();

/// test whether the algorithm course is completed based on a given number of selected features:

  bool completed(int nsels) const
  { return nsels>=selmax(); }

/// return a flag of algorithm completeness based on state of a given data set:

  bool completed(const Data::set &set) const;

/// return a flag of algorithm completeness based on state of a given data file:

  bool completed(const Data::file &file) const;
};

/////////////////////////////
///
/// Register Of Algorithm Collection
///
/// This template is used for 
/// registering packages and 
/// classes with algorithm 
/// implementation. 
//!
//! \internal This template is
//! used by the REGALGC macro. 
//!

template <typename TClass>
class algs_collection_register_ : private algs_collection_base_ {
  int ireg_; //!< index of collection register for this algorithm instance
 public:
/// create a register object for given algorithm package and class:

  algs_collection_register_(const char *pack)
  {
   ireg_=algs_collection_base_::register_(pack,new TClass);
  }

/// destroy the register object:

  ~algs_collection_register_()
  {
   algs_collection_base_::unregister_(ireg_);
  }
};

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of ALPRG_H_ header file

