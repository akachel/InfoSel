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
/// Density Field At Algorithm Implementation Tools
/**
 * @file       field.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains declarations of algorithm implementation tools.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef FIELD_H_
#define FIELD_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Field Definition Formulas 
///
/// These classes are used for 
/// constructing syntax tree of 
/// multiterm formula which  
/// can define a density field 
/// and evaluate field functors.  
//!
//! \note If there are 
//! common terms in formula 
//! then its syntax tree is 
//! optimized so it becomes 
//! directed acyclic graph.
//!

namespace fieldforms {

/////////////////////////////
///
/// Base Node Of Field 
/// Formula Syntax Tree 
///

class formula {
  short lnkcnt_; //!< counter of links to the formula object
  mutable table lstval_; //!< latest result of field evaluation using the formula
  mutable bool is_lstval_; //!< flag for indicating whether latest field evaluation has been performed
  mutable table defval_; //!< defualt result of field evaluation implied by the formula
  mutable bool is_defval_; //!< flag for indicating whether default field evaluation has been performed
 private:
//! end create the formula object:
  virtual void ecreate_() 
  {}
  
//! do destroy the formula object:
  virtual void destroy_() //! \pre this object is of dynamic class of storage
  { delete this; }

//! return number of input arguments in the formula:
  virtual int getnargs_() const = 0; //!< \post nargs>=0

//! return dimension of the k-th formula argument:
  virtual int getadimn_(int k) const = 0; //!< \pre nargs>0 , 0 <= k < nargs  \post adimn(k)>0

//! return real range as formula domain for the k-th argument and its q-th component:
  virtual void getrange_(int k,int q,real &min,real &max) const = 0; //!< \pre nargs>0 , 0 <= k < nargs , 0 <= q < adimn(k)  \post min<max

//! reset calculation performed with use of the formula:
  virtual void recalcval_() const = 0;

//! calculate output field value for given input variables using the formula:
  virtual table calcval_(const realtuple &x) const = 0; //!< \pre x.count>=sum(adimn(:))

//! print unique source code signature for class of this formula:
  virtual void printsig_(ostream &out) const = 0;
  
//! return number of direct expression terms composing the formula:
  virtual int getnterms_() const = 0; //!< \post nterms>=0

//! return count of direct expression terms appended to the formula:
  virtual int getcterms_() const = 0; //!< \post 0 <= cterms <= nterms

//! append a given expression term to the formula:
  virtual void appterm_(formula *trp) = 0; //!< \pre trp!=0 , cterms<nterms

//! exclude a given common expression term from this formula or its subexpressions and return completeness flag:
  virtual bool excterm_(const string &trs,formula *trp) = 0; //!< \pre trp!=0 , trs==trp->signat , cterms>0

//! exclude from a given formula all common expression terms composing this formula:
  virtual void excform_(formula &frm) = 0; //!< \pre cterms==nterms
//@{
 private: //! <b> Not implemented/allowed. </b>
  formula(formula &);
  void operator=(formula &);
//@}
 public:
/// create formula object:

  formula();

/// destroy formula object:

  virtual ~formula();
  
/// do share the formula object: 

  void do_share() 
  { if (++lnkcnt_==1) ecreate_(); }

/// do relieve the formula object: 

  void do_relieve() 
  { if (--lnkcnt_==0) destroy_(); }

//// return field characteristics specified by the formula:

  /// return number of input arguments:
  int nargs() const //! \post nargs>=0
  { return getnargs_(); }

  /// return dimension for a given input argument:
  int adimn(int k) const //! \pre nargs>0 , 0 <= k < nargs  \post adimn(k)>0
  { return getadimn_(k); }

  /// return number of degrees as total number of input variables:
  int ndegs() const; //!< \post ndegs==sum(adimn(:)) >= 0

  /// return number of dimensions, i.e. output value dimensions:
  int ndims() const; //!< \post ndims>=0  \post ndims==eval().ndims
  
  /// return range for a given input argument:
  void range(int k,realtuple &min,realtuple &max) const; //!< \pre nargs>0 , 0 <= k < nargs  \post min<max

  /// return range for a given input argument component:
  void range(int k,int q,real &min,real &max) const //! \pre nargs>0 , 0 <= k < nargs , 0 <= q < adimn(k)  \post min<max
  { getrange_(k,q,min,max); }

  /// return range for a given input variable:
  void range(int h,real &min,real &max) const; //!< \pre ndegs>0 , 0 <= h < ndegs  \post min<max

/// return default field value implied by the formula:

  const table &eval() const;

/// return a field value calculated by applying the formula to given arguments:

  table eval(const realtuple *x0...) const; //!< \pre nargs>0 , count(x0...)>=nargs , xk.count>=adimn(k) , 0 <= k < nargs  \post ret.ndims==ndims

/// return a field value calculated by applying the formula to given variables:
//!
//! \note Value of field variables
//! can exceed specified domain range
//! and can be any real number.
//! The domain range [min,max] contains
//! main values for field variables.
//!
  table eval(const realtuple &x) const; //!< \pre x.count>=ndegs  \post ret.ndims==ndims

/// print signature of the formula:

  void print(ostream &out) const
  { printsig_(out); }

/// return signature of the formula:

  string signat() const;

/// return number of direct expression terms composing the formula:
 
  int nterms() const //! \post nterms>=0
  { return getnterms_(); }

//// methods for composing the formula with given expression terms:

  /// append a given expression term to the formula:
  void append(formula &trm);
  
  /// exclude a given common expression term from this formula or its subexpressions and return completeness flag:
  bool exclude(const string &trs,formula *trp)
  { return excterm_(trs,trp); }
  
  /// exclude from a given formula all common expression terms composing this formula:
  void exclude(formula &trm);

//// low-level methods for evaluating the formula at given variables:

  /// reset calculation of field value:
  void reval() const;
  
  /// calculate and return latest output field value for given input variables:
  table retval(const realtuple &x) const;
};

/////////////////////////////
///
/// Nullary Formula
///
/// It is a terminal node of formula tree 
/// for representing nullary expression.
///

class nullarform : public virtual formula {
 private:
  void recalcval_() const;
  int getnterms_() const;
  int getcterms_() const;
  void appterm_(formula *trp);
  bool excterm_(const string &trs,formula *trp);
  void excform_(formula &frm);
 public:
  nullarform();
  ~nullarform();
};

/////////////////////////////
///
/// Multary Formula
///
/// It is a non-terminal node of formula tree 
/// for representing multary expression with 
/// a given number of terms.
///

class multarform : public virtual formula {
  const int num_;
 private:
  int getnterms_() const;
 public:
  explicit multarform(int n); //!< \pre (asserted) n>0
  ~multarform();
};

/////////////////////////////
///
/// Unary Formula
///

class unarform : public multarform {
  formula *term_;
 private:
  void recalcval_() const;
  int getcterms_() const;
  void appterm_(formula *trp);
  bool excterm_(const string &trs,formula *trp);
  void excform_(formula &frm);
 public:
  explicit unarform(formula &trm);
  ~unarform();
  int subnargs() const
  { return term_->nargs(); }
  int subadimn(int k) const
  { return term_->adimn(k); }
  void subrange(int k,int q,real &min,real &max) const
  { term_->range(k,q,min,max); }
  table evaltrm(const realtuple &x) const
  { return term_->retval(x); }
  void printtrm(ostream &out) const
  { term_->print(out); }
};

/////////////////////////////
///
/// Binary Formula
///

class binarform : public multarform {
  formula *term1_;
  formula *term2_;
 private:
  void recalcval_() const;
  int getcterms_() const;
  void appterm_(formula *trp);
  bool excterm_(const string &trs,formula *trp);
  void excform_(formula &frm);
 public:
  binarform(formula &trm1,formula &trm2);
  ~binarform();
  int subnargs() const
  { return Common::max2(term1_->nargs(),term2_->nargs()); }
  int subadimn(int k) const
  { return Common::max2(k<term1_->nargs() ? term1_->adimn(k) : 0 , k<term2_->nargs() ? term2_->adimn(k) : 0); }
  void subrange(int k,int q,real &min,real &max) const
  {  
   real a1=-rInf,a2=-rInf,b1=+rInf,b2=+rInf;
   if (k<term1_->nargs() && q<term1_->adimn(k)) term1_->range(k,q,a1,b1); 
   if (k<term2_->nargs() && q<term2_->adimn(k)) term2_->range(k,q,a2,b2);
   min=Common::max2(a1,a2); max=Common::min2(b1,b2);
  }
  table evaltrm1(const realtuple &x) const
  { return term1_->retval(x); }
  table evaltrm2(const realtuple &x) const
  { return term2_->retval(x); }
  void printtrm1(ostream &out) const
  { term1_->print(out); }
  void printtrm2(ostream &out) const
  { term2_->print(out); }
};

/////////////////////////////
///
/// Formula Functions
/// 
/// They are used for generating output 
/// values of formula at given input 
/// arguments.
///

class formfunc : public virtual formula {
 public:
  formfunc();
  ~formfunc();
};

/////////////////////////////
///
/// Function Formula
///
/// It is nullary formula function 
/// with calculated values for given
/// arguments.
///

class function : public nullarform , public formfunc {
 public:
  function();
  ~function();
};

/////////////////////////////
///
/// Constant Formula 
///
/// It is nullary formula function 
/// with a given value and no arguments.
///

class constant : public nullarform , public formfunc {
  table cstval_;
 private:
  int getnargs_() const;
  int getadimn_(int k) const;
  void getrange_(int k,int q,real &min,real &max) const;
  table calcval_(const realtuple &x) const;
  void printsig_(ostream &out) const;
 public:
  constant();
  constant(const table &val);
  ~constant();
};

/////////////////////////////
///
/// Formula Operations 
///
/// They are used for manipulating 
/// only output values of formula.
///

class formoper : public virtual formula {
 public:
  formoper();
  ~formoper();
};

/////////////////////////////
///
/// Unary Formula Operation
///

class unaroper : public unarform , public formoper {
 private:
  int getnargs_() const;
  int getadimn_(int k) const;
  void getrange_(int k,int q,real &min,real &max) const;
 public:
  explicit unaroper(formula &trm);
  ~unaroper();
};

/////////////////////////////
///
/// Binary Formula Operation
///

class binaroper : public binarform , public formoper {
 private:
  int getnargs_() const;
  int getadimn_(int k) const;
  void getrange_(int k,int q,real &min,real &max) const;
 public:
  binaroper(formula &trm1,formula &trm2);
  ~binaroper();
};

/////////////////////////////
///
/// Formula Transformations 
///
/// They are used for operating 
/// both on arguments and values.
///

class formtrans : public virtual formula {
 public:
  formtrans();
  ~formtrans();
};

/////////////////////////////
///
/// Unary Formula Transformation
///

class unartrans : public unarform , public formtrans {
 public:
  explicit unartrans(formula &trm);
  ~unartrans();
};

/////////////////////////////
///
/// Binary Formula Transformation
///

class binartrans : public binarform , public formtrans {
 public:
  binartrans(formula &trm1,formula &trm2);
  ~binartrans();
};

} // end of fieldforms namespace

/////////////////////////////
///
/// Multivariable and 
/// Multidimensional 
/// Density Field 
///
/// This class/namespace is used for processing 
/// smoothed input data. Fields are implemented 
/// as a symbolicly formulated functor which
/// can associate real vector arguments to 
/// dense table values.
///

class field {
  fieldforms::formula *form_; //!< non-null root node of syntax tree of formula defining the field
 public:
//// field formula types:

  typedef fieldforms::formula formula; ///< type of base field formula
  typedef fieldforms::function function; ///< type of field function formula
  typedef fieldforms::constant constant; ///< type of field constant formula
  typedef fieldforms::unaroper unaroper; ///< type of field unary operation formula
  typedef fieldforms::binaroper binaroper; ///< type of field binary operation formula
  typedef fieldforms::unartrans unartrans; ///< type of field unary transformation formula
  typedef fieldforms::binartrans binartrans; ///< type of field binary transformation formula
 
/// create a field object as defined by a given formula:

  explicit field(formula &frm);

/// create a field object with no arguments defined as zero value function:

  field(); //!< \post nargs==0 , ndims==0
  
/// create a field object with no arguments defined as constant function of a given table value:

  field(const table &val); //!< \post nargs==0 , ndims==val.ndims
  
/// create a field object and assign to it a given field by sharing inner structure:

  field(const field &fld);
  
/// relieve inner structure and destroy the field object:

  ~field();

/// reinitiate the field object as defined by a given formula:

  field &reinit(formula &frm);
  
/// assign a given field to the field object by sharing inner structure:

  field &operator=(const field &fld);

/// return number of input vector arguments of the field:

  int nargs() const //! \post nargs>=0
  { return form_->nargs(); }
  
/// return dimension of a given input vector argument as number of its components:

  int adimn(int k) const //! \pre nargs>0 , 0 <= k < nargs  \post adimn(k)>0
  { return form_->adimn(k); }
  
/// return number of degrees of freedom for the field as number of all its input scalar variables:

  int ndegs() const //! \post ndegs>=0
  { return form_->ndegs(); }
  
/// return rank for each table being an output value of the field:

  int rank() const //! \post rank>=0
  { return form_->ndims(); }
  
/// return number of dimensions for each output table value of the field:

  int ndims() const //! \post ndims>=0
  { return form_->ndims(); }
  
/// return domain range of the field for given input vector argument:

  void range(int k,realtuple &min,realtuple &max) const //! \pre nargs>0 , 0 <= k < nargs  \post min<max
  { form_->range(k,min,max); }
  
/// return domain range of the field for given component of input vector argument:

  void range(int k,int q,real &min,real &max) const //! \pre nargs>0 , 0 <= k < nargs , 0 <= q < adimn(k)  \post min<max
  { form_->range(k,q,min,max); }
  
/// return domain range of the field for a given input scalar variable:

  void range(int h,real &min,real &max) const //! \pre ndegs>0 , 0 <= h < ndegs  \post min<max
  { form_->range(h,min,max); }
  
/// return a table value of the field for all default arguments or the only one value for the constant, argument-less field:

  const table &operator()() const
  { return form_->eval(); }

/// return a table value of the field at a given vector argument:

  table operator()(const realtuple &x0) const //! \pre nargs==1 , xk.count>=adimn(k) , 0 <= k < nargs
  { return form_->eval(&x0); }
  
/// return a table value of the field at two given vector arguments:

  table operator()(const realtuple &x0,const realtuple &x1) const //! \pre nargs==2
  { return form_->eval(&x0,&x1); }
  
/// return a table value of the field at three given vector arguments:

  table operator()(const realtuple &x0,const realtuple &x1,const realtuple &x2) const //! \pre nargs==3
  { return form_->eval(&x0,&x1,&x2); }
  
/// return a table value of the field at four given vector arguments:

  table operator()(const realtuple &x0,const realtuple &x1,const realtuple &x2,const realtuple &x3) const //! \pre nargs==4
  { return form_->eval(&x0,&x1,&x2,&x3); }
  
/// return a table value of the field at five given vector arguments:

  table operator()(const realtuple &x0,const realtuple &x1,const realtuple &x2,const realtuple &x3,const realtuple &x4) const //! \pre nargs==5
  { return form_->eval(&x0,&x1,&x2,&x3,&x4); }
  
/// return a table value of the field at a given tuple of all scalar variables:

  table operator[](const realtuple &x) const //! \pre x.count>=ndegs
  { return form_->eval(x); }
//@{   
/// perform simple math operations on fields:
//! 
//! \note This operation is applied 
//! only to tables which are output 
//! values for given fields.
//! \see The table class contains
//! almost the same operations,
//! described in more detail.
//!
  field &operator+=(real a);
  field &operator-=(real a);
  field &operator*=(real a);
  field &operator/=(real a);
  friend field abs(const field &y);
  friend field log(const field &y);
  friend field log2(const field &y);
  friend field sqr(const field &y);
  friend field sqrt(const field &y);
  friend field cub(const field &y);
  friend field pow(const field &y,real a);
  friend field cum(const field &y);
  friend field sum(const field &y);
  friend field avg(const field &y);
  friend field sum(const Data::space &_F_,const field &y);
  friend field avg(const Data::space &_F_,const field &y);
  friend field prod(const field &y);
  friend field prod(const Data::space &_F_,const field &y);
  friend field max(const field &y);
  friend field max(const Data::space &_F_,const field &y);
  friend field min(const field &y);
  friend field min(const Data::space &_F_,const field &y);
  friend field max2(const field &y1,const field &y2);
  friend field min2(const field &y1,const field &y2);
  friend field diag(const field &y);
  friend field diagadd(const field &y1,const field &y2);
  friend field diagsub(const field &y1,const field &y2);
  friend field diagmul(const field &y1,const field &y2);
  friend field diagdiv(const field &y1,const field &y2);
  friend field dyadadd(const field &y1,const field &y2);
  friend field dyadsub(const field &y1,const field &y2);
  friend field dyadmul(const field &y1,const field &y2);
  friend field dyaddiv(const field &y1,const field &y2);
  friend field operator*(const field &y,const permutation &p);
  friend field operator~(const field &y);
  friend field operator|(const field &y,int s);
  friend field operator|(int s,const field &y);
  friend field operator+(const field &y1,const field &y2);
  friend field operator+(real a,const field &y);
  friend field operator+(const field &y,real a);
  friend field operator-(const field &y);
  friend field operator-(const field &y1,const field &y2);
  friend field operator-(real a,const field &y);
  friend field operator-(const field &y,real a);
  friend field operator*(const field &y1,const field &y2);
  friend field operator*(real a,const field &y);
  friend field operator*(const field &y,real a);
  friend field operator/(const field &y1,const field &y2);
  friend field operator/(real a,const field &y);
  friend field operator/(const field &y,real a);
  friend field operator^(const field &y1,const field &y2);
//@}
/// calculate multiple integral of the continuous field:
//!
//! \note The integral is evaluated 
//! at the widest interval with extra
//! scaling by the factor "fo". 
//! The integration is performed only
//! for the last, rightmost vector 
//! argument of the filed.
//!
  friend field integral(const field &y,real fo=0.0); //!< \pre (asserted) fo>=0  \post ret==integral(y,ao,bo) , ao==min-fo*(max-min)/2 , bo==max+fo*(max-min)/2 , y.range(y.nargs-1,min,max)

/// calculate multiple integral of the continuous field at a given interval:
//!
//! \note The integration is performed
//! only for the last, rightmost vector 
//! argument of the filed.
//!
  friend field integral(const field &y,const realtuple &ao,const realtuple &bo); //!< \pre (asserted) y.nargs>0 && ao.count>=y.adimn(y.nargs-1) && bo.count>=y.adimn(y.nargs-1) && ao<bo  \post y.nargs>0 && ret.nargs==y.nargs-1 , y.nargs==0 => ret==sum(y)

/// calculate multiple cumulative integral of the continuous field:
//!
//! \note The integral is evaluated 
//! at an interval from the lowest 
//! limit (with an extra scaling 
//! by the factor "fo") to running
//! upper limit. Therefore it is
//! an incomplete integral. 
//! The integration is performed only
//! for the last, rightmost vector 
//! argument of the filed.
//!
  friend field cumugral(const field &y,real fo=0.0); //!< \pre (asserted) fo>=0  \post ret(...,xo)~integral(y,ao,bo) , ao==min-fo*(max-min)/2 , bo==xo , y.range(y.nargs-1,min,max) => ret.nargs==y.nargs

/// print formula signature of the field to a given output stream:

  void print_forms(ostream &out) const
  { form_->print(out); }
  
/// print signature to output stream:

  friend ostream &operator<<(ostream &out,const field &fld)
  { 
   fld.print_forms(out); 
   return out; 
  }
//@{
/// auxiliary method templates for defining field expressions:

  template <typename TForm,typename TParm>
  static field nulexpr(const TParm &a)
  { 
   TForm *f=new TForm(a);
   return field(*f); 
  }
   
  template <typename TForm,typename TParm1,typename TParm2>
  static field nulexpr(const TParm1 &a1,const TParm2 &a2)
  { 
   TForm *f=new TForm(a1,a2);
   return field(*f); 
  }
   
  template <typename TForm,typename TParm1,typename TParm2,typename TParm3>
  static field nulexpr(const TParm1 &a1,const TParm2 &a2,const TParm3 &a3)
  { 
   TForm *f=new TForm(a1,a2,a3);
   return field(*f); 
  }
   
  template <typename TForm,typename TParm1,typename TParm2,typename TParm3,typename TParm4>
  static field nulexpr(const TParm1 &a1,const TParm2 &a2,const TParm3 &a3,const TParm4 &a4)
  { 
   TForm *f=new TForm(a1,a2,a3,a4);
   return field(*f); 
  }
   
  template <typename TForm,typename TParm1,typename TParm2,typename TParm3,typename TParm4,typename TParm5>
  static field nulexpr(const TParm1 &a1,const TParm2 &a2,const TParm3 &a3,const TParm4 &a4,const TParm5 &a5)
  { 
   TForm *f=new TForm(a1,a2,a3,a4,a5);
   return field(*f); 
  }
   
  template <typename TForm,typename TParm>
  field &unrexpr(const TParm &a)
  { 
   TForm *f=new TForm(*form_,a);
   return reinit(*f); 
  }
   
  template <typename TForm>
  static field unexpr(const field &y)
  { 
   TForm *f=new TForm(*y.form_);
   return field(*f); 
  }
   
  template <typename TForm,typename TParm>
  static field unexpr(const field &y,const TParm &a)
  { 
   TForm *f=new TForm(*y.form_,a);
   return field(*f); 
  }
   
  template <typename TForm,typename TParm1,typename TParm2>
  static field unexpr(const field &y,const TParm1 &a1,const TParm2 &a2)
  { 
   TForm *f=new TForm(*y.form_,a1,a2);
   return field(*f); 
  }
   
  template <typename TForm>
  static field binexpr(const field &y1,const field &y2)
  { 
   TForm *f=new TForm(*y1.form_,*y2.form_);
   return field(*f); 
  }
//@}
};

/////////////////////////////
///
/// Auxiliary Field Manipulators
///
/// These classes are used for
/// writing subexpressions of
/// field definition formulas 
/// in a more algebraical 
/// notation.
///

namespace fieldmanips {

//////
/// Integration Manipulator with 1 Argument
///
class integmanip1 {
  const real &fo_;
 public:
  integmanip1(const real &fo): fo_(fo)
  {}
  
  field operator()(const field &y) const
  { return integral(y,fo_); }
};

//////
/// Integration Manipulator with 2 Arguments
///
class integmanip2 {
  const realtuple &ao_;
  const realtuple &bo_;
 public:
  integmanip2(const realtuple &ao,const realtuple &bo): ao_(ao), bo_(bo)
  {}
  
  field operator()(const field &y) const
  { return integral(y,ao_,bo_); }
};

//////
/// Cumulative Integration Manipulator
///
class cumugmanip {
  const real &fo_;
 public:
  cumugmanip(const real &fo): fo_(fo)
  {}
  
  field operator()(const field &y) const
  { return cumugral(y,fo_); }
};

} // end of fieldmanips namespace

//@{
/// manipulator functions for performing complex math operations on fields:

inline fieldmanips::integmanip1 integ(const real &fo=0.0)
{ return fieldmanips::integmanip1(fo); }

inline fieldmanips::integmanip2 integ(const realtuple &ao,const realtuple &bo)
{ return fieldmanips::integmanip2(ao,bo); }

inline fieldmanips::cumugmanip cumug(const real &fo=0.0)
{ return fieldmanips::cumugmanip(fo); }
//@}

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of FIELD_H_ header file

