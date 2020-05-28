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
/// Data Vector At Input Data Structures
/**
 * @file       dvec.h
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

#ifndef DVEC_H_
#define DVEC_H_

namespace InfoSelCore {
 
namespace Data {

/////////////////////////////
///
/// Input Data File Vector 
///
/// This auxiliary type is used for 
/// representing and manipulating 
/// whole data vector instance of 
/// studied attribute set.
/// This type is designed for 
/// temporary manipulator-like 
/// objects.
///

class datvector {
  friend class datset;
  friend class subdatset;
  friend class datfile;
 private:
  datum **orgcmps_; //!< array of non-null vector components being data instances of columns selected at their set according with the original layout
  int ncmps_; //!< component array length
  int nsglparts_; //!< maximal number of parts at the single domains of selected columns
  
////! evaluate and return two representations of value instance for the vector:

  //! Vector Representation For Vector Value
  //! \details This is tuple of instant values 
  //! for all selected columns at the data vector. 
  class valtuple_proxy_ {
    const datvector &that_; //!< manipulated object reference
   public:
    valtuple_proxy_(const datvector &that): that_(that) {}
    bool operator()(realtuple &ret) const;
  };
  friend class valtuple_proxy_;
  
  //! Scalar Representation For Vector Value
  //! \details This is total summative value 
  //! for instant values of all selected columns 
  //! at the data vector. 
  class totalval_proxy_ {
    const datvector &that_; //!< manipulated object reference
   public:
    totalval_proxy_(const datvector &that): that_(that) {}
    bool operator()(real &ret) const;
  };
  friend class totalval_proxy_;

  //! type of record for vector value:
  typedef realtuple::map<totalval_proxy_,valtuple_proxy_> val_rec_;
  //! type of returned value for vector value:
  typedef val_rec_::ret_t val_ret_;
  //! field of representation for vector value:
  val_rec_ val_rep_;
  
////! evaluate and return two representations of partition indexing for the vector:

  //! Vector Representation For Vector Part
  //! \details This is tuple of single partition 
  //! indices for all selected columns at the data 
  //! vector.
  class parttuple_proxy_ {
    const datvector &that_; //!< manipulated object reference
   public:
    parttuple_proxy_(const datvector &that): that_(that) {}
    bool operator()(inttuple &ret) const;
  };
  friend class parttuple_proxy_;
  
  //! Scalar Representation For Vector Part
  //! \details This is total linear index of 
  //! a partition cell at the multi-dimensional 
  //! domain for all selected columns at the data 
  //! vector.
  class totalpart_proxy_ {
    const datvector &that_; //!< manipulated object reference
   public:
    totalpart_proxy_(const datvector &that): that_(that) {}
    bool operator()(int &ret) const;
  };
  friend class totalpart_proxy_;

  //! type of record for vector part:
  typedef inttuple::map<totalpart_proxy_,parttuple_proxy_> part_rec_;
  //! type of returned value for vector part:
  typedef part_rec_::ret_t part_ret_;
  //! field of representation for vector part:
  part_rec_ part_rep_;

//! create and set up empty structure for containing non-null components of the vector:
  void setup_orgcmps_(int ncmps,int nsglparts);
  
//! read all component instant values of the vector from a given input stream:
  void read_val_(istream &in);
//@{
 private: //! <b> Not implemented/allowed. </b>
  datvector(datvector &);
  void operator=(datvector &);
//@}
 public:
/// create a vector object:

  datvector();

/// destroy the vector object:

  ~datvector();

/// return number of components of the vector:
  
  int size() const //! \post size>=0
  { return ncmps_; }

/// return componental datum of the vector giving its ordinal number:

  datum &operator()(int k) //! \pre size>0 , 0 <= k < size
  { return *orgcmps_[k]; }

/// return componental datum of the vector giving its ordinal number:

  const datum &operator()(int k) const //! \pre size>0 , 0 <= k < size
  { return *orgcmps_[k]; }

/// return first componental datum of the vector:

  datum &operator()() //! \pre size>0
  { return *orgcmps_[0]; }

/// return first componental datum of the vector:

  const datum &operator()() const //! \pre size>0
  { return *orgcmps_[0]; }

/// return values or their total summative value of componental data for the vector:

  val_ret_ val() const //! \post val == sum(orgcmps[:].val) || val == (orgcmps[0].val,...,orgcmps[size-1].val)
  { return val_rep_.get(); }

/// return indices or their total linear index of partition cell at the multi-dimensional domain for the vector:
//!
//! \note A finit-bits integer variable 
//! is used to store the value of total index 
//! of multi-dimesional partition cell, so in 
//! the case of numeric overflow an exception 
//! will be thrown; the value of total index
//! is designated based on the maximal number 
//! of parts 'nsglparts' among the single data 
//! domains rather than the individual numbers 
//! of parts for consecutive domains.  
//!
  part_ret_ part() const //! \post 0 <= part < nsglparts^size || part == (orgcmps[0].part,...,orgcmps[size-1].part)
  { return part_rep_.get(); }
    
/// implicit integer conversions for the vector object:

  operator int() const
  { return part_rep_.getdst(); }
    
/// implicit integer-tuple conversions for the vector object:

  operator const inttuple &() const
  { return part_rep_.getsrc(); }

/// implicit real conversions for the vector object:

  operator real() const
  { return val_rep_.getdst(); }

/// implicit real-tuple conversions for the vector object:

  operator const realtuple &() const
  { return val_rep_.getsrc(); }

/// print info about contents of the vector to a given output stream:

  void print_conts(ostream &out) const;

/// print info to output stream:

  friend ostream &operator<<(ostream &out,const datvector &v)
  {
   v.print_conts(out);
   return out;
  }
};

typedef datvector instance; //!< alias name, used outside the namespace

} // end of Data namespace

} // end of InfoSelCore namespace

#endif // end of DVEC_H_ header file

