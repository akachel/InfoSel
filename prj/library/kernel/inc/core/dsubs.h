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
/// Data Subset At Input Data Structures
/**
 * @file       dsubs.h
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

#ifndef DSUBS_H_
#define DSUBS_H_

namespace InfoSelCore {
 
namespace Data {

/////////////////////////////
///
/// Input Data Set Subset
///
/// This auxiliary type is used 
/// for representing and iterating 
/// all logical subsets of input 
/// data set at its space.
/// This type is designed for 
/// temporary manipulator-like 
/// objects.
///

class subdatset {
  mutable int nlinks_; //!< number of holding links to the subset object
  datset *refset_; //!< pointer to the referred set containing this subset
  datspace *refspace_; //!< pointer to the referred space including this subset 
  datcolumn **orgcols_; //!< array of pointers to columns in the referred set included by the subset with the original layout of input file
  datcolumn **odrcols_; //!< array of pointers to above columns with the insertion ordering in the referred set
  int nelms_; //!< number of columns included by the subset as its elements
  intcomb curloc_; //!< current combination of indices or ordinal numbers of columns in the subset as its location inside the referred space
  mutable datvector curvec_; //!< manipulator of the current vector instance
  mutable int curvec_idx_; //!< index of the current vector instance to which the manipulator has been assigned
  bool issmoothed_; //!< flag of whether the subset's data distribution is smoothed

////! evaluate and return two representations of identification of the subset:

  //! Vector Representation For Subset Identification
  //! \details This is tuple of indices of included 
  //! columns at current combination.
  class idntuple_proxy_ {
    const subdatset &that_; //!< manipulated object reference
   public:
    idntuple_proxy_(const subdatset &that): that_(that) {}
    bool operator()(inttuple &ret) const;
  };
  friend class idntuple_proxy_;
  
  //! Scalar Representation For Subset Identification
  //! \details This is total overall index of 
  //! current combination of included column.
  class totalidn_proxy_ {
    const subdatset &that_; //!< manipulated object reference
   public:
    totalidn_proxy_(const subdatset &that): that_(that) {}
    bool operator()(int &ret) const;
  };
  friend class totalidn_proxy_;
  
  //! type of record for subset identification:
  typedef inttuple::map<totalidn_proxy_,idntuple_proxy_> idn_rec_;
  //! type of returned value for subset identification:
  typedef idn_rec_::ret_t idn_ret_;
  //! field of representation for subset identification:
  idn_rec_ idn_rep_;

//! return zero-based index of the k-th element column included by this subset corresponding to the original layout:
  int orgidx_(int k) const
  { return odrcols_!=0 ? odrcols_[curloc_[k]]->orgidx_ : curloc_[k]; }
  
//! return the k-th element column included by this subset according with the original layout in the referred set:
  datcolumn &orgelms_(int k) const;
  
//! return the k-th element column included by this subset according with the insertion ordering in the referred set:
  datcolumn &odrelms_(int k) const;
  
//! preset a given range this subset to a new current location by iterating through all non-null tuples of columns of the referred set:
  bool preset_curloc_(const intcomb &bound1,const intcomb &bound2); 

//! postset a given range this subset to a new current location by iterating through all non-null tuples of columns of the referred set:
  void postset_curloc_(const intcomb &bound1,const intcomb &bound2); 

//! move in a given range this subset to a new current location by iterating through all non-null tuples of columns of the referred set:
  void move_curloc_(const intcomb &bound1,const intcomb &bound2,int inc); 
        
//! make a given vector instance as current and assign it to its manipulator:
  void setup_curvec_(int i) const;
  
 public:
/// create a null subset object:

  subdatset();

/// create a subset object as the nil in a given space, optionally forcing no ordering layout:
 
  subdatset(const datspace &s,nil_literal,bool noord=false);

/// create a subset object as the first non-empty subset in a given space, optionally forcing no ordering layout:
 
  explicit subdatset(const datspace &s,bool noord=false);

/// create a subset object as the last non-empty subset in a given space, optionally forcing no ordering layout:
 
  subdatset(const datspace &s,last_literal,bool noord=false);

/// create a subset object as duplicate of a given subset object:

  subdatset(const subdatset &s);

/// destroy the subset object:

  ~subdatset();

/// copy a given subset object:
  
  subdatset &operator=(const subdatset &s);

/// do hold the subset object:

  bool do_hold() const
  { nlinks_++; return nlinks_==1; }
  
/// do release the subset object:

  bool do_release() const
  { nlinks_--; return nlinks_<1; }
  
/// return identifical number of the subset object related to the original layout in the referred set:

  idn_ret_ idn() const //! \pre !isempty  \post 0 <= idn < refspace.card || idn == (orgelms[0].idn,...,orgelms[card-1].idn)
  { return idn_rep_.get(); }
  
/// implicit integer conversions:

  operator int() const 
  { return idn_rep_.getdst(); }
  
/// implicit integer-tuple conversions:

  operator const inttuple &() const 
  { return idn_rep_.getsrc(); }
  
/// return cardinal number of elements included by the subset:

  int card() const //! \post card>=0
  { return refspace_!=0 && !curloc_.isnil() ? nelms_ : 0; }

/// test whether the subset contains no elements:

  bool isempty() const //! \post !isempty <=> !isnull && !isnil
  { return refspace_==0 || curloc_.isnil(); }    

/// test whether the layout of the subset and its space is reversed:

  bool isreversed() const
  { return refspace_!=0 && refset_->isreversed_; }

/// test whether the layout of the subset and its space is ordered according with the insertion ordering in the referred set:

  bool isordered() const
  { return odrcols_!=0; }

/// return an attribute as elemental column of the subset giving its ordinal number according with the original layout:

  datattr &operator()(int k) //! \pre card>0 , 0 <= k < card
  { return orgelms_(k); }

/// return an attribute as elemental column of the subset giving its ordinal number according with the original layout:

  const datattr &operator()(int k) const //! \pre card>0 , 0 <= k < card
  { return orgelms_(k); }

/// return an attribute as first elemental column in the subset according with the original layout:

  datattr &operator()() //! \pre card>0
  { return orgelms_(0); }

/// return an attribute as first elemental column in the subset according with the original layout:

  const datattr &operator()() const //! \pre card>0
  { return orgelms_(0); }

/// return an element of the subset according with the layout of insertion ordering:

  datattr &order(int k=0) //! \pre card>0 , 0 <= k < card
  { return odrelms_(k); }

/// return an element of the subset according with the layout of insertion ordering:

  const datattr &order(int k=0) const //! \pre card>0 , 0 <= k < card
  { return odrelms_(k); }

/// test whether the subset is empty and outside any space:

  bool isnull() const //! \post isnull => isempty , !isnull <=> refspace!=0 <=> refset!=0
  { return refspace_==0; }    

/// test whether the non-null subset is empty and equal to the nil of the space:

  bool isnil() const //! \post isnil => isempty
  { return refspace_!=0 && curloc_.isnil(); }    

/// test whether the non-null subset is equal to the minimum/maximum of the space:

  bool ismin() const
  { return refspace_!=0 && refset_->isreversed_ ? curloc_.ismax() : curloc_.ismin(); }    

/// test whether the non-null subset is equal to the minimum/maximum of the space:

  bool ismax() const
  { return refspace_!=0 && refset_->isreversed_ ? curloc_.ismin() : curloc_.ismax(); }    

/// make the non-null subset as the empty nil inside the space:

  void asnil();
  
/// make the non-null subset as the first non-empty subset inside the space:

  void asfirst();

/// make the non-null subset as the last non-empty subset inside the space:

  void aslast();
  
/// make the non-null subset as the previous non-empty subset inside the space:

  void operator--() //! \pre !isnull
  { 
   if (refset_->isreversed_)
    move_curloc_(refspace_->infloc_,refspace_->suploc_,+1);
   else
    move_curloc_(refspace_->suploc_,refspace_->infloc_,-1);
  }

/// make the non-null subset as the behind non-empty subset inside the space:

  void operator-=(int inc) //! \pre !isnull
  { 
   if (refset_->isreversed_)
    move_curloc_(refspace_->infloc_,refspace_->suploc_,+abs(inc));
   else
    move_curloc_(refspace_->suploc_,refspace_->infloc_,-abs(inc));
  }

/// make the non-null subset as the next non-empty subset inside the space:

  void operator++() //! \pre !isnull
  { 
   if (refset_->isreversed_)
    move_curloc_(refspace_->suploc_,refspace_->infloc_,-1);
   else
    move_curloc_(refspace_->infloc_,refspace_->suploc_,+1);
  }

/// make the non-null subset as the after non-empty subset inside the space:

  void operator+=(int inc) //! \pre !isnull
  { 
   if (refset_->isreversed_)
    move_curloc_(refspace_->suploc_,refspace_->infloc_,-abs(inc));
   else
    move_curloc_(refspace_->infloc_,refspace_->suploc_,+abs(inc));
  }

/// perform this relational operation between any subset and a given subset:

  bool operator==(const subdatset &s) const;

/// perform this relational operation between any subset and a given subset:

  bool operator!=(const subdatset &s) const
  { return !((*this)==s); }
  
/// perform this relational operation between any set and a given subset:

  friend bool datset::operator==(const subdatset &s) const;

/// perform this relational operation between any set and a given subset:

  friend bool datset::operator!=(const subdatset &s) const;

/// insert columns by performing this set-operation between any set and a given subset:

  friend void datset::operator-=(const subdatset &s);

/// remove columns by performing this set-operation between any set and a given subset:

  friend void datset::operator+=(const subdatset &s);

/// insert/remove columns by performing this set-operation between any set and a given subset:

  friend void datset::operator*=(const subdatset &s);
//@{
/// shortcut operations using the literal Nil:

  bool operator==(nil_literal)
  { return isnil(); }

  bool operator!=(nil_literal)
  { return !isnil(); }
  
  void operator=(nil_literal)
  { asnil(); }
//@}
//@{
/// shortcut operations using the literals First:

  bool operator==(first_literal)
  { return ismin(); }
  
  bool operator!=(first_literal)
  { return !ismin(); }
  
  void operator=(first_literal)
  { asfirst(); }
//@}
//@{
/// shortcut operations using the literals Last:

  bool operator==(last_literal)
  { return ismax(); }

  bool operator!=(last_literal)
  { return !ismax(); }

  void operator=(last_literal)
  { aslast(); }
//@}
/// set up weight coefficient for all column attributes included by the subset to perform weighted ordering:

  void weight(real weg); //!< \pre 0 <= weg < rMax || weg==noweight

/// set up weight coefficient for all column attributes included by the subset to perform weighted ordering:

  void operator|=(real weg);
  
/// set up weight coefficient for all column attributes included by the subset to perform weighted ordering:

  subdatset &operator|(real weg) 
  { (*this)|=weg; return *this; }
  
/// set up weight coefficient for all column attributes included by the subset to perform weighted ordering:

  const subdatset &operator|(real weg) const 
  { const_cast<subdatset &>(*this)|=weg; return *this; }
  
/// return weight coefficient for all column attributes included by the subset to perform weighted ordering:

  real weight() const; //!< \post weight == <odrelms[:].weight()>

/// return number of data vector instances at the referred set:

  int ninsts() const //! \pre !isnull  \post ninsts>0
  { return refset_->nvecs_; }

/// return the i-th data vector instance of the subset:

  datvector &operator[](int i) //! \pre !isempty , 0 <= i < ninsts
  { 
   if (i!=curvec_idx_) setup_curvec_(i);
   return curvec_;
  }
   
/// return the i-th data vector instance of the subset:

  const datvector &operator[](int i) const //! \pre !isempty , 0 <= i < ninsts
  { 
   if (i!=curvec_idx_) setup_curvec_(i);
   return curvec_;
  }
   
/// test whether the subset's data distribution is smoothed:
//!
//! \note If at least one column is smoothed
//! the whole subset is also treated as
//! smoothed. Otherwise if there are
//! no smoothed columns the subset will 
//! be finest as whole.
//!
  bool issmoothed() const
  { return issmoothed_; }

/// test whether the subset's data distribution is finest:

  bool isfinest() const
  { return !issmoothed_; }

/// return maximal number of parts at the single domains of the referred set:

  int nsglparts() const //! \pre !isnull
  { return refset_->nsglparts_; }

/// return number of partition cells at the multi-dimensional domain for the subset's columns:

  datspace::nparts_ret_ nparts() const //! \pre !isnull  \post !isempty => (nparts == nsglparts^card || nparts == inttuple(card,nsglparts))
  { return refspace_->nparts_rep_.get(); }

/// print info about contents of the subset to a given output stream:

  void print_conts(ostream &out) const;

/// print info to output stream:

  friend ostream &operator<<(ostream &out,const subdatset &s)
  {
   s.print_conts(out);
   return out;
  }
};

typedef subdatset subset; //!< alias name, used outside the namespace

} // end of Data namespace

} // end of InfoSelAPI namespace

#endif // end of DSUBS_H_ header file

