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
/// Definitions For Dense Collection
/**
 * @file       table.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains definitions of algorithm tool related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dspac.h"
#include "dsubs.h"
#include "dfile.h"
#include "albas.h"
#include "altls.h"
#include "sparse.h"
#include "table.h"

namespace InfoSelAPI {
             
namespace Algorithms {

namespace Tools {

/////////////////////////////
////
//// table class definition
////

table::table(int ndims,const int sizes[]):
 lnkcnt_(new int(1)), dimbass_(0)
{
 ndims_ = ndims<0 ? 0 : ndims;
 dimsizs_ = ndims_>0 ? new int[ndims_] : 0;
 nelms_=1;
 for (int k=0; k<ndims_; k++) {
  int sk=sizes[k];
  if (sk<1) sk=1;
  dimsizs_[k]=sk;
  if (!chkmul(nelms_,nelms_,sk)) throw xTableSizeOverflow();
 }
 linelms_=new real[nelms_];
 memset(linelms_,0,nelms_*sizeof(real));
}

table::table(int ndims1,int ndims2,const int sizes1[],const int sizes2[]):
 lnkcnt_(new int(1)), dimbass_(0)
{
 int ndims=ndims1+ndims2;
 ndims_ = ndims<0 ? 0 : ndims;
 dimsizs_ = ndims_>0 ? new int[ndims_] : 0;
 nelms_=1;
 int k=0;
 for (int k1=0; k1<ndims1; k1++) {
  int sk=sizes1[k1];
  if (sk<1) sk=1;
  dimsizs_[k++]=sk;
  if (!chkmul(nelms_,nelms_,sk)) throw xTableSizeOverflow();
 }
 for (int k2=0; k2<ndims2; k2++) {
  int sk=sizes2[k2];
  if (sk<1) sk=1;
  dimsizs_[k++]=sk;
  if (!chkmul(nelms_,nelms_,sk)) throw xTableSizeOverflow();
 }
 linelms_=new real[nelms_];
 memset(linelms_,0,nelms_*sizeof(real));
}

table::table():
 lnkcnt_(new int(1)), ndims_(0), 
 dimsizs_(0), dimbass_(0), 
 linelms_(new real[1]), nelms_(1)
{
 *linelms_=0.0; 
}

void table::construct_(int ndims,...) //! \pre ndims>0 , count(...sk...)>=ndims , 0 <= k < ndims
{
 lnkcnt_=new int(1);
 dimbass_=0;
 ndims_=ndims;
 dimsizs_=new int[ndims_];
 nelms_=1;
 va_list ap;
 va_start(ap,ndims);
 for (int k=0; k<ndims_; k++) {
  int sk=va_arg(ap,int);
  if (sk<1) sk=1;
  dimsizs_[k]=sk;
  if (!chkmul(nelms_,nelms_,sk)) throw xTableSizeOverflow();
 }
 va_end(ap);
 linelms_=new real[nelms_];
 memset(linelms_,0,nelms_*sizeof(real));
}

void table::create_dimbass_() const //! \pre dimbass_==0
{
 if (ndims_>0) {
  dimbass_=new int[ndims_];
  int k=ndims_-1;
  dimbass_[k--]=1;
  for (; k>=0; k--)
  dimbass_[k]=dimsizs_[k+1]*dimbass_[k+1]; // dimbass_[k]<nelms_
 }
}

table::table(const sparse &spr,bool cpy):
 lnkcnt_(new int(1)), dimbass_(0)
{
 ndims_=spr.rank();
 dimsizs_ = ndims_>0 ? new int[ndims_] : 0; 
 nelms_=1;
 for (int k=0; k<ndims_; k++) {
  const inttuple &Sk=spr.sizes(k);
  int m=Sk.count();
  int sk=1;
  for (int q=0; q<m; q++)
  if (!chkmul(sk,sk,Sk[q])) throw xTableSizeOverflow();
  dimsizs_[k]=sk;
  if (!chkmul(nelms_,nelms_,sk)) throw xTableSizeOverflow();
 }
 linelms_=new real[nelms_];
 memset(linelms_,0,nelms_*sizeof(real));
 if (cpy) {
  create_dimbass_();
  sparse::const_iterator P(spr);
  while (P) {
   int I=0;
   int h=0;
   for (int k=0; k<ndims_; k++) {
    const inttuple &Sk=spr.sizes(k);
    int m=Sk.count();
    int ik=0;
    int dq=1;
    for (int q=m-1; q>=0; q--) {
     int jq=P[h+q]; // h+q<P.length
     ik+=jq*dq; // ik<dimbass_[k]
     dq*=Sk[q]; // dq<dimbass_[k]
    }
    h+=m;
    I+=ik*dimbass_[k]; // I<nelms_
   }
   linelms_[I]=*P;
   ++P;
  }
 }
}

table::table(const table &tab,bool cpy):
 lnkcnt_(new int(1)), ndims_(tab.ndims_), 
 dimbass_(0), nelms_(tab.nelms_)
{
 dimsizs_ = ndims_>0 ? new int[ndims_] : 0;
 for (int k=0; k<ndims_; k++) dimsizs_[k]=tab.dimsizs_[k]; 
 linelms_ = new real[nelms_];
 if (cpy) memcpy(linelms_,tab.linelms_,nelms_*sizeof(real));
 else memset(linelms_,0,nelms_*sizeof(real));
}

table::table(const table &tab):
 lnkcnt_(tab.lnkcnt_), ndims_(tab.ndims_), 
 dimsizs_(tab.dimsizs_), dimbass_(tab.dimbass_),
 linelms_(tab.linelms_), nelms_(tab.nelms_)
{
 (*lnkcnt_)++;
}

table::~table()
{
 if (--(*lnkcnt_) <= 0) {
  delete[] linelms_;
  delete[] dimbass_;
  delete[] dimsizs_;
  delete lnkcnt_;
 }
}

table &table::operator=(const table &tab)
{
 if (--(*lnkcnt_) <= 0) {
  delete[] linelms_;
  delete[] dimbass_;
  delete[] dimsizs_;
  delete lnkcnt_;
 }
 
 lnkcnt_=tab.lnkcnt_; ndims_=tab.ndims_; 
 dimsizs_=tab.dimsizs_; dimbass_=tab.dimbass_;
 linelms_=tab.linelms_; nelms_=tab.nelms_;
 
 (*lnkcnt_)++;
 return *this;
}

bool table::isshared() const
{ 
 return (*lnkcnt_)>1; 
}

table &table::clone()
{
 if ((*lnkcnt_)>1) *this = table(*this,true);
 return *this;
}

real &table::dimelms_(int i0...) const //! \pre ndims>0 , count(i0...ik...)>=ndims , 0 <= ik < size(k) , 0 <= k < ndims
{
 if (dimbass_==0) create_dimbass_();
 va_list ap;
 va_start(ap,i0);
 int I=0;
 for (int k=0; k<ndims_; k++) {
  int ik = k==0 ? i0 : va_arg(ap,int);
  I+=ik*dimbass_[k]; // I<nelms_
 }
 va_end(ap);
 return linelms_[I];
}

real &table::dimelms_(const inttuple &ii) const //! \pre ii.count>=ndims , 0 <= ii[k] < size(k) , 0 <= k < ndims
{
 if (dimbass_==0) create_dimbass_();
 int I=0;
 for (int k=0; k<ndims_; k++) 
 I+=ii[k]*dimbass_[k]; // I<nelms_
 return linelms_[I];
}

table &table::zero()
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=zero(x):" << endl;
 cdeb << "x = " << *this << endl;
#endif
 for (int i=0; i<nelms_; i++)
 linelms_[i]=0.0;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

table &table::unit()
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=unit(x):" << endl;
 cdeb << "x = " << *this << endl;
#endif
 for (int i=0; i<nelms_; i++)
 linelms_[i]=1.0;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

table &table::rand()
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=rand(x):" << endl;
 cdeb << "x = " << *this << endl;
#endif
 for (int i=0; i<nelms_; i++)
 linelms_[i]=Common::unirand();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

table &table::operator=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=op=(x,a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 for (int i=0; i<nelms_; i++)
 linelms_[i]=a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

bool operator==(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "ret=op==(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.nelms_>x2.nelms_) {
  int so=x1.nelms_/x2.nelms_;
  if (x1.nelms_!=x2.nelms_*so) throw xPrecondFail();
  bool ret=true;
  int j=0;
  real x2i;
  for (int i=0; ret && i<x1.nelms_; i++) {
   if (i%so==0) x2i=x2.linelms_[j++];
   ret = x1.linelms_[i]==x2i;
  }
#ifdef SOURCE_DEBUG
  cdeb << "ret = " << ret << endl;
#endif
  return ret;
 } else
 if (x1.nelms_<x2.nelms_) {
  int so=x2.nelms_/x1.nelms_;
  if (x1.nelms_*so!=x2.nelms_) throw xPrecondFail();
  bool ret=true;
  int j=0;
  real x1i;
  for (int i=0; ret && i<x2.nelms_; i++) {
   if (i%so==0) x1i=x1.linelms_[j++];
   ret = x1i==x2.linelms_[i];
  }
#ifdef SOURCE_DEBUG
  cdeb << "ret = " << ret << endl;
#endif
  return ret;
 } else {
  bool ret=true;
  for (int i=0; ret && i<x1.nelms_; i++)
  ret = x1.linelms_[i]==x2.linelms_[i];
#ifdef SOURCE_DEBUG
  cdeb << "ret = " << ret << endl;
#endif
  return ret;
 }
}

bool operator==(real a,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "ret=op==(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 bool ret=true;
 for (int i=0; ret && i<x.nelms_; i++)
 ret = a==x.linelms_[i];
#ifdef SOURCE_DEBUG
 cdeb << "ret = " << ret << endl;
#endif
 return ret;
}

bool operator==(const table &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "ret=op==(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 bool ret=true;
 for (int i=0; ret && i<x.nelms_; i++)
 ret = x.linelms_[i]==a;
#ifdef SOURCE_DEBUG
 cdeb << "ret = " << ret << endl;
#endif
 return ret;
}

table &table::operator+=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=op+=(x,a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 for (int i=0; i<nelms_; i++)
 linelms_[i]+=a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

table &table::operator-=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=op-=(x,a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 for (int i=0; i<nelms_; i++)
 linelms_[i]-=a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

table &table::operator*=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=op*=(x,a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 for (int i=0; i<nelms_; i++)
 linelms_[i]*=a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

table &table::operator/=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=op/=(x,a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 if (a!=0.0)
  for (int i=0; i<nelms_; i++)
  linelms_[i]/=a;
 else 
  memset(linelms_,0,nelms_*sizeof(real));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

table abs(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=abs(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i]=Common::abs(xi);
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table log(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=log(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i] = xi==0.0 ? 0.0 : std::log(xi);
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table log2(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=log2(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i] = xi==0.0 ? 0.0 : Common::log2(xi);
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table sqr(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=sqr(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i]=Common::sqr(xi);
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table sqrt(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=sqrt(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i] = xi<0.0 ? 0.0 : std::sqrt(xi);
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table cub(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=cub(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i]=Common::cub(xi);
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table pow(const table &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=pow(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i]=std::pow(xi,a);
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table cum(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=cum(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,true);
 int so = x.ndims_>0 ? x.dimsizs_[x.ndims_-1] : 1;
 int i=0;
 while (i<y.nelms_) {
  real uo=y.linelms_[i];
  i++;
  for (int io=1; io<so; io++) uo=(y.linelms_[i]+=uo), i++;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table sum(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=sum(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 table y(n,x.dimsizs_);
 int so = n>=0 ? x.dimsizs_[n] : 1;
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=0.0;
  for (int io=0; io<so; io++) uo+=x.linelms_[j++];
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table avg(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=avg(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 table y(n,x.dimsizs_);
 int so = n>=0 ? x.dimsizs_[n] : 1;
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=0.0;
  for (int io=0; io<so; io++) uo+=x.linelms_[j++];
  y.linelms_[i]=uo/so;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table sum(const Data::space &_F_,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=sum(_F_,x):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 int so = n>=0 ? x.dimsizs_[n] : 0;
 if (so!=_F_.card()) throw xPrecondFail();
 table y(n,x.dimsizs_);
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=0.0;
  for (Data::subset f(_F_); !f.isnil(); ++f) uo+=x.linelms_[j+f];
  j+=so;
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table avg(const Data::space &_F_,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=avg(_F_,x):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 int so = n>=0 ? x.dimsizs_[n] : 0;
 if (so!=_F_.card()) throw xPrecondFail();
 table y(n,x.dimsizs_);
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=0.0;
  int No=0;
  for (Data::subset f(_F_); !f.isnil(); ++f) uo+=x.linelms_[j+f], No++;
  j+=so;
  y.linelms_[i] = No>0 ? uo/No : 0.0;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prod(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prod(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 table y(n,x.dimsizs_);
 int so = n>=0 ? x.dimsizs_[n] : 1;
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=1.0;
  for (int io=0; io<so; io++) uo*=x.linelms_[j++];
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prod(const Data::space &_F_,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prod(_F_,x):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 int so = n>=0 ? x.dimsizs_[n] : 0;
 if (so!=_F_.card()) throw xPrecondFail();
 table y(n,x.dimsizs_);
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=1.0;
  for (Data::subset f(_F_); !f.isnil(); ++f) uo*=x.linelms_[j+f];
  j+=so;
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table max(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=max(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 table y(n,x.dimsizs_);
 int so = n>=0 ? x.dimsizs_[n] : 1;
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=-rInf;
  for (int io=0; io<so; io++) {
   real xj=x.linelms_[j++];
   if (xj>uo) uo=xj;
  } 
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table max(const Data::space &_F_,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=max(_F_,x):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 int so = n>=0 ? x.dimsizs_[n] : 0;
 if (so!=_F_.card()) throw xPrecondFail();
 table y(n,x.dimsizs_);
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=-rInf;
  for (Data::subset f(_F_); !f.isnil(); ++f) {
   real xjf=x.linelms_[j+f];
   if (xjf>uo) uo=xjf;
  }
  j+=so;
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table min(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=min(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 table y(n,x.dimsizs_);
 int so = n>=0 ? x.dimsizs_[n] : 1;
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=+rInf;
  for (int io=0; io<so; io++) {
   real xj=x.linelms_[j++];
   if (xj<uo) uo=xj;
  } 
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table min(const Data::space &_F_,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=min(_F_,x):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 int so = n>=0 ? x.dimsizs_[n] : 0;
 if (so!=_F_.card()) throw xPrecondFail();
 table y(n,x.dimsizs_);
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  real uo=+rInf;
  for (Data::subset f(_F_); !f.isnil(); ++f) {
   real xjf=x.linelms_[j+f];
   if (xjf<uo) uo=xjf;
  }
  j+=so;
  y.linelms_[i]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table max2(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=max2(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.nelms_>x2.nelms_) {
  int so=x1.nelms_/x2.nelms_;
  if (x1.nelms_!=x2.nelms_*so) throw xPrecondFail();
  table y(x1,false);
  int j=0;
  real x2i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x2i=x2.linelms_[j++];
   y.linelms_[i]=Common::max2(x1.linelms_[i],x2i);
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else
 if (x1.nelms_<x2.nelms_) {
  int so=x2.nelms_/x1.nelms_;
  if (x1.nelms_*so!=x2.nelms_) throw xPrecondFail();
  table y(x2,false);
  int j=0;
  real x1i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x1i=x1.linelms_[j++];
   y.linelms_[i]=Common::max2(x1i,x2.linelms_[i]);
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else {
  table y(x1,false);
  for (int i=0; i<y.nelms_; i++)
  y.linelms_[i]=Common::max2(x1.linelms_[i],x2.linelms_[i]);
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 }
}

table min2(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=min2(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.nelms_>x2.nelms_) {
  int so=x1.nelms_/x2.nelms_;
  if (x1.nelms_!=x2.nelms_*so) throw xPrecondFail();
  table y(x1,false);
  int j=0;
  real x2i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x2i=x2.linelms_[j++];
   y.linelms_[i]=Common::min2(x1.linelms_[i],x2i);
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else
 if (x1.nelms_<x2.nelms_) {
  int so=x2.nelms_/x1.nelms_;
  if (x1.nelms_*so!=x2.nelms_) throw xPrecondFail();
  table y(x2,false);
  int j=0;
  real x1i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x1i=x1.linelms_[j++];
   y.linelms_[i]=Common::min2(x1i,x2.linelms_[i]);
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else {
  table y(x1,false);
  for (int i=0; i<y.nelms_; i++)
  y.linelms_[i]=Common::min2(x1.linelms_[i],x2.linelms_[i]);
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 }
}

table diag(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diag(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_-1;
 table y(n,x.dimsizs_);
 int sa = n>=0 ? x.dimsizs_[n] : 0;
 int sb = n>=1 ? x.dimsizs_[n-1] : 0;
 int so=Common::min2(sa,sb);
 int j=0, io=0;
 real uo;
 for (int i=0; i<y.nelms_; i++,j+=sa) {
  if (io<so) uo=x.linelms_[j+io];
  else if (io<sb) uo=0.0;
  else uo=x.linelms_[j];
  y.linelms_[i]=uo;
  if (++io>=sb) io=0;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table diagadd(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diagadd(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 int n1=x1.ndims_-1;
 int n2=x2.ndims_-1;
 int s1 = n1>=0 ? x1.dimsizs_[n1] : 1;
 int s2 = n2>=0 ? x2.dimsizs_[n2] : 1;
 if (s1!=s2) throw xPrecondFail();
 table y(n1,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int so=s1;
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1+=so)
 for (int j2=0; j2<x2.nelms_; j2+=so)
 for (int io=0; io<so; io++) y.linelms_[i++]=x1.linelms_[j1+io]+x2.linelms_[j2+io];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table diagsub(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diagsub(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 int n1=x1.ndims_-1;
 int n2=x2.ndims_-1;
 int s1 = n1>=0 ? x1.dimsizs_[n1] : 1;
 int s2 = n2>=0 ? x2.dimsizs_[n2] : 1;
 if (s1!=s2) throw xPrecondFail();
 table y(n1,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int so=s1;
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1+=so)
 for (int j2=0; j2<x2.nelms_; j2+=so)
 for (int io=0; io<so; io++) y.linelms_[i++]=x1.linelms_[j1+io]-x2.linelms_[j2+io];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table diagmul(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diagmul(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 int n1=x1.ndims_-1;
 int n2=x2.ndims_-1;
 int s1 = n1>=0 ? x1.dimsizs_[n1] : 1;
 int s2 = n2>=0 ? x2.dimsizs_[n2] : 1;
 if (s1!=s2) throw xPrecondFail();
 table y(n1,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int so=s1;
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1+=so)
 for (int j2=0; j2<x2.nelms_; j2+=so)
 for (int io=0; io<so; io++) y.linelms_[i++]=x1.linelms_[j1+io]*x2.linelms_[j2+io];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table diagdiv(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diagdiv(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 int n1=x1.ndims_-1;
 int n2=x2.ndims_-1;
 int s1 = n1>=0 ? x1.dimsizs_[n1] : 1;
 int s2 = n2>=0 ? x2.dimsizs_[n2] : 1;
 if (s1!=s2) throw xPrecondFail();
 table y(n1,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int so=s1;
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1+=so)
 for (int j2=0; j2<x2.nelms_; j2+=so)
 for (int io=0; io<so; io++) {
  real x2ji=x2.linelms_[j2+io];
  y.linelms_[i++] = x2ji!=0.0 ? x1.linelms_[j1+io]/x2ji : 0.0;
 } 
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table dyadadd(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=dyadadd(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 table y(x1.ndims_,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1++)
 for (int j2=0; j2<x2.nelms_; j2++)
 y.linelms_[i++]=x1.linelms_[j1]+x2.linelms_[j2];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table dyadsub(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=dyadsub(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 table y(x1.ndims_,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1++)
 for (int j2=0; j2<x2.nelms_; j2++)
 y.linelms_[i++]=x1.linelms_[j1]-x2.linelms_[j2];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table dyadmul(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=dyadmul(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 table y(x1.ndims_,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1++)
 for (int j2=0; j2<x2.nelms_; j2++)
 y.linelms_[i++]=x1.linelms_[j1]*x2.linelms_[j2];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table dyaddiv(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=dyaddiv(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 table y(x1.ndims_,x2.ndims_,x1.dimsizs_,x2.dimsizs_);
 int i=0;
 for (int j1=0; j1<x1.nelms_; j1++)
 for (int j2=0; j2<x2.nelms_; j2++) {
  real x2j=x2.linelms_[j2];
  y.linelms_[i++] = x2j!=0.0 ? x1.linelms_[j1]/x2j : 0.0;
 } 
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator*(const table &x,const permutation &p)
{
 int n=x.ndims_;
 if (n<2) return table(x,true);
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(x,p):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "p = " << p << endl;
#endif
 if (p.length()!=n) throw xPrecondFail();
 int *pxsizs=new int[n];
 for (int k=0; k<n; k++) {
  int l=p[k];
  pxsizs[k]=x.dimsizs_[l];
 }
 table y(n,pxsizs);
 delete[] pxsizs;
 y.create_dimbass_();
 int *ii=new int[n];
 memset(ii,0,n*sizeof(int));
 int I=0;
 for (;;) {
  int pI=0;
  for (int k=0; k<n; k++) {
   int l=p[k];
   int pi=ii[l];
   pI+=pi*y.dimbass_[k];
  }
  y.linelms_[pI]=x.linelms_[I++];
  int l=n-1;
  do {
   ii[l]++;
   if (ii[l]<x.dimsizs_[l]) break;
   else ii[l--]=0;
  } while (l>=0);
  if (l<0) break;
 }
 delete[] ii;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator~(const table &x)
{
 int n=x.ndims_;
 if (n<2) return table(x,true);
#ifdef SOURCE_DEBUG
 cdbg << "y=op~(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int *pxsizs=new int[n];
 for (int k=0; k<n; k++) {
  int l=n-1-k;
  pxsizs[k]=x.dimsizs_[l];
 }
 table y(n,pxsizs);
 delete[] pxsizs;
 y.create_dimbass_();
 int *ii=new int[n];
 memset(ii,0,n*sizeof(int));
 int I=0;
 for (;;) {
  int pI=0;
  for (int k=0; k<n; k++) {
   int l=n-1-k;
   int pi=ii[l];
   pI+=pi*y.dimbass_[k];
  }
  y.linelms_[pI]=x.linelms_[I++];
  int l=n-1;
  do {
   ii[l]++;
   if (ii[l]<x.dimsizs_[l]) break;
   else ii[l--]=0;
  } while (l>=0);
  if (l<0) break;
 }
 delete[] ii;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator|(const table &x,int s)
{
 if (s<1) s=1;
#ifdef SOURCE_DEBUG
 cdbg << "y=op|(x,s):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "s = " << s << endl;
#endif
 int so=s;
 table y(x.ndims_,1,x.dimsizs_,&so);
 int j=0;
 real xi;
 for (int i=0; i<y.nelms_; i++) {
  if (i%so==0) xi=x.linelms_[j++];
  y.linelms_[i]=xi;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
} 

table operator|(int s,const table &x)
{
 if (s<1) s=1;
#ifdef SOURCE_DEBUG
 cdbg << "y=op|(s,x):" << endl;
 cdeb << "s = " << s << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(1,x.ndims_,&s,x.dimsizs_);
 int j=0;
 for (int i=0; i<y.nelms_; i++) {
  y.linelms_[i]=x.linelms_[j++];
  if (j>=x.nelms_) j=0;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
} 

table operator+(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op+(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.nelms_>x2.nelms_) {
  int so=x1.nelms_/x2.nelms_;
  if (x1.nelms_!=x2.nelms_*so) throw xPrecondFail();
  table y(x1,true);
  int j=0;
  real x2i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x2i=x2.linelms_[j++];
   y.linelms_[i]+=x2i;
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else
 if (x1.nelms_<x2.nelms_) {
  int so=x2.nelms_/x1.nelms_;
  if (x1.nelms_*so!=x2.nelms_) throw xPrecondFail();
  table y(x2,false);
  int j=0;
  real x1i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x1i=x1.linelms_[j++];
   y.linelms_[i]=x1i+x2.linelms_[i];
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else {
  table y(x1,true);
  for (int i=0; i<y.nelms_; i++)
  y.linelms_[i]+=x2.linelms_[i];
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 }
}

table operator+(real a,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op+(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]=a+x.linelms_[i];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator+(const table &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op+(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 table y(x,true);
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]+=a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator-(const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]=-x.linelms_[i];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator-(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.nelms_>x2.nelms_) {
  int so=x1.nelms_/x2.nelms_;
  if (x1.nelms_!=x2.nelms_*so) throw xPrecondFail();
  table y(x1,true);
  int j=0;
  real x2i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x2i=x2.linelms_[j++];
   y.linelms_[i]-=x2i;
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else
 if (x1.nelms_<x2.nelms_) {
  int so=x2.nelms_/x1.nelms_;
  if (x1.nelms_*so!=x2.nelms_) throw xPrecondFail();
  table y(x2,false);
  int j=0;
  real x1i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x1i=x1.linelms_[j++];
   y.linelms_[i]=x1i-x2.linelms_[i];
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else {
  table y(x1,true);
  for (int i=0; i<y.nelms_; i++)
  y.linelms_[i]-=x2.linelms_[i];
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } 
}

table operator-(real a,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]=a-x.linelms_[i];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator-(const table &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 table y(x,true);
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]-=a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator*(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.nelms_>x2.nelms_) {
  int so=x1.nelms_/x2.nelms_;
  if (x1.nelms_!=x2.nelms_*so) throw xPrecondFail();
  table y(x1,true);
  int j=0;
  real x2i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x2i=x2.linelms_[j++];
   y.linelms_[i]*=x2i;
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else
 if (x1.nelms_<x2.nelms_) {
  int so=x2.nelms_/x1.nelms_;
  if (x1.nelms_*so!=x2.nelms_) throw xPrecondFail();
  table y(x2,false);
  int j=0;
  real x1i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x1i=x1.linelms_[j++];
   y.linelms_[i]=x1i*x2.linelms_[i];
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else {
  table y(x1,true);
  for (int i=0; i<y.nelms_; i++)
  y.linelms_[i]*=x2.linelms_[i];
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } 
}

table operator*(real a,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]=a*x.linelms_[i];
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator*(const table &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 table y(x,true);
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]*=a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator/(const table &x1,const table &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op/(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.nelms_>x2.nelms_) {
  int so=x1.nelms_/x2.nelms_;
  if (x1.nelms_!=x2.nelms_*so) throw xPrecondFail();
  table y(x1,true);
  int j=0;
  real x2i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x2i=x2.linelms_[j++];
   if (x2i!=0.0) y.linelms_[i]/=x2i;
   else y.linelms_[i]=0.0;
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else
 if (x1.nelms_<x2.nelms_) {
  int so=x2.nelms_/x1.nelms_;
  if (x1.nelms_*so!=x2.nelms_) throw xPrecondFail();
  table y(x2,false);
  int j=0;
  real x1i;
  for (int i=0; i<y.nelms_; i++) {
   if (i%so==0) x1i=x1.linelms_[j++];
   real x2i=x2.linelms_[i];
   y.linelms_[i] = x2i==0.0 ? 0.0 : x1i/x2i;
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } else {
  table y(x1,true);
  for (int i=0; i<y.nelms_; i++) {
   real x2i=x2.linelms_[i];
   if (x2i!=0.0) y.linelms_[i]/=x2i;
   else y.linelms_[i]=0.0;
  }
#ifdef SOURCE_DEBUG
  cdeb << "y = " << y << endl;
#endif
  return table(y);
 } 
}

table operator/(real a,const table &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op/(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 table y(x,false);
 for (int i=0; i<y.nelms_; i++) {
  real xi=x.linelms_[i];
  y.linelms_[i] = xi==0.0 ? 0.0 : a/xi;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table operator/(const table &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op/(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 table y(x,false);
 if (a!=0.0)
 for (int i=0; i<y.nelms_; i++)
 y.linelms_[i]=x.linelms_[i]/a;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

void table::print_conts(ostream &out) const
{
 if (ndims_<=0) {
  out << *linelms_;
  return;
 }
 const int nshr=10;
 bool isshr = iosex::isshortened(out);
 bool isdimshr = isshr && ndims_>nshr;
 int n = isdimshr ? nshr : ndims_;
 bool *issizshr=new bool[n];
 for (int l=0; l<n; l++) {
  int siz=dimsizs_[l]; 
  issizshr[l] = isshr && siz>nshr;
 }
 int *cnt=new int[n];
 memset(cnt,0,n*sizeof(int));
 int wid=out.precision()+5;
 int k=0;
 int i=0;
 do {
  for (; k<n; k++) {
   cnt[k] = issizshr[k] ? nshr : dimsizs_[k];
   if (k==0) out << '{';
   else if (k==1) out << endl << "\t  {";
   else out << " {";
  }
  out << ' ' << setw(wid) << linelms_[i++];
  k--;
  while (k>=0) {
   cnt[k]--;
   if (cnt[k]>0) break;
   else {
    if (k>0 || n<=1) {
     if (issizshr[k]) out << " ...";
     out << " }";
    } else {
     if (issizshr[k]) out << endl << "\t  ...";
     out << endl << "\t}";
    } 
    k--;
   }
  }
  k++;
 } while (k>0);
 if (isdimshr) out << "...";
 delete[] cnt;
 delete[] issizshr;
} 

namespace { namespace table_ {

struct sortitem_rec_ {
 typedef sortitem_rec_ rec;
 int idx;
 real val;
 static int decord(const void *x1,const void *x2);
 static int incord(const void *x1,const void *x2);
};

int sortitem_rec_::decord(const void *x1,const void *x2)
{
 const rec &r1=*reinterpret_cast<const rec *>(x1);
 const rec &r2=*reinterpret_cast<const rec *>(x2);
 const real eps=1e5*rPrec; // eps<1.0
 return r1.val<r2.val-eps ? +1 : r1.val>r2.val+eps ? -1 : 0;
}

int sortitem_rec_::incord(const void *x1,const void *x2)
{
 const rec &r1=*reinterpret_cast<const rec *>(x1);
 const rec &r2=*reinterpret_cast<const rec *>(x2);
 const real eps=1e5*rPrec; // eps<1.0
 return r1.val<r2.val-eps ? -1 : r1.val>r2.val+eps ? +1 : 0;
}

}}

void table::print_sorts(ostream &out) const
{
 if (nelms_<=1) {
  out << *linelms_;
  return;
 }
 using namespace table_;
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && nelms_>nshr;
 sortitem_rec_ *itms=new sortitem_rec_[nelms_];
 for (int i=0; i<nelms_; i++) {
  sortitem_rec_ &itm=itms[i];
  itm.idx=i+1;
  itm.val=linelms_[i];
 }
 qsort(itms,nelms_,sizeof(sortitem_rec_),sortitem_rec_::incord);
 int n = isshr ? nshr : nelms_;
 int wid=out.precision()+5;
 out << '{' << endl << "\t  ";
 for (int i=0; i<n; i++) {
  const sortitem_rec_ &itm=itms[i];
  if (i>0) out << setw(wid+1) << ',' << ' ';
  out << setw(3) << itm.idx << ": ";
 }
 if (isshr) out << " ...";
 out << endl << "\t  ";
 for (int i=0; i<n; i++) {
  const sortitem_rec_ &itm=itms[i];
  if (i>0) out << ", ";
  out << "   " << "  " << setw(wid) << itm.val;
 }
 if (isshr) out << " ...";
 out << endl << "\t}";
 delete[] itms;
} 

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

