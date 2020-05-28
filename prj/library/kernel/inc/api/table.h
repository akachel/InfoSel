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
/// Dense Collection At Algorithm Implementation Tools 
/**
 * @file       table.h
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

#ifndef TABLE_H_
#define TABLE_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Small Dimensional Dense Table
///
/// Used for storing results of input 
/// data processing, implemented 
/// as linear array structure of 
/// real type elements addressed 
/// directly, zeroised initially.
/// 

class table {
  int *lnkcnt_; //!< counter of links to inner structure shared by table objects
  int ndims_; //!< dimensionality of the table as number of all its indices
  int *dimsizs_; //!< array of table sizes along consecutive dimensions of the table
  mutable int *dimbass_; //!< array of table bases at preceding dimensions of the table
  real *linelms_; //!< array of real type elements of the table as underlying linear structure
  int nelms_; //!< element array length
  
//! create a table structure with given number of dimensions and list of their sizes: 
  void construct_(int ndims,...);

//! create separately an array of table bases for all dimensions:
  void create_dimbass_() const;
  
//! return the table's element of an array subscripted by a given list of indices for all dimensions:
  real &dimelms_(int i0...) const;

//! return the table's element of an array subscripted by a given tuple of indices for all dimensions:
  real &dimelms_(const inttuple &ii) const;

 public:
/// create a table object with given parameters of its structure: 

  table(int ndims,const int sizes[]); //!< \pre count(sizes)>=ndims
  
/// create a table object as combination of given two substructures:

  table(int ndims1,int ndims2,const int sizes1[],const int sizes2[]); //!< \pre count(sizes1)>=ndims1 , count(sizes2)>=ndims2  \post ndims==ndims1+ndims2
  
/// create a table object with no dimensions as single value point:

  table(); //!< \post ndims==0 , count==1

/// create a non-point table object with given sizes for all its dimensions:

  explicit table(int s0) //! \post ndims==1
  { construct_(1,s0); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1) //! \post ndims==2 
  { construct_(2,s0,s1); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2) //! \post ndims==3 
  { construct_(3,s0,s1,s2); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2,int s3) //! \post ndims==4 
  { construct_(4,s0,s1,s2,s3); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2,int s3,int s4) //! \post ndims==5 
  { construct_(5,s0,s1,s2,s3,s4); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2,int s3,int s4,int s5) //! \post ndims==6 
  { construct_(6,s0,s1,s2,s3,s4,s5); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2,int s3,int s4,int s5,int s6) //! \post ndims==7 
  { construct_(7,s0,s1,s2,s3,s4,s5,s6); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2,int s3,int s4,int s5,int s6,int s7) //! \post ndims==8 
  { construct_(8,s0,s1,s2,s3,s4,s5,s6,s7); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2,int s3,int s4,int s5,int s6,int s7,int s8) //! \post ndims==9 
  { construct_(9,s0,s1,s2,s3,s4,s5,s6,s7,s8); }
  
/// create a non-point table object with given sizes for all its dimensions:

  table(int s0,int s1,int s2,int s3,int s4,int s5,int s6,int s7,int s8,int s9) //! \post ndims==10 
  { construct_(10,s0,s1,s2,s3,s4,s5,s6,s7,s8,s9); }
  
/// create a table object as duplication of a given sparse structure with optional copy of its elements:

  table(const sparse &spr,bool cpy=true);

/// create a table object as duplication of a given dense structure with optional copy of its elements:

  table(const table &tab,bool cpy);

/// create a table object and assign to it a given table by sharing inner structure:

  table(const table &tab);

/// relieve inner structure and destroy the table object:

  ~table();

/// assign a given table to the table object by sharing inner structure:

  table &operator=(const table &tab);
  
/// test whether this table object has the inner structure shared by another tables:

  bool isshared() const;

/// make clone of the table object by copying all its elements when the inner structure is shared:

  table &clone();

/// return rank of the table:
//!
//! \note The rank is identical 
//! with a number of all table 
//! dimensions or its indices.
//!
  int rank() const //! \post rank>=0
  { return ndims_; }
  
/// return number of all table dimensions or indices:

  int ndims() const //! \post ndims>=0
  { return ndims_; }
  
/// return size of a given dimension for the non-point table:

  int size(int k=0) const //! \pre ndims>0 , 0 <= k < ndims  \post size>0
  { return dimsizs_[k]; }
  
/// return base of a given dimension for the non-point table:

  int base(int k=0) const //! \pre ndims>0 , 0 <= k < ndims  \post base(ndims-1)==1 , base(k<ndims-1)==size(k+1)*...*size(ndims-1) > 0
  { if (dimbass_==0) create_dimbass_(); return dimbass_[k]; }
  
/// return total number of all elements in the table:

  int count() const //! \post count>0
  { return nelms_; }
  
/// return the table's element giving its total linear index:

  real &operator[](int I) //! \pre 0 <= I < count
  { return linelms_[I]; }
  
/// return the table's element giving its total linear index:

  real operator[](int I) const
  { return linelms_[I]; }

/// return first element of the table or the only one element of the point table:

  real &operator()()
  { return *linelms_; }

/// return first element of the table or the only one element of the point table:

  real operator()() const
  { return *linelms_; }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0) //! \pre ndims==1 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0) const //! \pre ndims==1 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1) //! \pre ndims==2 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1) const //! \pre ndims==2 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2) //! \pre ndims==3 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2) const //! \pre ndims==3 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2,int i3) //! \pre ndims==4 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2,int i3) const //! \pre ndims==4 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2,int i3,int i4) //! \pre ndims==5 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2,int i3,int i4) const //! \pre ndims==5 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5) //! \pre ndims==6 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5) const //! \pre ndims==6 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6) //! \pre ndims==7 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6) const //! \pre ndims==7 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7) //! \pre ndims==8 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6,i7); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7) const //! \pre ndims==8 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6,i7); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8) //! \pre ndims==9 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6,i7,i8); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8) const //! \pre ndims==9 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6,i7,i8); }

/// return an element of the non-point table giving its indices for all dimensions:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8,int i9) //! \pre ndims==10 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6,i7,i8,i9); }

/// return an element of the non-point table giving its indices for all dimensions:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8,int i9) const //! \pre ndims==10 , 0 <= ik < size(k) , 0 <= k < ndims , I==sum(ik*base(k)) < count
  { return dimelms_(i0,i1,i2,i3,i4,i5,i6,i7,i8,i9); }

/// return an element of the table for a given tuple of all indices:

  real &operator[](const inttuple &ii) //! \pre ii.count>=ndims , 0 <= ii[k] < size(k) , 0 <= k < ndims , I==sum(ii[k]*base(k)) < count
  { return dimelms_(ii); }
  
/// return an element of the table for a given tuple of all indices:

  real operator[](const inttuple &ii) const
  { return dimelms_(ii); }

/// set up zero values for all elements of the table:

  table &zero(); //!< \invariant ndims

/// set up unit values for all elements of the table:

  table &unit(); //!< \invariant ndims

/// set up random values for all elements of the table:

  table &rand(); //!< \invariant ndims

/// set up given values for all elements of the table:

  table &operator=(real a); //!< \invariant ndims

/// perform this relational operation for two tables:

  friend bool operator==(const table &x1,const table &x2); //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post ret ~ "x1[i]==x2[i] , 0<=i<min(x1.count,x2.count)"

/// perform this relational operation for two tables:

  friend bool operator!=(const table &x1,const table &x2) //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post ret ~ "x1[i]!=x2[i] , 0<=i<min(x1.count,x2.count)"
  { return !(x1==x2); }
  
/// perform this relational operation for number and table:

  friend bool operator==(real a,const table &x); //!< \post ret ~ "a==x[i] , 0<=i<x.count"

/// perform this relational operation for number and table:

  friend bool operator!=(real a,const table &x) //!< \post ret ~ "a!=x[i] , 0<=i<x.count"
  { return !(a==x); }
  
/// perform this relational operation for table and number:

  friend bool operator==(const table &x,real a); //!< \post ret ~ "x[i]==a , 0<=i<x.count"

/// perform this relational operation for table and number:

  friend bool operator!=(const table &x,real a) //!< \post ret ~ "x[i]!=a , 0<=i<x.count"
  { return !(x==a); }

//// perform math operations on tables:

  /// add a number to this table:
  table &operator+=(real a); //!< \invariant ndims

  /// subtract a number from this table:
  table &operator-=(real a); //!< \invariant ndims

  /// multiply this table by a number:
  table &operator*=(real a); //!< \invariant ndims

  /// divide this table by a number:
  table &operator/=(real a); //!< \invariant ndims

  /// apply this math function to a table:
  friend table abs(const table &x); //!< \post y.ndims==x.ndims

  /// apply this math function to a table:
  friend table log(const table &x); //!< \post y.ndims==x.ndims

  /// apply this math function to a table:
  friend table log2(const table &x); //!< \post y.ndims==x.ndims

  /// apply this math function to a table:
  friend table sqr(const table &x); //!< \post y.ndims==x.ndims

  /// apply this math function to a table:
  friend table sqrt(const table &x); //!< \post y.ndims==x.ndims

  /// apply this math function to a table:
  friend table cub(const table &x); //!< \post y.ndims==x.ndims

  /// apply this math function to a table:
  friend table pow(const table &x,real a); //!< \post y.ndims==x.ndims

  /// sum cumulatively a table at its last, rightmost dimension:
  friend table cum(const table &x); //!< \post y.ndims==x.ndims

  /// sum a table at its last, rightmost dimension:
  friend table sum(const table &x); //!< \post x.ndims>0 && y.ndims==x.ndims-1

  /// average a table at its last, rightmost dimension:
  friend table avg(const table &x); //!< \post x.ndims>0 && y.ndims==x.ndims-1

  /// sum a table at its last, rightmost dimension according to a given data space:
  friend table sum(const Data::space &_F_,const table &x); //!< \pre (asserted) x.ndims>=1 && x.size(x.ndims-1)==_F_.card  \post x.ndims>0 && y.ndims==x.ndims-1

  /// average a table at its last, rightmost dimension according to a given data space:
  friend table avg(const Data::space &_F_,const table &x); //!< \pre (asserted) x.ndims>=1 && x.size(x.ndims-1)==_F_.card  \post x.ndims>0 && y.ndims==x.ndims-1

  /// obtain product of elements for a table at its last, rightmost dimension:
  friend table prod(const table &x); //!< \post x.ndims>0 && y.ndims==x.ndims-1

  /// obtain product of elements for a table at its last, rightmost dimension according to a given data space:
  friend table prod(const Data::space &_F_,const table &x); //!< \pre (asserted) x.ndims>=1 && x.size(x.ndims-1)==_F_.card  \post x.ndims>0 && y.ndims==x.ndims-1

  /// maximize a table at its last, rightmost dimension:
  friend table max(const table &x); //!< \post x.ndims>0 && y.ndims==x.ndims-1

  /// maximize a table at its last, rightmost dimension according to a given data space:
  friend table max(const Data::space &_F_,const table &x); //!< \pre (asserted) x.ndims>=1 && x.size(x.ndims-1)==_F_.card  \post x.ndims>0 && y.ndims==x.ndims-1

  /// minimize a table at its last, rightmost dimension:
  friend table min(const table &x); //!< \post x.ndims>0 && y.ndims==x.ndims-1

  /// minimize a table at its last, rightmost dimension according to a given data space:
  friend table min(const Data::space &_F_,const table &x); //!< \pre (asserted) x.ndims>=1 && x.size(x.ndims-1)==_F_.card  \post x.ndims>0 && y.ndims==x.ndims-1

  /// return table with maximal elements among elements of two tables:
  friend table max2(const table &x1,const table &x2); //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post x1.count>=x2.count && y.ndims==x1.ndims || x1.count<x2.count && y.ndims==x2.ndims

  /// return table with minimal elements among elements of two tables:
  friend table min2(const table &x1,const table &x2); //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post x1.count>=x2.count && y.ndims==x1.ndims || x1.count<x2.count && y.ndims==x2.ndims

  /// return table being diagonal part of a table at its last, rightmost dimension:
  friend table diag(const table &x); //!< \pre x.ndims>=2 && x.size(x.ndims-2)==x.size(x.ndims-1)  \post x.ndims>0 && y.ndims==x.ndims-1

  /// return table being diagonal part of sum of two tables at their last, rightmost dimension:
  friend table diagadd(const table &x1,const table &x2); //!< \pre (asserted) x1.ndims>=1 && x2.ndims>=1 && x1.size(x1.ndims-1)==x2.size(x2.ndims-1)  \post (x1.ndims>0 || x2.ndims>0) && y.ndims==x1.ndims+x2.ndims-1

  /// return table being diagonal part of difference of two tables at their last, rightmost dimension:
  friend table diagsub(const table &x1,const table &x2); //!< \pre (asserted) x1.ndims>=1 && x2.ndims>=1 && x1.size(x1.ndims-1)==x2.size(x2.ndims-1)  \post (x1.ndims>0 || x2.ndims>0) && y.ndims==x1.ndims+x2.ndims-1

  /// return table being diagonal part of product of two tables at their last, rightmost dimension:
  friend table diagmul(const table &x1,const table &x2); //!< \pre (asserted) x1.ndims>=1 && x2.ndims>=1 && x1.size(x1.ndims-1)==x2.size(x2.ndims-1)  \post (x1.ndims>0 || x2.ndims>0) && y.ndims==x1.ndims+x2.ndims-1

  /// return table being diagonal part of quotient of two tables at their last, rightmost dimension:
  friend table diagdiv(const table &x1,const table &x2); //!< \pre (asserted) x1.ndims>=1 && x2.ndims>=1 && x1.size(x1.ndims-1)==x2.size(x2.ndims-1)  \post (x1.ndims>0 || x2.ndims>0) && y.ndims==x1.ndims+x2.ndims-1

  /// obtain dyadic addition of two tables:
  friend table dyadadd(const table &x1,const table &x2); //!< \post y.ndims==x1.ndims+x2.ndims

  /// obtain dyadic subtraction of two tables:
  friend table dyadsub(const table &x1,const table &x2); //!< \post y.ndims==x1.ndims+x2.ndims

  /// obtain dyadic multiplication of two tables:
  friend table dyadmul(const table &x1,const table &x2); //!< \post y.ndims==x1.ndims+x2.ndims

  /// obtain dyadic division of two tables:
  friend table dyaddiv(const table &x1,const table &x2); //!< \post y.ndims==x1.ndims+x2.ndims

  /// permutate indices of a table according to a given permutation:
  friend table operator*(const table &x,const permutation &p); //!< \pre (asserted) x.ndims>=2 && p.length==x.ndims  \post y.ndims==x.ndims

  /// invert indices of a table:
  friend table operator~(const table &x); //!< \post y.ndims==x.ndims

  /// insert new dimension to a table before its first, leftmost dimension and set up a new size:
  friend table operator|(const table &x,int s); //!< \post y.ndims==x.ndims+1

  /// insert new dimension to a table after its last, rightmost dimension and set up a new size:
  friend table operator|(int s,const table &x); //!< \post y.ndims==1+x.ndims

  /// add two tables:
  friend table operator+(const table &x1,const table &x2); //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post x1.count>=x2.count && y.ndims==x1.ndims || x1.count<x2.count && y.ndims==x2.ndims

  /// add a number to a table:
  friend table operator+(real a,const table &x); //!< \post y.ndims==x.ndims

  /// add a table to a number:
  friend table operator+(const table &x,real a); //!< \post y.ndims==x.ndims

  /// negate a table:
  friend table operator-(const table &x); //!< \post y.ndims==x.ndims

  /// subtract two tables:
  friend table operator-(const table &x1,const table &x2); //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post x1.count>=x2.count && y.ndims==x1.ndims || x1.count<x2.count && y.ndims==x2.ndims

  /// subtract a number from a table:
  friend table operator-(real a,const table &x); //!< \post y.ndims==x.ndims

  /// subtract a table from a number:
  friend table operator-(const table &x,real a); //!< \post y.ndims==x.ndims

  /// multiply two tables:
  friend table operator*(const table &x1,const table &x2); //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post x1.count>=x2.count && y.ndims==x1.ndims || x1.count<x2.count && y.ndims==x2.ndims

  /// multiply a number by a table:
  friend table operator*(real a,const table &x); //!< \post y.ndims==x.ndims

  /// multiply a table by a number:
  friend table operator*(const table &x,real a); //!< \post y.ndims==x.ndims

  /// divide two tables:
  friend table operator/(const table &x1,const table &x2); //!< \pre (asserted) c1*x1.count==c2*x2.count && c1,c2 is int  \post x1.count>=x2.count && y.ndims==x1.ndims || x1.count<x2.count && y.ndims==x2.ndims

  /// divide a number by a table:
  friend table operator/(real a,const table &x); //!< \post y.ndims==x.ndims

  /// divide a table by a number:
  friend table operator/(const table &x,real a); //!< \post y.ndims==x.ndims

  /// obtain dyadic product of two tables:
  //! \note Be sure to enclose between brackets 
  //! any dyadic product of two tables as a part
  //! of larger expression that is one must write 
  //! ...(A^B)... instead of ...A^B... which is 
  //! caused by the C++ operator precedence rules.
  friend table operator^(const table &x1,const table &x2)
  { return table(dyadmul(x1,x2)); }

/// print normal contents of the table to a given output stream:

  void print_conts(ostream &out) const;
  
/// print sorted contents of the table to a given output stream:

  void print_sorts(ostream &out) const;

/// print contents to output stream:

  friend ostream &operator<<(ostream &out,const table &tab)
  {
   bool issrt=iosex::issorted(out);
   if (issrt) tab.print_sorts(out);
   else tab.print_conts(out);
   return out;
  }
};

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of TABLE_H_ header file

