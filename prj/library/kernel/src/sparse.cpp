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
/// Definitions For Sparse Collection
/**
 * @file       sparse.cpp
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
#include "dfile.h"
#include "albas.h"
#include "altls.h"
#include "sparse.h"

namespace InfoSelAPI {
 
namespace Algorithms {
 
namespace Tools {

/////////////////////////////
////
//// sparse class definition
////

sparse::sparse(int rank,const inttuple sizes[],int ndirs):
 lnkcnt_(new int(1)),
 lnkelms_(0), nelms_(0),
 isnullable_(false)
{
 dirtab_.dimsizs=0; dirtab_.dimbass=0; 
 dirtab_.llnelms=0; dirtab_.nelms=0;
 rank_ = rank<0 ? 0 : rank;
 rnksizs_ = rank_>0 ? new inttuple[rank_] : 0;
 ndims_=0;
 for (int k=0; k<rank_; k++) {
  const inttuple &sk=sizes[k];
  if (sk.count()<=0) throw xPrecondFail();
  ndims_+=sk.count();
  rnksizs_[k]=sk;
 }
 dirtab_.ndims=ndirs; 
 curdmids_ = ndims_>0 ? new int[ndims_] : 0;
}

sparse::sparse(int rank1,int rank2,const inttuple sizes1[],const inttuple sizes2[],int ndirs1,int ndirs2):
 lnkcnt_(new int(1)),
 lnkelms_(0), nelms_(0),
 isnullable_(false)
{
 dirtab_.dimsizs=0; dirtab_.dimbass=0; 
 dirtab_.llnelms=0; dirtab_.nelms=0;
 int rank=rank1+rank2;
 rank_ = rank<0 ? 0 : rank;
 rnksizs_ = rank_>0 ? new inttuple[rank_] : 0;
 ndims_=0;
 int k=0;
 for (int k1=0; k1<rank1; k1++) {
  const inttuple &sk=sizes1[k1];
  if (sk.count()<=0) throw xPrecondFail();
  ndims_+=sk.count();
  rnksizs_[k++]=sk;
 }
 for (int k2=0; k2<rank2; k2++) {
  const inttuple &sk=sizes1[k2];
  if (sk.count()<=0) throw xPrecondFail();
  ndims_+=sk.count();
  rnksizs_[k++]=sk;
 }
 dirtab_.ndims = rank1>0 ? ndirs1 : ndirs2; 
 curdmids_ = ndims_>0 ? new int[ndims_] : 0;
}

sparse::sparse():
 lnkcnt_(new int(1)),
 rank_(0), rnksizs_(0), ndims_(0),
 lnkelms_(0), nelms_(0), curdmids_(0),
 isnullable_(false)
{
 dirtab_.ndims=0; dirtab_.dimsizs=0; dirtab_.dimbass=0; 
 dirtab_.llnelms=0; dirtab_.nelms=0;
}

void sparse::construct_(int rank,...) //! \pre rank>0 , count(...sk...)>=rank , 0 <= k < rank  \pre (asserted) sk.count>0
{
 lnkcnt_=new int(1);
 dirtab_.dimsizs=0; dirtab_.dimbass=0; 
 dirtab_.llnelms=0; dirtab_.nelms=0;
 lnkelms_=0; nelms_=0;
 rank_=rank;
 rnksizs_=new inttuple[rank_];
 ndims_=0;
 va_list ap;
 va_start(ap,rank);
 for (int k=0; k<rank_; k++) {
  const inttuple &sk=*va_arg(ap,const inttuple *);
  if (sk.count()<=0) throw xPrecondFail();
  ndims_+=sk.count(); 
  rnksizs_[k]=sk;
 }
 va_end(ap);
 dirtab_.ndims=1; 
 curdmids_=new int[ndims_];
 isnullable_=false;
}

void sparse::create_llnelms_() const //! \pre dirtab_.llnelms==0
{
 dirtab_.ndims = rank_>0 ? (dirtab_.ndims<0 ? 1 : dirtab_.ndims>=ndims_ ? ndims_ : dirtab_.ndims) : 0;
 dirtab_.dimsizs = dirtab_.ndims>0 ? new int[dirtab_.ndims] : 0;
 dirtab_.nelms=1;
 int l=0;
 for (int k=0; k<rank_ && l<dirtab_.ndims; k++) {
  const inttuple &sk=rnksizs_[k];
  int m=sk.count();
  for (int q=0; q<m && l<dirtab_.ndims; q++) {
   int skq=sk[q];
   dirtab_.dimsizs[l++]=skq;
   if (!chkmul(dirtab_.nelms,dirtab_.nelms,skq)) throw xSparseDirOverflow();
  }
 }
 dirtab_.llnelms=new elem_list_[dirtab_.nelms];
 memset(dirtab_.llnelms,0,dirtab_.nelms*sizeof(elem_list_));
 if (dirtab_.ndims>0) {
  dirtab_.dimbass=new int[dirtab_.ndims];
  l=dirtab_.ndims-1;
  dirtab_.dimbass[l--]=1;
  for (; l>=0; l--)
  dirtab_.dimbass[l]=dirtab_.dimsizs[l+1]*dirtab_.dimbass[l+1]; // dirtab_.dimbass[l]<dirtab_.nelms
 }
}
  
void sparse::create_lnkelms_() const //! \pre lnkelms==0
{
 if (dirtab_.llnelms==0) create_llnelms_();
 elem_rec_ *F=0, *L=0;
 for (int i=0; i<dirtab_.nelms; i++) {
  const elem_list_ &elms=dirtab_.llnelms[i];
  if (elms.first!=0) {
   if (L!=0) L->next=elms.first;
   else F=elms.first;
   L=elms.last;
  }
 }
 if (L!=0) L->next=0;
 lnkelms_=F;
}

sparse::sparse(const sparse &spr,bool cpy):
 lnkcnt_(new int(1)),
 rank_(spr.rank_), ndims_(spr.ndims_),
 lnkelms_(0), nelms_(0),
 isnullable_(spr.isnullable_)
{
 dirtab_.ndims=spr.dirtab_.ndims; dirtab_.dimsizs=0; dirtab_.dimbass=0; 
 dirtab_.llnelms=0; dirtab_.nelms=0;
 rnksizs_ = rank_>0 ? new inttuple[rank_] : 0; 
 for (int k=0; k<rank_; k++) rnksizs_[k]=spr.rnksizs_[k]; 
 curdmids_ = ndims_>0 ? new int[ndims_] : 0;
 if (cpy) {
  if (spr.dirtab_.llnelms==0) spr.create_llnelms_();
  create_llnelms_(); 
  for (int i=0; i<spr.dirtab_.nelms; i++) { 
   const elem_list_ &sprelms=spr.dirtab_.llnelms[i];
   if (sprelms.first!=0) {
    elem_list_ &elms=dirtab_.llnelms[i];
    const elem_rec_ *P=sprelms.first, *L=sprelms.last;
    elem_rec_ *Q=0;
    for (;;) {
     elem_rec_ *e=new elem_rec_;
     e->next=0;
     if (Q!=0) Q->next=e;
     else elms.first=e;
     e->inval=P->inval;
     e->dmids = ndims_>0 ? new int[ndims_] : 0;
     for (int h=0; h<ndims_; h++) e->dmids[h]=P->dmids[h];
     Q=e;
     if (P==L) break;
     P=P->next;
    }
    elms.last=Q;
   }
  }
  nelms_=spr.nelms_;
 }
}

sparse::sparse(const sparse &spr):
 lnkcnt_(spr.lnkcnt_), rank_(spr.rank_),
 rnksizs_(spr.rnksizs_), ndims_(spr.ndims_),
 dirtab_(spr.dirtab_), lnkelms_(spr.lnkelms_), 
 nelms_(spr.nelms_), curdmids_(spr.curdmids_),
 isnullable_(spr.isnullable_)
{
 (*lnkcnt_)++;
}

void sparse::destroy_allelms_()
{
 if (lnkelms_==0) create_lnkelms_();
 while (lnkelms_) { 
  elem_rec_ *P=lnkelms_; 
  lnkelms_=lnkelms_->next; 
  delete[] P->dmids; 
  delete P; 
 }
 nelms_=0;
 delete[] dirtab_.llnelms;
 dirtab_.llnelms=0;
 dirtab_.nelms=0;
 delete[] dirtab_.dimbass;
 dirtab_.dimbass=0;
 delete[] dirtab_.dimsizs;
 dirtab_.dimsizs=0;
}

sparse::~sparse()
{
 if (--(*lnkcnt_) <= 0) {
  delete[] curdmids_;
  destroy_allelms_();
  delete[] rnksizs_;
  delete lnkcnt_;
 }
}

sparse &sparse::operator=(const sparse &spr)
{
 if (--(*lnkcnt_) <= 0) {
  delete[] curdmids_;
  destroy_allelms_();
  delete[] rnksizs_;
  delete lnkcnt_;
 }
 
 lnkcnt_=spr.lnkcnt_; rank_=spr.rank_;
 rnksizs_=spr.rnksizs_; ndims_=spr.ndims_;
 dirtab_=spr.dirtab_; lnkelms_=spr.lnkelms_; 
 nelms_=spr.nelms_; curdmids_=spr.curdmids_;
 isnullable_=spr.isnullable_;
 
 (*lnkcnt_)++;
 return *this;
}

bool sparse::isshared() const
{ 
 return (*lnkcnt_)>1; 
}

sparse &sparse::clone()
{
 if ((*lnkcnt_)>1) *this = sparse(*this,true);
 return *this;
}

sparse &sparse::dilute()
{
 clone();
 int n=ndims_;
 if (n<=0) {
  access_curelem_();
  return *this;
 }
 int *ss=new int[n];
 int h=0;
 for (int k=0; k<rank_; k++) {
  const inttuple &sk=rnksizs_[k];
  int m=sk.count();
  for (int q=0; q<m; q++) ss[h++]=sk[q];
 }
 int *ii=new int[n];
 memset(ii,0,n*sizeof(int));
 for (;;) {
  memcpy(curdmids_,ii,n*sizeof(int));
  access_curelem_();
  int h=n-1;
  do {
   ii[h]++;
   if (ii[h]<ss[h]) break;
   else ii[h--]=0;
  } while (h>=0);
  if (h<0) break;
 }
 delete[] ii;
 delete[] ss;
 return *this;
}

sparse &sparse::thicken()
{
 clone();
 if (dirtab_.llnelms==0) create_llnelms_();
 for (int i=0; i<dirtab_.nelms; i++) {
  elem_list_ &elms=dirtab_.llnelms[i];
  if (elms.first!=0) {
   elms.last->next=0;
   elem_rec_ *P=elms.first, *Q=0;
   while (Q!=elms.last) {
    elem_rec_ *e=P;
    P=P->next;
    if (e->inval!=0.0) Q=e;
    else {
     if (Q!=0) Q->next=P;
     else elms.first=P;
     if (e==elms.last) elms.last=Q;
     delete[] e->dmids; 
     delete e; 
     nelms_--;
    }
   }
  }
 }
 lnkelms_=0;
 return *this;
}

int sparse::ndims(int r) const
{ 
 if (r<0) r=0;
 int k=rank_;
 int no=0;
 do {
  if (k<=r) return ndims_-no;
  int m=rnksizs_[--k].count();
  no+=m;
 } while (true);
}

int sparse::size(int h) const
{
 int s=-1;
 int r=rank_;
 for (int k=0; k<r; k++) {
  int m=rnksizs_[k].count();
  int q=h;
  if (q<m) {
   s=rnksizs_[k][q]; 
   break;
  }
  h-=m;
 }
 return s;
}

real &sparse::linelms_(int G) const //! \pre rank>0 && 0 <= G < count , rank<=0 && G==0
{
 elem_rec_ *P=lkelems_();
 while (--G>=0) P=P->next;
 return P->inval;
} 

real *sparse::linelms_() const
{
 elem_rec_ *P=lkelems_();
 if (P==0) { // curdmids_!=0
  memset(curdmids_,0,ndims_*sizeof(int));
  P=&access_curelem_();
 } 
 return &P->inval;
} 

real &sparse::rnkelms_(const inttuple *i0...) const //! \pre rank>0 , count(i0...ik...)>=rank , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
{
 va_list ap;
 va_start(ap,i0);
 int h=0;
 for (int k=0; k<rank_; k++) {
  const inttuple &ik=*(k==0 ? i0 : va_arg(ap,const inttuple *));
  int m=rnksizs_[k].count(); // m<=ik.count
  for (int q=0; q<m; q++) curdmids_[h++]=ik[q];
 }
 va_end(ap);
 return access_curelem_().inval;
}

real &sparse::dimelms_(const inttuple &ii) const //! \pre ii.count>=ndims , 0 <= ii[h] < size(h) , 0 <= h < ndims
{
 if (curdmids_!=0) memcpy(curdmids_,ii.c_ptr(),ndims_*sizeof(int));
 return access_curelem_().inval;
}

sparse::elem_rec_ &sparse::dmelems_(const int ii[]) const //! \pre count(ii)>=ndims , 0 <= ii[h] < size(h) , 0 <= h < ndims
{
 if (curdmids_!=0) memcpy(curdmids_,ii,ndims_*sizeof(int));
 return access_curelem_();
}

sparse::elem_rec_ &sparse::dmelems_(int n1,int n2,const int ii1[],const int ii2[]) const //! \pre n1+n2==ndims , count(ii1)>=n1 , count(ii2)>=n2 , 0 <= ii12[h] < size(h) , 0 <= h < ndims
{
 int h=0;
 for (int h1=0; h1<n1; h1++) curdmids_[h++]=ii1[h1];
 for (int h2=0; h2<n2; h2++) curdmids_[h++]=ii2[h2];
 return access_curelem_();
}

sparse::elem_rec_ *sparse::lkelems_() const
{ 
 if (rank_<=0 && nelms_<=0) access_curelem_();
 if (lnkelms_==0) create_lnkelms_(); 
 return lnkelms_; 
}

sparse::elem_rec_ &sparse::access_curelem_() const //! \pre curdmids==0 <=> ndirs<=ndims<=0
{
 if (dirtab_.llnelms==0) create_llnelms_();
 int *ii=curdmids_;
 int id=0;
 for (int kd=0; kd<dirtab_.ndims; kd++)
 id+=ii[kd]*dirtab_.dimbass[kd];
 elem_list_ &elms=dirtab_.llnelms[id];

 elem_rec_ *P=elms.first;
 if (P==0) {
  elem_rec_ *e=new elem_rec_;
  if (!chkinc(nelms_)) throw xSparseCountOverflow();
  e->next=0;
  e->dmids=ii;
  e->inval=0.0;
  elms.first=e;
  elms.last=e;
  if (curdmids_!=0) curdmids_=new int[ndims_];
  lnkelms_=0;
  return *e;
 } else 
 if (ii==0) return *P;
 else { // ndims>0
  elem_rec_ *F=P, *Q=0;
  elem_rec_ *L=elms.last;
  P=L;
  for (;;) {
   int *jj=P->dmids;
   int h=dirtab_.ndims;
   for (;;) {
    if (h>=ndims_) return *P;
    int ih=ii[h], jh=jj[h];
    if (jh==ih) h++;
    else 
    if (jh<ih) {
     if (P==L) {
      elem_rec_ *e=new elem_rec_;
      if (!chkinc(nelms_)) throw xSparseCountOverflow();
      e->next=0;
      P->next=e;
      e->dmids=ii;
      e->inval=0.0;
      elms.last=e;
      curdmids_=new int[ndims_];
      lnkelms_=0;
      return *e;
     }
     Q=P;
     P=P->next;
     break;
    } else 
    if (F!=0) {
     P=F;
     F=0;
     break;
    } else {
     elem_rec_ *e=new elem_rec_;
     if (!chkinc(nelms_)) throw xSparseCountOverflow();
     e->next=P;
     if (Q!=0) Q->next=e;
     else elms.first=e;
     e->dmids=ii;
     e->inval=0.0;
     curdmids_=new int[ndims_];
     lnkelms_=0;
     return *e;
    }
   }
  }
 }
}

sparse &sparse::zero()
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=zero(x):" << endl;
 cdeb << "x = " << *this << endl;
#endif
 destroy_allelms_();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

sparse &sparse::unit()
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=unit(x):" << endl;
 cdeb << "x = " << *this << endl;
#endif
 bool nonull = !isnullable_;
 if (nonull) dilute();
 elem_rec_ *P=lkelems_();
 while (P) {
  P->inval=1.0;
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

sparse &sparse::rand()
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=rand(x):" << endl;
 cdeb << "x = " << *this << endl;
#endif
 bool nonull = !isnullable_;
 if (nonull) dilute();
 elem_rec_ *P=lkelems_();
 while (P) {
  P->inval=Common::unirand();
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

sparse &sparse::operator=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=x.op=(a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 if (a==0.0) zero();
 else {
  bool nonull = !isnullable_;
  if (nonull) dilute();
  elem_rec_ *P=lkelems_();
  while (P) {
   P->inval=a;
   P=P->next;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

bool operator==(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "ret=op==(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.rank_!=x2.rank_ || x1.ndims_!=x2.ndims_) throw xPrecondFail();
 bool ret=true;
 int n=x1.ndims_;
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 const sparse::elem_rec_ *P1=x1.lkelems_();
 const sparse::elem_rec_ *P2=x2.lkelems_();
 while (ret && (P1 || P2))
 if (P2==0) {
  if (nonull) ret = P1->inval==0.0;
  P1=P1->next;
 } else 
 if (P1==0) {
  if (nonull) ret = 0.0==P2->inval;
  P2=P2->next;
 } else {
  const int *ii1=P1->dmids;
  const int *ii2=P2->dmids;
  int h=0;
  for (;;) {
   if (h>=n) {
    ret = P1->inval==P1->inval;
    P1=P1->next;
    P2=P2->next;
    break;
   }
   if (ii1[h]==ii2[h]) h++;
   else 
   if (ii1[h]<ii2[h]) {
    if (nonull) ret = P1->inval==0.0;
    P1=P1->next;
    break;
   } else {
    if (nonull) ret = 0.0==P2->inval;
    P2=P2->next;
    break;
   }
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "ret = " << ret << endl;
#endif
 return ret;
}

bool operator==(real a,const sparse &x_)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "ret=op==(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 bool nonull = a!=0.0 && !x.isnullable_;
 if (nonull) x.dilute();
 bool ret=true;
 const sparse::elem_rec_ *P=x.lkelems_();
 while (ret && P) {
  ret = a==P->inval;
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "ret = " << ret << endl;
#endif
 return ret;
}

bool operator==(const sparse &x_,real a)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "ret=op==(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 bool nonull = a!=0.0 && !x.isnullable_;
 if (nonull) x.dilute();
 bool ret=true;
 const sparse::elem_rec_ *P=x.lkelems_();
 while (ret && P) {
  ret = P->inval==a;
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "ret = " << ret << endl;
#endif
 return ret;
}

sparse &sparse::operator+=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=x.op+=(a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 bool nonull = !isnullable_;
 if (nonull) dilute();
 elem_rec_ *P=lkelems_();
 while (P) {
  P->inval+=a;
  P=P->next;
 }
 if (nonull) thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

sparse &sparse::operator-=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=x.op-=(a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 bool nonull = !isnullable_;
 if (nonull) dilute();
 elem_rec_ *P=lkelems_();
 while (P) {
  P->inval-=a;
  P=P->next;
 }
 if (nonull) thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

sparse &sparse::operator*=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=x.op*=(a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 if (a==0.0) zero();
 else {
  elem_rec_ *P=lkelems_();
  while (P) {
   P->inval*=a;
   P=P->next;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

sparse &sparse::operator/=(real a)
{
 clone();
#ifdef SOURCE_DEBUG
 cdbg << "y=x.op/=(a):" << endl;
 cdeb << "x = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif
 if (a==0.0) zero();
 else {
  elem_rec_ *P=lkelems_();
  while (P) {
   P->inval/=a;
   P=P->next;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << *this << endl;
#endif
 return *this;
}

sparse abs(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=abs(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v = Common::abs(v);
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse log(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=log(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v = v!=0.0 ? std::log(v) : 0.0;
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse log2(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=log2(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v = v!=0.0 ? Common::log2(v) : 0.0;
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse sqr(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=sqr(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v = Common::sqr(v);
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse sqrt(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=sqrt(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v = v<0.0 ? 0.0 : std::sqrt(v);
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse cub(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=cub(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v = Common::cub(v);
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse pow(const sparse &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=pow(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v = std::pow(v,a);
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}
     
sparse cum(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=cum(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 int no=x.ndims(x.rank_-1);
 bool nonull = !y.isnullable_;
 if (nonull) y.dilute();
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  const int *iio=P->dmids;
  real uo=P->inval;
  P=P->next;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   uo=(P->inval+=uo);
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse sum(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=sum(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const int *iio=P->dmids;
  sparse::elem_rec_ &e=y.dmelems_(iio);
  real uo=P->inval;
  P=P->next;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   uo+=P->inval;
   P=P->next;
  }
  e.inval=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse avg(const sparse &x_)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=avg(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const int *iio=P->dmids;
  sparse::elem_rec_ &e=y.dmelems_(iio);
  real uo=P->inval;
  P=P->next;
  int No=1;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   uo+=P->inval;
   No++;
   P=P->next;
  }
  e.inval=uo/No;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse prod(const sparse &x_)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=prod(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const int *iio=P->dmids;
  sparse::elem_rec_ &e=y.dmelems_(iio);
  real uo=P->inval;
  P=P->next;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   uo*=P->inval;
   P=P->next;
  }
  e.inval=uo;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse max(const sparse &x_)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=max(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const int *iio=P->dmids;
  sparse::elem_rec_ &e=y.dmelems_(iio);
  real uo=P->inval;
  P=P->next;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval>uo) uo=P->inval;
   P=P->next;
  }
  e.inval=uo;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse min(const sparse &x_)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=min(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const int *iio=P->dmids;
  sparse::elem_rec_ &e=y.dmelems_(iio);
  real uo=P->inval;
  P=P->next;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval<uo) uo=P->inval;
   P=P->next;
  }
  e.inval=uo;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse max_at(const sparse &x_,const inttuple &io)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=max_at(x,io):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "io = " << io << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 int mo=Common::min2(x.ndims_-no,io.count());
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  P=P->next;
  const int *iio=Q->dmids;
  real uo=Q->inval;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval>uo) uo=P->inval;
   P=P->next;
  }
  P=Q;
  bool done=false;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (!done && P->inval==uo) {
    int q=0;
    while (q<mo && io[q]==ii[no+q]) q++;
    if (q>=mo) {
     sparse::elem_rec_ &e=y.dmelems_(ii);
     e.inval=uo;
     done=true;
    }
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse min_at(const sparse &x_,const inttuple &io)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=min_at(x,io):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "io = " << io << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 int mo=Common::min2(x.ndims_-no,io.count());
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  P=P->next;
  const int *iio=Q->dmids;
  real uo=Q->inval;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval<uo) uo=P->inval;
   P=P->next;
  }
  P=Q;
  bool done=false;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (!done && P->inval==uo) {
    int q=0;
    while (q<mo && io[q]==ii[no+q]) q++;
    if (q>=mo) {
     sparse::elem_rec_ &e=y.dmelems_(ii);
     e.inval=uo;
     done=true;
    }
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse max_nat(const sparse &x_,const inttuple &io)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=max_nat(x,io):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "io = " << io << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 int mo=Common::min2(x.ndims_-no,io.count());
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  P=P->next;
  const int *iio=Q->dmids;
  real uo=Q->inval;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval>uo) uo=P->inval;
   P=P->next;
  }
  P=Q;
  bool done=false;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (!done && P->inval==uo) {
    int q=0;
    while (q<mo && io[q]==ii[no+q]) q++;
    if (q<mo) {
     sparse::elem_rec_ &e=y.dmelems_(ii);
     e.inval=uo;
     done=true;
    }
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse min_nat(const sparse &x_,const inttuple &io)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=min_nat(x,io):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "io = " << io << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x.rank_-1,x.rnksizs_,x.dirtab_.ndims);
 int no=y.ndims_;
 int mo=Common::min2(x.ndims_-no,io.count());
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  P=P->next;
  const int *iio=Q->dmids;
  real uo=Q->inval;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval<uo) uo=P->inval;
   P=P->next;
  }
  P=Q;
  bool done=false;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (!done && P->inval==uo) {
    int q=0;
    while (q<mo && io[q]==ii[no+q]) q++;
    if (q<mo) {
     sparse::elem_rec_ &e=y.dmelems_(ii);
     e.inval=uo;
     done=true;
    }
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse rgn_max(const sparse &x_)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=rgn_max(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x,false);
 int no=x.ndims(x.rank_-1);
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  P=P->next;
  const int *iio=Q->dmids;
  real uo=Q->inval;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && iio[h]==ii[h]) h++;
   if (h<no) break;
   if (P->inval>uo) uo=P->inval;
   P=P->next;
  }
  P=Q;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && iio[h]==ii[h]) h++;
   if (h<no) break;
   if (P->inval==uo) {
    sparse::elem_rec_ &e=y.dmelems_(ii);
    e.inval=uo;
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse rgn_min(const sparse &x_)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=rgn_min(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x,false);
 int no=x.ndims(x.rank_-1);
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  P=P->next;
  const int *iio=Q->dmids;
  real uo=Q->inval;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval<uo) uo=P->inval;
   P=P->next;
  }
  P=Q;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval==uo) {
    sparse::elem_rec_ &e=y.dmelems_(ii);
    e.inval=uo;
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse rgn_eq(const sparse &x_,real uo)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=rgn_eq(x,uo):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "uo = " << uo << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x,false);
 int no=x.ndims(x.rank_-1);
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  const int *iio=Q->dmids;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval==uo) {
    sparse::elem_rec_ &e=y.dmelems_(ii);
    e.inval=uo;
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse rgn_neq(const sparse &x_,real uo)
{
 sparse x=x_;
#ifdef SOURCE_DEBUG
 cdbg << "y=rgn_neq(x,uo):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "uo = " << uo << endl;
#endif
 bool nonull = !x.isnullable_;
 if (nonull) x.dilute();
 sparse y(x,false);
 int no=x.ndims(x.rank_-1);
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const sparse::elem_rec_ *Q=P;
  const int *iio=Q->dmids;
  while (P) {
   const int *ii=P->dmids;
   int h=0;
   while (h<no && ii[h]==iio[h]) h++;
   if (h<no) break;
   if (P->inval!=uo) {
    sparse::elem_rec_ &e=y.dmelems_(ii);
    e.inval=P->inval;
   }
   P=P->next;
  }
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse max2(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=max2(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.rank_!=x2.rank_ || x1.ndims_!=x2.ndims_) throw xPrecondFail();
 sparse y(x1,false);
 int n=x1.ndims_;
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 const sparse::elem_rec_ *P1=x1.lkelems_();
 const sparse::elem_rec_ *P2=x2.lkelems_();
 while (P1 || P2)
 if (P2==0) {
  if (nonull) y.dmelems_(P1->dmids).inval=Common::max2(P1->inval,0.0);
  P1=P1->next;
 } else 
 if (P1==0) {
  if (nonull) y.dmelems_(P2->dmids).inval=Common::max2(0.0,P2->inval);
  P2=P2->next;
 } else {
  const int *ii1=P1->dmids;
  const int *ii2=P2->dmids;
  int h=0;
  for (;;) {
   if (h>=n) {
    y.dmelems_(ii1).inval=Common::max2(P1->inval,P2->inval);
    P1=P1->next;
    P2=P2->next;
    break;
   }
   if (ii1[h]==ii2[h]) h++;
   else 
   if (ii1[h]<ii2[h]) {
    if (nonull) y.dmelems_(ii1).inval=Common::max2(P1->inval,0.0);
    P1=P1->next;
    break;
   } else {
    if (nonull) y.dmelems_(ii2).inval=Common::max2(0.0,P2->inval);
    P2=P2->next;
    break;
   }
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse min2(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=min2(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.rank_!=x2.rank_ || x1.ndims_!=x2.ndims_) throw xPrecondFail();
 sparse y(x1,false);
 int n=x1.ndims_;
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 const sparse::elem_rec_ *P1=x1.lkelems_();
 const sparse::elem_rec_ *P2=x2.lkelems_();
 while (P1 || P2)
 if (P2==0) {
  if (nonull) y.dmelems_(P1->dmids).inval=Common::min2(P1->inval,0.0);
  P1=P1->next;
 } else 
 if (P1==0) {
  if (nonull) y.dmelems_(P2->dmids).inval=Common::min2(0.0,P2->inval);
  P2=P2->next;
 } else {
  const int *ii1=P1->dmids;
  const int *ii2=P2->dmids;
  int h=0;
  for (;;) {
   if (h>=n) {
    y.dmelems_(ii1).inval=Common::min2(P1->inval,P2->inval);
    P1=P1->next;
    P2=P2->next;
    break;
   }
   if (ii1[h]==ii2[h]) h++;
   else 
   if (ii1[h]<ii2[h]) {
    if (nonull) y.dmelems_(ii1).inval=Common::min2(P1->inval,0.0);
    P1=P1->next;
    break;
   } else {
    if (nonull) y.dmelems_(ii2).inval=Common::min2(0.0,P2->inval);
    P2=P2->next;
    break;
   }
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse diag(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diag(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int r=x.rank_-1;
 sparse y(r,x.rnksizs_,x.dirtab_.ndims);
 int n=x.ndims_;
 int ma = r>=0 ? x.rnksizs_[r].count() : 0;
 int no=n-ma;
 int mb = r>=1 ? x.rnksizs_[r-1].count() : 0;
 int mo=Common::min2(ma,mb);
 const sparse::elem_rec_ *P=x.lkelems_();
 while (P) {
  const int *ii=P->dmids;
  int q=1;
  while (q<=mo && ii[no-q]==ii[n-q]) q++;
  if (q>mo) {
   y.dmelems_(ii).inval=P->inval;
   if (mo<=0) break;
  } 
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse diagadd(const sparse &x1_,const sparse &x2_)
{
 sparse x1=x1_;
 sparse x2=x2_;
#ifdef SOURCE_DEBUG
 cdbg << "y=diagadd(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 if (nonull) {
  x1.dilute();
  x2.dilute();
 } 
 int r1=x1.rank_-1;
 int r2=x2.rank_-1;
 int m1 = r1>=0 ? x1.rnksizs_[r1].count() : 0;
 int m2 = r2>=0 ? x2.rnksizs_[r2].count() : 0;
 if (m1!=m2) throw xPrecondFail();
 sparse y(r1,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 int n1=x1.ndims_;
 int n2=x2.ndims_;
 int no1=n1-m1;
 int mo=m1;
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const int *ii1=P1->dmids;
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   const int *ii2=P2->dmids;
   int q=1;
   while (q<=mo && ii1[n1-q]==ii2[n2-q]) q++;
   if (q>mo) y.dmelems_(no1,n2,ii1,ii2).inval=P1->inval+P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse diagsub(const sparse &x1_,const sparse &x2_)
{
 sparse x1=x1_;
 sparse x2=x2_;
#ifdef SOURCE_DEBUG
 cdbg << "y=diagsub(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 if (nonull) {
  x1.dilute();
  x2.dilute();
 } 
 int r1=x1.rank_-1;
 int r2=x2.rank_-1;
 int m1 = r1>=0 ? x1.rnksizs_[r1].count() : 0;
 int m2 = r2>=0 ? x2.rnksizs_[r2].count() : 0;
 if (m1!=m2) throw xPrecondFail();
 sparse y(r1,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 int n1=x1.ndims_;
 int n2=x2.ndims_;
 int no1=n1-m1;
 int mo=m1;
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const int *ii1=P1->dmids;
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   const int *ii2=P2->dmids;
   int q=1;
   while (q<=mo && ii1[n1-q]==ii2[n2-q]) q++;
   if (q>mo) y.dmelems_(no1,n2,ii1,ii2).inval=P1->inval-P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse diagmul(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diagmul(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 int r1=x1.rank_-1;
 int r2=x2.rank_-1;
 int m1 = r1>=0 ? x1.rnksizs_[r1].count() : 0;
 int m2 = r2>=0 ? x2.rnksizs_[r2].count() : 0;
 if (m1!=m2) throw xPrecondFail();
 sparse y(r1,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 int n1=x1.ndims_;
 int n2=x2.ndims_;
 int no1=n1-m1;
 int mo=m1;
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const int *ii1=P1->dmids;
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   const int *ii2=P2->dmids;
   int q=1;
   while (q<=mo && ii1[n1-q]==ii2[n2-q]) q++;
   if (q>mo) y.dmelems_(no1,n2,ii1,ii2).inval=P1->inval*P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse diagdiv(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=diagdiv(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 int r1=x1.rank_-1;
 int r2=x2.rank_-1;
 int m1 = r1>=0 ? x1.rnksizs_[r1].count() : 0;
 int m2 = r2>=0 ? x2.rnksizs_[r2].count() : 0;
 if (m1!=m2) throw xPrecondFail();
 sparse y(r1,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 int n1=x1.ndims_;
 int n2=x2.ndims_;
 int no1=n1-m1;
 int mo=m1;
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const int *ii1=P1->dmids;
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   const int *ii2=P2->dmids;
   int q=1;
   while (q<=mo && ii1[n1-q]==ii2[n2-q]) q++;
   if (q>mo)
   if (P2->inval!=0.0) y.dmelems_(no1,n2,ii1,ii2).inval=P1->inval/P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse dyadadd(const sparse &x1_,const sparse &x2_)
{
 sparse x1=x1_;
 sparse x2=x2_;
#ifdef SOURCE_DEBUG
 cdbg << "y=dyadadd(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 if (nonull) {
  x1.dilute();
  x2.dilute();
 } 
 sparse y(x1.rank_,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   y.dmelems_(x1.ndims_,x2.ndims_,P1->dmids,P2->dmids).inval=P1->inval+P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse dyadsub(const sparse &x1_,const sparse &x2_)
{
 sparse x1=x1_;
 sparse x2=x2_;
#ifdef SOURCE_DEBUG
 cdbg << "y=dyadsub(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 if (nonull) {
  x1.dilute();
  x2.dilute();
 } 
 sparse y(x1.rank_,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   y.dmelems_(x1.ndims_,x2.ndims_,P1->dmids,P2->dmids).inval=P1->inval-P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse dyadmul(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=dyadmul(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 sparse y(x1.rank_,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   y.dmelems_(x1.ndims_,x2.ndims_,P1->dmids,P2->dmids).inval=P1->inval*P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse dyaddiv(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=dyaddiv(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 sparse y(x1.rank_,x2.rank_,x1.rnksizs_,x2.rnksizs_,x1.dirtab_.ndims,x2.dirtab_.ndims);
 const sparse::elem_rec_ *F1=x1.lkelems_();
 const sparse::elem_rec_ *F2=x2.lkelems_();
 const sparse::elem_rec_ *P1=F1;
 while (P1) {
  const sparse::elem_rec_ *P2=F2;
  while (P2) {
   if (P2->inval!=0.0) y.dmelems_(x1.ndims_,x2.ndims_,P1->dmids,P2->dmids).inval=P1->inval/P2->inval;
   P2=P2->next;
  }
  P1=P1->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator*(const sparse &x,const permutation &p)
{
 int r=x.rank_;
 if (r<2) return sparse(x,true);
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(x,p):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "p = " << p << endl;
#endif
 if (p.length()!=r) throw xPrecondFail();
 int n=x.ndims_;
 int *pp=new int[n];
 inttuple *pxsizs=new inttuple[r];
 {
  int *xndims=new int[r];
  int h=0;
  for (int k=0; k<r; k++) {
   const inttuple &s=x.rnksizs_[k];
   xndims[k]=h;
   h+=s.count();
  }
  h=0;
  for (int k=0; k<r; k++) {
   int l=p[k];
   const inttuple &ps=x.rnksizs_[l];
   int pn=xndims[l];
   int pm=ps.count();
   for (int q=0; q<pm; q++) pp[h++]=pn++;
   pxsizs[k]=ps;
  }
  delete[] xndims;
 } 
 sparse y(r,pxsizs,x.dirtab_.ndims);
 delete[] pxsizs;
 int *pii=new int[n];
 const sparse::elem_rec_ *G=x.lkelems_();
 while (G) {
  const int *ii=G->dmids;
  for (int h=0; h<n; h++) {
   int l=pp[h];
   pii[h]=ii[l];
  } 
  y.dmelems_(pii).inval=G->inval;
  G=G->next;
 }
 delete[] pii;
 delete[] pp;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator~(const sparse &x)
{
 int r=x.rank_;
 if (r<2) return sparse(x,true);
#ifdef SOURCE_DEBUG
 cdbg << "y=op~(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.ndims_;
 int *pp=new int[n];
 inttuple *pxsizs=new inttuple[r];
 {
  int *xndims=new int[r];
  int h=0;
  for (int k=0; k<r; k++) {
   const inttuple &s=x.rnksizs_[k];
   xndims[k]=h;
   h+=s.count();
  }
  h=0;
  for (int k=0; k<r; k++) {
   int l=r-1-k;
   const inttuple &ps=x.rnksizs_[l];
   int pn=xndims[l];
   int pm=ps.count();
   for (int q=0; q<pm; q++) pp[h++]=pn++;
   pxsizs[k]=ps;
  }
  delete[] xndims;
 } 
 sparse y(r,pxsizs,x.dirtab_.ndims);
 delete[] pxsizs;
 int *pii=new int[n];
 const sparse::elem_rec_ *G=x.lkelems_();
 while (G) {
  const int *ii=G->dmids;
  for (int h=0; h<n; h++) {
   int l=pp[h];
   pii[h]=ii[l];
  } 
  y.dmelems_(pii).inval=G->inval;
  G=G->next;
 }
 delete[] pii;
 delete[] pp;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator+(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op+(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.rank_!=x2.rank_ || x1.ndims_!=x2.ndims_) throw xPrecondFail();
 sparse y(x1,false);
 int n=x1.ndims_;
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 const sparse::elem_rec_ *P1=x1.lkelems_();
 const sparse::elem_rec_ *P2=x2.lkelems_();
 while (P1 || P2)
 if (P2==0) {
  if (nonull) y.dmelems_(P1->dmids).inval=P1->inval;
  P1=P1->next;
 } else 
 if (P1==0) {
  if (nonull) y.dmelems_(P2->dmids).inval=P2->inval;
  P2=P2->next;
 } else {
  const int *ii1=P1->dmids;
  const int *ii2=P2->dmids;
  int h=0;
  for (;;) {
   if (h>=n) {
    y.dmelems_(ii1).inval=P1->inval+P2->inval;
    P1=P1->next;
    P2=P2->next;
    break;
   }
   if (ii1[h]==ii2[h]) h++;
   else 
   if (ii1[h]<ii2[h]) {
    if (nonull) y.dmelems_(ii1).inval=P1->inval;
    P1=P1->next;
    break;
   } else {
    if (nonull) y.dmelems_(ii2).inval=P2->inval;
    P2=P2->next;
    break;
   }
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator+(real a,const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op+(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 bool nonull = !y.isnullable_;
 if (nonull) y.dilute();
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v=a+v;
  P=P->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator+(const sparse &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op+(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 sparse y(x,true);
 bool nonull = !y.isnullable_;
 if (nonull) y.dilute();
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  P->inval+=a;
  P=P->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator-(const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v=-v;
  P=P->next;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator-(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 if (x1.rank_!=x2.rank_ || x1.ndims_!=x2.ndims_) throw xPrecondFail();
 sparse y(x1,false);
 int n=x1.ndims_;
 bool nonull = !x1.isnullable_ || !x2.isnullable_;
 const sparse::elem_rec_ *P1=x1.lkelems_();
 const sparse::elem_rec_ *P2=x2.lkelems_();
 while (P1 || P2)
 if (P2==0) {
  if (nonull) y.dmelems_(P1->dmids).inval=P1->inval;
  P1=P1->next;
 } else 
 if (P1==0) {
  if (nonull) y.dmelems_(P2->dmids).inval=-P2->inval;
  P2=P2->next;
 } else {
  const int *ii1=P1->dmids;
  const int *ii2=P2->dmids;
  int h=0;
  for (;;) {
   if (h>=n) {
    y.dmelems_(ii1).inval=P1->inval-P2->inval;
    P1=P1->next;
    P2=P2->next;
    break;
   }
   if (ii1[h]==ii2[h]) h++;
   else 
   if (ii1[h]<ii2[h]) {
    if (nonull) y.dmelems_(ii1).inval=P1->inval;
    P1=P1->next;
    break;
   } else {
    if (nonull) y.dmelems_(ii2).inval=-P2->inval;
    P2=P2->next;
    break;
   }
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator-(real a,const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,true);
 bool nonull = !y.isnullable_;
 if (nonull) y.dilute();
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  real &v=P->inval;
  v=a-v;
  P=P->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator-(const sparse &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op-(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 sparse y(x,true);
 bool nonull = !y.isnullable_;
 if (nonull) y.dilute();
 sparse::elem_rec_ *P=y.lkelems_();
 while (P) {
  P->inval-=a;
  P=P->next;
 }
 if (nonull) y.thicken();
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator*(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 sparse y(x1.rank_>=x2.rank_ ? x1 : x2,false);
 int n=Common::min2(x1.ndims_,x2.ndims_);
 const sparse::elem_rec_ *P1=x1.lkelems_();
 const sparse::elem_rec_ *P2=x2.lkelems_();
 const sparse::elem_rec_ **Q = x1.rank_>=x2.rank_ ? &P1 : &P2;
 while (P1 && P2) {
  const int *ii1=P1->dmids;
  const int *ii2=P2->dmids;
  int h=0;
  for (;;) {
   if (h>=n) {
    y.dmelems_((*Q)->dmids).inval=P1->inval*P2->inval;
    (*Q)=(*Q)->next;
    break;
   }
   if (ii1[h]==ii2[h]) h++;
   else 
   if (ii1[h]<ii2[h]) {
    P1=P1->next;
    break;
   } else {
    P2=P2->next;
    break;
   }
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator*(real a,const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,a!=0.0);
 if (a!=0.0) {
  sparse::elem_rec_ *P=y.lkelems_();
  while (P) {
   real &v=P->inval;
   v=a*v;
   P=P->next;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator*(const sparse &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op*(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 sparse y(x,a!=0.0);
 if (a!=0.0) {
  sparse::elem_rec_ *P=y.lkelems_();
  while (P) {
   P->inval*=a;
   P=P->next;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator/(const sparse &x1,const sparse &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op/(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 sparse y(x1.rank_>=x2.rank_ ? x1 : x2,false);
 int n=Common::min2(x1.ndims_,x2.ndims_);
 const sparse::elem_rec_ *P1=x1.lkelems_();
 const sparse::elem_rec_ *P2=x2.lkelems_();
 const sparse::elem_rec_ **Q = x1.rank_>=x2.rank_ ? &P1 : &P2;
 while (P1 && P2) {
  const int *ii1=P1->dmids;
  const int *ii2=P2->dmids;
  int h=0;
  for (;;) {
   if (h>=n) {
    if (P2->inval!=0.0) y.dmelems_((*Q)->dmids).inval=P1->inval/P2->inval;
    (*Q)=(*Q)->next;
    break;
   }
   if (ii1[h]==ii2[h]) h++;
   else 
   if (ii1[h]<ii2[h]) {
    P1=P1->next;
    break;
   } else {
    P2=P2->next;
    break;
   }
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator/(real a,const sparse &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op/(a,x):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "x = " << x << endl;
#endif
 sparse y(x,a!=0.0);
 if (a!=0.0) {
  sparse::elem_rec_ *P=y.lkelems_();
  while (P) {
   real &v=P->inval;
   v = v!=0.0 ? a/v : 0.0;
   P=P->next;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

sparse operator/(const sparse &x,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op/(x,a):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "a = " << a << endl;
#endif
 sparse y(x,a!=0.0);
 if (a!=0.0) {
  sparse::elem_rec_ *P=y.lkelems_();
  while (P) {
   P->inval/=a;
   P=P->next;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return sparse(y);
}

void sparse::print_conts(ostream &out) const
{
 if (rank_<=0) {
  if (nelms_<=0) out << "{}";
  else out << *linelms_();
  return;
 }
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && nelms_>nshr;
 int wid=out.precision()+5;
 out << '{';
 int n = isshr ? nshr : nelms_;
 const elem_rec_ *P=lkelems_();
 while (P && --n>=0) {
  const int *ii=P->dmids;
  out << endl << "\t  ([";
  for (int h=0; h<ndims_; h++) {
   if (h>0) out << ',';
   out << setw(3) << ii[h];
  }
  out << "] -> ";
  out << setw(wid) << P->inval;
  out << ')';
  P=P->next;
 }
 if (isshr) out << endl << "\t  ...";
 out << endl << "\t}";
} 

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

