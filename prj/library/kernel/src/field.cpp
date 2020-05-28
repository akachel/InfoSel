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
/// Definitions For Density Field 
/**
 * @file       field.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.12.02
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
#include "field.h"

namespace InfoSelAPI {
 
namespace Algorithms {
 
namespace Tools {

namespace fieldforms {

/////////////////////////////
////
//// formula class definition
////

formula::formula():
 lnkcnt_(0), 
 is_lstval_(false), is_defval_(false)
{}

formula::~formula()
{
 if (lnkcnt_>0) throw xFieldFormLinkFail();
}

int formula::ndegs() const
{
 int N=0;
 int n=getnargs_();
 for (int k=0; k<n; k++) {
  int m=getadimn_(k);
  N+=m;
 }
 return N;
}

int formula::ndims() const
{
 if (!is_defval_) eval();
 return defval_.ndims();
}

void formula::range(int k,realtuple &min,realtuple &max) const
{ 
 int m=getadimn_(k);
 min.setup(m,Nil);
 max.setup(m,Nil);
 for (int q=0; q<m; q++) {
  real a,b;
  getrange_(k,q,a,b); 
  min[q]=a; max[q]=b;
 }
}
  
void formula::range(int h,real &min,real &max) const
{ 
 int n=getnargs_();
 for (int k=0; k<n; k++) {
  int m=getadimn_(k);
  int q=h;
  if (q<m) {
   getrange_(k,q,min,max); 
   break;
  }
  h-=m;
 }
}
  
const table &formula::eval() const
{ 
 if (!is_defval_) {
  int n=getnargs_();
  if (n<=0) {
   realtuple o;
   defval_=eval(o);
  } else {
   int N=0, h=0;
   for (int k=0; k<n; k++) N+=getadimn_(k);
   realtuple x(N,Nil);
   for (int k=0; k<n; k++) {
    int m=getadimn_(k);
    for (int q=0; q<m; q++) {
     real a,b;
     getrange_(k,q,a,b);
     x[h++]=a+0.5*(b-a);
    }
   }
   defval_=eval(x);
  }
  is_defval_=true;
 }
 return defval_;
}

table formula::eval(const realtuple *x0...) const
{
 va_list ap;
 va_start(ap,x0);
 int n=getnargs_(), N=0, h=0;
 for (int k=0; k<n; k++) N+=getadimn_(k);
 realtuple x(N,Nil);
 for (int k=0; k<n; k++) {
  const realtuple &xk=*(k==0 ? x0 : va_arg(ap,const realtuple *));
  int m=xk.count();
  for (int q=0; q<m; q++) x[h++]=xk[q];
 }
 va_end(ap);
 return eval(x); 
}

table formula::eval(const realtuple &x) const
{ 
 reval();
 return retval(x); 
}

void formula::reval() const
{ 
 is_lstval_=false;
 recalcval_();
}

table formula::retval(const realtuple &x) const
{ 
 if (!is_lstval_) {
  lstval_=calcval_(x); 
  is_lstval_=true;
 } 
 return table(lstval_); 
}

string formula::signat() const
{
 ostringstream bout;
 printsig_(bout);
 return bout.str();
}

void formula::append(formula &trm)
{
 int n=getnterms_();
 int c=getcterms_();
 if (c>=n) return;
 formula *tp=&trm;
 if (c<=0) appterm_(tp);
 else {
  string ts=trm.signat();
  formula &frm=*this;
  bool done=frm.excterm_(ts,tp);
  if (!done) trm.excform_(frm);
  appterm_(tp);
 } 
}

void formula::exclude(formula &trm)
{
 string ts=trm.signat();
 formula *tp=&trm;
 formula &frm=*this;
 bool done=frm.excterm_(ts,tp);
 if (!done) trm.excform_(frm);
}

/////////////////////////////
////
//// nullarform class definition
////

nullarform::nullarform()
{}

nullarform::~nullarform()
{}

void nullarform::recalcval_() const
{}

int nullarform::getnterms_() const
{
 return 0; 
}

int nullarform::getcterms_() const
{
 return 0; 
}

void nullarform::appterm_(formula *)
{}

bool nullarform::excterm_(const string &,formula *)
{
 return false;
}

void nullarform::excform_(formula &)
{}

/////////////////////////////
////
//// multarform class definition
////

multarform::multarform(int n):
 num_(n<1 ? 1 : n)
{}

multarform::~multarform()
{}

int multarform::getnterms_() const
{
 return num_; 
}

/////////////////////////////
////
//// unarform class definition
////

unarform::unarform(formula &trm):
 multarform(1), term_(0)
{ 
 append(trm); // term_!=0 
}

unarform::~unarform()
{ 
 term_->do_relieve(); 
}

void unarform::recalcval_() const
{
 term_->reval(); 
}

int unarform::getcterms_() const
{
 return term_==0 ? 0 : nterms(); 
}

void unarform::appterm_(formula *trp)
{
 term_=trp;
 term_->do_share(); 
}

bool unarform::excterm_(const string &trs,formula *trp)
{
 if (trs==term_->signat()) {
  term_->do_relieve(); 
  term_=trp;
  term_->do_share(); 
  return true;
 } else return term_->exclude(trs,trp);
}

void unarform::excform_(formula &frm)
{
 frm.exclude(*term_);
}

/////////////////////////////
////
//// binarform class definition
////

binarform::binarform(formula &trm1,formula &trm2):
 multarform(2), term1_(0), term2_(0)
{ 
 append(trm1); // term1_!=0
 append(trm2); // term2_!=0
}

binarform::~binarform()
{ 
 term2_->do_relieve(); 
 term1_->do_relieve(); 
}

void binarform::recalcval_() const
{
 term1_->reval(); 
 term2_->reval(); 
}

int binarform::getcterms_() const
{
 return term1_==0 ? 0 : term2_==0 ? 1 : nterms(); 
}

void binarform::appterm_(formula *trp)
{
 if (term1_==0) {
  term1_=trp;
  term1_->do_share(); 
 } else {
  term2_=trp;
  term2_->do_share(); 
 } 
}

bool binarform::excterm_(const string &trs,formula *trp)
{
 bool done1;
 if (trs==term1_->signat()) {
  term1_->do_relieve(); 
  term1_=trp;
  term1_->do_share(); 
  done1=true;
 } else done1=term1_->exclude(trs,trp);
 if (term2_==0) return done1;

 bool done2;
 if (trs==term2_->signat()) {
  term2_->do_relieve(); 
  term2_=trp;
  term2_->do_share(); 
  done2=true;
 } else done2=term2_->exclude(trs,trp);
  
 return done1 && done2; 
}

void binarform::excform_(formula &frm)
{
 frm.exclude(*term1_);
 frm.exclude(*term2_);
}

/////////////////////////////
////
//// formfunc class definition
////

formfunc::formfunc()
{}

formfunc::~formfunc()
{}

/////////////////////////////
////
//// function class definition
////

function::function()
{}

function::~function()
{}

/////////////////////////////
////
//// constant class definition
////

constant::constant()
{}

constant::constant(const table &val):
 cstval_(val)
{}

constant::~constant()
{}

int constant::getnargs_() const
{ 
 return 0;  
}

int constant::getadimn_(int) const
{ 
 return -1;  
}

void constant::getrange_(int,int,real &min,real &max) const
{ 
 min=0.0; max=-1.0; 
}

table constant::calcval_(const realtuple &) const
{ 
 return table(cstval_,true); 
}

void constant::printsig_(ostream &out) const
{ 
 out << "const(" << cstval_ << ')'; 
}

/////////////////////////////
////
//// formoper class definition
////

formoper::formoper()
{}

formoper::~formoper()
{}

/////////////////////////////
////
//// unaroper class definition
////

unaroper::unaroper(formula &trm):
 unarform(trm)
{}

unaroper::~unaroper()
{}

int unaroper::getnargs_() const
{ 
 return subnargs(); 
}

int unaroper::getadimn_(int k) const
{ 
 return subadimn(k); 
}

void unaroper::getrange_(int k,int q,real &min,real &max) const
{ 
 subrange(k,q,min,max); 
}

/////////////////////////////
////
//// binaroper class definition
////

binaroper::binaroper(formula &trm1,formula &trm2):
 binarform(trm1,trm2)
{}

binaroper::~binaroper()
{}

int binaroper::getnargs_() const
{ 
 return subnargs(); 
}

int binaroper::getadimn_(int k) const
{ 
 return subadimn(k); 
}

void binaroper::getrange_(int k,int q,real &min,real &max) const
{ 
 subrange(k,q,min,max); 
}

/////////////////////////////
////
//// formtrans class definition
////

formtrans::formtrans()
{}

formtrans::~formtrans()
{}

/////////////////////////////
////
//// unartrans class definition
////

unartrans::unartrans(formula &trm):
 unarform(trm)
{}

unartrans::~unartrans()
{}

/////////////////////////////
////
//// binartrans class definition
////

binartrans::binartrans(formula &trm1,formula &trm2):
 binarform(trm1,trm2)
{}

binartrans::~binartrans()
{}

} // end of fieldforms namespace

/////////////////////////////
////
//// field class definition
////

field::field(formula &frm):
 form_(&frm)
{ 
 form_->do_share(); 
}

field::field()
{
 form_=new constant;
 form_->do_share(); 
#ifdef SOURCE_DEBUG
 cdbg << "z=const(o):" << endl;
 cdeb << "o = " << form_->eval() << endl;
#endif
}

field::field(const table &val)
{
 form_=new constant(val);
 form_->do_share(); 
#ifdef SOURCE_DEBUG
 cdbg << "z=const(v):" << endl;
 cdeb << "v = " << val << endl;
#endif
}

field::field(const field &fld):
 form_(fld.form_)
{ 
 form_->do_share(); 
}

field::~field()
{ 
 form_->do_relieve(); 
}

field &field::reinit(formula &frm)
{ 
 form_->do_relieve(); 
 form_=&frm; 
 form_->do_share(); 
 return *this; 
}

field &field::operator=(const field &fld)
{ 
 form_->do_relieve(); 
 form_=fld.form_; 
 form_->do_share(); 
 return *this; 
}

namespace { namespace field_ { class op_addeq_r_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)+=a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << "+=" << a_; }
 public:
  op_addeq_r_(formula &trm,real a): unaroper(trm), a_(a)
  {}
};}}
field &field::operator+=(real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op+=(y,a):" << endl;
 cdeb << "y = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif

 return unrexpr<field_::op_addeq_r_>(a); 
}

namespace { namespace field_ { class op_subeq_r_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)-=a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << "-=" << a_; }
 public:
  op_subeq_r_(formula &trm,real a): unaroper(trm), a_(a)
  {}
};}}
field &field::operator-=(real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op-=(y,a):" << endl;
 cdeb << "y = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif

 return unrexpr<field_::op_subeq_r_>(a); 
}

namespace { namespace field_ { class op_muleq_r_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)*=a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << "*=" << a_; }
 public:
  op_muleq_r_(formula &trm,real a): unaroper(trm), a_(a)
  {}
};}}
field &field::operator*=(real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op*=(y,a):" << endl;
 cdeb << "y = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif

 return unrexpr<field_::op_muleq_r_>(a); 
}

namespace { namespace field_ { class op_diveq_r_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)/=a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << "/=" << a_; }
 public:
  op_diveq_r_(formula &trm,real a): unaroper(trm), a_(a)
  {}
};}}
field &field::operator/=(real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op/=(y,a):" << endl;
 cdeb << "y = " << *this << endl;
 cdeb << "a = " << a << endl;
#endif

 return unrexpr<field_::op_diveq_r_>(a); 
}

namespace { namespace field_ { class op_abs_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return abs(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "abs("; printtrm(out); out << ')'; }
 public:
  op_abs_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field abs(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=abs(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_abs_f_>(y); 
}

namespace { namespace field_ { class op_log_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return log(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "log("; printtrm(out); out << ')'; }
 public:
  op_log_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field log(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=log(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_log_f_>(y); 
}

namespace { namespace field_ { class op_log2_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return log2(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "log2("; printtrm(out); out << ')'; }
 public:
  op_log2_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field log2(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=log2(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_log2_f_>(y); 
}

namespace { namespace field_ { class op_sqr_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return sqr(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "sqr("; printtrm(out); out << ')'; }
 public:
  op_sqr_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field sqr(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=sqr(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_sqr_f_>(y); 
}

namespace { namespace field_ { class op_sqrt_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return sqrt(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "sqrt("; printtrm(out); out << ')'; }
 public:
  op_sqrt_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field sqrt(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=sqrt(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_sqrt_f_>(y); 
}

namespace { namespace field_ { class op_cub_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return cub(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "cub("; printtrm(out); out << ')'; }
 public:
  op_cub_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field cub(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=cub(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_cub_f_>(y); 
}

namespace { namespace field_ { class op_pow_fr_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return pow(evaltrm(x),a_); }
  void printsig_(ostream &out) const
  { out << "pow("; printtrm(out); out << ',' << a_ << ')'; }
 public:
  op_pow_fr_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field pow(const field &y,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=pow(y,a):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "a = " << a << endl;
#endif

 return field::unexpr<field_::op_pow_fr_>(y,a); 
}

namespace { namespace field_ { class op_cum_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return cum(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "cum("; printtrm(out); out << ')'; }
 public:
  op_cum_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field cum(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=cum(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_cum_f_>(y); 
}

namespace { namespace field_ { class op_sum_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return sum(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "sum("; printtrm(out); out << ')'; }
 public:
  op_sum_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field sum(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=sum(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_sum_f_>(y); 
}

namespace { namespace field_ { class op_avg_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return avg(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "avg("; printtrm(out); out << ')'; }
 public:
  op_avg_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field avg(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=avg(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_avg_f_>(y); 
}

namespace { namespace field_ { class op_sum_sf_ : public field::unaroper {
 private:
  const Data::space &_F__;
  table calcval_(const realtuple &x) const
  { return sum(_F__,evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "sum(_F_=" << _F__ << ", "; printtrm(out); out << ')'; }
 public:
  op_sum_sf_(field::formula &trm,const Data::space &_F_): field::unaroper(trm), _F__(_F_)
  { _F__.do_hold(); }
  ~op_sum_sf_()
  { _F__.do_release(); }
};}}
field sum(const Data::space &_F_,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=sum(_F_,y):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_sum_sf_>(y,_F_); 
}

namespace { namespace field_ { class op_avg_sf_ : public field::unaroper {
 private:
  const Data::space &_F__;
  table calcval_(const realtuple &x) const
  { return avg(_F__,evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "avg(_F_=" << _F__ << ", "; printtrm(out); out << ')'; }
 public:
  op_avg_sf_(field::formula &trm,const Data::space &_F_): field::unaroper(trm), _F__(_F_)
  { _F__.do_hold(); }
  ~op_avg_sf_()
  { _F__.do_release(); }
};}}
field avg(const Data::space &_F_,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=avg(_F_,y):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_avg_sf_>(y,_F_); 
}

namespace { namespace field_ { class op_prod_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return prod(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "prod("; printtrm(out); out << ')'; }
 public:
  op_prod_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field prod(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=prod(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_prod_f_>(y); 
}

namespace { namespace field_ { class op_prod_sf_ : public field::unaroper {
 private:
  const Data::space &_F__;
  table calcval_(const realtuple &x) const
  { return prod(_F__,evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "prod(_F_=" << _F__ << ", "; printtrm(out); out << ')'; }
 public:
  op_prod_sf_(field::formula &trm,const Data::space &_F_): field::unaroper(trm), _F__(_F_)
  { _F__.do_hold(); }
  ~op_prod_sf_()
  { _F__.do_release(); }
};}}
field prod(const Data::space &_F_,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=prod(_F_,y):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_prod_sf_>(y,_F_); 
}

namespace { namespace field_ { class op_max_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return max(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "max("; printtrm(out); out << ')'; }
 public:
  op_max_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field max(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=max(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_max_f_>(y); 
}

namespace { namespace field_ { class op_max_sf_ : public field::unaroper {
 private:
  const Data::space &_F__;
  table calcval_(const realtuple &x) const
  { return max(_F__,evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "max(_F_=" << _F__ << ", "; printtrm(out); out << ')'; }
 public:
  op_max_sf_(field::formula &trm,const Data::space &_F_): field::unaroper(trm), _F__(_F_)
  { _F__.do_hold(); }
  ~op_max_sf_()
  { _F__.do_release(); }
};}}
field max(const Data::space &_F_,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=max(_F_,y):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_max_sf_>(y,_F_); 
}

namespace { namespace field_ { class op_min_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return min(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "min("; printtrm(out); out << ')'; }
 public:
  op_min_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field min(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=min(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_min_f_>(y); 
}

namespace { namespace field_ { class op_min_sf_ : public field::unaroper {
 private:
  const Data::space &_F__;
  table calcval_(const realtuple &x) const
  { return min(_F__,evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "min(_F_=" << _F__ << ", "; printtrm(out); out << ')'; }
 public:
  op_min_sf_(field::formula &trm,const Data::space &_F_): field::unaroper(trm), _F__(_F_)
  { _F__.do_hold(); }
  ~op_min_sf_()
  { _F__.do_release(); }
};}}
field min(const Data::space &_F_,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=min(_F_,y):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_min_sf_>(y,_F_); 
}

namespace { namespace field_ { class op_max2_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return max2(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "max2("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_max2_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field max2(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=max2(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_max2_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_min2_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return min2(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "min2("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_min2_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field min2(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=min2(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_min2_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_diag_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return diag(evaltrm(x)); }
  void printsig_(ostream &out) const
  { out << "diag("; printtrm(out); out << ')'; }
 public:
  op_diag_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field diag(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=diag(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_diag_f_>(y); 
}

namespace { namespace field_ { class op_diagadd_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return diagadd(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "diagadd("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_diagadd_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field diagadd(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=diagadd(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_diagadd_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_diagsub_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return diagsub(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "diagsub("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_diagsub_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field diagsub(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=diagsub(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_diagsub_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_diagmul_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return diagmul(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "diagmul("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_diagmul_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field diagmul(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=diagmul(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_diagmul_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_diagdiv_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return diagdiv(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "diagdiv("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_diagdiv_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field diagdiv(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=diagdiv(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_diagdiv_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_dyadadd_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return dyadadd(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "dyadadd("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_dyadadd_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field dyadadd(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=dyadadd(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_dyadadd_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_dyadsub_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return dyadsub(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "dyadsub("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_dyadsub_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field dyadsub(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=dyadsub(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_dyadsub_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_dyadmul_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return dyadmul(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "dyadmul("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_dyadmul_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field dyadmul(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=dyadmul(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_dyadmul_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_dyaddiv_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return dyaddiv(evaltrm1(x),evaltrm2(x)); }
  void printsig_(ostream &out) const
  { out << "dyaddiv("; printtrm1(out); out << ','; printtrm2(out); out << ')'; }
 public:
  op_dyaddiv_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field dyaddiv(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=dyaddiv(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_dyaddiv_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_mul_fp_ : public field::unaroper {
 private:
  permutation p_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)*p_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << '*' << p_; }
 public:
  op_mul_fp_(field::formula &trm,const permutation &p): field::unaroper(trm), p_(p)
  {}
};}}
field operator*(const field &y,const permutation &p)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op*(y,p):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "p = " << p << endl;
#endif

 return field::unexpr<field_::op_mul_fp_>(y,p); 
}

namespace { namespace field_ { class op_bnot_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return ~evaltrm(x); }
  void printsig_(ostream &out) const
  { out << '~'; printtrm(out); }
 public:
  op_bnot_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field operator~(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op~(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_bnot_f_>(y); 
}

namespace { namespace field_ { class op_bor_if_ : public field::unaroper {
 private:
  int s_;
  table calcval_(const realtuple &x) const
  { return s_|evaltrm(x); }
  void printsig_(ostream &out) const
  { out << s_ << '|'; printtrm(out); }
 public:
  op_bor_if_(field::formula &trm,int s): field::unaroper(trm), s_(s)
  {}
};}}
field operator|(int s,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op|(s,y):" << endl;
 cdeb << "s = " << s << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_bor_if_>(y,s); 
}

namespace { namespace field_ { class op_bor_fi_ : public field::unaroper {
 private:
  int s_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)|s_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << '|' << s_; }
 public:
  op_bor_fi_(field::formula &trm,int s): field::unaroper(trm), s_(s)
  {}
};}}
field operator|(const field &y,int s)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op|(y,s):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "s = " << s << endl;
#endif

 return field::unexpr<field_::op_bor_fi_>(y,s); 
}

namespace { namespace field_ { class op_add_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return evaltrm1(x)+evaltrm2(x); }
  void printsig_(ostream &out) const
  { printtrm1(out); out << '+'; printtrm2(out); }
 public:
  op_add_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field operator+(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op+(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_add_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_add_rf_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return a_+evaltrm(x); }
  void printsig_(ostream &out) const
  { out << a_ << '+'; printtrm(out); }
 public:
  op_add_rf_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator+(real a,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op+(a,y):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_add_rf_>(y,a); 
}

namespace { namespace field_ { class op_add_fr_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)+a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << '+' << a_; }
 public:
  op_add_fr_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator+(const field &y,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op+(y,a):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "a = " << a << endl;
#endif

 return field::unexpr<field_::op_add_fr_>(y,a); 
}

namespace { namespace field_ { class op_neg_f_ : public field::unaroper {
 private:
  table calcval_(const realtuple &x) const
  { return -evaltrm(x); }
  void printsig_(ostream &out) const
  { out << '-'; printtrm(out); }
 public:
  op_neg_f_(field::formula &trm): field::unaroper(trm)
  {}
};}}
field operator-(const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op-(y):" << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_neg_f_>(y); 
}

namespace { namespace field_ { class op_sub_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return evaltrm1(x)-evaltrm2(x); }
  void printsig_(ostream &out) const
  { printtrm1(out); out << '-'; printtrm2(out); }
 public:
  op_sub_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field operator-(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op-(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_sub_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_sub_rf_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return a_-evaltrm(x); }
  void printsig_(ostream &out) const
  { out << a_ << '-'; printtrm(out); }
 public:
  op_sub_rf_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator-(real a,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op-(a,y):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_sub_rf_>(y,a); 
}

namespace { namespace field_ { class op_sub_fr_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)-a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << '-' << a_; }
 public:
  op_sub_fr_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator-(const field &y,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op-(y,a):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "a = " << a << endl;
#endif

 return field::unexpr<field_::op_sub_fr_>(y,a); 
}

namespace { namespace field_ { class op_mul_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return evaltrm1(x)*evaltrm2(x); }
  void printsig_(ostream &out) const
  { printtrm1(out); out << '*'; printtrm2(out); }
 public:
  op_mul_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field operator*(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op*(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_mul_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_mul_rf_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return a_*evaltrm(x); }
  void printsig_(ostream &out) const
  { out << a_ << '*'; printtrm(out); }
 public:
  op_mul_rf_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator*(real a,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op*(a,y):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_mul_rf_>(y,a); 
}

namespace { namespace field_ { class op_mul_fr_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)*a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << '*' << a_; }
 public:
  op_mul_fr_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator*(const field &y,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op*(y,a):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "a = " << a << endl;
#endif

 return field::unexpr<field_::op_mul_fr_>(y,a); 
}

namespace { namespace field_ { class op_div_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return evaltrm1(x)/evaltrm2(x); }
  void printsig_(ostream &out) const
  { printtrm1(out); out << '/'; printtrm2(out); }
 public:
  op_div_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field operator/(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op/(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_div_ff_>(y1,y2); 
}

namespace { namespace field_ { class op_div_rf_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return a_/evaltrm(x); }
  void printsig_(ostream &out) const
  { out << a_ << '/'; printtrm(out); }
 public:
  op_div_rf_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator/(real a,const field &y)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op/(a,y):" << endl;
 cdeb << "a = " << a << endl;
 cdeb << "y = " << y << endl;
#endif

 return field::unexpr<field_::op_div_rf_>(y,a); 
}

namespace { namespace field_ { class op_div_fr_ : public field::unaroper {
 private:
  real a_;
  table calcval_(const realtuple &x) const
  { return evaltrm(x)/a_; }
  void printsig_(ostream &out) const
  { printtrm(out); out << '/' << a_; }
 public:
  op_div_fr_(field::formula &trm,real a): field::unaroper(trm), a_(a)
  {}
};}}
field operator/(const field &y,real a)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op/(y,a):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "a = " << a << endl;
#endif

 return field::unexpr<field_::op_div_fr_>(y,a); 
}

namespace { namespace field_ { class op_bxor_ff_ : public field::binaroper {
 private:
  table calcval_(const realtuple &x) const
  { return evaltrm1(x)^evaltrm2(x); }
  void printsig_(ostream &out) const
  { printtrm1(out); out << '^'; printtrm2(out); }
 public:
  op_bxor_ff_(field::formula &trm1,field::formula &trm2): field::binaroper(trm1,trm2)
  {}
};}}
field operator^(const field &y1,const field &y2)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=op^(y1,y2):" << endl;
 cdeb << "y1 = " << y1 << endl;
 cdeb << "y2 = " << y2 << endl;
#endif

 return field::binexpr<field_::op_bxor_ff_>(y1,y2); 
}

namespace { namespace field_ {

void integ_trapzd_(const field &fun,realtuple &var,int npars,int nintgs,real lowlim,real upplim,int nsteps,table &ret) //! \pre npars>=0 , npars+1<=var.count , nintgs>0 , nintgs<=ret.count
{
#ifdef SOURCE_DEBUG
 cdbg << "y=integ_trapzd(fun,var,npars,nintgs,lowlim,upplim,nsteps):" << endl;
 cdeb << "nintgs = " << nintgs << endl;
 cdeb << "nsteps = " << nsteps << endl;
#endif
 int io=npars, n=nintgs;
 real ao=lowlim, bo=upplim;
 static realtuple intg, sumy;
 if (nsteps==1) {
  var[io]=ao;
  table y1=fun[var];
  var[io]=bo;
  table y2=fun[var];
  intg.setup(n,Nil);
  sumy.setup(n,Nil);
  for (int k=0; k<n; k++)
  ret[k]=intg[k]=0.5*(bo-ao)*(y1[k]+y2[k]);
 } else {
  int nbins=1;
  for (int step=1; step<nsteps-1; step++) nbins<<=1;
  real nxo=nbins;
  real dxo=(bo-ao)/nxo;
  real xo=ao+0.5*dxo;
  for (int k=0; k<n; k++) sumy[k]=0.0;
  for (int bin=1; bin<=nbins; bin++,xo+=dxo) {
   var[io]=xo;
   table y=fun[var];
   for (int k=0; k<n; k++) sumy[k]+=y[k];
  }
  for (int k=0; k<n; k++)
  ret[k]=intg[k]=0.5*(intg[k]+(bo-ao)*sumy[k]/nxo);
 }
#ifdef SOURCE_DEBUG
 cdbg << "y=integ_trapzd(...):" << endl;
 cdeb << "y = " << ret << endl;
#endif
}

table integ_qtrap_(const field &fun,realtuple &var,int npars,real lowlim,real upplim) //! \pre fun.nargs>0 , fun.adimn(fun.nargs-1)>=1
{
#ifdef SOURCE_DEBUG
 cdbg << "y=integ_qtrap(fun,var,npars,lowlim,upplim):" << endl;
 cdeb << "fun = " << fun << endl;
 cdeb << "var = " << var << endl;
 cdeb << "npars = " << npars << endl;
 cdeb << "lowlim = " << lowlim << endl;
 cdeb << "upplim = " << upplim << endl;
#endif
 const int minnsteps=4;
 const int maxnsteps=10;
 const real accur=1.0e-5;

 table intg; {
  int io=npars;
  real ao=lowlim, bo=upplim;
  var[io]=0.5*(ao+bo);
  table y=fun[var];
  intg=y;
 } 
 int nintgs=intg.count(), n=nintgs; // nintgs>0
 realtuple previntg(n,Nil);
 int inistep=minnsteps;
 for (int step=1; step<=maxnsteps; step++) {
  integ_trapzd_(fun,var,npars,nintgs,lowlim,upplim,step,intg);
  if (inistep>=0) {
   if (inistep<=0)
   for (int k=0; k<n; k++) previntg[k]=intg[k];
   inistep--;
  } else {
   real mincvg=+rInf, previntgmin=0.0, intgmin=0.0;
   for (int k=0; k<n; k++) {
    real cvg=accur*Common::abs(previntg[k])-Common::abs(intg[k]-previntg[k]);
    if (cvg<mincvg) {
     mincvg=cvg;
     previntgmin=previntg[k];
     intgmin=intg[k];
    }
   }
   if (Common::abs(intgmin-previntgmin)<=accur*Common::abs(previntgmin)) break;
   for (int k=0; k<n; k++) previntg[k]=intg[k];
  }
 }
#ifdef SOURCE_DEBUG
 cdbg << "y=integ_qtrap(...):" << endl;
 cdeb << "y = " << intg << endl;
#endif
 return table(intg);
}

table integ_MC_(const field &fun,realtuple &var,int npars,int intgdim,const realtuple &lowlim,const realtuple &upplim) //! \pre fun.nargs>0 , intgdim>1 , intgdim<=fun.adimn(fun.nargs-1) , npars>=0 , npars+intgdim<=var.count , intgdim<=lowlim.count , intgdim<=upplim.count
{
#ifdef SOURCE_DEBUG
 cdbg << "y=integ_MC(fun,var,npars,intgdim,lowlim,upplim):" << endl;
 cdeb << "fun = " << fun << endl;
 cdeb << "var = " << var << endl;
 cdeb << "npars = " << npars << endl;
 cdeb << "intgdim = " << intgdim << endl;
 cdeb << "lowlim = " << lowlim << endl;
 cdeb << "upplim = " << upplim << endl;
#endif
 const int nsteps=100;
 
 int io=npars, mo=intgdim;
 const realtuple &ao=lowlim, &bo=upplim;
 table intg; {
  for (int q=0; q<mo; q++) var[io+q]=unirand(ao[q],bo[q]);
  table y=fun[var];
  intg=y;
 } 
 int nintgs=intg.count(), n=nintgs; // nintgs>0
 for (int step=1; step<nsteps; step++) {
  for (int q=0; q<mo; q++) var[io+q]=unirand(ao[q],bo[q]);
  table y=fun[var];
  for (int k=0; k<n; k++) intg[k]+=y[k];
 }
 intg/=nsteps;
#ifdef SOURCE_DEBUG
 cdbg << "y=integ_MC(...):" << endl;
 cdeb << "y = " << intg << endl;
#endif
 return table(intg);
}

}} // end of field_ namespace

namespace { namespace field_ { class op_integral_frtrt_ : public field::unartrans {
 private:
  field fun_;
  mutable realtuple var_;
  int nargso_, ndegso_, adimno_;
  realtuple ao_, bo_;
  int getnargs_() const
  { return nargso_;  }
  int getadimn_(int k) const
  { return subadimn(k); }
  void getrange_(int k,int q,real &min,real &max) const
  { subrange(k,q,min,max); }
  table calcval_(const realtuple &x) const
  { 
   for (int h=0; h<ndegso_; h++) var_[h]=x[h];
   if (adimno_==1) return integ_qtrap_(fun_,var_,ndegso_,*ao_,*bo_);
   else return integ_MC_(fun_,var_,ndegso_,adimno_,ao_,bo_);
  }
  void printsig_(ostream &out) const
  { out << "integ(ao=" << ao_ << ", bo=" << bo_ << ")("; printtrm(out); out << ')'; }
 public:
  op_integral_frtrt_(field::formula &trm,const realtuple &ao,const realtuple &bo):
   field::unartrans(trm),
   fun_(trm), ao_(ao), bo_(bo)
  {
   nargso_=fun_.nargs()-1; // nargso_>=0
   adimno_=fun_.adimn(nargso_); // adimno_>0
   int N=fun_.ndegs();
   ndegso_=N-adimno_;
   var_.setup(N,Nil);
  }
};}}
field integral(const field &y,const realtuple &ao,const realtuple &bo)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=integral(y,ao,bo):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "ao = " << ao << endl;
 cdeb << "bo = " << bo << endl;
#endif

 int n=y.nargs();
 if (n<1) return sum(y);
 int mo=y.adimn(n-1);
 if (ao.count()<mo || bo.count()<mo) throw xPrecondFail();
 for (int q=0; q<mo; q++)
 if (bo[q]<=ao[q]) throw xPrecondFail();

 return field::unexpr<field_::op_integral_frtrt_>(y,ao,bo); 
}

field integral(const field &y,real fo)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=integral(y,fo):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "fo = " << fo << endl;
#endif
 int n=y.nargs();
 if (n<1) return sum(y);
 if (fo<0.0) fo=0.0;
 int ko=n-1;
 int mo=y.adimn(ko);
 realtuple ao(mo,Nil); 
 realtuple bo(mo,Nil);
 for (int q=0; q<mo; q++) {
  real min,max;
  y.range(ko,q,min,max);
  if (fo>0.0) {
   real inc=fo*0.5*(max-min);
   min-=inc;
   max+=inc;
  }
  ao[q]=min; bo[q]=max;
 }
 return integral(y,ao,bo);
}
      
namespace { namespace field_ { class op_cumugral_fr_ : public field::unartrans {
 private:
  field fun_;
  mutable realtuple var_;
  int ndegso_, adimno_;
  real fo_;
  realtuple ao_;
  mutable realtuple bo_;
  int getnargs_() const
  { return subnargs();  }
  int getadimn_(int k) const
  { return subadimn(k); }
  void getrange_(int k,int q,real &min,real &max) const
  { subrange(k,q,min,max); }
  table calcval_(const realtuple &x) const
  { 
   for (int h=0; h<ndegso_; h++) var_[h]=x[h];
   for (int q=0; q<adimno_; q++) bo_[q]=x[ndegso_+q];
   if (adimno_==1) return integ_qtrap_(fun_,var_,ndegso_,*ao_,*bo_);
   else return integ_MC_(fun_,var_,ndegso_,adimno_,ao_,bo_);
  }
  void printsig_(ostream &out) const
  { out << "cumug(fo=" << fo_ << ")("; printtrm(out); out << ')'; }
 public:
  op_cumugral_fr_(field::formula &trm,real fo):
   field::unartrans(trm),
   fun_(trm), fo_(fo<0.0 ? 0.0 : fo)
  {
   int ko=fun_.nargs()-1; // ko>=0
   adimno_=fun_.adimn(ko); // adimno_>0
   int N=fun_.ndegs();
   ndegso_=N-adimno_;
   var_.setup(N,Nil);
   for (int q=0; q<adimno_; q++) {
    real min,max;
    fun_.range(ko,q,min,max);
    if (fo_>0.0) {
     real inc=fo_*0.5*(max-min);
     min-=inc;
    }
    ao_[q]=min;
   }
  }
};}}
field cumugral(const field &y,real fo)
{
#ifdef SOURCE_DEBUG
 cdbg << "z=cumugral(y,fo):" << endl;
 cdeb << "y = " << y << endl;
 cdeb << "fo = " << fo << endl;
#endif

 if (y.nargs()<1) return sum(y);
 else return field::unexpr<field_::op_cumugral_fr_>(y,fo); 
}

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

