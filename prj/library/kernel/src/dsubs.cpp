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
/// Definitions For Input Data Subset
/**
 * @file       dsubs.cpp
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
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dspac.h"
#include "dsubs.h"

namespace InfoSelCore {

namespace Data {

/////////////////////////////
////
//// subdatset class definition
////

subdatset::subdatset():
 nlinks_(0), refset_(0), refspace_(0), 
 orgcols_(0), odrcols_(0), nelms_(0), curvec_idx_(-1), 
 idn_rep_(totalidn_proxy_(*this),idntuple_proxy_(*this)),
 issmoothed_(false)
{}

subdatset::subdatset(const datspace &s,nil_literal,bool noord):
 nlinks_(0), refset_(s.refset_), refspace_(const_cast<datspace *>(&s)), 
 nelms_(s.nsubelms_), curloc_(s.nilloc_), curvec_idx_(-1), 
 idn_rep_(totalidn_proxy_(*this),idntuple_proxy_(*this)),
 issmoothed_(false)
{
 orgcols_=new datcolumn*[refset_->nptrcols_];
 memset(orgcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
 if (!s.isordered_ || noord) odrcols_=0;
 else {
  odrcols_=new datcolumn*[refset_->nptrcols_];
  memset(odrcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
 } 
 refspace_->nlinks_++;
}

subdatset::subdatset(const datspace &s,bool noord):
 nlinks_(0), refset_(s.refset_), refspace_(const_cast<datspace *>(&s)), 
 nelms_(s.nsubelms_), curloc_(s.refset_->isreversed_ ? s.suploc_ : s.infloc_), 
 curvec_idx_(-1), idn_rep_(totalidn_proxy_(*this),idntuple_proxy_(*this)),
 issmoothed_(false)
{
 orgcols_=new datcolumn*[refset_->nptrcols_];
 memset(orgcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
 if (!s.isordered_ || noord) odrcols_=0;
 else {
  odrcols_=new datcolumn*[refset_->nptrcols_];
  memset(odrcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
 } 
 refspace_->nlinks_++;
 ++(*this);
}

subdatset::subdatset(const datspace &s,last_literal,bool noord):
 nlinks_(0), refset_(s.refset_), refspace_(const_cast<datspace *>(&s)), 
 nelms_(s.nsubelms_), curloc_(s.refset_->isreversed_ ? s.infloc_ : s.suploc_), 
 curvec_idx_(-1), idn_rep_(totalidn_proxy_(*this),idntuple_proxy_(*this)),
 issmoothed_(false)
{
 orgcols_=new datcolumn*[refset_->nptrcols_];
 memset(orgcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
 if (!s.isordered_ || noord) odrcols_=0;
 else {
  odrcols_=new datcolumn*[refset_->nptrcols_];
  memset(odrcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
 } 
 refspace_->nlinks_++;
 --(*this);
}

subdatset::subdatset(const subdatset &s):
 nlinks_(0), refset_(s.refset_), refspace_(s.refspace_), 
 nelms_(s.nelms_), curloc_(s.curloc_), curvec_idx_(-1), 
 idn_rep_(totalidn_proxy_(*this),idntuple_proxy_(*this)),
 issmoothed_(s.issmoothed_)
{
 if (refspace_==0) orgcols_=0, odrcols_=0;
 else {
  orgcols_=new datcolumn*[refset_->nptrcols_];
  memset(orgcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
  if (s.odrcols_!=0) {
   odrcols_=new datcolumn*[refset_->nptrcols_];
   memset(odrcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
   if (!curloc_.isnil())
   for (int k=0; k<refset_->nptrcols_; k++) {
    datcolumn *cp=s.odrcols_[k];
    if (cp==0) break;
    int i=cp->orgidx_;
    cp->nlinks_++;
    orgcols_[i]=cp;
    odrcols_[k]=cp;
   }
  } else {
   odrcols_=0;
   if (!curloc_.isnil())
   for (int k=0; k<nelms_; k++) {
    int i=curloc_[k];
    datcolumn *c=s.orgcols_[i]; // c!=0
    c->nlinks_++;
    orgcols_[i]=c;
   }
  } 
  refspace_->nlinks_++;
 }
}
  
subdatset::~subdatset()
{
 if (nlinks_>0) throw xSubsetLinkFail();
 if (refspace_!=0) {
  if (!curloc_.isnil())
  if (odrcols_!=0) {
   for (int k=0; k<refset_->nptrcols_; k++) {
    datcolumn *cp=odrcols_[k];
    if (cp==0) break;
    if (--cp->nlinks_<=0) delete cp;
   } 
  } else {
   for (int k=0; k<nelms_; k++) {
    int i=curloc_[k];
    datcolumn *c=orgcols_[i]; // c!=0
    if (--c->nlinks_<=0) delete c;
   }
  } 
  delete[] odrcols_;
  delete[] orgcols_;
  refspace_->nlinks_--;
 }
}

subdatset &subdatset::operator=(const subdatset &s)
{
 int n;
 if (refspace_==0) n=0;
 else {
  n=refset_->nptrcols_;
  if (!curloc_.isnil())
  if (odrcols_!=0) {
   for (int k=0; k<n; k++) {
    datcolumn *cp=odrcols_[k];
    if (cp==0) break;
    int i=cp->orgidx_;
    orgcols_[i]=0;
    odrcols_[k]=0;
    if (--cp->nlinks_<=0) delete cp;
   }
  } else {  
   for (int k=0; k<nelms_; k++) {
    int i=curloc_[k];
    datcolumn *c=orgcols_[i]; // c!=0
    orgcols_[i]=0;
    if (--c->nlinks_<=0) delete c;
   }
  } 
  refspace_->nlinks_--;
 }
 refset_=s.refset_; refspace_=s.refspace_; 
 nelms_=s.nelms_; curloc_=s.curloc_;
 curvec_idx_=-1; issmoothed_=s.issmoothed_;
 idn_rep_.nul();
 if (refspace_==0) {
  delete[] odrcols_;
  delete[] orgcols_;
  orgcols_=0; 
  odrcols_=0;
 } else {
  if (n!=refset_->nptrcols_) {
   delete[] odrcols_;
   delete[] orgcols_;
   orgcols_=new datcolumn*[refset_->nptrcols_];
   odrcols_ = s.odrcols_!=0 ? new datcolumn*[refset_->nptrcols_] : 0;
  } else 
  if ((odrcols_!=0) != (s.odrcols_!=0)) {
   delete[] odrcols_;
   odrcols_ = s.odrcols_!=0 ? new datcolumn*[refset_->nptrcols_] : 0;
  }
  memset(orgcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
  if (s.odrcols_!=0)
  memset(odrcols_,0,refset_->nptrcols_*sizeof(datcolumn *)); 
  if (!curloc_.isnil())
  if (odrcols_!=0) {
   for (int k=0; k<refset_->nptrcols_; k++) {
    datcolumn *cp=s.odrcols_[k];
    if (cp==0) break;
    int i=cp->orgidx_;
    cp->nlinks_++;
    orgcols_[i]=cp;
    odrcols_[k]=cp;
   }
  } else {
   for (int k=0; k<nelms_; k++) {
    int i=curloc_[k];
    datcolumn *c=s.orgcols_[i]; // c!=0
    c->nlinks_++;
    orgcols_[i]=c;
   }
  } 
  refspace_->nlinks_++;
 }
 return *this;
}

bool subdatset::idntuple_proxy_::operator()(inttuple &ret) const //! \pre !curloc_.isnil
{
 if (that_.odrcols_==0) ret=that_.curloc_.tuple();
 else {
  for (int k=0; k<that_.nelms_; k++) {
   int l=that_.curloc_[k];
   datcolumn *c=that_.odrcols_[l]; // c!=0
   int i=c->idn();
   ret[k]=i;
  }
  ret.sortinc();
 } 
 return true;
}

bool subdatset::totalidn_proxy_::operator()(int &ret) const //! \pre !curloc_.isnil
{
 ret=0;
 int I;
 if (that_.odrcols_==0) I=that_.curloc_.index();
 else 
 if (that_.nelms_==1) {
  int l=that_.curloc_[0];
  datcolumn *c=that_.odrcols_[l]; // c!=0
  I=c->idn();
 } else {
  inttuple tup(that_.nelms_,Nil);
  for (int k=0; k<that_.nelms_; k++) {
   int l=that_.curloc_[k];
   datcolumn *c=that_.odrcols_[l]; // c!=0
   int i=c->idn();
   tup[k]=i;
  }
  tup.sortinc();
  I=intcomb::combidx(that_.curloc_.limit(),tup);
 } 
 if (I<0) throw xSubsetIdnOverflow();
 ret=I;
 return true;
}

datcolumn &subdatset::orgelms_(int k) const //! \pre !curloc_.isnil , nelms_>0 , 0 <= k < nelms_
{                            
 int rk = isreversed() ? nelms_-1-k : k; 
 if (odrcols_==0) {
  int i=curloc_[rk];
  datcolumn *c=orgcols_[i]; // c!=0
  return *c; 
 } else 
 if (nelms_==1) { // rk==0
  int l=curloc_[0];
  datcolumn *c=odrcols_[l]; // c!=0
  return *c; 
 } else {
  inttuple tup(nelms_,Nil);
  for (int p=0; p<nelms_; p++) {
   int q=curloc_[p];
   datcolumn *c=odrcols_[q]; // c!=0
   int j=c->orgidx_;
   tup[p]=j;
  } 
  tup.sortinc();
  int i=tup[rk];
  datcolumn *c=orgcols_[i]; // c!=0
  return *c; 
 } 
}

datcolumn &subdatset::odrelms_(int k) const //! \pre !curloc_.isnil , nelms_>0 , 0 <= k < nelms_
{                            
 int rk = isreversed() ? nelms_-1-k : k; 
 if (odrcols_!=0) {
  int l=curloc_[rk];
  datcolumn *c=odrcols_[l]; // c!=0
  return *c; 
 } else {
  int i=curloc_[rk];
  datcolumn *c=orgcols_[i]; // c!=0
  return *c; 
 } 
}

bool subdatset::preset_curloc_(const intcomb &bound1,const intcomb &bound2) //! \pre refspace_!=0
{
 if (bound1==bound2 || curloc_==bound2) return false;

 if (curloc_==bound1) {
  if (odrcols_!=0)
  for (int k=0; k<refset_->nelms_; k++) {
   datcolumn *c=refset_->odrcols_[k]; // c!=0
   int i=c->orgidx_;
   c->nlinks_++;
   orgcols_[i]=c;
   odrcols_[k]=c;
  }
 } else {
  if (odrcols_==0)
  for (int k=0; k<nelms_; k++) {
   int i=curloc_[k];
   datcolumn *c=orgcols_[i]; // c!=0
   orgcols_[i]=0;
   if (--c->nlinks_<=0) delete c;
  }
 } 
 
 return true;
}

void subdatset::postset_curloc_(const intcomb &bound1,const intcomb &bound2) //! \pre refspace_!=0 , bound1!=bound2 , curloc_!=bound1
{
 curvec_idx_=-1;
 idn_rep_.nul();

 if (curloc_==bound2) {
  if (odrcols_!=0)
  for (int k=0; k<refset_->nptrcols_; k++) {
   datcolumn *cp=odrcols_[k];
   if (cp==0) break;
   int i=cp->orgidx_;
   orgcols_[i]=0;
   odrcols_[k]=0;
   if (--cp->nlinks_<=0) delete cp;
  }
  issmoothed_=false;
 } else {
  issmoothed_=false;
  if (odrcols_!=0) {
   for (int k=0; k<nelms_; k++) {
    int l=curloc_[k];
    datcolumn *c=odrcols_[l]; // c!=0
    if (c->issmoothed_) issmoothed_=true;
   }
  } else {
   for (int k=0; k<nelms_; k++) {
    int i=curloc_[k];
    datcolumn *c=orgcols_[i]; // c!=0
    if (c->issmoothed_) issmoothed_=true;
   }
  } 
 }
}

void subdatset::move_curloc_(const intcomb &bound1,const intcomb &bound2,int inc) //! \pre refspace_!=0 , inc!=0
{
 if (!preset_curloc_(bound1,bound2)) return;

 curloc_+=inc;

 if (odrcols_!=0) {
  while (curloc_!=bound2) {
   int k=0;
   while (k<nelms_) {
    int l=curloc_[k];
    datcolumn *cp=odrcols_[l];
    if (cp!=0) {
     int kk=0;
     while (kk<refset_->nelms_) {
      datcolumn *d=refset_->odrcols_[kk]; // d!=0
      if (cp==d) break;
      kk++;
     } 
     if (kk>=refset_->nelms_) cp=0;
    } 
    if (cp==0) {
     cp=refset_->odrcols_[l];
     if (cp!=0) {
      int kk=0;
      while (kk<refset_->nptrcols_) {
       datcolumn *dp=odrcols_[kk];
       if (dp==0) break;
       if (cp==dp) { cp=0; break; }
       kk++;
      }
      if (cp!=0) {
       datcolumn *dp=odrcols_[l];
       if (dp!=0) {
        int j=dp->orgidx_;
        orgcols_[j]=0;
        if (--dp->nlinks_<=0) delete dp;
       }
       int i=cp->orgidx_;
       cp->nlinks_++;
       orgcols_[i]=cp;
       odrcols_[l]=cp;
      }
     }  
    } 
    if (cp==0) break;
    k++;
   }
   if (k>=nelms_) break;
   curloc_+=inc;
  }
 } else {
  while (curloc_!=bound2) {
   int k=0;
   while (k<nelms_) {
    int i=curloc_[k];
    datcolumn *cp=refset_->orgcols_[i];
    if (cp==0) break;
    k++;
   }
   if (k>=nelms_) {
    for (k=0; k<nelms_; k++) {
     int i=curloc_[k];
     datcolumn *c=refset_->orgcols_[i]; // c!=0
     c->nlinks_++;
     orgcols_[i]=c;
    }
    break;
   }
   curloc_+=inc;
  }
 }
 
 postset_curloc_(bound1,bound2);
}

void subdatset::asnil()
{
 if (refspace_==0) return;
 if (preset_curloc_(refspace_->suploc_,refspace_->nilloc_)) {
  curloc_=refspace_->nilloc_;
  postset_curloc_(refspace_->suploc_,refspace_->nilloc_);
 } else
 if (preset_curloc_(refspace_->infloc_,refspace_->nilloc_)) {
  curloc_=refspace_->nilloc_;
  postset_curloc_(refspace_->infloc_,refspace_->nilloc_);
 }
}

void subdatset::asfirst()
{
 if (refspace_==0) return;
 if (refset_->isreversed_) {
  if (preset_curloc_(refspace_->infloc_,refspace_->suploc_)) {
   curloc_=refspace_->suploc_;
   postset_curloc_(refspace_->infloc_,refspace_->suploc_);
  } 
 } else {
  if (preset_curloc_(refspace_->suploc_,refspace_->infloc_)) {
   curloc_=refspace_->infloc_;
   postset_curloc_(refspace_->suploc_,refspace_->infloc_);
  } 
 } 
 ++(*this);
}

void subdatset::aslast()
{
 if (refspace_==0) return;
 if (refset_->isreversed_) {
  if (preset_curloc_(refspace_->suploc_,refspace_->infloc_)) {
   curloc_=refspace_->infloc_;
   postset_curloc_(refspace_->suploc_,refspace_->infloc_);
  } 
 } else {
  if (preset_curloc_(refspace_->infloc_,refspace_->suploc_)) {
   curloc_=refspace_->suploc_;
   postset_curloc_(refspace_->infloc_,refspace_->suploc_);
  } 
 } 
 --(*this);
}

bool subdatset::operator==(const subdatset &s) const
{
 bool f1,f2;
 if ((f1=refspace_==0) || (f2=s.refspace_==0)) return f1 && f2;
 if (refset_->nptrcols_!=s.refset_->nptrcols_) return false;
 if ((f1=curloc_.isnil()) || (f2=s.curloc_.isnil())) return f1 && f2;
 if (nelms_!=s.nelms_) return false;
 for (int k=0; k<s.nelms_; k++) {
  int i=s.orgidx_(k);
  const datcolumn *d=s.orgcols_[i]; // d!=0
  const datcolumn *c=orgcols_[i]; // c!=0
  if (c!=d) return false;
 }
 return true;
}

void subdatset::weight(real weg)
{
 if (refspace_==0 || curloc_.isnil()) return;
 for (int k=0; k<nelms_; k++) {
  int i=orgidx_(k);
  datcolumn *c=orgcols_[i]; // c!=0
  c->ordweg_def_=c->ordweg_=weg;
 }
}

void subdatset::operator|=(real weg)
{
 if (refspace_==0 || curloc_.isnil()) return;
 for (int k=0; k<nelms_; k++) {
  int i=orgidx_(k);
  datcolumn *c=orgcols_[i]; // c!=0
  c->ordweg_def_=c->ordweg_; c->ordweg_=weg;
 }
}

real subdatset::weight() const
{
 if (refspace_==0 || curloc_.isnil()) return datcolumn::noweight;
 real weg=0.0;
 int n=0;
 for (int k=0; k<nelms_; k++) {
  int i=orgidx_(k);
  const datcolumn *c=orgcols_[i]; // c!=0
  if (c->ordweg_!=datcolumn::noweight) weg+=c->ordweg_, n++;
 }
 if (n<=0) return datcolumn::noweight;
 weg/=n;
 return weg;
}

void subdatset::setup_curvec_(int i) const //! \pre refset_!=0 && !curloc_.isnil , 0 <= i < nvecs_
{                           
 curvec_idx_=i;
 curvec_.setup_orgcmps_(nelms_,refset_->nsglparts_);
 for (int k=0; k<nelms_; k++) {
  int j=orgidx_(k);
  datcolumn *c=orgcols_[j]; // c!=0
  curvec_.orgcmps_[k]=&c->data_[i];
 }
}

void subdatset::print_conts(ostream &out) const
{
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && nelms_>nshr;
 bool isweg=iosex::isweighted(out);
 if (refspace_==0) out << "$null$";
 else
 if (curloc_.isnil()) out << "{}";
 else { // !isempty
  out << '{';
  int n = isshr ? nshr : nelms_;
  bool isnorm=!isreversed();
  for (int k = isnorm ? 0 : n-1; isnorm ? k<n : k>=0; isnorm ? k++ : k--) {
   int i=orgidx_(k);
   out << ' ' << (i+1);
   if (isweg) {
    const datcolumn *c=orgcols_[i]; // c!=0
    if (c->ordweg_!=datcolumn::noweight) out << '|' << c->ordweg_;
   }
  } 
  if (isshr) out << " ...";
  out << " }";
 }
}

} // end of Data namespace

} // end of InfoSelCore namespace

