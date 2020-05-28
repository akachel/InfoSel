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
/// Definitions For Input Data Column
/**
 * @file       dcol.cpp
 * @version    1.00
 * @author     Adam Kachel, Jacek Biesiada
 * @since      2002.05.29
 * @date       2009.07.12
 *
 * @contents  
 *             This file contains defintions of input data related elements.
 */

#include "comm.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"

namespace InfoSelCore {

namespace Data {

/////////////////////////////
////
//// datcolumn class definition
////

const real datcolumn::noweight=rMax;

datcolumn::datcolumn(const string &tag,int idn,int ninsts): 
 nlinks_(0), objtag_(tag), ownset_(0), orgidx_(idn<0 ? 0 : idn), orgidx_tup_(idn), 
 ordweg_(noweight), ordweg_def_(noweight), ndata_(ninsts<1 ? 1 : ninsts), 
 issmoothed_(false), minval_(0.0), maxval_(0.0), meanval_(0.0), 
 svaldev_(0.0), nuniques_(0), uniques_(0), nparts_(0), partnorm_(0.0) 
{
 data_=new datum[ndata_];
}

datcolumn::~datcolumn()
{
 if (nlinks_>0) throw xColumnLinkFail();
 delete[] uniques_;
 delete[] data_;
}

void datcolumn::reinitiate()
{
 ordweg_def_=ordweg_=noweight;
}

void datcolumn::setup_prec_(real prec,real r_prec)
{
 for (int i=0; i<ndata_; i++) {
  datum &dat=data_[i];
  dat.setup_val_( (real)(((int)(dat.val_*r_prec+0.5))*prec) );
 }
 if (nuniques_>0) {
  nuniques_=0;
  delete[] uniques_;
  minval_=maxval_=meanval_=svaldev_=0.0;
  partmeth_=partmeth_t();
  nparts_=0;
  partnorm_=0.0;
  for (int i=0; i<ndata_; i++) {
   datum &dat=data_[i];
   dat.setup_part_(-1);
  }
 } 
}

void datcolumn::precision(real prec,real r_prec)
{
 if (ownset_!=0) ownset_->precision(prec,r_prec);
 else if (nlinks_<=0) setup_prec_(prec,r_prec);
}

int datcolumn::unique_rec_::decord(const void *x1,const void *x2)
{
 const rec &r1=*reinterpret_cast<const rec *>(x1);
 const rec &r2=*reinterpret_cast<const rec *>(x2);
 return r1.val<r2.val ? +1 : r1.val>r2.val ? -1 : 0;
}

int datcolumn::unique_rec_::incord(const void *x1,const void *x2)
{
 const rec &r1=*reinterpret_cast<const rec *>(x1);
 const rec &r2=*reinterpret_cast<const rec *>(x2);
 return r1.val<r2.val ? -1 : r1.val>r2.val ? +1 : 0;
}

void datcolumn::calc_stats_() const //! \pre nuniques_<=0
{
 unique_rec_ *unis=new unique_rec_[ndata_]; // ndata_>0
 int n=0;
 minval_=+rInf;
 maxval_=-rInf;
 real sum=0.0, sum_sq=0.0;
 for (int i=0; i<ndata_; i++) {
  real datval=data_[i].val_;
  if (datval<minval_) minval_=datval;
  if (datval>maxval_) maxval_=datval;
  sum+=datval;
  sum_sq+=sqr(datval);
  int k=0;
  while (k<n && datval!=unis[k].val) k++;
  if (k>=n) {
   unique_rec_ &uni=unis[k];
   uni.val=datval;
   uni.frq=1;
   n++;
  } else unis[k].frq++;
 }
 meanval_=sum/ndata_;
 real sdev_sq = ndata_>=100 ? (sum_sq/ndata_-sqr(meanval_)) : 
                ndata_>1 ? (sum_sq/(ndata_-1)-ndata_*sqr(meanval_)/(ndata_-1)) :
                0.0;
 svaldev_ = sdev_sq>0.0 ? sqrt(sdev_sq) : 0.0;
 qsort(unis,n,sizeof(unique_rec_),unique_rec_::incord); // minval_==unis[0].val<unis[1].val<...<unis[n-1].val==maxval_ , sum(unis.frq)==ndata_>=n
 nuniques_=n;
 uniques_=new unique_rec_[n];
 for (int k=0; k<n; k++) uniques_[k]=unis[k];
 delete[] unis;
}

real corrc(const datcolumn &c1,const datcolumn &c2)
{
 int n=min2(c1.ninsts(),c2.ninsts()); // n>0
 real xo1=c1.meanval();
 real xo2=c2.meanval();
 real dx1=c1.svaldev();
 real dx2=c2.svaldev();
 real y=0.0;
 for (int i=0; i<n; i++) {
  real x1i=c1.insval(i);
  real x2i=c2.insval(i);
  y+=(x1i-xo1)*(x2i-xo2);
 } 
 if (n>=100) y/=n;
 else if (n>1) y/=n-1; 
 if (dx1!=0.0) y/=dx1;
 if (dx2!=0.0) y/=dx2;
 return y;
}

real fscor(const datcolumn &c1,const datcolumn &c2)
{
 int n=min2(c1.ninsts(),c2.ninsts()); // n>0
 int m2=c2.nuniques(); // 0 < m2 <= n
 real xo1=c1.meanval();
 
 real ynum=0.0;
 real yden=0.0;
 for (int k=0; k<m2; k++) {
  real x2k=c2.unival(k); 
  int n2k=c2.unifrq(k); // 0 < n2k <= n
  
  real xo1k=0.0;
  for (int i=0; i<n; i++) 
  if (c2.insval(i)==x2k)
  xo1k+=c1.insval(i);   
  xo1k/=n2k;

  real dx1k_sq=0.0;
  for (int i=0; i<n; i++) 
  if (c2.insval(i)==x2k)
  dx1k_sq+=sqr(c1.insval(i)-xo1k);   

  ynum+=n2k*sqr(xo1k-xo1);
  yden+=dx1k_sq;
 }
 if (m2>=100) ynum/=m2; 
 else if (m2>1) ynum/=m2-1;
 if (m2<n) yden/=n-m2;
 real y=ynum;
 if (yden!=0.0) y/=yden;
 return y;
}

real tscor(const datcolumn &c1,const datcolumn &c2)
{
 int n=min2(c1.ninsts(),c2.ninsts()); // n>0
 const int m2=2; // 0 < m2 <= n
 if (c2.nuniques()!=m2) throw xNumUniqDistrbReq(m2,c2.name());
 
 real ynum=0.0;
 real yden=0.0;
 for (int k=0; k<m2; k++) {
  real x2k=c2.unival(k);
  int n2k=c2.unifrq(k); // n2k>0
   
  real xo1k=0.0;
  for (int i=0; i<n; i++)
  if (c2.insval(i)==x2k)
  xo1k+=c1.insval(i);
  xo1k/=n2k;

  real dx1k_sq=0.0;
  for (int i=0; i<n; i++) 
  if (c2.insval(i)==x2k)
  dx1k_sq+=sqr(c1.insval(i)-xo1k);   
  if (n2k>=100) dx1k_sq/=n2k; 
  else if (n2k>1) dx1k_sq/=n2k-1;

  ynum += k>0 ? -xo1k : +xo1k;
  yden+=dx1k_sq/n2k;
 }
 real y=ynum;
 if (yden>0.0) y/=sqrt(yden);
 return y;
}

int datcolumn::setup_partmeth_(const partmeth_t &meth)
{
 if (nuniques_<=0) calc_stats_();

 if (nuniques_<=1) { // !(minval_<maxval_)
  throw xOneUniqDistrb(name());
  return -1;
 }

 partmeth_=meth;
 istringstream parsin(partmeth_.pars());
 switch (partmeth_.code()) {
  default: case PartMethSet::pmNone: none_partition_(parsin); break;
  case PartMethSet::pmFineGrain: finegrain_partition_(parsin); break;
  case PartMethSet::pmEquiWidth: equiwidth_partition_(parsin); break;
  case PartMethSet::pmEquiUniqs: equiuniqs_partition_(parsin); break;
  case PartMethSet::pmEquiFreqs: equifreqs_partition_(parsin); break;
  case PartMethSet::pmMaxUniDif: maxunidif_partition_(parsin); break;
  case PartMethSet::pmMaxFrqDif: maxfrqdif_partition_(parsin); break;
  // ...
 }
 bool isfinest = partmeth_.code()==PartMethSet::pmFineGrain;
 
 issmoothed_ = !isfinest;
 return nparts_;
}

int datcolumn::partition(const partmeth_t &meth)
{
 if (ownset_!=0) return ownset_->partition(meth);
 else return nlinks_<=0 ? setup_partmeth_(meth) : -1;
}

void datcolumn::print_stats(ostream &out) const
{
 string prefix=objtag_;
 if (prefix.empty()) prefix="The";
 else mkupper(prefix[0]);
 out << prefix << " attribute statistics:" << endl;
 out << "  number of data instances: " << ndata_ << endl;
 out << "    min = " << minval_ << "  max = " << maxval_ << endl;
 out << "    mean = " << meanval_ << "  sdev = " << svaldev_ << endl;
 out << "  size of data distribution: " << nuniques_ << endl;
 if (issmoothed_) {
  out << "  smoothing partition: " << partmeth_.name() << endl;
  out << "  number of parts: " << nparts_ << endl;
  out << "  partition norm: " << partnorm_ << endl;
 } else {
  out << "  finest partition norm: " << partnorm_ << endl;
  out << "  data distribution (frq. x val.): { ";
  for (int k=0; k<nuniques_; k++) {
   const unique_rec_ &uni=uniques_[k];
   if (k>0) out << " , ";
   out << uni.frq << " x " << uni.val;
  }
  out << " }" << endl;
 }
}

void datcolumn::print_ident(ostream &out) const
{
 bool isweg=iosex::isweighted(out);
 out << '(' << (orgidx_+1);
 if (isweg && ordweg_!=noweight) out << " | " << ordweg_;
 out << ')';
}

/////////////////////////////
////
//// partition method definitions
////

namespace { namespace datcolumn_ {

const int def_nparts_=24;

struct unidif_rec_ {
 typedef unidif_rec_ rec;
 real val;
 real pos;
 static int decord(const void *x1,const void *x2);
 static int incord(const void *x1,const void *x2);
};

int unidif_rec_::decord(const void *x1,const void *x2)
{
 const rec &r1=*reinterpret_cast<const rec *>(x1);
 const rec &r2=*reinterpret_cast<const rec *>(x2);
 return r1.val<r2.val ? +1 : r1.val>r2.val ? -1 : 0;
}

int unidif_rec_::incord(const void *x1,const void *x2)
{
 const rec &r1=*reinterpret_cast<const rec *>(x1);
 const rec &r2=*reinterpret_cast<const rec *>(x2);
 return r1.val<r2.val ? -1 : r1.val>r2.val ? +1 : 0;
}

}}

string PartMethSet::name(int code)
{
 switch (code) {
  default: case pmNone: return "NoParts";
  case pmFineGrain:     return "FineGrain";
  case pmEquiWidth:     return "EquiWidth";
  case pmEquiUniqs:     return "EquiUniqs";
  case pmEquiFreqs:     return "EquiFreqs";
  case pmMaxUniDif:     return "MaxUniDif";
  case pmMaxFrqDif:     return "MaxFrqDif";
  // ...
 }
}

string PartMethSet::description(int code)
{
 string np=string("int n = ")+tostring(datcolumn_::def_nparts_);
 string desc=name(code);
 switch (code) {
  default: case pmNone: desc+="( void )\n    No partitioning is performed on attribute domain\nwhich is the null/default case."; break;
  case pmFineGrain:     desc+="( void )\n    Leaves the original distribution unchanged but all unique values\nare replaced by their indices counting from zero."; break;
  case pmEquiWidth:     desc+="("+np+")\n    Partitions the domain into n parts of equal size."; break;
  case pmEquiUniqs:     desc+="("+np+")\n    Partitions the domain into n parts of equal number of unique values."; break;
  case pmEquiFreqs:     desc+="("+np+")\n    Partitions the domain into n parts of equal sum of data frequences."; break;
  case pmMaxUniDif:     desc+="("+np+")\n    Partitions the domain into n parts separated\nat maximal unique value differences."; break;
  case pmMaxFrqDif:     desc+="("+np+")\n    Partitions the domain into n parts separated\nat maximal data frequency differences."; break;
  // ...
 }
 return desc;
}

partmeth_t partmeth_t::create(int code,ostream &prompt,istream &pars)
{
 ostringstream bout;
 switch (code) {
  case PartMethSet::pmNone: break;
  case PartMethSet::pmFineGrain: break;
  // ...
  default: {
   int nparts;
   prompt << "Number of data domain parts (e.g. " << datcolumn_::def_nparts_ << "): " << flush; 
   pars >> nparts;
   bout << nparts << endl;
   break;
  }
 }
 return partmeth_t(code,bout.str());
}

void datcolumn::none_partition_(istream &)
{
 nparts_=0;
 partnorm_=+rInf;
 throw xNoPartMeth();
}

void datcolumn::finegrain_partition_(istream &)
{
 nparts_=nuniques_; // nuniques_>=2
 for (int i=0; i<ndata_; i++) {
  datum &dat=data_[i];
  int k=0;
  while (k<nuniques_ && dat.val_!=uniques_[k].val) k++;
  dat.setup_part_(k); // k<nparts_
 }
 partnorm_=0.0;
 for (int k=1; k<nuniques_; k++) partnorm_+=uniques_[k].val-uniques_[k-1].val;
 partnorm_/=nuniques_-1; // partnorm_>0
}

void datcolumn::equiwidth_partition_(istream &pars)
{
 using namespace datcolumn_;
 nparts_=def_nparts_;
 pars >> nparts_;
 if (nparts_<2) nparts_=2;
 if (nuniques_<=nparts_) partmeth_=partmeth_t::FineGrain(), finegrain_partition_(nullstream::in);
 else {
  const real eps=1e2*rPrec; // eps<1.0
  real d=(maxval_-minval_)/nparts_; // minval_<maxval_ => d>0
  real r_d=1.0/d;
  for (int i=0; i<ndata_; i++) {
   datum &dat=data_[i];
   real w=(dat.val_-minval_)*r_d;
   register int k=(int)(w+eps); // k>=0
   dat.setup_part_(k<nparts_ ? k : nparts_-1);
  }
  partnorm_=d; // partnorm_>0
 }
}

void datcolumn::equiuniqs_partition_(istream &pars)
{
 using namespace datcolumn_;
 nparts_=def_nparts_;
 pars >> nparts_;
 if (nparts_<2) nparts_=2;
 if (nuniques_<=nparts_) partmeth_=partmeth_t::FineGrain(), finegrain_partition_(nullstream::in);
 else {
  real *bnds;
  {
   int m=nuniques_/nparts_; // m>=1
   int *incs;
   {
    int r=nuniques_%nparts_; // r=nuniques_-m*nparts_ => 0<=r<nparts_
    int s=nparts_-r; // 1<=s<=nparts_
    int t = r<s ? r : s;
    int v = r<s ? 0 : 1, w = r<s ? 1 : 0;
    incs=new int[nparts_];
    for (int k=0; k<nparts_; k++) incs[k] = v;
    int l=0;
    while (t>0) {
     if (unirand()<0.5 && incs[l]!=w) incs[l]=w, t--;
     if (l++>=nparts_) l=0;
    }
    incs[nparts_-1]--; // sum(incs)==r-1
   }
   bnds=new real[nparts_];
   int l=0;
   for (int k=0; k<nparts_; k++) {
    l+=m+incs[k]; // l<nuniques_
    bnds[k]=uniques_[l].val; // minval_<bnds[0]<bnds[1]<...<bnds[nparts_-1]==maxval_
   }
   delete[] incs;
  }
  for (int i=0; i<ndata_; i++) {
   datum &dat=data_[i];
   int k=0;
   while (k<nparts_ && bnds[k]<=dat.val_) k++;
   dat.setup_part_(k<nparts_ ? k : nparts_-1);
  }
  partnorm_=bnds[0]-minval_;
  for (int k=1; k<nparts_; k++) partnorm_+=bnds[k]-bnds[k-1];
  partnorm_/=nparts_; // partnorm_>0
  delete[] bnds;
 }
}

void datcolumn::equifreqs_partition_(istream &pars)
{
 using namespace datcolumn_;
 nparts_=def_nparts_;
 pars >> nparts_;
 if (nparts_<2) nparts_=2;
 if (nuniques_<=nparts_) partmeth_=partmeth_t::FineGrain(), finegrain_partition_(nullstream::in);
 else {
  real *bnds;
  {
   real g=0.0;
   for (int k=0; k<nuniques_; k++) g+=uniques_[k].frq;
   real d=g/nparts_; // g>=nuniques_ => d>1
   bnds=new real[nparts_];
   g=0.0;
   int l=0;
   for (int k=0; k<nuniques_; k++) {
    const unique_rec_ &uni=uniques_[k];
    g+=uni.frq;
    if (g>=d) { // l<nparts_
     bnds[l++]=uni.val;
     g=0.0;
    } 
   }
   while (l<nparts_) bnds[l++]=maxval_; // minval_<=bnds[0]<=bnds[1]<=...<=bnds[nparts_-1]==maxval_
  } 
  for (int i=0; i<ndata_; i++) {
   datum &dat=data_[i];
   int k=0;
   while (k<nparts_ && bnds[k]<=dat.val_) k++;
   dat.setup_part_(k<nparts_ ? k : nparts_-1);
  }
  partnorm_=bnds[0]-minval_;
  for (int k=1; k<nparts_; k++) partnorm_+=bnds[k]-bnds[k-1];
  partnorm_/=nparts_; // partnorm_>=0
  delete[] bnds;
 }
}

void datcolumn::maxunidif_partition_(istream &pars)
{
 using namespace datcolumn_;
 nparts_=def_nparts_;
 pars >> nparts_;
 if (nparts_<2) nparts_=2;
 if (nuniques_<=nparts_) partmeth_=partmeth_t::FineGrain(), finegrain_partition_(nullstream::in);
 else {
  real *bnds;
  {
   int n=nuniques_-1; // n>0
   unidif_rec_ *difs=new unidif_rec_[n];
   for (int k=0; k<n; k++) {
    unidif_rec_ &dif=difs[k];
    real v1=uniques_[k].val;
    real v2=uniques_[k+1].val;
    dif.val=v2-v1; // dif.val>0
    dif.pos=v2; // dif.pos>minval_
   }
   qsort(difs,n,sizeof(unidif_rec_),unidif_rec_::decord); // difs[0].val>=difs[1].val>=...>=difs[n-1].val>0
   int k=nparts_-2, l; // k>=0
   real d=difs[k++].val;
   while (k<n && difs[k].val==d) k++;
   nparts_=k+1; // nparts_<=nuniques_
   bnds=new real[nparts_];
   for (l=0; l<k; l++) bnds[l]=difs[l].pos;
   bnds[l]=maxval_; // l==nparts_-1
   qsort(bnds,nparts_,sizeof(real),realincord); // minval_<bnds[0]<bnds[1]<...<bnds[nparts_-1]==maxval_
   delete[] difs;
  } 
  for (int i=0; i<ndata_; i++) {
   datum &dat=data_[i];
   int k=0;
   while (k<nparts_ && bnds[k]<=dat.val_) k++;
   dat.setup_part_(k<nparts_ ? k : nparts_-1);
  }
  partnorm_=bnds[0]-minval_;
  for (int k=1; k<nparts_; k++) partnorm_+=bnds[k]-bnds[k-1];
  partnorm_/=nparts_; // partnorm_>0
  delete[] bnds;
 }
}

void datcolumn::maxfrqdif_partition_(istream &pars)
{
 using namespace datcolumn_;
 nparts_=def_nparts_;
 pars >> nparts_;
 if (nparts_<2) nparts_=2;
 if (nuniques_<=nparts_) partmeth_=partmeth_t::FineGrain(), finegrain_partition_(nullstream::in);
 else {
  real *bnds;
  {
   int n=nuniques_-1; // n>0
   unidif_rec_ *difs=new unidif_rec_[n];
   for (int k=0; k<n; k++) {
    unidif_rec_ &dif=difs[k];
    const unique_rec_ &u1=uniques_[k];
    const unique_rec_ &u2=uniques_[k+1];
    dif.val=abs(u2.frq-u1.frq); // dif.val>=0
    dif.pos=u2.val; // dif.pos>minval_
   }
   qsort(difs,n,sizeof(unidif_rec_),unidif_rec_::decord); // difs[0].val>=difs[1].val>=...>=difs[n-1].val>0
   int k=nparts_-2, l; // k>=0
   real d=difs[k++].val;
   while (k<n && difs[k].val==d) k++;
   nparts_=k+1; // nparts_<=nuniques_
   bnds=new real[nparts_];
   for (l=0; l<k; l++) bnds[l]=difs[l].pos;
   bnds[l]=maxval_; // l==nparts_-1
   qsort(bnds,nparts_,sizeof(real),realincord); // minval_<bnds[0]<bnds[1]<...<bnds[nparts_-1]==maxval_
   delete[] difs;
  } 
  for (int i=0; i<ndata_; i++) {
   datum &dat=data_[i];
   int k=0;
   while (k<nparts_ && bnds[k]<=dat.val_) k++;
   dat.setup_part_(k<nparts_ ? k : nparts_-1);
  }
  partnorm_=bnds[0]-minval_;
  for (int k=1; k<nparts_; k++) partnorm_+=bnds[k]-bnds[k-1];
  partnorm_/=nparts_; // partnorm_>0
  delete[] bnds;
 }
}

} // end of Data namespace

} // end of InfoSelCore namespace

