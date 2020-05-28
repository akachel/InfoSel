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
/// Data Set At Input Data Structures
/**
 * @file       dset.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains declarations of input data structures.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef DSET_H_
#define DSET_H_

namespace InfoSelCore {
 
namespace Data {

/////////////////////////////
///
/// Input Data Set 
///
/// It represents logical set of
/// studied independent attributes 
/// (i.e. features). It stores these
/// attributes and exposes their data 
/// instances.
///

class datset {
  friend class datspace;
  friend class subdatset;
  friend class datfile;
 private:
  mutable int nlinks_; //!< number of holding links to the set object
  string objtag_; //!< short name of the set object
  string elmtag_; //!< common tag for all elements in the set
  datcolumn **orgcols_; //!< array of pointers to all columns inserted into the set with the original layout of input file
  datcolumn **odrcols_; //!< array of pointers to above columns with the insertion ordering
  real *odrcols_weg_; //!< additional array of weight coefficients for inserting above columns into the set in weight-ascendant order
  int nptrcols_; //!< pointer-to-column array length 
  int nelms_; //!< current number of columns inserted into the set as its elements (i.e. number of non-null pointers to columns)
  mutable bool isreversed_, isreversed_def_; //!< flag of whether the set's original layout or insertion ordering is reversed
  int nvecs_; //!< common number of vector instances for inserted columns
  mutable datvector curvec_; //!< manipulator of the current vector instance
  mutable int curvec_idx_; //!< index of the current vector instance to which the manipulator has been assigned
  bool issmoothed_; //!< flag of whether the set's data distribution is smoothed
  int nsglparts_; //!< maximal number of parts at the single domains of inserted columns

////! evaluate and return two representations for number of parts at the set:

  //! Vector Representation For Set Number of Parts
  //! \details This is tuple of numbers of parts 
  //! at successive single domains of inserted columns.
  class npartstuple_proxy_ {
    const datset &that_; //!< manipulated object reference
   public:
    npartstuple_proxy_(const datset &that): that_(that) {}
    bool operator()(inttuple &ret) const;
  };
  friend class npartstuple_proxy_;
  
  //! Scalar Representation For Set Number of Parts
  //! \details This is total number of partition 
  //! cells at the multi-dimensional domain for 
  //! all inserted columns.
  class totalnparts_proxy_ {
    const datset &that_; //!< manipulated object reference
   public:
    totalnparts_proxy_(const datset &that): that_(that) {}
    bool operator()(int &ret) const;
  };
  friend class totalnparts_proxy_;

  //! type of record for set number of parts:
  typedef inttuple::map<totalnparts_proxy_,npartstuple_proxy_> nparts_rec_;
  //! type of returned value for set number of parts:
  typedef nparts_rec_::ret_t nparts_ret_;
  //! field of representation for set number of parts:
  nparts_rec_ nparts_rep_;
  
//! return the k-th element column inserted into this set counting only non-null pointers to columns in the original layout:
  datcolumn &orgelms_(int k) const;
  
//! return the k-th element column inserted into this set according with the insertion ordering:
  datcolumn &odrelms_(int k) const;
  
//! make a given vector instance as current and assign it to its manipulator:
  void setup_curvec_(int i) const;
  
 public:
/// create a tagged set object with card<=supercard tagged elements as columns containing ninsts data instances:

  datset(const string &otag,const string &etag,int supercard,int card,int ninsts);

/// create a tagged set object as empty duplicate of a given set object:

  datset(const string &otag,const datset &s);

/// create a set object as full duplicate of a given set object:

  datset(const datset &s);

/// destroy the set object:

  ~datset();

/// copy a given set object:

  datset &operator=(const datset &s);

/// reinitiate the set object for successive processing runs:

  void reinitiate();
  
/// do hold the set object:

  bool do_hold() const
  { nlinks_++; return nlinks_==1; }
  
/// do release the set object:

  bool do_release() const
  { nlinks_--; return nlinks_<1; }
  
/// return short name of the set object:

  const string &tag() const
  { return objtag_; }

/// return full name of the set object:

  const string &name() const
  { return objtag_+" set of "+elmtag_+" attributes"; }

/// return cardinality of a maximal superset containing this set:

  int supercard() const  //!< \post supercard>0
  { return nptrcols_; }

/// return cardinal number of elements inserted into the set:

  int card() const //! \post 0 <= card <= supercard
  { return nelms_; }

/// test whether the set contains no elements:

  bool isempty() const
  { return nelms_<=0; }    

/// set up flag for reversing any layout of elements in the set:

  bool reversion(bool rev)
  { bool revo=isreversed_; isreversed_def_=isreversed_=rev; return revo; }

/// set up flag for reversing any layout of elements in the set:

  datset &operator~()
  { isreversed_def_=isreversed_; isreversed_=!isreversed_; return *this; }
  
/// set up flag for reversing any layout of elements in the set:

  const datset &operator~() const
  { isreversed_def_=isreversed_; isreversed_=!isreversed_; return *this; }
  
/// return flag for reversing any layout of elements in the set:

  bool isreversed() const
  { return isreversed_; }

/// return an attribute as elemental column of the set giving its ordinal number according with the original layout:

  datattr &operator()(int k) //! \pre card>0 , 0 <= k < card
  { return orgelms_(k); }

/// return an attribute as elemental column of the set giving its ordinal number according with the original layout:

  const datattr &operator()(int k) const //! \pre card>0 , 0 <= k < card
  { return orgelms_(k); }

/// return an attribute as first elemental column in the set according with the original layout:

  datattr &operator()() //! \pre card>0
  { return orgelms_(0); }

/// return an attribute as first elemental column in the set according with the original layout:

  const datattr &operator()() const //! \pre card>0
  { return orgelms_(0); }

/// return an element of the set according with the layout of insertion ordering:

  datattr &order(int k=0) //! \pre card>0 , 0 <= k < card
  { return odrelms_(k); }

/// return an element of the set according with the layout of insertion ordering:

  const datattr &order(int k=0) const //! \pre card>0 , 0 <= k < card
  { return odrelms_(k); }

/// perform this relational operation between any set and a given subset:

  bool operator==(const subdatset &s) const;

/// perform this relational operation between any set and a given subset:

  bool operator!=(const subdatset &s) const
  { return !((*this)==s); }

/// perform this relational operation between any set and a given set:

  bool operator==(const datset &s) const;

/// perform this relational operation between any set and a given set:

  bool operator!=(const datset &s) const
  { return !((*this)==s); }

/// remove columns by performing this set-operation between any set and a given subset:

  void operator-=(const subdatset &s);

/// insert columns by performing this set-operation between any set and a given subset:

  void operator+=(const subdatset &s);

/// insert/remove columns by performing this set-operation between any set and a given subset:

  void operator*=(const subdatset &s);

/// remove columns by performing this set-operation between any set and a given set:

  void operator-=(const datset &s);

/// insert columns by performing this set-operation between any set and a given set:

  void operator+=(const datset &s);

/// insert/remove columns by performing this set-operation between any set and a given set:

  void operator*=(const datset &s);

/// move all columns from a given set to this set:

  void operator<<=(datset &s);
//@{
/// shortcut operations using the literal Nil:

  bool operator==(nil_literal)
  { return isempty(); }
  
  bool operator!=(nil_literal)
  { return !isempty(); }
  
  void operator=(nil_literal)
  { (*this)-=(*this); }
  
  void operator-=(nil_literal)
  {}
  
  void operator+=(nil_literal)
  {}
  
  void operator*=(nil_literal)
  { (*this)-=(*this); }
//@}
/// set up weight coefficient for all column attributes inserted into the set to perform weighted ordering:

  void weight(real weg); //!< \pre 0 <= weg < rMax || weg==noweight
  
/// set up weight coefficient for all column attributes inserted into the set to perform weighted ordering:

  void operator|=(real weg);
  
/// set up weight coefficient for all column attributes inserted into the set to perform weighted ordering:

  datset &operator|(real weg) 
  { (*this)|=weg; return *this; }
  
/// set up weight coefficient for all column attributes inserted into the set to perform weighted ordering:

  const datset &operator|(real weg) const 
  { const_cast<datset &>(*this)|=weg; return *this; }
  
/// return weight coefficient for all column attributes inserted into the set to perform weighted ordering:

  real weight() const; //!< \post weight == <odrelms[:].weight()>

/// return number of data vector instances at the set:

  int ninsts() const //! \post ninsts>0
  { return nvecs_; }

/// return the i-th data vector instance of the set:

  datvector &operator[](int i) //! \pre 0 <= i < ninsts
  { 
   if (i!=curvec_idx_) setup_curvec_(i);
   return curvec_;
  }
   
/// return the i-th data vector instance of the set:

  const datvector &operator[](int i) const //! \pre 0 <= i < ninsts
  { 
   if (i!=curvec_idx_) setup_curvec_(i);
   return curvec_;
  }
  
/// test whether the set's data distribution is smoothed:
//!
//! \note If at least one column is smoothed
//! the whole set is also treated as
//! smoothed. Otherwise if there are
//! no smoothed columns the set will 
//! be finest as whole.
//!
  bool issmoothed() const
  { return issmoothed_; }

/// test whether the set's data distribution is finest:

  bool isfinest() const
  { return !issmoothed_; }

/// set up data precision for the set's columns:

  void precision(real prec,real r_prec); //!< \pre rPrec < prec < 1 , r_prec==1/prec

/// perform data domain partitioning for the set's columns:

  int partition(const partmeth_t &meth);

/// return maximal number of parts at the single domains of the set's columns:

  int nsglparts() const //! \post nsglparts>=2
  { return nsglparts_; }

/// return number of partition cells at the multi-dimensional domain for the set's columns:

  nparts_ret_ nparts() const //! \post nparts == nsglparts^card || nparts == inttuple(card,nsglparts)
  { return nparts_rep_.get(); }

/// print info about statistics of the set to a given output stream:

  void print_stats(ostream &out,bool ownonly=false) const;

/// print info about ordering of the set to a given output stream:

  void print_order(ostream &out,bool ownonly=false) const;

/// print info about contents of the set to a given output stream:

  void print_conts(ostream &out,bool ownonly=false) const;

/// print info to output stream:

  friend ostream &operator<<(ostream &out,const datset &s)
  {
   bool isord=iosex::isordered(out);
   if (isord) s.print_order(out);
   else s.print_conts(out);
   return out;
  }
};   

typedef datset set; //!< alias name, used outside the namespace

} // end of Data namespace

} // end of InfoSelCore namespace

#endif // end of DSET_H_ header file

