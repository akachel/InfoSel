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
/// Defintions Of Gaussian Field Routines
/**
 * @file       gauss.cpp
 * @version    1.00
 * @author     Adam Kachel, Jacek Biesiada
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
#include "gauss.h"

namespace InfoSelAPI {
 
namespace Algorithms {
 
namespace Tools {

namespace gausses {

/////////////////////////////
////
//// gaussian field
//// routine definitions
////

namespace {

class gauss_field_function_ : public field::function {
  real sd1_, sd2_;

  real funcval_(real x,real u) const
  { 
   real dx=x-u;
   return sd1_*exp(-dx*dx*sd2_); 
  }

  real funcmul_(const realtuple &x,const realtuple &u) const //! \pre x.count>=ndegs , u.count==ndegs 
  {
   real y=1.0;
   int N=u.count();
   for (int h=0; h<N; h++)
   y*=funcval_(x[h],u[h]);
   return y;
  }
  real funcmul_(const realtuple &x,const realtuple *u[]) const //! \pre x.count>=ndegs , count(u)>=nargs , u[k].count==adimn(k) , 0 <= k < nargs
  {
   real y=1.0;
   int h=0;
   int n=nargs_;
   for (int k=0; k<n; k++) {
    const realtuple &uk=*u[k];
    int m=uk.count();
    for (int q=0; q<m; q++)
    y*=funcval_(x[h++],uk[q]);
   }  
   return y;
  }
 protected:
  real sd_;
  int nargs_, ninsts_;
  mutable table ret_;

  real gaussfunc_(const realtuple &x,const realtuple &u0) const //! \pre nargs==1 , x.count>=ndegs , uk.count==adimn(k) , 0 <= k < nargs
  {
   const realtuple &u=u0;
   real y=funcmul_(x,u);
   return y;
  }
  real gaussfunc_(const realtuple &x,const realtuple &u0,const realtuple &u1) const //! \pre nargs==2
  {
   const realtuple *u[]={&u0,&u1};
   real y=funcmul_(x,u);
   return y;
  }
  real gaussfunc_(const realtuple &x,const realtuple &u0,const realtuple &u1,const realtuple &u2) const //! \pre nargs==3
  {
   const realtuple *u[]={&u0,&u1,&u2};
   real y=funcmul_(x,u);
   return y;
  }
  real gaussfunc_(const realtuple &x,const realtuple &u0,const realtuple &u1,const realtuple &u2,const realtuple &u3) const //! \pre nargs==4
  {
   const realtuple *u[]={&u0,&u1,&u2,&u3};
   real y=funcmul_(x,u);
   return y;
  }
  real gaussfunc_(const realtuple &x,const realtuple &u0,const realtuple &u1,const realtuple &u2,const realtuple &u3,const realtuple &u4) const //! \pre nargs==5
  {
   const realtuple *u[]={&u0,&u1,&u2,&u3,&u4};
   real y=funcmul_(x,u);
   return y;
  }
 public:
  gauss_field_function_(real sd): sd_(sd)
  {
   sd1_=1.0/(std::sqrt(2.0*mPI)*sd); 
   sd2_=1.0/(2.0*sd*sd); 
  }
};

}

namespace { class fun_gauss_ssar_ : public gauss_field_function_ {
 private:
  const Data::space &_F__;
  const Data::space &_S__;
  const Data::attrib &C_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && _F__.issmoothed() ? _F__.subcard() : _S__.subcard();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   min=+rInf; max=-rInf;
   if (k<=0 && _F__.issmoothed())
   for (Data::subset f(_F__); !f.isnil(); ++f) {
    real a,b;
    f(q).range(a,b); 
    if (a<min) min=a;
    if (b>max) max=b;
   } else
   for (Data::subset s(_S__); !s.isnil(); ++s) {
    real a,b;
    s(q).range(a,b); 
    if (a<min) min=a;
    if (b>max) max=b;
   }
  }
  table calcval_(const realtuple &x) const
  { 
   if (_F__.issmoothed() && _S__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s,C_[i])+=gaussfunc_(x,f[i],s[i]);
    y/=n;
    return table(y);
   } else
   if (_F__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s,s[i],C_[i])+=gaussfunc_(x,f[i]);
    y/=n;
    return table(y);
   } else
   if (_S__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s,f[i],C_[i])+=gaussfunc_(x,s[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s,f[i],s[i],C_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(" << "_F_=" << _F__ << ", _S_=" << _S__ << ", C=" << C_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_ssar_(const Data::space &_F_,const Data::space &_S_,const Data::attrib &C,real sd):
   gauss_field_function_(sd),
   _F__(_F_), _S__(_S_), C_(C)
  { 
   _F__.do_hold(); _S__.do_hold(); C_.do_hold(); 
   ninsts_=min3(_F__.ninsts(),_S__.ninsts(),C_.ninsts());
   if (_F__.issmoothed() && _S__.issmoothed()) {
    nargs_=2;
    ret_=table(_F__.card(),_S__.card(),C_.nparts());
   } else
   if (_F__.issmoothed()) {
    nargs_=1;
    ret_=table(_F__.card(),_S__.card(),_S__.nparts(),C_.nparts());
   } else
   if (_S__.issmoothed()) {
    nargs_=1;
    ret_=table(_F__.card(),_S__.card(),_F__.nparts(),C_.nparts());
   } else {
    nargs_=0;
    ret_=table(_F__.card(),_S__.card(),_F__.nparts(),_S__.nparts(),C_.nparts());
   }
  }
  ~fun_gauss_ssar_()
  { C_.do_release(); _S__.do_release(); _F__.do_release(); }
};}
field gauss(const Data::space &_F_,const Data::space &_S_,const Data::attrib &C,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(_F_,_S_,C,sd):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "_S_ = " << _S_ << endl;
 cdeb << "C = " << C << endl;
 cdeb << "sd = " << sd << endl;
#endif

 if (C.issmoothed()) throw xPrecondFail();
 return field::nulexpr<fun_gauss_ssar_>(_F_,_S_,C,sd);
}

namespace { class fun_gauss_ssr_ : public gauss_field_function_ {
 private:
  const Data::space &_F__;
  const Data::space &_S__;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && _F__.issmoothed() ? _F__.subcard() : _S__.subcard();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   min=+rInf; max=-rInf;
   if (k<=0 && _F__.issmoothed())
   for (Data::subset f(_F__); !f.isnil(); ++f) {
    real a,b;
    f(q).range(a,b); 
    if (a<min) min=a;
    if (b>max) max=b;
   } else
   for (Data::subset s(_S__); !s.isnil(); ++s) {
    real a,b;
    s(q).range(a,b); 
    if (a<min) min=a;
    if (b>max) max=b;
   }
  }
  table calcval_(const realtuple &x) const
  { 
   if (_F__.issmoothed() && _S__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s)+=gaussfunc_(x,f[i],s[i]);
    y/=n;
    return table(y);
   } else
   if (_F__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s,s[i])+=gaussfunc_(x,f[i]);
    y/=n;
    return table(y);
   } else
   if (_S__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s,f[i])+=gaussfunc_(x,s[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (Data::subset s(_S__); !s.isnil(); ++s)
    for (int i=0; i<n; i++) y(f,s,f[i],s[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(" << "_F_=" << _F__ << ", _S_=" << _S__ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_ssr_(const Data::space &_F_,const Data::space &_S_,real sd):
   gauss_field_function_(sd),
   _F__(_F_), _S__(_S_)
  { 
   _F__.do_hold(); _S__.do_hold();
   ninsts_=Common::min2(_F__.ninsts(),_S__.ninsts());
   if (_F__.issmoothed() && _S__.issmoothed()) {
    nargs_=2;
    ret_=table(_F__.card(),_S__.card());
   } else
   if (_F__.issmoothed()) {
    nargs_=1;
    ret_=table(_F__.card(),_S__.card(),_S__.nparts());
   } else
   if (_S__.issmoothed()) {
    nargs_=1;
    ret_=table(_F__.card(),_S__.card(),_F__.nparts());
   } else {
    nargs_=0;
    ret_=table(_F__.card(),_S__.card(),_F__.nparts(),_S__.nparts());
   }
  }
  ~fun_gauss_ssr_()
  { _S__.do_release(); _F__.do_release(); }
};}
field gauss(const Data::space &_F_,const Data::space &_S_,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(_F_,_S_,sd):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "_S_ = " << _S_ << endl;
 cdeb << "sd = " << sd << endl;
#endif

 return field::nulexpr<fun_gauss_ssr_>(_F_,_S_,sd);
}

namespace { class fun_gauss_sar_ : public gauss_field_function_ {
 private:
  const Data::space &_F__;
  const Data::attrib &C_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int) const
  { return _F__.subcard();  }
  void getrange_(int,int q,real &min,real &max) const
  { 
   min=+rInf; max=-rInf;
   for (Data::subset f(_F__); !f.isnil(); ++f) {
    real a,b;
    f(q).range(a,b); 
    if (a<min) min=a;
    if (b>max) max=b;
   }
  }
  table calcval_(const realtuple &x) const
  { 
   if (_F__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (int i=0; i<n; i++) y(f,C_[i])+=gaussfunc_(x,f[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (int i=0; i<n; i++) y(f,f[i],C_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(" << "_F_=" << _F__ << ", C=" << C_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_sar_(const Data::space &_F_,const Data::attrib &C,real sd):
   gauss_field_function_(sd),
   _F__(_F_), C_(C)
  { 
   _F__.do_hold(); C_.do_hold(); 
   ninsts_=Common::min2(_F__.ninsts(),C_.ninsts());
   if (_F__.issmoothed()) {
    nargs_=1;
    ret_=table(_F__.card(),C_.nparts());
   } else {
    nargs_=0;
    ret_=table(_F__.card(),_F__.nparts(),C_.nparts());
   }
  }
  ~fun_gauss_sar_()
  { C_.do_release(); _F__.do_release(); }
};}
field gauss(const Data::space &_F_,const Data::attrib &C,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(_F_,C,sd):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "C = " << C << endl;
 cdeb << "sd = " << sd << endl;
#endif

 if (C.issmoothed()) throw xPrecondFail();
 return field::nulexpr<fun_gauss_sar_>(_F_,C,sd);
}

namespace { class fun_gauss_sr_ : public gauss_field_function_ {
 private:
  const Data::space &_F__;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int) const
  { return _F__.subcard();  }
  void getrange_(int,int q,real &min,real &max) const
  { 
   min=+rInf; max=-rInf;
   for (Data::subset f(_F__); !f.isnil(); ++f) {
    real a,b;
    f(q).range(a,b); 
    if (a<min) min=a;
    if (b>max) max=b;
   }
  }
  table calcval_(const realtuple &x) const
  { 
   if (_F__.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (int i=0; i<n; i++) y(f)+=gaussfunc_(x,f[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (Data::subset f(_F__); !f.isnil(); ++f)
    for (int i=0; i<n; i++) y(f,f[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(" << "_F_=" << _F__ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_sr_(const Data::space &_F_,real sd):
   gauss_field_function_(sd),
   _F__(_F_)
  { 
   _F__.do_hold(); 
   ninsts_=_F__.ninsts();
   if (_F__.issmoothed()) {
    nargs_=1;
    ret_=table(_F__.card());
   } else {
    nargs_=0;
    ret_=table(_F__.card(),_F__.nparts());
   }
  }
  ~fun_gauss_sr_()
  { _F__.do_release(); }
};}
field gauss(const Data::space &_F_,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(_F_,sd):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "sd = " << sd << endl;
#endif

 return field::nulexpr<fun_gauss_sr_>(_F_,sd);
}

namespace { class fun_gauss_bbar_ : public gauss_field_function_ {
 private:
  const Data::subset &f_;
  const Data::subset &s_;
  const Data::attrib &C_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && f_.issmoothed() ? f_.card() : s_.card();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   if (k<=0 && f_.issmoothed()) f_(q).range(min,max); 
   else s_(q).range(min,max);
  }
  table calcval_(const realtuple &x) const
  { 
   if (f_.issmoothed() && s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(C_[i])+=gaussfunc_(x,f_[i],s_[i]);
    y/=n;
    return table(y);
   } else
   if (f_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(s_[i],C_[i])+=gaussfunc_(x,f_[i]);
    y/=n;
    return table(y);
   } else
   if (s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(f_[i],C_[i])+=gaussfunc_(x,s_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(f_[i],s_[i],C_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(f=" << f_ << ", s=" << s_ << ", C=" << C_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_bbar_(const Data::subset &f,const Data::subset &s,const Data::attrib &C,real sd):
   gauss_field_function_(sd),
   f_(f), s_(s), C_(C)
  { 
   f_.do_hold(); s_.do_hold(); C_.do_hold(); 
   ninsts_=min3(f_.ninsts(),s_.ninsts(),C_.ninsts());
   if (f_.issmoothed() && s_.issmoothed()) {
    nargs_=2;
    ret_=table(C_.nparts());
   } else
   if (f_.issmoothed()) {
    nargs_=1;
    ret_=table(s_.nparts(),C_.nparts());
   } else
   if (s_.issmoothed()) {
    nargs_=1;
    ret_=table(f_.nparts(),C_.nparts());
   } else {
    nargs_=0;
    ret_=table(f_.nparts(),s_.nparts(),C_.nparts());
   }
  }
  ~fun_gauss_bbar_()
  { C_.do_release(); s_.do_release(); f_.do_release(); }
};}
field gauss(const Data::subset &f,const Data::subset &s,const Data::attrib &C,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(f,s,C,sd):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
 cdeb << "C = " << C << endl;
 cdeb << "sd = " << sd << endl;
#endif

 if (C.issmoothed()) throw xPrecondFail();
 return field::nulexpr<fun_gauss_bbar_>(f,s,C,sd);
}

namespace { class fun_gauss_bbr_ : public gauss_field_function_ {
 private:
  const Data::subset &f_;
  const Data::subset &s_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && f_.issmoothed() ? f_.card() : s_.card();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   if (k<=0 && f_.issmoothed()) f_(q).range(min,max); 
   else s_(q).range(min,max);
  }
  table calcval_(const realtuple &x) const
  { 
   if (f_.issmoothed() && s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y()+=gaussfunc_(x,f_[i],s_[i]);
    y/=n;
    return table(y);
   } else
   if (f_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(s_[i])+=gaussfunc_(x,f_[i]);
    y/=n;
    return table(y);
   } else
   if (s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(f_[i])+=gaussfunc_(x,s_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(f_[i],s_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(f=" << f_ << ", s=" << s_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_bbr_(const Data::subset &f,const Data::subset &s,real sd):
   gauss_field_function_(sd),
   f_(f), s_(s)
  { 
   f_.do_hold(); s_.do_hold(); 
   ninsts_=Common::min2(f_.ninsts(),s_.ninsts());
   if (f_.issmoothed() && s_.issmoothed()) {
    nargs_=2;
    ret_=table();
   } else
   if (f_.issmoothed()) {
    nargs_=1;
    ret_=table(s_.nparts());
   } else
   if (s_.issmoothed()) {
    nargs_=1;
    ret_=table(f_.nparts());
   } else {
    nargs_=0;
    ret_=table(f_.nparts(),s_.nparts());
   }
  }
  ~fun_gauss_bbr_()
  { s_.do_release(); f_.do_release(); }
};}
field gauss(const Data::subset &f,const Data::subset &s,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(f,s,sd):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
 cdeb << "sd = " << sd << endl;
#endif

 return field::nulexpr<fun_gauss_bbr_>(f,s,sd);
}

namespace { class fun_gauss_bar_ : public gauss_field_function_ {
 private:
  const Data::subset &f_;
  const Data::attrib &C_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int) const
  { return f_.card();  }
  void getrange_(int,int q,real &min,real &max) const
  { f_(q).range(min,max); }
  table calcval_(const realtuple &x) const
  { 
   if (f_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(C_[i])+=gaussfunc_(x,f_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(f_[i],C_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(f=" << f_ << ", C=" << C_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_bar_(const Data::subset &f,const Data::attrib &C,real sd):
   gauss_field_function_(sd),
   f_(f), C_(C)
  { 
   f_.do_hold(); C_.do_hold(); 
   ninsts_=Common::min2(f_.ninsts(),C_.ninsts());
   if (f_.issmoothed()) {
    nargs_=1;
    ret_=table(C_.nparts());
   } else {
    nargs_=0;
    ret_=table(f_.nparts(),C_.nparts());
   }
  }
  ~fun_gauss_bar_()
  { C_.do_release(); f_.do_release(); }
};}
field gauss(const Data::subset &f,const Data::attrib &C,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(f,C,sd):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "C = " << C << endl;
 cdeb << "sd = " << sd << endl;
#endif

 if (C.issmoothed()) throw xPrecondFail();
 return field::nulexpr<fun_gauss_bar_>(f,C,sd);
}

namespace { class fun_gauss_br_ : public gauss_field_function_ {
 private:
  const Data::subset &f_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int) const
  { return f_.card();  }
  void getrange_(int,int q,real &min,real &max) const
  { f_(q).range(min,max); }
  table calcval_(const realtuple &x) const
  { 
   if (f_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y()+=gaussfunc_(x,f_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(f_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(f=" << f_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_br_(const Data::subset &f,real sd): 
   gauss_field_function_(sd),
   f_(f)
  { 
   f_.do_hold(); 
   ninsts_=f_.ninsts();
   if (f_.issmoothed()) {
    nargs_=1;
    ret_=table();
   } else {
    nargs_=0;
    ret_=table(f_.nparts());
   }
  }
  ~fun_gauss_br_()
  { f_.do_release(); }
};}
field gauss(const Data::subset &f,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(f,sd):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "sd = " << sd << endl;
#endif

 return field::nulexpr<fun_gauss_br_>(f,sd);
}

namespace { class fun_gauss_ttar_ : public gauss_field_function_ {
 private:
  const Data::set &F_;
  const Data::set &S_;
  const Data::attrib &C_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && F_.issmoothed() ? F_.card() : S_.card();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   if (k<=0 && F_.issmoothed()) F_(q).range(min,max); 
   else S_(q).range(min,max);
  }
  table calcval_(const realtuple &x) const
  { 
   if (F_.issmoothed() && S_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(C_[i])+=gaussfunc_(x,F_[i],S_[i]);
    y/=n;
    return table(y);
   } else
   if (F_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(S_[i],C_[i])+=gaussfunc_(x,F_[i]);
    y/=n;
    return table(y);
   } else
   if (S_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i],C_[i])+=gaussfunc_(x,S_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i],S_[i],C_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(F=" << F_ << ", S=" << S_ << ", C=" << C_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_ttar_(const Data::set &F,const Data::set &S,const Data::attrib &C,real sd):
   gauss_field_function_(sd),
   F_(F), S_(S), C_(C)
  { 
   F_.do_hold(); S_.do_hold(); C_.do_hold();
   ninsts_=min3(F_.ninsts(),S_.ninsts(),C_.ninsts());
   if (F_.issmoothed() && S_.issmoothed()) {
    nargs_=2;
    ret_=table(C_.nparts());
   } else
   if (F_.issmoothed()) {
    nargs_=1;
    ret_=table(S_.nparts(),C_.nparts());
   } else
   if (S_.issmoothed()) {
    nargs_=1;
    ret_=table(F_.nparts(),C_.nparts());
   } else {
    nargs_=0;
    ret_=table(F_.nparts(),S_.nparts(),C_.nparts());
   }
  }
  ~fun_gauss_ttar_()
  { C_.do_release(); S_.do_release(); F_.do_release(); }
};}
field gauss(const Data::set &F,const Data::set &S,const Data::attrib &C,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(F,S,C,sd):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
 cdeb << "C = " << C << endl;
 cdeb << "sd = " << sd << endl;
#endif

 if (C.issmoothed()) throw xPrecondFail();
 return field::nulexpr<fun_gauss_ttar_>(F,S,C,sd);
}

namespace { class fun_gauss_ttr_ : public gauss_field_function_ {
 private:
  const Data::set &F_;
  const Data::set &S_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && F_.issmoothed() ? F_.card() : S_.card();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   if (k<=0 && F_.issmoothed()) F_(q).range(min,max); 
   else S_(q).range(min,max);
  }
  table calcval_(const realtuple &x) const
  { 
   if (F_.issmoothed() && S_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y()+=gaussfunc_(x,F_[i],S_[i]);
    y/=n;
    return table(y);
   } else
   if (F_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(S_[i])+=gaussfunc_(x,F_[i]);
    y/=n;
    return table(y);
   } else
   if (S_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i])+=gaussfunc_(x,S_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i],S_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(F=" << F_ << ", S=" << S_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_ttr_(const Data::set &F,const Data::set &S,real sd): 
   gauss_field_function_(sd),
   F_(F), S_(S)
  { 
   F_.do_hold(); S_.do_hold(); 
   ninsts_=Common::min2(F_.ninsts(),S_.ninsts());
   if (F_.issmoothed() && S_.issmoothed()) {
    nargs_=2;
    ret_=table();
   } else
   if (F_.issmoothed()) {
    nargs_=1;
    ret_=table(S_.nparts());
   } else
   if (S_.issmoothed()) {
    nargs_=1;
    ret_=table(F_.nparts());
   } else {
    nargs_=0;
    ret_=table(F_.nparts(),S_.nparts());
   }
  }
  ~fun_gauss_ttr_()
  { S_.do_release(); F_.do_release(); }
};}
field gauss(const Data::set &F,const Data::set &S,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(F,S,sd):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
 cdeb << "sd = " << sd << endl;
#endif

 return field::nulexpr<fun_gauss_ttr_>(F,S,sd);
}

namespace { class fun_gauss_tar_ : public gauss_field_function_ {
 private:
  const Data::set &F_;
  const Data::attrib &C_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int) const
  { return F_.card();  }
  void getrange_(int,int q,real &min,real &max) const
  { F_(q).range(min,max); }
  table calcval_(const realtuple &x) const
  { 
   if (F_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(C_[i])+=gaussfunc_(x,F_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i],C_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(F=" << F_ << ", C=" << C_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_tar_(const Data::set &F,const Data::attrib &C,real sd): 
   gauss_field_function_(sd),
   F_(F), C_(C)
  { 
   F_.do_hold(); C_.do_hold(); 
   ninsts_=Common::min2(F_.ninsts(),C_.ninsts());
   if (F_.issmoothed()) {
    nargs_=1;
    ret_=table(C_.nparts());
   } else {
    nargs_=0;
    ret_=table(F_.nparts(),C_.nparts());
   }
  }
  ~fun_gauss_tar_()
  { C_.do_release(); F_.do_release(); }
};}
field gauss(const Data::set &F,const Data::attrib &C,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(F,C,sd):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "C = " << C << endl;
 cdeb << "sd = " << sd << endl;
#endif

 if (C.issmoothed()) throw xPrecondFail();
 return field::nulexpr<fun_gauss_tar_>(F,C,sd);
}

namespace { class fun_gauss_tr_ : public gauss_field_function_ {
 private:
  const Data::set &F_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int) const
  { return F_.card();  }
  void getrange_(int,int q,real &min,real &max) const
  { F_(q).range(min,max); }
  table calcval_(const realtuple &x) const
  { 
   if (F_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y()+=gaussfunc_(x,F_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(F=" << F_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_tr_(const Data::set &F,real sd):
   gauss_field_function_(sd),
   F_(F)
  { 
   F_.do_hold(); 
   ninsts_=F_.ninsts();
   if (F_.issmoothed()) {
    nargs_=1;
    ret_=table();
   } else {
    nargs_=0;
    ret_=table(F_.nparts());
   }
  }
  ~fun_gauss_tr_()
  { F_.do_release(); }
};}
field gauss(const Data::set &F,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(F,sd):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "sd = " << sd << endl;
#endif

 return field::nulexpr<fun_gauss_tr_>(F,sd);
}

namespace { class fun_gauss_tbar_ : public gauss_field_function_ {
 private:
  const Data::set &F_;
  const Data::subset &s_;
  const Data::attrib &C_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && F_.issmoothed() ? F_.card() : s_.card();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   if (k<=0 && F_.issmoothed()) F_(q).range(min,max); 
   else s_(q).range(min,max);
  }
  table calcval_(const realtuple &x) const
  { 
   if (F_.issmoothed() && s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(C_[i])+=gaussfunc_(x,F_[i],s_[i]);
    y/=n;
    return table(y);
   } else
   if (F_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(s_[i],C_[i])+=gaussfunc_(x,F_[i]);
    y/=n;
    return table(y);
   } else
   if (s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i],C_[i])+=gaussfunc_(x,s_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i],s_[i],C_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(F=" << F_ << ", s=" << s_ << ", C=" << C_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_tbar_(const Data::set &F,const Data::subset &s,const Data::attrib &C,real sd):
   gauss_field_function_(sd),
   F_(F), s_(s), C_(C)
  { 
   F_.do_hold(); s_.do_hold(); C_.do_hold(); 
   ninsts_=min3(F_.ninsts(),s_.ninsts(),C_.ninsts());
   if (F_.issmoothed() && s_.issmoothed()) {
    nargs_=2;
    ret_=table(C_.nparts());
   } else
   if (F_.issmoothed()) {
    nargs_=1;
    ret_=table(s_.nparts(),C_.nparts());
   } else
   if (s_.issmoothed()) {
    nargs_=1;
    ret_=table(F_.nparts(),C_.nparts());
   } else {
    nargs_=0;
    ret_=table(F_.nparts(),s_.nparts(),C_.nparts());
   }
  }
  ~fun_gauss_tbar_()
  { C_.do_release(); s_.do_release(); F_.do_release(); }
};}
field gauss(const Data::set &F,const Data::subset &s,const Data::attrib &C,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(F,s,C,sd):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
 cdeb << "C = " << C << endl;
 cdeb << "sd = " << sd << endl;
#endif

 if (C.issmoothed()) throw xPrecondFail();
 return field::nulexpr<fun_gauss_tbar_>(F,s,C,sd);
}

namespace { class fun_gauss_tbr_ : public gauss_field_function_ {
 private:
  const Data::set &F_;
  const Data::subset &s_;
  int getnargs_() const
  { return nargs_;  }
  int getadimn_(int k) const
  { return k<=0 && F_.issmoothed() ? F_.card() : s_.card();  }
  void getrange_(int k,int q,real &min,real &max) const
  { 
   if (k<=0 && F_.issmoothed()) F_(q).range(min,max); 
   else s_(q).range(min,max);
  }
  table calcval_(const realtuple &x) const
  { 
   if (F_.issmoothed() && s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y()+=gaussfunc_(x,F_[i],s_[i]);
    y/=n;
    return table(y);
   } else
   if (F_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(s_[i])+=gaussfunc_(x,F_[i]);
    y/=n;
    return table(y);
   } else
   if (s_.issmoothed()) {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i])+=gaussfunc_(x,s_[i]);
    y/=n;
    return table(y);
   } else {
    table &y=ret_;
    int n=ninsts_;
    for (int i=0; i<n; i++) y(F_[i],s_[i])++;
    y/=n;
    return table(y);
   }
  }
  void printsig_(ostream &out) const
  { out << "gauss(F=" << F_ << ", s=" << s_ << ", sd=" << sd_ << ')'; }
 public:
  fun_gauss_tbr_(const Data::set &F,const Data::subset &s,real sd):
   gauss_field_function_(sd),
   F_(F), s_(s)
  { 
   F_.do_hold(); s_.do_hold(); 
   ninsts_=Common::min2(F_.ninsts(),s_.ninsts());
   if (F_.issmoothed() && s_.issmoothed()) {
    nargs_=2;
    ret_=table();
   } else
   if (F_.issmoothed()) {
    nargs_=1;
    ret_=table(s_.nparts());
   } else
   if (s_.issmoothed()) {
    nargs_=1;
    ret_=table(F_.nparts());
   } else {
    nargs_=0;
    ret_=table(F_.nparts(),s_.nparts());
   }
  }
  ~fun_gauss_tbr_()
  { s_.do_release(); F_.do_release(); }
};}
field gauss(const Data::set &F,const Data::subset &s,real sd)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=gauss(F,s,sd):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
 cdeb << "sd = " << sd << endl;
#endif

 return field::nulexpr<fun_gauss_tbr_>(F,s,sd);
}

} // end of gausses namespace

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

