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
/// Data Space At Input Data Structures
/**
 * @file       dspac.h
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

#ifndef DSPAC_H_
#define DSPAC_H_

namespace InfoSelCore {
 
namespace Data {

/////////////////////////////
///
/// Input Data Set Space
///
/// This auxiliary type is used for 
/// representing and manipulating 
/// logical space being family of 
/// all equinumerous subsets of 
/// input data set. 
/// This type is designed for 
/// temporary manipulator-like 
/// objects.
//!
//! \note Any space is itself a subset 
//! of the power set over the input set.
//!

class datspace {
  friend class subdatset;
 private:
  mutable int nlinks_; //!< number of holding links to the space object
  datset *refset_; //!< non-null pointer to the referred set
  int nsubs_; //!< number of all subsets in the space
  int nsubelms_; //!< fixed positive number of columns at each subset in the space
  bool isordered_; //!< flag of whether the space's subset layout is ordered with relation to the insertion ordering at the referred set 
  intcomb nilloc_, infloc_, minloc_, maxloc_, suploc_; //!< auxiliary storage for selected location of subsets in the space

////! evaluate and return two representations of cardinality of the space:

  //! Vector Representation For Space Cardinality
  //! \details This is tuple of maximal numbers 
  //! of columns at the referred set.
  class cardtuple_proxy_ {
    const datspace &that_; //!< manipulated object reference
   public:
    cardtuple_proxy_(const datspace &that): that_(that) {}
    bool operator()(inttuple &ret) const;
  };
  friend class cardtuple_proxy_;
  
  //! Scalar Representation For Space Cardinality
  //! \details This is total number of column combinations.
  class totalcard_proxy_ {
    const datspace &that_; //!< manipulated object reference
   public:
    totalcard_proxy_(const datspace &that): that_(that) {}
    bool operator()(int &ret) const;
  };
  friend class totalcard_proxy_;
  
  //! type of record for space cardinality:
  typedef inttuple::map<totalcard_proxy_,cardtuple_proxy_> card_rec_;
  //! type of returned value for space cardinality:
  typedef card_rec_::ret_t card_ret_;
  //! field of representation for space cardinality:
  card_rec_ card_rep_;

////! evaluate and return two representations for number of parts at the space:

  //! Vector Representation For Space Number of Parts
  //! \details This is tuple of numbers of parts 
  //! at the single domains for successive columns.
  class npartstuple_proxy_ {
    const datspace &that_; //!< manipulated object reference
   public:
    npartstuple_proxy_(const datspace &that): that_(that) {}
    bool operator()(inttuple &ret) const;
  };
  friend class npartstuple_proxy_;
  
  //! Scalar Representation For Space Number of Parts
  //! \details This is total number of partition 
  //! cells at the multi-dimensional domain for 
  //! successive columns.
  class totalnparts_proxy_ {
    const datspace &that_; //!< manipulated object reference
   public:
    totalnparts_proxy_(const datspace &that): that_(that) {}
    bool operator()(int &ret) const;
  };
  friend class totalnparts_proxy_;

  //! type of record for space number of parts:
  typedef inttuple::map<totalnparts_proxy_,npartstuple_proxy_> nparts_rec_;
  //! type of returned value for space number of parts:
  typedef nparts_rec_::ret_t nparts_ret_;
  //! field of representation for space number of parts:
  nparts_rec_ nparts_rep_;
  
//! maximal number of columns at the referred set:
  int nsuperelms_() const 
  { return refset_->nptrcols_; }
//@{
 private: //! <b> Not implemented/allowed. </b> 
  datspace(datspace &);
  void operator=(datspace &);
//@}
 public:
/// create a space object of all subsets containing subcard columns of a given set:

  datspace(const datset &s,int subcard);

/// destroy the space object:

  ~datspace();
  
/// do hold the space object:

  bool do_hold() const
  { nlinks_++; return nlinks_==1; }
  
/// do release the space object:

  bool do_release() const
  { nlinks_--; return nlinks_<1; }
  
/// return cardinal number of subsets in the space:

  card_ret_ card() const //! \post card == card(C(refset.supercard,subcard)) || card == inttuple(subcard,refset.supercard)
  { return card_rep_.get(); }

/// return cardinality of any subset at the space:

  int subcard() const //! \post 0 < subcard <= refset.supercard
  { return nsubelms_; }

/// test whether the space layout is reversed:

  bool isreversed() const
  { return refset_->isreversed_; }

/// set up flag for ordering the space layout according with the insertion ordering in the referred set:

  bool ordering(bool ord)
  { bool ordo=isordered_; isordered_=ord; return ordo; }

/// return flag for ordering the space layout according with the insertion ordering in the referred set:

  bool isordered() const
  { return isordered_; }

/// perform this relational operation between any set and a given subset:

  friend bool datset::operator==(const subdatset &s) const;

/// perform this relational operation between any set and a given subset:

  friend bool datset::operator!=(const subdatset &s) const;

/// perform this set-operation between any set and a given subset:

  friend void datset::operator-=(const subdatset &s);

/// perform this set-operation between any set and a given subset:

  friend void datset::operator+=(const subdatset &s);

/// perform this set-operation between any set and a given subset:

  friend void datset::operator*=(const subdatset &s);

/// return number of data vector instances at the referred set:

  int ninsts() const //! \post ninsts>0
  { return refset_->nvecs_; }

/// test whether the referred set contains smoothed subsets:

  bool issmoothed() const
  { return refset_->issmoothed_; }

/// test whether the referred set contains finest subsets:

  bool isfinest() const
  { return !refset_->issmoothed_; }

/// return maximal number of parts at the single domains of the referred set:

  int nsglparts() const //! \post nsglparts>=2
  { return refset_->nsglparts_; }

/// return number of partition cells at the multi-dimensional domain for any subset's columns:

  nparts_ret_ nparts() const //! \post nparts == nsglparts^subcard || nparts == inttuple(subcard,nsglparts)
  { return nparts_rep_.get(); }

/// print info about contents of the space to a given output stream:

  void print_conts(ostream &out) const;

/// print info to output stream:

  friend ostream &operator<<(ostream &out,const datspace &s)
  {
   s.print_conts(out);
   return out;
  }
};

typedef datspace space; //!< alias name, used outside the namespace

} // end of Data namespace

} // end of InfoSelAPI namespace

#endif // end of DSPAC_H_ header file

