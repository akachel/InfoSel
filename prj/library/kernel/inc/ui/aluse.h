 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////          Library User Interface        ////////
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
/// Algorithm User Elements
/**
 * @file       aluse.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains elements for supporting 
 *             users of any studied feature selection algorithms
 *             which is provided on a programmatic, internal level.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef ALUSE_H_
#define ALUSE_H_

/////////////////////////////
///
/// Macro-Directive for Including 
/// and Using Algorithm Packages 
///
/// This directive can be applied at 
/// any place of library application 
/// code. 
//!
//! \note Each used package must be 
//! first registered at the library
//! kernel.
//!

#ifndef USEPACK
#define USEPACK(name) \
 extern "C" int infosel_##name##_package_; \
 static int using_infosel_##name##_package_=InfoSelUI::Algorithms::algs_collection_accessor_::apply_using_directive_(#name,infosel_##name##_package_);
#endif

namespace InfoSelUI /** \copybrief InfoSelUI.h \copydetails InfoSelUI.h */ { // namespace head
 
namespace Core = InfoSelCore;
using namespace Core;
 
namespace Algorithms /** \copybrief aluse.h \copydetails aluse.h */ { // namespace head

using namespace InfoSelCore::Algorithms;

/////////////////////////////
///
/// Type of Used Algorithms 
///

typedef AlgorithmBase Algorithm;

/////////////////////////////
///
/// Accessor Of Algorithm Collection
///
/// It is used for accessing 
/// studied algorithm instances 
/// have been assembled by user 
/// directives.
///

class algs_collection_accessor_ : public algs_collection_base_ {
  static char usepacks_[maxnitems_][maxbuflen_]; //!< auxiliary array of algorithm packages given in all user directives
  static int nusepacks_; //!< current number of used algorithm packages
  static AlgorithmBase *useinsts_[maxnitems_]; //!< array of pointers to registered algorithm instances assembled by user directives
  static int nuseinsts_; //!< current number of used algorithm instances

 public:
//! \internal Apply the user directive for assembling a given algorithm package.
  static int apply_using_directive_(const char *pack,int &iuse);

 private:
//! do assemble a given algorithm package:
  static bool assemble_(const char *pack); 
  
//! end up building the assembly's inner structure:
  static void construct_(); 
  
 public:
/// create accessor object:

  algs_collection_accessor_() 
  {}
  
/// destroy accessor object:

  ~algs_collection_accessor_()
  {}

/// return number of accessable algorithm instances in the assembly:

  int count() const
  { if (isunstructed_) construct_(); return nuseinsts_; }

/// access an algorithm instance in the assembly giving its index:

  Algorithm &operator[](int idx) //! \pre 0 <= idx < count
  { if (isunstructed_) construct_(); return *useinsts_[idx]; }

/// test whether the assembly contains an algorithm instance with a given tag:

  bool contains(const string &tag) const;

/// access an algorithm instance in the assembly giving its tag:

  Algorithm *operator()(const string &tag);

/// access an algorithm instance in the assembly giving its tag:

  Algorithm &operator[](const string &tag);
};

/// the global object of algorithm collection:
extern algs_collection_accessor_ Algs;
extern algs_collection_accessor_ &SelAlgSet; //!< full alias name
extern algs_collection_accessor_ &Assembly; //!< alias name for using outside the namespace

} // end of Algorithms namespace

} // end of InfoSelUI namespace

#endif // end of ALUSE_H_ header file

