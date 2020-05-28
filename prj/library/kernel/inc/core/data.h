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
/// Input Data Structures
/**
 * @file       data.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains structures for storing 
 *             and preprocessing studied input data processed by 
 *             selection algorithms. 
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef DATA_H_
#define DATA_H_

namespace InfoSelCore {

namespace Data /** \copybrief data.h \copydetails data.h */ { // namespace head

/////////////////////////////
///
/// Data Specific Errors
///

struct xDataError : xError { 
 string dnam;
 xDataError(const string &dn="") throw(): dnam(dn) {}
 ~xDataError() throw() {}
 private: void printmsg_(ostream &out) const; 
 private: virtual void print_msg_(ostream &out) const = 0;
};

/// Error Of One-Unique-Value Distribution
struct xOneUniqDistrb : xDataError { xOneUniqDistrb(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Number Unique Distribution Required
struct xNumUniqDistrbReq : xDataError { int numu; xNumUniqDistrbReq(int nu,const string &dn=""): numu(nu), xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of No Partition Method
struct xNoPartMeth : xDataError { xNoPartMeth(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Column Link Failed
struct xColumnLinkFail : xDataError { xColumnLinkFail(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Set Link Failed
struct xSetLinkFail : xDataError { xSetLinkFail(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Space Link Failed
struct xSpaceLinkFail : xDataError { xSpaceLinkFail(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Subset Link Failed
struct xSubsetLinkFail : xDataError { xSubsetLinkFail(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Space Cardinalty Overflow
struct xSpaceCardOverflow : xDataError { xSpaceCardOverflow(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Subset Identification Overflow
struct xSubsetIdnOverflow : xDataError { xSubsetIdnOverflow(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Domain Number-Of-Partitions Overflow
struct xDomainNPartsOverflow : xDataError { xDomainNPartsOverflow(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Domain Partition Overflow
struct xDomainPartOverflow : xDataError { xDomainPartOverflow(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Too Many Intermediate Sets
struct xTooManyImdSets : xDataError { xTooManyImdSets(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of Too Many Auxiliary Sets
struct xTooManyAuxSets : xDataError { xTooManyAuxSets(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };
/// Error Of File Processing Failed
struct xFileProcFail : xDataError { xFileProcFail(const string &dn=""): xDataError(dn) {} private: void print_msg_(ostream &out) const; };

/////////////////////////////
////
//// All Main Data Structures
////

class datum;
class datcolumn;
class datvector;
class datset;
class datspace;
class subdatset;
class datfile;

/////////////////////////////
///
/// Input Data File Item
/// 
/// It represents any single 
/// data instance of studied 
/// attributes.
///

class datum {
  friend class datcolumn;
  friend class datvector;
  friend class datfile;
 private:
  real val_; //!< a real value, read directly from the input numerical-valued file 
  int part_; //!< a zero-based integer index, given as a result of value domain partitioning
  realtuple val_tup_; //!< real implicit conversion support
  inttuple part_tup_; //!< integer implicit conversion support

//! set up the datum's instant value from a real number domain:
  void setup_val_(real v)
  { val_=v; *val_tup_=v; }
  
//! read the datum's instant value from a given input stream:
  void read_val_(istream &in);
  
//! set up the datum's part index at a partition of the value domain:
  void setup_part_(int p) //! \pre p>=0
  { part_=p; *part_tup_=p; }
//@{
 private: //! <b> Not implemented/allowed. </b> 
  datum(datum &);
  void operator=(datum &);
//@}
 public:
/// create a zeroised datum object:

  datum();

/// destroy the datum object:

  ~datum();

/// return the datum's instant value from a real number domain:
//!
//! \note Input data values 
//! must be of real type only
//! including integer sub-type,
//! no support for another value
//! types is provided.
//!
  real val() const
  { return val_; }

/// return the datum's part index at a partition of the value domain:

  int part() const //! \post part>=-1
  { return part_; }

/// implicit integer conversions:

  operator int() const
  { return part_; }

/// implicit integer-tuple conversions:

  operator const inttuple &() const
  { return part_tup_; }

/// implicit real conversions:

  operator real() const
  { return val_; }

/// implicit real-tuple conversions:

  operator const realtuple &() const
  { return val_tup_; }
     
/// print info about instance of the datum to a given output stream:

  void print_instn(ostream &out) const;

/// print info to output stream:

  friend ostream &operator<<(ostream &out,const datum &d)
  {
   d.print_instn(out);
   return out;
  }
};

typedef datum item; //!< alias name, used outside the namespace

} // end of Data namespace

} // end of InfoSelCore namespace

#endif // end of DATA_H_ header file

