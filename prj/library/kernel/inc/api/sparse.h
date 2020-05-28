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
/// Sparse Collection At Algorithm Implementation Tools
/**
 * @file       sparse.h
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

#ifndef SPARSE_H_
#define SPARSE_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Any Dimensional Sparse Collection
/// 
/// Used for storing results of input data 
/// processing, implemented as mixed linear 
/// array - linked structure of real type 
/// elements with both direct and hash 
/// addressing, and  also with initial 
/// zeroising.
///

class sparse {
  int *lnkcnt_; //!< counter of links to inner structure shared by sparse objects
  int rank_; //!< rank of the sparse as number of its vector indices
  inttuple *rnksizs_; //!< array of sizes for consecutive vector indices of the sparse
  int ndims_; //!< dimensionality of the sparse as number of all its scalar indices
  
//! Record of Element of Sparse Collection
  struct elem_rec_ {
   elem_rec_ *next; //!< pointer to next element relative to this element
   int *dmids; //!< hashable key of the element as array of all its scalar indices given for all dimensions
   real inval; //!< instant value of the real type element
  };
  
//! Type of Linked List of Sparse Elements  
  struct elem_list_ {
   elem_rec_ *first; //!< pointer to first element of the list
   elem_rec_ *last; //!< pointer to last element of the list
  };
  
//! Type of Arrayed Table of Sparse Elements
//! \note It is addressed directly by linear indexing.
  struct elem_table_ {
   int ndims; //!< dimensionality of the table as number of all table indices of the sparse
   int *dimsizs; //!< array of sizes along consecutive table dimensions of the sparse
   int *dimbass; //!< array of bases at preceding table dimensions of the sparse
   elem_list_ *llnelms; //!< linear array of slots with elements of the sparse arranged as linked list to avoid hash collisions when table dimensions are exceeded
   int nelms; //!< element array length
  };

  mutable elem_table_ dirtab_; //!< object of directly addressed element table with dimensionality including a small beginning subset of all dimensions of the sparse
  mutable elem_rec_ *lnkelms_; //!< linked structure of all elements of the sparse
  mutable int nelms_; //!< element structure length
  mutable int *curdmids_; //!< array of all indices for current element of the sparse given for all dimensions
  bool isnullable_; //!< flag of whether the sparse's elements can be nullified during any non-zero-invariant math operation

//! create a sparse structure with given rank and list of its vector sizes: 
  void construct_(int ndims,...);

//! create separately a linear array of linked elements:
  void create_llnelms_() const;

//! create separately a linked structure of all elements:
  void create_lnkelms_() const;

//! destroy separately the structure of all elements:
  void destroy_allelms_();

//! return the sparse's element of an array subscripted by a given total linear index being ordinal number of inserted elements:
  real &linelms_(int G) const;

//! return the first sparse's element of an linear array:
  real *linelms_() const;

//! return the sparse's element of an array subscripted by a given list of vector indices for whole rank:
  real &rnkelms_(const inttuple *i0...) const;

//! return the sparse's element of an array subscripted by a given tuple of scalar indices for all dimensions:
  real &dimelms_(const inttuple &ii) const;

//! expose the sparse's element of an array subscripted by a given array of scalar indices for all dimensions:
  elem_rec_ &dmelems_(const int ii[]) const;

//! expose the sparse's element of an array subscripted by given subarrays of scalar indices for all dimensions:
  elem_rec_ &dmelems_(int n1,int n2,const int ii1[],const int ii2[]) const;

//! expose the first sparse's element of a structure with all linked elements:
  elem_rec_ *lkelems_() const;

//! insert a new element into the sparse at the current location and/or expose it with use of hash addressing:
  elem_rec_ &access_curelem_() const;

 public:
/// create a sparse object with given parameters of its structure: 

  sparse(int rank,const inttuple sizes[],int ndirs=1); //!< \pre count(sizes)>=rank  \pre (asserted) sizes[k].count>0 , 0 <= k < rank

/// create a sparse object as combination of given two substructures:

  sparse(int rank1,int rank2,const inttuple sizes1[],const inttuple sizes2[],int ndirs1=1,int ndirs2=1); //!< \pre count(sizes1)>=rank1 , count(sizes2)>=rank2  \pre (asserted) sizes1[k].count>0 && 0 <= k < rank1 , sizes2[k].count>0 && 0 <= k < rank2  \post rank==rank1+rank2 , ndirs == rank1>0 ? ndirs1 : ndirs2

/// create a sparse object with no dimensions as single value point:

  sparse(); //!< \post ndims==0 , count==0
  
/// create a non-point sparse object with given sizes for whole its rank:

  explicit sparse(const inttuple &s0) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==1
  { construct_(1,&s0); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==2 
  { construct_(2,&s0,&s1); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==3 
  { construct_(3,&s0,&s1,&s2); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2,const inttuple &s3) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==4 
  { construct_(4,&s0,&s1,&s2,&s3); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2,const inttuple &s3,const inttuple &s4) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==5 
  { construct_(5,&s0,&s1,&s2,&s3,&s4); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2,const inttuple &s3,const inttuple &s4,const inttuple &s5) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==6 
  { construct_(6,&s0,&s1,&s2,&s3,&s4,&s5); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2,const inttuple &s3,const inttuple &s4,const inttuple &s5,const inttuple &s6) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==7 
  { construct_(7,&s0,&s1,&s2,&s3,&s4,&s5,&s6); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2,const inttuple &s3,const inttuple &s4,const inttuple &s5,const inttuple &s6,const inttuple &s7) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==8 
  { construct_(8,&s0,&s1,&s2,&s3,&s4,&s5,&s6,&s7); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2,const inttuple &s3,const inttuple &s4,const inttuple &s5,const inttuple &s6,const inttuple &s7,const inttuple &s8) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==9 
  { construct_(9,&s0,&s1,&s2,&s3,&s4,&s5,&s6,&s7,&s8); }

/// create a non-point sparse object with given sizes for whole its rank:

  sparse(const inttuple &s0,const inttuple &s1,const inttuple &s2,const inttuple &s3,const inttuple &s4,const inttuple &s5,const inttuple &s6,const inttuple &s7,const inttuple &s8,const inttuple &s9) //! \pre (asserted) sk.count>0 , 0 <= k < rank  \post rank==10 
  { construct_(10,&s0,&s1,&s2,&s3,&s4,&s5,&s6,&s7,&s8,&s9); }
  
/// create a sparse object as duplication of a given structure with optional copy of its elements:

  sparse(const sparse &spr,bool cpy);

/// create a sparse object and assign to it a given sparse by sharing inner structure:

  sparse(const sparse &spr);

/// relieve inner structure and destroy the sparse object:

  ~sparse();

/// assign a given sparse to the sparse object by sharing inner structure:

  sparse &operator=(const sparse &spr);

/// test whether this sparse object has the inner structure shared by another sparses:

  bool isshared() const;

/// make clone of the sparse object by copying all its elements when the inner structure is shared:

  sparse &clone(); //!< \invariant rank, count  \post isshared==false

/// dilute the sparse object by inserting all zero valued elements into its structure:

  sparse &dilute(); //!< \invariant rank  \post count>0

/// thicken the sparse object by removing all zero valued elements form its structure:

  sparse &thicken(); //!< \invariant rank  \post count>=0

/// return rank of the sparse as number of vector indices:

  int rank() const //! \post rank>=0
  { return rank_; }
  
/// return for the non-point sparse a dimension of a given vector index as number of its components:

  int idimn(int k) const //! \pre rank>0 , 0 <= k < rank  \post idimn(k)>0
  { return rnksizs_[k].count(); }
  
/// return number of scalar dimensions for the sparse up to a given subrank:

  int ndims(int r) const; //!< \pre (asserted) 0 <= r <= rank  \post 0 <= ret==sum(0<=k<r)(idimn(k)) <= ndims
  
/// return number of all scalar dimensions for the sparse at whole its rank:

  int ndims() const //! \post ndims==sum(idimn(:)) >= 0
  { return ndims_; }
  
/// set up number of beginning dimensions of the sparse for direct addressing:

  int ndirs(int nd) //! \pre 0 <= nd <= ndims
  { int ndo=dirtab_.ndims; if (dirtab_.llnelms==0) dirtab_.ndims=nd; return ndo; }
    
/// return number of beginning dimensions of the sparse for direct addressing:

  int ndirs() const //! \post 0 <= ndirs <= ndims
  { return dirtab_.ndims; }
    
/// set up flag for hash addressing of the sparse:

  bool hashing(bool hsh)
  { bool hsho=dirtab_.ndims<ndims_; if (dirtab_.llnelms==0) dirtab_.ndims = hsh ? 0 : ndims_; return hsho; }
    
/// return flag for hash addressing of the sparse:

  bool ishashed() const
  { return dirtab_.ndims<ndims_; }
    
/// return sizes of the non-point sparse for given vector index:

  const inttuple &sizes(int k) const //! \pre rank>0 , 0 <= k < rank  \post sizes>0
  { return rnksizs_[k]; }
  
/// return sizes of the non-point sparse for given vector index and its components:

  int size(int k,int q) const //! \pre rank>0 , 0 <= q < idimn(k) , 0 <= k < rank  \post size>0
  { return rnksizs_[k][q]; }
  
/// return size of a given dimension for the non-point sparse:

  int size(int h=0) const; //!< \pre ndims>0 , 0 <= h < ndims  \post size>0
  
/// return total number of all elements inserted into the sparse:

  int count() const //! \post count>=0
  { return nelms_; }
  
/// return successive elements inserted into the sparse giving their ordinal number:

  real &operator[](int G) //! \pre rank>0 && 0 <= G < count , rank<=0 && G==0  \post rank<=0 && count==1
  { return linelms_(G); }
  
/// return successive elements inserted into the sparse giving their ordinal number:

  real operator[](int G) const //! \pre rank>0 && 0 <= G < count , rank<=0 && G==0  \post rank<=0 && count==1
  { return linelms_(G); }

/// return first inserted element of the sparse or the only one element of the point sparse:

  real &operator()() //! \post rank<=0 && count==1
  { return *linelms_(); }

/// return first inserted element of the sparse or the only one element of the point sparse:

  real operator()() const //! \post rank<=0 && count==1
  { return *linelms_(); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0) //! \pre rank==1 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0) const //! \pre rank==1 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1) //! \pre rank==2 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1); }                        

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1) const //! \pre rank==2 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2) //! \pre rank==3 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2) const //! \pre rank==3 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3) //! \pre rank==4 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3) const //! \pre rank==4 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4) //! \pre rank==5 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4) const //! \pre rank==5 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5) //! \pre rank==6 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5) const //! \pre rank==6 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6) //! \pre rank==7 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6) const //! \pre rank==7 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6,const inttuple &i7) //! \pre rank==8 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6,const inttuple &i7) const //! \pre rank==8 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6,const inttuple &i7,const inttuple &i8) //! \pre rank==9 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7,&i8); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6,const inttuple &i7,const inttuple &i8) const //! \pre rank==9 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7,&i8); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real &operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6,const inttuple &i7,const inttuple &i8,const inttuple &i9) //! \pre rank==10 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7,&i8,&i9); }

/// insert/return an element of the non-point sparse giving its vector indices for whole rank:

  real operator()(const inttuple &i0,const inttuple &i1,const inttuple &i2,const inttuple &i3,const inttuple &i4,const inttuple &i5,const inttuple &i6,const inttuple &i7,const inttuple &i8,const inttuple &i9) const //! \pre rank==10 , ik.count>=idimn(k) , 0 <= ik[q] < size(k,q) , 0 <= q < idimn(k) , 0 <= k < rank
  { return rnkelms_(&i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7,&i8,&i9); }

/// insert/return an element of the sparse for a given tuple of all scalar indices:

  real &operator[](const inttuple &ii) //! \pre ii.count>=ndims , 0 <= ii[h] < size(h) , 0 <= h < ndims
  { return dimelms_(ii); }
  
/// insert/return an element of the sparse for a given tuple of all scalar indices:

  real operator[](const inttuple &ii) const //! \pre ii.count>=ndims , 0 <= ii[h] < size(h) , 0 <= h < ndims
  { return dimelms_(ii); }
  
 private:
  template <typename TSparObj,typename TElemRec,typename TElemVal>
  class iterator_type_;

  template <typename TSparObj,typename TElemRec,typename TElemVal>
  friend class iterator_type_;
  
//////
//! Iterator Type For Sparse Collection  
//!
//! It is used for traversing through 
//! all inserted elements of any sparse 
//! collection.
//!
  template <typename TSparObj,typename TElemRec,typename TElemVal>
  class iterator_type_ {
    int ndims_; //!< copy of dimensionality of the traversed sparse
    TElemRec *curelm_; //!< current element of the traversed sparse
   public:
  /// create a null iterator object: 

    iterator_type_(): ndims_(0), curelm_(0)
    {}

  /// create an iterator object attached to first inserted element of a given sparse: 

    explicit iterator_type_(TSparObj &that): ndims_(that.ndims_)
    { curelm_=that.lkelems_(); }

  /// make iterator object attached to first inserted element of a given sparse: 

    iterator_type_ &operator()(TSparObj &that)
    { ndims_=that.ndims_; curelm_=that.lkelems_(); return *this; }
    
  /// test whether the iterator is not attached to a sparse element: 

    bool isnil() const
    { return curelm_==0; }
    
  /// test whether the iterator is not attached to a sparse element: 

    bool operator!() const
    { return curelm_==0; }
    
  /// test whether the iterator is attached to a sparse element: 

    operator bool() const
    { return curelm_!=0; }
    
  /// premove forward the iterator to next inserted element of the sparse: 

    iterator_type_ &operator++() //! \pre !isnil 
    { curelm_=curelm_->next; return *this; }
    
  /// postmove forward the iterator to next inserted element of the sparse: 

    iterator_type_ operator++(int) //! \pre !isnil 
    { iterator_type_ Po=*this; curelm_=curelm_->next; return Po; }
    
  /// return number of indices of the current element: 
   
    int nids() const //! \post nids==that.ndims >= 0
    { return ndims_; }
    
  /// return length for the current element: 
   
    int length() const //! \post length==that.ndims >= 0
    { return ndims_; }
    
  /// return tuple of indices of the current element: 
   
    inttuple ids() const //! \pre !isnil  \post 0 <= ids[h] < that.size(h) , 0 <= h < that.ndims
    { return inttuple::create(ndims_,curelm_->dmids); }
    
  /// return tuple of indices of the current element: 
   
    operator inttuple() const //! \pre !isnil  \post 0 <= ret[h] < that.size(h) , 0 <= h < that.ndims
    { return inttuple::create(ndims_,curelm_->dmids); }
    
  /// return a given index of the current element: 
   
    int ids(int h) const //! \pre !isnil , 0 <= h < that.ndims  \post 0 <= ids < that.size(h)
    { return curelm_->dmids[h]; }
    
  /// return a given index of the current element: 
   
    int operator[](int h) const //! \pre !isnil , 0 <= h < that.ndims  \post 0 <= ret < that.size(h)
    { return curelm_->dmids[h]; }
    
  /// set up / return value of the current element: 
   
    TElemVal val(TElemVal v) const //! \pre !isnil
    { real vo=curelm_->inval; curelm_->inval=v; return vo; }
    
  /// return value of the current element: 
   
    TElemVal val() const //! \pre !isnil
    { return curelm_->inval; }
    
  /// return value of the current element: 
   
    TElemVal &operator*() const //! \pre !isnil
    { return curelm_->inval; }
  };
//||||  
  
 public: 
  typedef iterator_type_<sparse,elem_rec_,real> iterator; ///< iterator type to variable sparses
  typedef iterator_type_<const sparse,const elem_rec_,const real> const_iterator; ///< iterator type to constant sparses

/// set up flag for enabling nullification of elements in the sparse by non-zero-invariant operations:
//!
//! \note Any zero valued 
//! element not inserted into 
//! this sparse collection is 
//! treated as a null one and 
//! is converted to zero only
//! when this flag is set off
//! for any arguments in that 
//! operations for which zero 
//! is not invariant.
//! 
  bool nullification(bool nul)
  { bool nulo=isnullable_; isnullable_=nul; return nulo; }

/// return flag for enabling nullification of elements in the sparse by non-zero-invariant operations:
                   
  bool isnullable() const
  { return isnullable_; }
  
/// set up zero values for all inserted elements of the sparse:

  sparse &zero(); //!< \invariant rank  \post count==0

/// set up unit values for all inserted elements of the sparse:

  sparse &unit(); //!< \invariant rank, count|isnullable

/// set up random values for all inserted elements of the sparse:

  sparse &rand(); //!< \invariant rank, count|isnullable

/// set up given values for all inserted elements of the sparse:

  sparse &operator=(real a); //!< \invariant rank, count|(isnullable , a!=0)  \post a==0 => count==0

/// perform this relational operation for two sparses:

  friend bool operator==(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank==x2.rank && x1.ndims==x2.ndims  \post x1.isnullable && x2.isnullable && ret ~ "x1[G]==x2[G] , 0<=G<min(x1.count,x2.count)"

/// perform this relational operation for two sparses:

  friend bool operator!=(const sparse &x1,const sparse &x2) //! \pre (asserted) x1.rank==x2.rank && x1.ndims==x2.ndims  \post x1.isnullable && x2.isnullable && ret ~ "x1[G]==x2[G] , 0<=G<min(x1.count,x2.count)"
  { return !(x1==x2); }

/// perform this relational operation for number and sparse:

  friend bool operator==(real a,const sparse &x); //!< \post x.isnullable && ret ~ "a==x[G] , 0<=G<x.count"

/// perform this relational operation for number and sparse:

  friend bool operator!=(real a,const sparse &x) //! \post x.isnullable && ret ~ "a==x[G] , 0<=G<x.count"
  { return !(a==x); }
  
/// perform this relational operation for sparse and number:

  friend bool operator==(const sparse &x,real a); //!< \post x.isnullable && ret ~ "x[G]==a , 0<=G<x.count"

/// perform this relational operation for sparse and number:

  friend bool operator!=(const sparse &x,real a) //! \post x.isnullable && ret ~ "x[G]==a , 0<=G<x.count"
  { return !(x==a); }

//// perform math operations on sparses:

  /// add a number to this sparse:
  sparse &operator+=(real a); //!< \invariant rank, count|isnullable

  /// subtract a number from this sparse:
  sparse &operator-=(real a); //!< \invariant rank, count|isnullable

  /// multiply this sparse by a number:
  sparse &operator*=(real a); //!< \invariant rank, count|(a!=0)  \post a==0 => count==0

  /// divide this sparse by a number:
  sparse &operator/=(real a); //!< \invariant rank, count|(a!=0)  \post a==0 => count==0

  /// apply this math function to a sparse:
  friend sparse abs(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// apply this math function to a sparse:
  friend sparse log(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// apply this math function to a sparse:
  friend sparse log2(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// apply this math function to a sparse:
  friend sparse sqr(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// apply this math function to a sparse:
  friend sparse sqrt(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// apply this math function to a sparse:
  friend sparse cub(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// apply this math function to a sparse:
  friend sparse pow(const sparse &x,real a); //!< \post y.rank==x.rank , y.count==x.count

  /// sum cumulatively a sparse at its last, rightmost dimension:
  friend sparse cum(const sparse &x); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// sum a sparse at its last, rightmost dimension:
  friend sparse sum(const sparse &x); //!< \post x.rank>0 && y.rank==x.rank-1 , y.count<=x.count

  /// average a sparse at its last, rightmost dimension:
  friend sparse avg(const sparse &x); //!< \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// obtain product of elements for a sparse at its last, rightmost dimension:
  friend sparse prod(const sparse &x); //!< \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// maximize a sparse at its last, rightmost dimension:
  friend sparse max(const sparse &x); //!< \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// minimize a sparse at its last, rightmost dimension:
  friend sparse min(const sparse &x); //!< \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// maximize a sparse at its last, rightmost dimension if its index equals to a given index:
  friend sparse max_at(const sparse &x,const inttuple &io); //!< \pre io.count>=x.idimn(x.rank-1)  \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// minimize a sparse at its last, rightmost dimension if its index equals to a given index:
  friend sparse min_at(const sparse &x,const inttuple &io); //!< \pre io.count>=x.idimn(x.rank-1)  \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// maximize a sparse at its last, rightmost dimension if its index does not equal to a given index:
  friend sparse max_nat(const sparse &x,const inttuple &io); //!< \pre io.count>=x.idimn(x.rank-1)  \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// minimize a sparse at its last, rightmost dimension if its index does not equal to a given index:
  friend sparse min_nat(const sparse &x,const inttuple &io); //!< \pre io.count>=x.idimn(x.rank-1)  \post x.rank>0 && y.rank==x.rank-1 , x.isnullable && y.count<=x.count

  /// return maximal region of a sparse at its last, rightmost dimension:
  friend sparse rgn_max(const sparse &x); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// return minmal region of a sparse at its last, rightmost dimension:
  friend sparse rgn_min(const sparse &x); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// return region of a sparse at its last, rightmost dimension, equal to a given value:
  friend sparse rgn_eq(const sparse &x,real uo); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// return region of a sparse at its last, rightmost dimension, not equal to a given value:
  friend sparse rgn_neq(const sparse &x,real uo); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// return sparse with maximal elements among elements of two sparses:
  friend sparse max2(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank==x2.rank && x1.ndims==x2.ndims  \post y.rank==x1.rank , x1.isnullable && x2.isnullable && y.count==min(x1.count,x2.count)

  /// return sparse with minimal elements among elements of two sparses:
  friend sparse min2(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank==x2.rank && x1.ndims==x2.ndims  \post y.rank==x1.rank , x1.isnullable && x2.isnullable && y.count==min(x1.count,x2.count)

  /// return sparse being diagonal part of a sparse at its last, rightmost dimension:
  friend sparse diag(const sparse &x); //!< \pre x.rank>=2 && x.idimn(x.rank-2)==x.idimn(x.rank-1)  \post x.rank>0 && y.rank==x.rank-1 , y.count<=x.count

  /// return sparse being diagonal part of sum of two sparses at their last, rightmost dimension:
  friend sparse diagadd(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank>=1 && x2.rank>=1 && x1.idimn(x1.rank-1)==x2.idimn(x2.rank-1)  \post (x1.rank>0 || x2.rank>0) && y.rank==x1.rank+x2.rank-1 , x1.isnullable && x2.isnullable && y.count<=x1.count*x2.count

  /// return sparse being diagonal part of difference of two sparses at their last, rightmost dimension:
  friend sparse diagsub(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank>=1 && x2.rank>=1 && x1.idimn(x1.rank-1)==x2.idimn(x2.rank-1)  \post (x1.rank>0 || x2.rank>0) && y.rank==x1.rank+x2.rank-1 , x1.isnullable && x2.isnullable && y.count<=x1.count*x2.count

  /// return sparse being diagonal part of product of two sparses at their last, rightmost dimension:
  friend sparse diagmul(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank>=1 && x2.rank>=1 && x1.idimn(x1.rank-1)==x2.idimn(x2.rank-1)  \post (x1.rank>0 || x2.rank>0) && y.rank==x1.rank+x2.rank-1 , y.count<=x1.count*x2.count

  /// return sparse being diagonal part of quotient of two sparses at their last, rightmost dimension:
  friend sparse diagdiv(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank>=1 && x2.rank>=1 && x1.idimn(x1.rank-1)==x2.idimn(x2.rank-1)  \post (x1.rank>0 || x2.rank>0) && y.rank==x1.rank+x2.rank-1 , y.count<=x1.count*x2.count

  /// obtain dyadic addition of two sparses:
  friend sparse dyadadd(const sparse &x1,const sparse &x2); //!< \post y.rank==x1.rank+x2.rank , x1.isnullable && x2.isnullable && y.count==x1.count*x2.count

  /// obtain dyadic subtraction of two sparses:
  friend sparse dyadsub(const sparse &x1,const sparse &x2); //!< \post y.rank==x1.rank+x2.rank , x1.isnullable && x2.isnullable && y.count==x1.count*x2.count

  /// obtain dyadic multiplication of two sparses:
  friend sparse dyadmul(const sparse &x1,const sparse &x2); //!< \post y.rank==x1.rank+x2.rank , y.count==x1.count*x2.count

  /// obtain dyadic division of two sparses:
  friend sparse dyaddiv(const sparse &x1,const sparse &x2); //!< \post y.rank==x1.rank+x2.rank , y.count==x1.count*x2.count

  /// permutate indices of a sparse according to a given permutation:
  friend sparse operator*(const sparse &x,const permutation &p); //!< \pre (asserted) x.rank>=2 && p.length==x.rank  \post y.rank==x.rank , y.count==x.count

  /// inverse indices of a sparse:
  friend sparse operator~(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// add two sparses:
  friend sparse operator+(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank==x2.rank && x1.ndims==x2.ndims  \post y.rank==x1.rank , x1.isnullable && x2.isnullable && y.count==min(x1.count,x2.count)

  /// add a number to a sparse:
  friend sparse operator+(real a,const sparse &x); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// add a sparse to a number:
  friend sparse operator+(const sparse &x,real a); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// negate a sparse:
  friend sparse operator-(const sparse &x); //!< \post y.rank==x.rank , y.count==x.count

  /// subtract two sparses:
  friend sparse operator-(const sparse &x1,const sparse &x2); //!< \pre (asserted) x1.rank==x2.rank && x1.ndims==x2.ndims  \post y.rank==x1.rank , x1.isnullable && x2.isnullable && y.count==min(x1.count,x2.count)

  /// subtract a number from a sparse:
  friend sparse operator-(real a,const sparse &x); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// subtract a sparse from a number:
  friend sparse operator-(const sparse &x,real a); //!< \post y.rank==x.rank , x.isnullable && y.count==x.count

  /// multiply two sparses:
  friend sparse operator*(const sparse &x1,const sparse &x2); //!< \post x1.rank>=x2.rank && y.rank==x1.rank || x1.rank<x2.rank && y.rank==x2.rank , y.count==min(x1.count,x2.count)

  /// multiply a number by a sparse:
  friend sparse operator*(real a,const sparse &x); //!< \post y.rank==x.rank , a!=0 && y.count==x.count || a==0 && y.count==0

  /// multiply a sparse by a number:
  friend sparse operator*(const sparse &x,real a); //!< \post y.rank==x.rank , a!=0 && y.count==x.count || a==0 && y.count==0

  /// divide two sparses:
  friend sparse operator/(const sparse &x1,const sparse &x2); //!< \post x1.rank>=x2.rank && y.rank==x1.rank || x1.rank<x2.rank && y.rank==x2.rank , y.count==min(x1.count,x2.count)

  /// divide a number by a sparse:
  friend sparse operator/(real a,const sparse &x); //!< \post y.rank==x.rank , a!=0 && y.count==x.count || a==0 && y.count==0

  /// divide a sparse by a number:
  friend sparse operator/(const sparse &x,real a); //!< \post y.rank==x.rank , a!=0 && y.count==x.count || a==0 && y.count==0

  /// obtain dyadic product of two tables:
  friend sparse operator^(const sparse &x1,const sparse &x2)
  { return sparse(dyadmul(x1,x2)); }

/// print contents of the sparse a given output stream:

  void print_conts(ostream &out) const;

/// print contents to output stream:

  friend ostream &operator<<(ostream &out,const sparse &spr)
  {
   spr.print_conts(out);
   return out;
  }
};

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of SPARSE_H_ header file

