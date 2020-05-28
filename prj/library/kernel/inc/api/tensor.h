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
/// Math Tensor At Algorithm Implementation Tools
/**
 * @file       tensor.h
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

#ifndef TENSOR_H_
#define TENSOR_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Auxiliary Type Of Math Tensor
///
/// It is used for expressing 
/// any math tensors represented 
/// by multidimensional equisized 
/// tables of real numbers. 
///

class tensor {
 protected:
  table rep_; //!< underlying tensor representation
  static int *sizes1_, *sizes2_; //!< sizes for a new tensor table

//! create an array of equal sizes for a new tensor table with given number of dimensions and their common size:
  static int *new_sizes_(int *&sizes,int ndims,int size);
  
//! check equality of all dimension sizes for a given tensor table:
  static const table &chk_sizes_(const table &tab);
  
//! convert the tensor object to its underlying representation:
  static table &torep_(tensor &tns) { return tns.rep_; }

//! convert the tensor object to its underlying representation:
  static const table &torep_(const tensor &tns) { return tns.rep_; }
  
 public:
/// create a tensor object with given parameters of its structure:

  tensor(int rank,int dimn):
   rep_(rank,new_sizes_(sizes1_,rank,dimn))
  { 
   delete[] sizes1_; 
  }

/// create a tensor object as combination of given two substructures:

  tensor(int rank1,int rank2,int dimn):
   rep_(rank1,rank2,new_sizes_(sizes1_,rank1,dimn),new_sizes_(sizes2_,rank2,dimn))
  { 
   delete[] sizes1_; 
   delete[] sizes2_; 
  }
  
/// create a tensor object of rank 0 as single scalar:

  tensor()
  {}
  
/// create a tensor object as duplication of a given sparse structure with optional copy of its components:

  tensor(const sparse &spr,bool cpy=true):
   rep_(chk_sizes_(table(spr,cpy)))
  {}

/// create a tensor object as duplication of a given dense structure with optional copy of its components:

  tensor(const table &tab,bool cpy):
   rep_(chk_sizes_(tab),cpy)
  {}

/// create a tensor object as duplication of a given tensor structure with optional copy of its components:

  tensor(const tensor &tns,bool cpy):
   rep_(tns.rep_,cpy)
  {}

/// create a tensor object and assign to it a given dense object by sharing inner structure:

  tensor(const table &tab):
   rep_(chk_sizes_(tab))
  {}
  
/// create a tensor object and assign to it a given tensor object by sharing inner structure:

  tensor(const tensor &tns):
   rep_(tns.rep_)
  {}
  
/// relieve inner structure and destroy the tensor object:

  ~tensor()
  {}

/// assign a given dense structure to the tensor object by sharing inner structure:

  tensor &operator=(const table &tab)
  { rep_=chk_sizes_(tab); return *this; }
  
/// assign a given tensor structure to the tensor object by sharing inner structure:

  tensor &operator=(const tensor &tns)
  { rep_=tns.rep_; return *this; }
  
/// convert the tensor object to its table:

  const table &totable() const
  { return rep_; }

/// convert the tensor object to its table:

  const table &operator*() const
  { return rep_; }

/// return rank of the tensor as number of dimensions of its table:

  int rank() const //! \post rank>=0
  { return rep_.ndims(); }

/// return dimension (order) of the non-scalar tensor as common size of each dimensions of its table:

  int dimn() const //! \pre rank>0  \post dimn>0
  { return rep_.size(); }

/// set up zero values for all components of the tensor:

  tensor &zero() //! \invariant rank, dimn
  { rep_.zero(); return *this; }

/// set up unit values for all components of the tensor:

  tensor &unit() //! \invariant rank, dimn
  { rep_.unit(); return *this; }

/// set up random values for all components of the tensor:

  tensor &rand() //! \invariant rank, dimn
  { rep_.rand(); return *this; }

/// set up given values for all components of the tensor:

  tensor &operator=(real a) //! \invariant rank, dimn
  { rep_=a; return *this; }

/// perform this relational operation for two tensors:

  friend bool operator==(const tensor &x1,const tensor &x2) //! \pre x1.rank==x2.rank , x1.rank>0 && x2.rank>0 && x1.dimn==x2.dimn
  { return x1.rep_==x2.rep_; }

/// perform this relational operation for two tensors:

  friend bool operator!=(const tensor &x1,const tensor &x2) //! \pre x1.rank==x2.rank , x1.rank>0 && x2.rank>0 && x1.dimn==x2.dimn
  { return x1.rep_!=x2.rep_; }

/// perform this relational operation for tensor and number:

  friend bool operator==(const tensor &x,real a)
  { return x.rep_==a; }

/// perform this relational operation for tensor and number:

  friend bool operator!=(const tensor &x,real a)
  { return x.rep_!=a; }

/// perform math operations on tensors:

  /// add a number to this tensor:
  tensor &operator+=(real a) //! \invariant rank, dimn
  { rep_+=a; return *this; }

  /// subtract a number from this tensor:
  tensor &operator-=(real a) //! \invariant rank, dimn
  { rep_-=a; return *this; }

  /// multiply this tensor by a number:
  tensor &operator*=(real a) //! \invariant rank, dimn
  { rep_*=a; return *this; }

  /// divide this tensor by a number:
  tensor &operator/=(real a) //! \invariant rank, dimn
  { rep_/=a; return *this; }

  /// obtain contraction of a tensor: 
  //! \note The operation is performed by obtaining a sum 
  //! of diagonal part of a tensor at its last, rightmost dimension.
  friend tensor con(const tensor &x) //! \post x.rank>1 && y.rank==x.rank-2
  { return tensor(sum(diag(x.rep_))); }

  /// obtain contraction of two tensors: 
  //! \note The operation is performed by obtaining a sum
  //! of diagonal part of product of two tensors at their last, 
  //! rightmost dimension.
  friend tensor con(const tensor &x1,const tensor &x2); //!< \pre (asserted) x1.rank>0 && x2.rank>0 && x1.dimn==x2.dimn  \post (x1.rank>0 || x2.rank>0) && y.rank==x1.rank+x2.rank-2 , y==sum(diagmul(x1.rep,~x2.rep))

  /// permutate indices of a tensor according to a given permutation:
  friend tensor operator*(const tensor &x,const permutation &p) //! \pre (asserted) x.rank>=2 && p.length==x.rank  \post y.rank==x.rank
  { return tensor(x.rep_*p); }

  /// add two tensors:
  friend tensor operator+(const tensor &x1,const tensor &x2) //! \pre x1.rank==x2.rank , x1.rank>0 && x2.rank>0 && x1.dimn==x2.dimn
  { return tensor(x1.rep_+x2.rep_); }

  /// negate a tensor:
  friend tensor operator-(const tensor &x) //! \post y.rank==x.rank
  { return tensor(-x.rep_); }

  /// subtract two tensors:
  friend tensor operator-(const tensor &x1,const tensor &x2) //! \pre x1.rank==x2.rank , x1.rank>0 && x2.rank>0 && x1.dimn==x2.dimn
  { return tensor(x1.rep_-x2.rep_); }

  /// multiply two tensors:
  //! \note The tensor multiplication 
  //! is performed by means of tensor 
  //! contraction (see con operation).
  //! It must be distinguished from
  //! the table multiplication which 
  //! is a direct element-by-element
  //! operation.
  friend tensor operator*(const tensor &x1,const tensor &x2)
  { return tensor(con(x1,x2)); }

  /// multiply a number by a tensor:
  friend tensor operator*(real a,const tensor &x) //! \post y.rank==x.rank
  { return tensor(a*x.rep_); }

  /// divide a tensor by a number:
  friend tensor operator/(const tensor &x,real a) //! \post y.rank==x.rank
  { return tensor(x.rep_/a); }

  /// obtain dyadic product of two tensors:
  friend tensor operator^(const tensor &x1,const tensor &x2) //! \post y.rank==x1.rank+x2.rank
  { return tensor(x1.rep_^x2.rep_); }
  
/// print table contents of the tensor to a given output stream:

  friend ostream &operator<<(ostream &out,const tensor &tns)
  { return out << tns.rep_; }
};

/////////////////////////////
///
/// Scalar as 0-Rank Tensor
///

class tensor0R : public tensor {
//! check rank for a given tensor:
  static const tensor &chk_rank_(const tensor &tns) 
  { chk_ndims_(torep_(tns)); return tns; }

//! check number of dimensions for a given table:
  static const table &chk_ndims_(const table &tab);
  
 public:
/// create a scalar object of zero value:

  tensor0R()
  {}

/// create a scalar object of a given value:

  tensor0R(real val)
  {
   rep_()=val;
  }

/// create a scalar object as duplication of a given sparse structure with optional copy of its components:

  tensor0R(const sparse &spr,bool cpy=true):
   tensor(chk_ndims_(table(spr,cpy)))
  {}

/// create a scalar object as duplication of a given dense structure with optional copy of its components:

  tensor0R(const table &tab,bool cpy):
   tensor(chk_ndims_(tab),cpy)
  {}
  
/// create a scalar object as duplication of a given tensor structure with optional copy of its components:

  tensor0R(const tensor &tns,bool cpy):
   tensor(chk_rank_(tns),cpy)
  {}
  
/// create a scalar object as duplication of a given 0-rank tensor structure with optional copy of its components:

  tensor0R(const tensor0R &scl,bool cpy):
   tensor(scl,cpy)
  {}
  
/// create a scalar object and assign to it a given dense object by sharing inner structure:

  tensor0R(const table &tab):
   tensor(chk_ndims_(tab))
  {}
  
/// create a scalar object and assign to it a given tensor object by sharing inner structure:

  tensor0R(const tensor &tns):
   tensor(chk_rank_(tns))
  {}
  
/// create a scalar object and assign to it a given 0-rank tensor object by sharing inner structure:

  tensor0R(const tensor0R &scl):
   tensor(scl)
  {}
  
/// relieve inner structure and destroy the scalar object:

  ~tensor0R()
  {}

/// assign a given dense structure to the scalar object by sharing inner structure:

  tensor0R &operator=(const table &tab)
  { (tensor &)*this=chk_ndims_(tab); return *this; }
  
/// assign a given tensor structure to the scalar object by sharing inner structure:

  tensor0R &operator=(const tensor &tns)
  { (tensor &)*this=chk_rank_(tns); return *this; }
  
/// assign a given 0-rank tensor structure to the scalar object by sharing inner structure:

  tensor0R &operator=(const tensor0R &scl)
  { (tensor &)*this=scl; return *this; }
  
/// return the only one component of the scalar:

  real &operator()()
  { return rep_(); }
  
/// return the only one component of the scalar:

  real operator()() const
  { return rep_(); }

/// set up a given value for the scalar:

  tensor0R &operator=(real a)
  { rep_()=a; return *this; }
};

typedef tensor0R scalar; //!< alias name

/////////////////////////////
///
/// Vector as 1st Rank Tensor
///

class tensor1R : public tensor {
//! check rank for a given tensor:
  static const tensor &chk_rank_(const tensor &tns) 
  { chk_ndims_(torep_(tns)); return tns; }

//! check number of dimensions for a given table:
  static const table &chk_ndims_(const table &tab);

 public:
/// create a vector object with a given dimension:

  explicit tensor1R(int dimn):
   tensor(1,dimn)
  {}

/// create a vector object as duplication of a given sparse structure with optional copy of its components:

  tensor1R(const sparse &spr,bool cpy=true):
   tensor(chk_ndims_(table(spr,cpy)))
  {}

/// create a vector object as duplication of a given dense structure with optional copy of its components:

  tensor1R(const table &tab,bool cpy):
   tensor(chk_ndims_(tab),cpy)
  {}
  
/// create a vector object as duplication of a given tensor structure with optional copy of its components:

  tensor1R(const tensor &tns,bool cpy):
   tensor(chk_rank_(tns),cpy)
  {}
  
/// create a vector object as duplication of a given 1-rank tensor structure with optional copy of its components:

  tensor1R(const tensor1R &vec,bool cpy):
   tensor(vec,cpy)
  {}
  
/// create a vector object and assign to it a given dense object by sharing inner structure:

  tensor1R(const table &tab):
   tensor(chk_ndims_(tab))
  {}
  
/// create a vector object and assign to it a given tensor object by sharing inner structure:

  tensor1R(const tensor &tns):
   tensor(chk_rank_(tns))
  {}
  
/// create a vector object and assign to it a given 1-rank tensor object by sharing inner structure:

  tensor1R(const tensor1R &vec):
   tensor(vec)
  {}
  
/// relieve inner structure and destroy the vector object:

  ~tensor1R()
  {}

/// assign a given dense structure to the vector object by sharing inner structure:

  tensor1R &operator=(const table &tab)
  { (tensor &)*this=chk_ndims_(tab); return *this; }
  
/// assign a given tensor structure to the vector object by sharing inner structure:

  tensor1R &operator=(const tensor &tns)
  { (tensor &)*this=chk_rank_(tns); return *this; }
  
/// assign a given 1-rank tensor structure to the vector object by sharing inner structure:

  tensor1R &operator=(const tensor1R &vec)
  { (tensor &)*this=vec; return *this; }
  
/// return an component of the vector giving its linear index:

  real &operator()(int i) //! \pre 0 <= i < dimn
  { return rep_[i]; }
  
/// return an component of the vector giving its linear index:

  real operator()(int i) const
  { return rep_[i]; }

/// set up a given value for all components of the vector:

  tensor1R &operator=(real a)
  { rep_=a; return *this; }

/// perform math operations on vectors:

  /// calculate norm of a vector:
  friend real norm(const tensor1R &x);

  /// find element of a given data space maximal at a given vector:
  friend Data::subset argmax(const Data::space &_F_,const tensor1R &x); //!< \pre (asserted) x.dimn==_F_.card  \post y in F

  /// find element of a given data space minimal at a given vector:
  friend Data::subset argmin(const Data::space &_F_,const tensor1R &x); //!< \pre (asserted) x.dimn==_F_.card  \post y in F
};

typedef tensor1R vector; //!< alias name

//// explicit friend function injections:

real norm(const tensor1R &x);
Data::subset argmax(const Data::space &_F_,const tensor1R &x); 
Data::subset argmin(const Data::space &_F_,const tensor1R &x);  

/////////////////////////////
///
/// Matrix as 2nd Rank Tensor
///

class tensor2R : public tensor {
//! check rank for a given tensor:
  static const tensor &chk_rank_(const tensor &tns) 
  { chk_ndims_(torep_(tns)); return tns; }

//! check number of dimensions for a given table:
  static const table &chk_ndims_(const table &tab);

 public:
/// create a matrix object with a given dimension:

  explicit tensor2R(int dimn):
   tensor(2,dimn)
  {}

/// create a matrix object as duplication of a given sparse structure with optional copy of its components:

  tensor2R(const sparse &spr,bool cpy=true):
   tensor(chk_ndims_(table(spr,cpy)))
  {}

/// create a matrix object as duplication of a given dense structure with optional copy of its components:

  tensor2R(const table &tab,bool cpy):
   tensor(chk_ndims_(tab),cpy)
  {}
  
/// create a matrix object as duplication of a given tensor structure with optional copy of its components:

  tensor2R(const tensor &tns,bool cpy):
   tensor(chk_rank_(tns),cpy)
  {}
  
/// create a matrix object as duplication of a given 2-rank tensor structure with optional copy of its components:

  tensor2R(const tensor2R &mtr,bool cpy):
   tensor(mtr,cpy)
  {}
  
/// create a matrix object and assign to it a given dense object by sharing inner structure:

  tensor2R(const table &tab):
   tensor(chk_ndims_(tab))
  {}
  
/// create a matrix object and assign to it a given tensor object by sharing inner structure:

  tensor2R(const tensor &tns):
   tensor(chk_rank_(tns))
  {}
  
/// create a matrix object and assign to it a given 2-rank tensor object by sharing inner structure:

  tensor2R(const tensor2R &mtr):
   tensor(mtr)
  {}
  
/// relieve inner structure and destroy the matrix object:

  ~tensor2R()
  {}

/// assign a given dense structure to the matrix object by sharing inner structure:

  tensor2R &operator=(const table &tab)
  { (tensor &)*this=chk_ndims_(tab); return *this; }
  
/// assign a given tensor structure to the matrix object by sharing inner structure:

  tensor2R &operator=(const tensor &tns)
  { (tensor &)*this=chk_rank_(tns); return *this; }
  
/// assign a given 2-rank tensor structure to the matrix object by sharing inner structure:

  tensor2R &operator=(const tensor2R &mtr)
  { (tensor &)*this=mtr; return *this; }
  
/// return an component of the matrix giving indices for its rows and columns:

  real &operator()(int i,int j) //! \pre 0 <= i,j < dimn
  { int b=rep_.size(); return rep_[i*b+j]; }
  
/// return an component of the matrix giving indices for its rows and columns:

  real operator()(int i,int j) const
  { int b=rep_.size(); return rep_[i*b+j]; }

/// set up identity values for all components of the matrix:

  tensor2R &idnt(); //!< \invariant dimn

/// set up given values for all components of the matrix:

  tensor2R &operator=(real a)
  { rep_=a; return *this; }

/// perform math operations on matrices:

  /// calculate determinant of a matrix:
  friend real det(const tensor2R &x);

  /// calculate trace of a matrix:
  friend real tr(const tensor2R &x);

  /// calculate inverse of a matrix:
  friend tensor2R inv(const tensor2R &x); //!< \pre (asserted) det(x)!=0  \post y.dimn==x.dimn

  /// calculate square of a matrix:
  friend tensor2R sqr(const tensor2R &x)
  { return tensor2R(x*x); }

  /// calculate cube of a matrix:
  friend tensor2R cub(const tensor2R &x)
  { return tensor2R(x*x*x); }

  /// calculate nth-power of a matrix:
  friend tensor2R npow(const tensor2R &x,int n);

  /// transpose a matrix:
  friend tensor2R operator~(const tensor2R &x); //!< \post y.dimn==x.dimn

  /// divide two matrices:
  friend tensor2R operator/(const tensor2R &x1,const tensor2R &x2)
  { return tensor2R(x1*inv(x2)); }

  /// divide a number by a matrix:
  friend tensor2R operator/(real a,const tensor2R &x) //! \post y.dimn==x.dimn
  { return tensor2R(a*inv(x)); }
};

typedef tensor2R matrix; //!< alias name

//// explicit friend function injections:

real det(const tensor2R &x);
real tr(const tensor2R &x);
tensor2R inv(const tensor2R &x);
tensor2R sqr(const tensor2R &x);
tensor2R cub(const tensor2R &x);
tensor2R npow(const tensor2R &x,int n);
tensor2R operator~(const tensor2R &x);
tensor2R operator/(const tensor2R &x1,const tensor2R &x2);
tensor2R operator/(real a,const tensor2R &x);

/////////////////////////////
///
/// Any Rank Tensor
///

class tensorNR : public tensor {
 public:
/// create a tensor object with given parameters of its structure:

  tensorNR(int rank,int dimn):
   tensor(rank,dimn)
  {}

/// create a tensor object as combination of given two substructures:

  tensorNR(int rank1,int rank2,int dimn):
   tensor(rank1,rank2,dimn)
  {}
  
/// create a tensor object as duplication of a given sparse structure with optional copy of its components:

  tensorNR(const sparse &spr,bool cpy=true):
   tensor(spr,cpy)
  {}

/// create a tensor object as duplication of a given dense structure with optional copy of its components:

  tensorNR(const table &tab,bool cpy):
   tensor(tab,cpy)
  {}
  
/// create a tensor object as duplication of a given tensor structure with optional copy of its components:

  tensorNR(const tensor &tns,bool cpy):
   tensor(tns,cpy)
  {}
  
/// create a tensor object and assign to it a given dense object by sharing inner structure:

  tensorNR(const table &tab):
   tensor(tab)
  {}
  
/// create a tensor object and assign to it a given tensor object by sharing inner structure:

  tensorNR(const tensor &tns):
   tensor(tns)
  {}
  
/// relieve inner structure and destroy the tensor object:

  ~tensorNR()
  {}

/// assign a given dense structure to the tensor object by sharing inner structure:

  tensorNR &operator=(const table &tab)
  { (tensor &)*this=tab; return *this; }
  
/// assign a given tensor structure to the tensor object by sharing inner structure:

  tensorNR &operator=(const tensor &tns)
  { (tensor &)*this=tns; return *this; }
  
/// return first component of the tensor or the only one component of the scalar:

  real &operator()()
  { return rep_(); }
  
/// return first component of the tensor or the only one component of the scalar:

  real operator()() const
  { return rep_(); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0) //! \pre rank==1 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0) const //! \pre rank==1 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1) //! \pre rank==2 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1) const //! \pre rank==2 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2) //! \pre rank==3 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2) const //! \pre rank==3 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2,int i3) //! \pre rank==4 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2,int i3) const //! \pre rank==4 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2,int i3,int i4) //! \pre rank==5 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2,int i3,int i4) const //! \pre rank==5 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5) //! \pre rank==6 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5) const //! \pre rank==6 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6) //! \pre rank==7 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6) const //! \pre rank==7 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7) //! \pre rank==8 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6,i7); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7) const //! \pre rank==8 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6,i7); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8) //! \pre rank==9 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6,i7,i8); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8) const //! \pre rank==9 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6,i7,i8); }

/// return an component of the non-scalar tensor giving its indices for whole rank:

  real &operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8,int i9) //! \pre rank==10 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6,i7,i8,i9); }
  
/// return an component of the non-scalar tensor giving its indices for whole rank:

  real operator()(int i0,int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8,int i9) const //! \pre rank==10 , 0 <= ik < dimn , 0 <= k < rank
  { return rep_(i0,i1,i2,i3,i4,i5,i6,i7,i8,i9); }

/// set up a given value for all components of the tensor:

  tensorNR &operator=(real a)
  { rep_=a; return *this; }
};

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of TENSOR_H_ header file

