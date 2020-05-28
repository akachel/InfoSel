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
/// Definitions For Input Data Set
/**
 * @file       dset.cpp
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
//// datset class definition
////

datset::datset(const string &otag,const string &etag,int supercard,int card,int ninsts): 
 nlinks_(0), objtag_(otag), elmtag_(etag),
 nptrcols_(supercard<1 ? 1 : supercard), isreversed_(false), isreversed_def_(false), 
 nvecs_(ninsts<1 ? 1 : ninsts), curvec_idx_(-1), issmoothed_(false), nsglparts_(0), 
 nparts_rep_(totalnparts_proxy_(*this),npartstuple_proxy_(*this))
{
 orgcols_=new datcolumn*[nptrcols_];
 odrcols_=new datcolumn*[nptrcols_]; odrcols_weg_=new real[nptrcols_];
 for (int i=0; i<nptrcols_; i++) {
  orgcols_[i]=0;
  odrcols_[i]=0; odrcols_weg_[i]=datcolumn::noweight;
 }
 nelms_ = card<nptrcols_ ? card : nptrcols_;
 for (int k=0; k<nelms_; k++) {
  string coltag=etag;
  if (coltag.empty()) coltag="column";
  coltag+=" #"+tostring(k+1);
  datcolumn *c=new datcolumn(coltag,k,nvecs_);
  int i=c->orgidx_; // 0 <= i < nptrcols_
  c->nlinks_++;
  c->ownset_=this;
  orgcols_[i]=c;
  odrcols_[k]=c; odrcols_weg_[k]=c->ordweg_; c->ordweg_=c->ordweg_def_;
 }
}

datset::datset(const string &tag,const datset &s):
 nlinks_(0), objtag_(tag), elmtag_(s.elmtag_), nptrcols_(s.nptrcols_), 
 nelms_(0), isreversed_(s.isreversed_), isreversed_def_(s.isreversed_def_), 
 nvecs_(s.nvecs_), curvec_idx_(-1), issmoothed_(s.issmoothed_), nsglparts_(s.nsglparts_), 
 nparts_rep_(totalnparts_proxy_(*this),npartstuple_proxy_(*this))
{
 orgcols_=new datcolumn*[nptrcols_];
 odrcols_=new datcolumn*[nptrcols_]; odrcols_weg_=new real[nptrcols_];
 for (int i=0; i<nptrcols_; i++) {
  orgcols_[i]=0;
  odrcols_[i]=0; odrcols_weg_[i]=datcolumn::noweight;
 }
}

datset::datset(const datset &s):
 nlinks_(0), objtag_(s.objtag_), elmtag_(s.elmtag_), nptrcols_(s.nptrcols_), 
 nelms_(0), isreversed_(s.isreversed_), isreversed_def_(s.isreversed_def_), 
 nvecs_(s.nvecs_), curvec_idx_(-1), issmoothed_(s.issmoothed_), nsglparts_(s.nsglparts_), 
 nparts_rep_(totalnparts_proxy_(*this),npartstuple_proxy_(*this))
{
 orgcols_=new datcolumn*[nptrcols_];
 odrcols_=new datcolumn*[nptrcols_]; odrcols_weg_=new real[nptrcols_];
 for (int i=0; i<nptrcols_; i++) {
  orgcols_[i]=0;
  odrcols_[i]=0; odrcols_weg_[i]=datcolumn::noweight;
 }
 (*this)+=s;
}

datset::~datset()
{ 
 if (nlinks_>0) throw xSetLinkFail();
 for (int k=0; k<nelms_; k++) {
  datcolumn *c=odrcols_[k]; // c!=0
  if (c->ownset_==this) c->ownset_=0; 
  if (--c->nlinks_<=0) delete c;
 }
 delete[] orgcols_; 
 delete[] odrcols_; delete[] odrcols_weg_; 
}

datset &datset::operator=(const datset &s)
{
 if (&s!=this && nptrcols_==s.nptrcols_) {
  (*this)-=(*this);
  (*this)+=s;
 }
 return *this;
}

void datset::reinitiate()
{
 isreversed_def_=isreversed_=false;
 curvec_idx_=-1;
 for (int i=0; i<nptrcols_; i++) {
  datcolumn *cp=orgcols_[i];
  if (cp!=0 && cp->ownset_==this)
  cp->reinitiate();
 } 
}

bool datset::npartstuple_proxy_::operator()(inttuple &ret) const
{
 int m=that_.nelms_;
 int n=that_.nsglparts_; // n >= that_.orgelms_(k)->nparts() , 0 <= k < m
 ret.setup(m,n);
 return true;
}

bool datset::totalnparts_proxy_::operator()(int &ret) const
{
 ret=0;
 int m=that_.nelms_;
 if (m>0) {
  int n=that_.nsglparts_; // n >= that_.orgelms_(k)->nparts() , 0 <= k < m
  int N;
  if (!chkmopow(N,n,m)) throw xDomainNPartsOverflow();
  ret=N;
 }
 return true;
}

datcolumn &datset::orgelms_(int k) const //! \pre nelms_>0 , 0 <= k < nelms_
{                              
 int rk = isreversed_ ? nelms_-1-k : k; 
 isreversed_=isreversed_def_;
 int l=0;
 datcolumn *cp;
 for (int i=0; i<nptrcols_; i++) {
  cp=orgcols_[i];
  if (cp!=0) // l<nelms_
  if (rk==l++) break;
 }
 return *cp; 
}

datcolumn &datset::odrelms_(int k) const //! \pre nelms_>0 , 0 <= k < nelms_
{                              
 int rk = isreversed_ ? nelms_-1-k : k; 
 isreversed_=isreversed_def_;
 datcolumn *c=odrcols_[rk]; // c!=0
 return *c; 
}

bool datset::operator==(const subdatset &s) const
{
 if (s.refspace_==0 || nptrcols_!=s.refset_->nptrcols_) return false;
 if (s.curloc_.isnil()) return nelms_<=0;
 if (nelms_!=s.nelms_) return false;
 for (int k=0; k<s.nelms_; k++) {
  int i=s.orgidx_(k);
  const datcolumn *d=s.orgcols_[i]; // d!=0
  const datcolumn *cp=orgcols_[i];
  if (cp!=d) return false;
 }
 return true;
}

bool datset::operator==(const datset &s) const
{
 if (&s==this) return true;
 if (nptrcols_!=s.nptrcols_) return false;
 if (nelms_!=s.nelms_) return false;
 for (int i=0; i<s.nptrcols_; i++) {
  const datcolumn *dp=s.orgcols_[i];
  const datcolumn *cp=orgcols_[i];
  if (cp!=dp) return false;
 }
 return true;
}

void datset::operator-=(const subdatset &s)
{
 if (s.refspace_==0 || s.curloc_.isnil()) return;
 int n=nelms_;
 if (s.refset_==this)
 for (int k=0; k<s.nelms_; k++) {
  int i=s.orgidx_(k);
  datcolumn *cp=orgcols_[i];
  if (cp!=0) {
   int l=0;
   while (l<nelms_ && odrcols_[l]!=cp) l++;
   while (++l<nelms_) 
   odrcols_[l-1]=odrcols_[l], odrcols_weg_[l-1]=odrcols_weg_[l];
   odrcols_[l-1]=0; odrcols_weg_[l-1]=datcolumn::noweight; 
   orgcols_[i]=0;
   if (cp->ownset_==this) cp->ownset_=0; 
   cp->nlinks_--;
   nelms_--;
  }
 } else 
 if (nptrcols_==s.refset_->nptrcols_)
 for (int k=0; k<s.nelms_; k++) {
  int i=s.orgidx_(k);
  datcolumn *d=s.orgcols_[i]; // d!=0
  datcolumn *cp=orgcols_[i];
  if (cp!=0 && cp==d) {
   int l=0;
   while (l<nelms_ && odrcols_[l]!=cp) l++;
   while (++l<nelms_) 
   odrcols_[l-1]=odrcols_[l], odrcols_weg_[l-1]=odrcols_weg_[l];
   odrcols_[l-1]=0; odrcols_weg_[l-1]=datcolumn::noweight;  
   orgcols_[i]=0;
   if (cp->ownset_==this) cp->ownset_=0;
   cp->nlinks_--;
   nelms_--;
  }
 }
 if (nelms_!=n) {
  curvec_idx_=-1;
  nparts_rep_.nul();
 } 
}
    
void datset::operator+=(const subdatset &s)
{
 if (s.refspace_==0 || s.curloc_.isnil()) return;
 int n=nelms_;
 if (s.refset_==this || nptrcols_==s.refset_->nptrcols_)
 for (int k=0; k<s.nelms_; k++) {
  int i=s.orgidx_(k);
  datcolumn *d=s.orgcols_[i]; // d!=0
  datcolumn *cp=orgcols_[i];
  if (cp==0) {
   int l=nelms_++;
   d->nlinks_++;
   if (d->ownset_==0) d->ownset_=this;
   orgcols_[i]=d;
   odrcols_[l]=d; odrcols_weg_[l]=d->ordweg_; d->ordweg_=d->ordweg_def_;
   while (--l>=0 && odrcols_weg_[l]>odrcols_weg_[l+1]) 
   swap(odrcols_[l],odrcols_[l+1]), swap(odrcols_weg_[l],odrcols_weg_[l+1]);
  } else
  if (cp==d)
  if (cp->ownset_==0) cp->ownset_=this;
 }
 if (nelms_!=n) {
  curvec_idx_=-1;
  nparts_rep_.nul();
 } 
}

void datset::operator*=(const subdatset &s)
{
 if (s.refspace_==0 || s.curloc_.isnil()) return;
 int n=nelms_;
 if (s.refset_==this || nptrcols_==s.refset_->nptrcols_)
 for (int i=0; i<nptrcols_; i++) {
  datcolumn *cp=orgcols_[i];
  datcolumn *dp=0;
  for (int k=0; k<s.nelms_; k++)
  if (i==s.orgidx_(k)) {
   dp=s.orgcols_[i];
   break;
  } 
  if (cp!=0 && cp!=dp) {
   int l=0;
   while (l<nelms_ && odrcols_[l]!=cp) l++;
   while (++l<nelms_) 
   odrcols_[l-1]=odrcols_[l], odrcols_weg_[l-1]=odrcols_weg_[l];;
   odrcols_[l-1]=0; odrcols_weg_[l-1]=datcolumn::noweight; 
   orgcols_[i]=0;
   if (cp->ownset_==this) cp->ownset_=0; 
   if (--cp->nlinks_<=0) delete cp;
   nelms_--;
  }
 }
 if (nelms_!=n) {
  curvec_idx_=-1;
  nparts_rep_.nul();
 } 
}
    
void datset::operator-=(const datset &s)
{
 int n=nelms_;
 if (&s==this) {
  for (int i=0; i<nptrcols_; i++) {
   datcolumn *cp=orgcols_[i];
   odrcols_[i]=0; odrcols_weg_[i]=datcolumn::noweight;
   orgcols_[i]=0;
   if (cp!=0) {
    if (cp->ownset_==this) cp->ownset_=0; 
    if (--cp->nlinks_<=0) delete cp;
   } 
  }
  nelms_=0;
 }
 else
 if (nptrcols_==s.nptrcols_)
 for (int i=0; i<s.nptrcols_; i++) {
  datcolumn *dp=s.orgcols_[i];
  datcolumn *cp=orgcols_[i];
  if (cp!=0 && cp==dp) {
   int l=0;
   while (l<nelms_ && odrcols_[l]!=cp) l++;
   while (++l<nelms_) 
   odrcols_[l-1]=odrcols_[l], odrcols_weg_[l-1]=odrcols_weg_[l];
   odrcols_[l-1]=0; odrcols_weg_[l-1]=datcolumn::noweight; 
   orgcols_[i]=0;
   if (cp->ownset_==this) cp->ownset_=0;
   cp->nlinks_--;
   nelms_--;
  }
 }
 if (nelms_!=n) {
  curvec_idx_=-1;
  nparts_rep_.nul();
 } 
}
    
void datset::operator+=(const datset &s)
{
 int n=nelms_;
 if (&s==this || nptrcols_==s.nptrcols_)
 for (int i=0; i<s.nptrcols_; i++) {
  datcolumn *dp=s.orgcols_[i];
  datcolumn *cp=orgcols_[i];
  if (cp==0 && dp!=0) {
   int l=nelms_++;
   dp->nlinks_++;
   if (dp->ownset_==0) dp->ownset_=this;
   orgcols_[i]=dp;
   odrcols_[l]=dp; odrcols_weg_[l]=dp->ordweg_; dp->ordweg_=dp->ordweg_def_;
   while (--l>=0 && odrcols_weg_[l]>odrcols_weg_[l+1]) 
   swap(odrcols_[l],odrcols_[l+1]), swap(odrcols_weg_[l],odrcols_weg_[l+1]);
  } else
  if (cp!=0 && cp==dp)
  if (cp->ownset_==0) cp->ownset_=this;
 }
 if (nelms_!=n) {
  curvec_idx_=-1;
  nparts_rep_.nul();
 } 
}

void datset::operator*=(const datset &s)
{
 int n=nelms_;
 if (&s==this); 
 else
 if (nptrcols_==s.nptrcols_)
 for (int i=0; i<s.nptrcols_; i++) {
  datcolumn *dp=s.orgcols_[i];
  datcolumn *cp=orgcols_[i];
  if (cp!=0 && cp!=dp) {
   int l=0;
   while (l<nelms_ && odrcols_[l]!=cp) l++;
   while (++l<nelms_) 
   odrcols_[l-1]=odrcols_[l], odrcols_weg_[l-1]=odrcols_weg_[l];
   odrcols_[l-1]=0; odrcols_weg_[l-1]=datcolumn::noweight; 
   orgcols_[i]=0;
   if (cp->ownset_==this) cp->ownset_=0;
   if (--cp->nlinks_<=0) delete cp;
   nelms_--;
  }
 }
 if (nelms_!=n) {
  curvec_idx_=-1;
  nparts_rep_.nul();
 } 
}

void datset::operator<<=(datset &s)
{
 int n=nelms_, sn=s.nelms_;
 isreversed_=s.isreversed_;
 isreversed_def_=s.isreversed_def_;
 if (&s==this || nptrcols_==s.nptrcols_)
 for (int k=0; k<s.nelms_; k++) {
  datcolumn *d=s.odrcols_[k]; // d!=0
  int i=d->orgidx_;
  datcolumn *cp=orgcols_[i];
  if (cp==0 || cp==d) {
   real dweg=s.odrcols_weg_[k];
   int l=k--;
   while (++l<s.nelms_) 
   s.odrcols_[l-1]=s.odrcols_[l], s.odrcols_weg_[l-1]=s.odrcols_weg_[l];
   s.odrcols_[l-1]=0; s.odrcols_weg_[l-1]=datcolumn::noweight; 
   s.orgcols_[i]=0;
   if (d->ownset_==&s) d->ownset_=0;
   d->nlinks_--;
   s.nelms_--;
   if (cp==0) {
    int l=nelms_++;
    d->nlinks_++;
    if (d->ownset_==0) d->ownset_=this;
    orgcols_[i]=d;
    odrcols_[l]=d; odrcols_weg_[l]=dweg;
    while (--l>=0 && odrcols_weg_[l]>odrcols_weg_[l+1]) 
    swap(odrcols_[l],odrcols_[l+1]), swap(odrcols_weg_[l],odrcols_weg_[l+1]);
   } else
   if (cp->ownset_==0) cp->ownset_=this;
  }
 }
 if (nelms_!=n) {
  curvec_idx_=-1;
  nparts_rep_.nul();
 } 
 if (s.nelms_!=sn) {
  s.curvec_idx_=-1;
  s.nparts_rep_.nul();
 } 
}

void datset::weight(real weg)
{
 for (int i=0; i<nptrcols_; i++) { 
  datcolumn *cp=orgcols_[i];
  if (cp!=0)
  cp->ordweg_def_=cp->ordweg_=weg;
 }
}

void datset::operator|=(real weg)
{
 for (int i=0; i<nptrcols_; i++) { 
  datcolumn *cp=orgcols_[i];
  if (cp!=0)
  cp->ordweg_def_=cp->ordweg_, cp->ordweg_=weg;
 }
}

real datset::weight() const
{
 real weg=0.0;
 int n=0;
 for (int i=0; i<nptrcols_; i++) { 
  const datcolumn *cp=orgcols_[i];
  if (cp!=0)
  if (cp->ordweg_!=datcolumn::noweight) weg+=cp->ordweg_, n++;
 }
 if (n<=0) return datcolumn::noweight;
 weg/=n;
 return weg;
}

void datset::setup_curvec_(int i) const //! \pre 0 <= i < nvecs_
{                           
 curvec_idx_=i;
 curvec_.setup_orgcmps_(nelms_,nsglparts_);
 int k=0;
 for (int j=0; j<nptrcols_; j++) { 
  datcolumn *cp=orgcols_[j];
  if (cp!=0) // k<nelms_ 
  curvec_.orgcmps_[k++]=&cp->data_[i];
 } 
}

void datset::precision(real prec,real r_prec)
{
 if (nlinks_>0) return;
 for (int i=0; i<nptrcols_; i++) {
  datcolumn *cp=orgcols_[i];
  if (cp!=0 && cp->ownset_==this)
  cp->setup_prec_(prec,r_prec);
 } 
 curvec_idx_=-1;
}

int datset::partition(const partmeth_t &meth)
{
 if (nlinks_>0) return -1;
 nsglparts_=2;
 issmoothed_=false;
 for (int i=0; i<nptrcols_; i++) {
  datcolumn *cp=orgcols_[i];
  if (cp!=0 && cp->ownset_==this) {
   try {
    int n=cp->setup_partmeth_(meth);
    if (n>nsglparts_) nsglparts_=n;
    if (cp->issmoothed_) issmoothed_=true;
   } 
   catch (const xOneUniqDistrb &)
   {
    int l=0;
    while (l<nelms_ && odrcols_[l]!=cp) l++;
    while (++l<nelms_) 
    odrcols_[l-1]=odrcols_[l], odrcols_weg_[l-1]=odrcols_weg_[l];
    odrcols_[l-1]=0; odrcols_weg_[l-1]=datcolumn::noweight;
    orgcols_[i]=0;
    if (cp->ownset_==this) cp->ownset_=0;
    if (--cp->nlinks_<=0) delete cp;
    nelms_--;
   }
  }
 } 
 curvec_idx_=-1;
 nparts_rep_.nul();
 return nsglparts_;
}

void datset::print_stats(ostream &out,bool ownonly) const
{
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && nelms_>nshr;
 string prefix=objtag_;
 if (prefix.empty()) prefix="The";
 else mkupper(prefix[0]);
 if (!elmtag_.empty()) prefix+=" "+elmtag_;
 out << prefix << " set statistics:" << endl;
 out << "  cardinal number of attributes: " << nelms_ << endl;
 out << "  maximal cardinality: " << nptrcols_ << endl;
 out << "  common number of vector instances: " << nvecs_ << endl;
 out << "  maximal number of single domain parts: " << nsglparts_ << endl;
 int n = isshr ? nshr : nelms_;
 int k=0;
 for (int i=0; i<nptrcols_; i++) {
  const datcolumn *cp=orgcols_[i];
  if (cp!=0 && (!ownonly || cp->ownset_==this)) {
   if (++k>n) break;
   out << "  " << setw(3) << (i+1) << ": min=" << cp->minval_ << " max=" << cp->maxval_
       << " mean=" << cp->meanval_ << " sdev=" << cp->svaldev_ << " nuniques=" << cp->nuniques_ << endl;
   out << "  " << "   " << "  ";
   if (cp->issmoothed_) 
    out << "smoothed by \'" << cp->partmeth().name()
        << "\' partition: nparts=" << cp->nparts_ << " norm=" << cp->partnorm_;
   else
    out << "leaved with the finest partition: norm=" << cp->partnorm_;
   out << endl;
  } 
 }
 if (isshr) out << "..." << endl;
}

void datset::print_order(ostream &out,bool ownonly) const
{
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && nelms_>nshr;
 bool isweg=iosex::isweighted(out);
 string prefix=objtag_;
 if (prefix.empty()) prefix="the";
 if (!elmtag_.empty()) prefix+=" "+elmtag_;
 out << prefix << " set ascendant-order: {";
 int n = isshr ? nshr : nelms_;
 bool isasco=!isreversed_;
 isreversed_=isreversed_def_;
 bool isasc=isasco;
 do {
  if (isasc) 
  for (int k=0; k<n; k++) {
   const datcolumn *c=odrcols_[k]; // c!=0
   if (!ownonly || c->ownset_==this) {
    out << ' ' << (c->orgidx_+1);
    if (isweg) {
     real cweg=odrcols_weg_[k];
     if (cweg!=datcolumn::noweight) out << '|' << cweg;
    } 
   } 
  } else
  for (int k=n-1; k>=0; k--) {
   const datcolumn *c=odrcols_[k]; // c!=0
   if (!ownonly || c->ownset_==this) {
    out << ' ' << (c->orgidx_+1);
    if (isweg) {
     real cweg=odrcols_weg_[k];
     if (cweg!=datcolumn::noweight) out << '|' << cweg;
    } 
   } 
  }
  if (isshr) out << " ...";
  out << " }";
  isasc=!isasc;
  if (isasc==isasco) break;
  out << endl;
  ios::fmtflags f0=out.flags();
  out.setf(ios::right,ios::adjustfield);
  out << setw(prefix.length()+3) << '&' << " descendant-order: {";
  out.flags(f0);
 } while (true);
}

void datset::print_conts(ostream &out,bool ownonly) const
{
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && nelms_>nshr;
 bool isweg=iosex::isweighted(out);
 out << '{';
 int n = isshr ? nshr : nelms_;
 int k=0;
 bool isnorm=!isreversed_;
 isreversed_=isreversed_def_;
 for (int i = isnorm ? 0 : nptrcols_-1; isnorm ? i<nptrcols_ : i>=0; isnorm ? i++ : i--) {
  const datcolumn *cp=orgcols_[i];
  if (cp!=0 && (!ownonly || cp->ownset_==this)) {
   if (++k>n) break;
   out << ' ' << (i+1);
   if (isweg && cp->ordweg_!=datcolumn::noweight) out << '|' << cp->ordweg_;
  } 
 }
 if (isshr) out << " ...";
 out << " }";
}

} // end of Data namespace

} // end of InfoSelCore namespace

