 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////              Library Core              ////////
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
/// Definitions For Input Data Vector
/**
 * @file       dvec.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains defintions of input data related elements.
 */

#include "comm.h"
#include "data.h"
#include "dvec.h"

namespace InfoSelCore {

namespace Data {

/////////////////////////////
////
//// datvector class definition
////

datvector::datvector():
 orgcmps_(0), ncmps_(0), nsglparts_(0), 
 val_rep_(totalval_proxy_(*this),valtuple_proxy_(*this)), 
 part_rep_(totalpart_proxy_(*this),parttuple_proxy_(*this))
{}

datvector::~datvector()
{
 delete[] orgcmps_;
}

void datvector::setup_orgcmps_(int ncmps,int nsglparts) //! \pre ncmps>=0 , nsglparts>0
{                                     
 if (ncmps!=ncmps_) {
  ncmps_=ncmps; 
  delete[] orgcmps_;
  orgcmps_ = ncmps>0 ? new datum*[ncmps] : 0;
 }
 nsglparts_=nsglparts;
 val_rep_.nul();
 part_rep_.nul();
}

bool datvector::valtuple_proxy_::operator()(realtuple &ret) const
{ 
 ret.setup(that_.ncmps_,Nil);
 for (int k=0; k<that_.ncmps_; k++) {
  const datum *dk=that_.orgcmps_[k]; // dk!=0
  real vk=dk->val();
  ret[k]=vk;
 }
 return true;
}

bool datvector::totalval_proxy_::operator()(real &ret) const
{ 
 ret=0.0;
 for (int k=0; k<that_.ncmps_; k++) {
  const datum *dk=that_.orgcmps_[k]; // dk!=0
  real vk=dk->val();
  ret+=vk;
 }
 return true;
}

bool datvector::parttuple_proxy_::operator()(inttuple &ret) const
{ 
 ret.setup(that_.ncmps_,Nil);
 for (int k=0; k<that_.ncmps_; k++) {
  const datum *dk=that_.orgcmps_[k]; // dk!=0
  int pk=dk->part();
  ret[k]=pk;
 }
 return true;
}

bool datvector::totalpart_proxy_::operator()(int &ret) const
{ 
 ret=-1;
 int m=that_.ncmps_;
 if (m>0) {
  int n=that_.nsglparts_;
  int N;
  if (!chkmopow(N,n,m)) throw xDomainPartOverflow();
  int P=0;
  N=1;
  int k=m-1;
  for (;;) {
   const datum *dk=that_.orgcmps_[k]; // dk!=0
   int pk=dk->part();
   P+=pk*N;
   if (k<=0) break;
   int nk=n; // nk >= 'set/subset.orgelms_(k)->nparts()'
   N*=nk;
   k--;
  }   
  ret=P;
 }
 return true;
}

void datvector::read_val_(istream &in)
{
 for (int k=0; k<ncmps_; k++) {
  datum *d=orgcmps_[k]; // d!=0
  d->read_val_(in);
 } 
}

void datvector::print_conts(ostream &out) const
{
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && ncmps_>nshr;
 out << '(';
 int n = isshr ? nshr : ncmps_;
 for (int k=0; k<n; k++) {
  const datum *d=orgcmps_[k]; // d!=0
  ostringstream bout;
  bout << d->val_ << ':' << d->part_;
  out << ' ' << setw(15) << bout.str();
 } 
 if (isshr) out << " ...";
 out << " )";
}

} // end of Data namespace

} // end of InfoSelCore namespace

