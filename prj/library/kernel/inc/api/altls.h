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
/// Algorithm Implementation Tools 
/**
 * @file       altls.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2009.03.20
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains programmatic tools for 
 *             implementing studied feature selection algorithms 
 *             based on information theory and with application of 
 *             probability distributions.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef ALTLS_H_
#define ALTLS_H_

namespace InfoSelAPI /** \copybrief InfoSelAPI.h \copydetails InfoSelAPI.h */ { // namespace head
 
namespace Core = InfoSelCore;
using namespace Core;

namespace Algorithms /** \copybrief alprg.h \copydetails alprg.h */ { // namespace head

using namespace InfoSelCore::Algorithms;

namespace Tools /** \copybrief altls.h \copydetails altls.h */ { // namespace head

/////////////////////////////
///
/// Algorithm Tools Specific Errors
///

struct xToolsError : xAlgsError {};

/// Error Of Sparse Direct Dimension Overflow
struct xSparseDirOverflow : xToolsError { private: void printmsg_(ostream &out) const; };
/// Error Of Sparse Count Overflow
struct xSparseCountOverflow : xToolsError { private: void printmsg_(ostream &out) const; };
/// Error Of Table Size Overflow
struct xTableSizeOverflow : xToolsError { private: void printmsg_(ostream &out) const; };
/// Error Of Field Formula Link Failed
struct xFieldFormLinkFail : xToolsError { private: void printmsg_(ostream &out) const; };
/// Error Of Tensor Equal-Sized Required
struct xTensorEqSizReq : xToolsError { private: void printmsg_(ostream &out) const; };
/// Error Of Tensor 0-Rank Required
struct xTensor0RankReq : xToolsError { private: void printmsg_(ostream &out) const; };
/// Error Of Tensor 1-Rank Required
struct xTensor1RankReq : xToolsError { private: void printmsg_(ostream &out) const; };
/// Error Of Tensor 2-Rank Required
struct xTensor2RankReq : xToolsError { private: void printmsg_(ostream &out) const; };

/////////////////////////////
///
/// Auxiliary Type Of Permutation
///
/// It is used for representing
/// and manipulating any permutation
/// of integer element sequence.
///

class permutation {
  mutable int *elms_; //!< underlying array of elements in the permutation
  mutable int nelms_; //!< element array length
  mutable int nclcs_; //!< number of elements in the given cyclic permutation
//@{  
 private: //! <b> Not implemented/allowed. </b>
  void operator=(permutation &);
//@}  
 public:
/// create an identity permutation object for sequence of a given length n:

  explicit permutation(int n); //!< \pre (asserted) n>1  \post length==n , cycle==0

/// create a m-cycle permutation object for sequence of a given length n:

  permutation(int n,int m); //!< \pre (asserted) n>1 , m<=n  \post length==n , cycle==m

/// set up the permutation object with given sequence elements:

  permutation &operator()(int p0,int p1...); //!< \pre cycle<=0 && count(p0,p1...)>=length || cycle>0 && count(p0,p1...)>=cycle  \pre (asserted) 0 <= p0,p1... < length-1

/// create a permutation object and assign to it a given permutation by moving inner structure:

  permutation(const permutation &prm):
   elms_(prm.elms_), nelms_(prm.nelms_), nclcs_(prm.nclcs_)
  { prm.elms_=0; prm.nelms_=0; prm.nclcs_=0; }

/// destroy the permutation object:

  ~permutation();

/// create and return a permutation object with a given sequence length n:

  friend permutation Per(int n)
  {
   return permutation(n);
  }

/// create and return a permutation object with a given sequence & cycle length n:

  friend permutation Per(int n,int m)
  {
   return permutation(n,m);
  }

/// return total length of the permutated sequence:

  int length() const //! \post length>=2
  { return nelms_; }

/// return length of given cycle in the permutated sequence:

  int cycle() const //! \post cycle<=length
  { return nclcs_; }

/// return value of the k-th element of the permutated sequence:

  int operator[](int k) const //! \pre 0 <= k < length
  { return elms_[k]; }

/// print contents of the permutation to a given output stream:
  
  void print_conts(ostream &out) const;

/// print contents to output stream:
  
  friend ostream &operator<<(ostream &out,const permutation &prm)
  {
   prm.print_conts(out);
   return out;
  }
};

//// explicit friend function injections:

permutation Per(int n);
permutation Per(int n,int m);

/////////////////////////////
////
//// All Main Tool Structures
////

class sparse;
class table;
class field;
class tensor;

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of ALTLS_H_ header file

