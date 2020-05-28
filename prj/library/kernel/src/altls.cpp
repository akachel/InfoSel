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
/// Definitions Of Basic Elements For Algorithm Implementation Tools
/**
 * @file       altls.cpp
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

namespace InfoSelAPI {

namespace Algorithms {
 
namespace Tools {

/////////////////////////////
////
//// exception type 
//// definitions
////

void xSparseDirOverflow::printmsg_(ostream &out) const
{
 out << "Error: Integer overflow of direct addressing in sparse collection";
}

void xSparseCountOverflow::printmsg_(ostream &out) const
{
 out << "Error: Integer overflow of number of sparse collection elements";
}

void xTableSizeOverflow::printmsg_(ostream &out) const
{
 out << "Error: Integer overflow of table sizes or their multiple";
}

void xFieldFormLinkFail::printmsg_(ostream &out) const
{
 out << "Fatal error: Field formula object linkage failure";
}

void xTensorEqSizReq::printmsg_(ostream &out) const
{
 out << "Error: Equisized table as tensor representation required";
}

void xTensor0RankReq::printmsg_(ostream &out) const
{
 out << "Error: Scalar as tensor of rank 0 required";
}

void xTensor1RankReq::printmsg_(ostream &out) const
{
 out << "Error: Vector as tensor of rank 1 required";
}

void xTensor2RankReq::printmsg_(ostream &out) const
{
 out << "Error: Matrix as tensor of rank 2 required";
}

/////////////////////////////
////
//// permutation class 
//// definition
////

permutation::permutation(int n):
 nelms_(n<2 ? 2 : n), nclcs_(0)
{
 elms_ = nelms_>0 ? new int[nelms_] : 0;
 for (int k=0; k<nelms_; k++) elms_[k]=k;
}

permutation::permutation(int n,int m):
 nelms_(n<2 ? 2 : n), nclcs_(m)
{
 if (nclcs_<0) nclcs_=0;
 else if (nclcs_>nelms_) nclcs_=nelms_;
 elms_ = nelms_>0 ? new int[nelms_] : 0;
 for (int k=0; k<nelms_; k++) elms_[k]=k;
}

permutation::~permutation()
{
 delete[] elms_;
}

permutation &permutation::operator()(int p0,int p1...)
{
 va_list ap;
 va_start(ap,p1);
 if (nclcs_>0) {
  int k=p0;
  for (int l=1; l<=nclcs_; l++) {
   int pk = l==1 ? p1 : l==nclcs_ ? p0 : va_arg(ap,int);
   if (pk<0) pk=0; 
   else if (pk>=nelms_) pk=nelms_-1;
   elms_[k]=pk;
   k=pk;
  }
 } else {
  for (int k=0; k<nelms_; k++) {
   int pk = k==0 ? p0 : k==1 ? p1 : va_arg(ap,int);
   if (pk<0) pk=0; 
   else if (pk>=nelms_) pk=nelms_-1;
   elms_[k]=pk;
  }
 } 
 va_end(ap);
 return *this;
}

void permutation::print_conts(ostream &out) const
{
 out << "Per(" << nelms_;
 if (nclcs_>0) out << ',' << nclcs_;
 out << ')';
 out << '(';
 for (int k=0; k<nelms_; k++)
 out << ' ' << elms_[k];
 out << " )";
}

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

