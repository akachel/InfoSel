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
/// Data Column At Input Data Structures
/**
 * @file       dcol.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.07.12
 *
 * @contents  
 *             This file contains declarations of input data structures.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef DCOL_H_
#define DCOL_H_

namespace InfoSelCore {

namespace Data {

/////////////////////////////
///
/// Static Set of 
/// Partition Methods
///

class PartMethSet {
//@{
 private: //! <b> Not implemented/allowed. </b> 
  PartMethSet();
  PartMethSet(PartMethSet &);
  void operator=(PartMethSet &);
//@}
 public:
//////  
/// Global Set of Predefined Method
///
  enum {                 
  //// methods leading to a resultant distribution, null or identical with the original one:
   pmNone=-1,   ///< no partitioning is performed, null/default case
   pmFineGrain, ///< finest-grained partition with one unique value of attribute per one part
   
  //// methods leading to a resultant distribution, smoothed relative to the original one:
   pmEquiWidth, ///< all parts are of equal size
   pmEquiUniqs, ///< number of unique values inside each part is equal 
   pmEquiFreqs, ///< sum of data frequencies inside each part is equal 
   pmMaxUniDif, ///< boundaries of each part are placed at maximal unique value difference
   pmMaxFrqDif, ///< boundaries of each part are placed at maximal data frequency difference
   
  /// number of non-null methods:
   pmCount
  };
//||||

/// return number of methods:

  static int count() 
  { return pmCount; }

//// browse the method set for its characteristics giving a method code:

  /// return method name:
  static string name(int code); //!< \pre (asserted) 0 <= code < count

  /// return method description:
  static string description(int code); //!< \pre (asserted) 0 <= code < count
};
  
/////////////////////////////
///
/// Descriptive Type of 
/// Partition Method 
///
/// It is used for partitioning attribute 
/// value domain into smaller parts or 
/// cells indexed by zero-Based integer 
/// numbers.  
///

class partmeth_t {
  int code_; //!< method code
  string pars_; //!< method parameters
 public: 
/// create a default method descriptor:

  partmeth_t(): code_(PartMethSet::pmNone)
  {}

/// create a given method descriptor:

  explicit partmeth_t(int code): code_(code)
  {}

/// create a given method descriptor with given parameters:

  partmeth_t(int code,const string &pars): code_(code), pars_(pars)
  {}
//@{
/// static operations for creating method descriptors:

  static partmeth_t create(int code,ostream &prompt,istream &pars);
  static partmeth_t FineGrain(const string &pars="") { return partmeth_t(PartMethSet::pmFineGrain,pars); }
  static partmeth_t EquiWidth(const string &pars="") { return partmeth_t(PartMethSet::pmEquiWidth,pars); }
  static partmeth_t EquiUniqs(const string &pars="") { return partmeth_t(PartMethSet::pmEquiUniqs,pars); }
  static partmeth_t EquiFreqs(const string &pars="") { return partmeth_t(PartMethSet::pmEquiFreqs,pars); }
  static partmeth_t MaxUniDif(const string &pars="") { return partmeth_t(PartMethSet::pmMaxUniDif,pars); }
  static partmeth_t MaxFrqDif(const string &pars="") { return partmeth_t(PartMethSet::pmMaxFrqDif,pars); }
//@}
/// return the method code:
  
  int code() const
  { return code_; }

/// return the method name:

  string name() const
  { return PartMethSet::name(code_); }

/// return the method parameters:

  const string &pars() const
  { return pars_; }
  
/// perform this relational operation for two descriptors:

  friend bool operator==(const partmeth_t &m1,const partmeth_t &m2)
  { return m1.code_==m2.code_ && m1.pars_==m2.pars_; }   

/// perform this relational operation for two descriptors:

  friend bool operator!=(const partmeth_t &m1,const partmeth_t &m2)
  { return !(m1==m2); }   
};  

/////////////////////////////
///
/// Input Data File Column
///
/// It represents any studied 
/// attribute and stores its 
/// data instances.
///

class datcolumn {
  friend class datset; 
  friend class subdatset;
  friend class datfile;
 private:
  mutable int nlinks_; //!< number of holding links to the column object
  string objtag_; //!< short name of the column object
  datset *ownset_; //!< pointer to data set owning the column attribute
  int orgidx_; //!< zero-based index of the column object corresponding to the original layout of input file
  inttuple orgidx_tup_; //!< integer implicit conversion support
  mutable real ordweg_, ordweg_def_; //!< weight coefficient of the column attribute for ordering any data set
  datum *data_; //!< array of input data objects
  int ndata_; //!< data array length
  bool issmoothed_; //!< flag of whether the column's data distribution is smoothed
  mutable real minval_, maxval_, meanval_, svaldev_; //!< statistics of original data distribution
  
//! Record of Data Uniques 
//! \details It is used for storing unique values 
//! and its occurrence frequency at the original 
//! data distribution. 
  struct unique_rec_ {
   typedef unique_rec_ rec; ///< type of record
   real val; ///< record value
   int frq; ///< record frequency 
   
  /// two-records comparision methods for decreasing order:
   static int decord(const void *x1,const void *x2);

  /// two-records comparision methods for increasing order:
   static int incord(const void *x1,const void *x2);
  };
  mutable unique_rec_ *uniques_; //!< array of unique records of the column
  mutable int nuniques_; //!< unique array length

  partmeth_t partmeth_; //!< descriptor of method for partitioning the column's data domain
  int nparts_; //!< cardinality of data domain partition
  real partnorm_; //!< norm of data domain partition
  
//! set up precision for all data values in the column: 
  void setup_prec_(real prec,real r_prec);

//! calculate statistics of data values in the column: 
  void calc_stats_() const;
  
//! set up and invoke partition method for the column's data domain:
  int setup_partmeth_(const partmeth_t &meth);
//@{
//! implementation of all partition methods, specific for datcolumn class:
  void none_partition_(istream &pars);
  void finegrain_partition_(istream &pars);
  void equiwidth_partition_(istream &pars); 
  void equiuniqs_partition_(istream &pars);
  void equifreqs_partition_(istream &pars);
  void maxunidif_partition_(istream &pars);
  void maxfrqdif_partition_(istream &pars);
//@}  
//@{
 private: //! <b> Not implemented/allowed. </b>
  datcolumn(datcolumn &);
  void operator=(datcolumn &);
//@}
 public:
  static const real noweight; ///< literal for no weighted ordering

/// create a tagged/identified column object containig ninsts data instances:

  datcolumn(const string &tag,int idn,int ninsts);

/// destroy the column object:

  ~datcolumn();

/// reinitiate the column object for successive processing runs:

  void reinitiate();
  
/// do hold the column object:

  bool do_hold() const
  { nlinks_++; return nlinks_==1; }
  
/// do release the column object:

  bool do_release() const
  { nlinks_--; return nlinks_<1; }
  
/// return short name of the column object:

  const string &tag() const
  { return objtag_; }

/// return full name of the column object:

  const string &name() const
  { return objtag_+" attribute"; }

/// return identifical number of the column object related to the original layout of its input file:

  int idn() const //! \post idn>=0
  { return orgidx_; }

/// implicit integer conversion:

  operator int() const
  { return orgidx_; }
  
/// implicit integer conversion:

  operator const inttuple &() const
  { return orgidx_tup_; }
  
/// set up weight coefficient of the column attribute to perform weighted ordering:

  real weight(real weg) //! \pre 0 <= weg < rMax || weg==noweight
  { real wego=ordweg_; ordweg_def_=ordweg_=weg; return wego; }

/// set up weight coefficient of the column attribute to perform weighted ordering:

  void operator|=(real weg)
  { ordweg_def_=ordweg_; ordweg_=weg; }

/// set up weight coefficient of the column attribute to perform weighted ordering:

  datcolumn &operator|(real weg)
  { ordweg_def_=ordweg_; ordweg_=weg; return *this; }

/// set up weight coefficient of the column attribute to perform weighted ordering:

  const datcolumn &operator|(real weg) const
  { ordweg_def_=ordweg_; ordweg_=weg; return *this; }

/// return weight coefficient of the column attribute to perform weighted ordering:

  real weight() const
  { return ordweg_; }

/// return number of data instances contained by the column:

  int ninsts() const //! \post ninsts>0
  { return ndata_; }

/// return the i-th data instance of the column:

  datum &operator[](int i) //! \pre 0 <= i < ninsts
  { return data_[i]; }

/// return the i-th data instance of the column:

  const datum &operator[](int i) const //! \pre 0 <= i < ninsts
  { return data_[i]; }

/// return the i-th instant data value and its part index:

  real insval(int i) const //! \pre 0 <= i < ninsts
  { return data_[i].val_; }

/// return the i-th instant data value and its part index:

  int inspart(int i) const //! \pre 0 <= i < ninsts
  { return data_[i].part_; }

/// test whether the column's data distribution is smoothed based on partition settings:

  bool issmoothed() const //! \post issmoothed <=> (nparts < nuniques)
  { return issmoothed_; }

/// test whether the column's data distribution is finest based on partition settings:

  bool isfinest() const //! \post isfinest <=> (nparts == nuniques)
  { return !issmoothed_; }

/// cut off all instant values of the column's data according with a given precision:

  void precision(real prec,real r_prec); //!< \pre rPrec < prec < 1 , r_prec==1/prec

//// return basic statistics of data values for the column:

  /// return minimal value among all data values:
  real minval() const
  { if (nuniques_<=0) calc_stats_(); return minval_; }

  /// return maximal value among all data values:
  real maxval() const
  { if (nuniques_<=0) calc_stats_(); return maxval_; }

  /// return mean value among all data values:
  real meanval() const
  { if (nuniques_<=0) calc_stats_(); return meanval_; }

  /// return standard deviation of all data values:
  real svaldev() const
  { if (nuniques_<=0) calc_stats_(); return svaldev_; }

/// return value range as the column's data domain:

  void range(real &min,real &max) const //! \post min<max
  { if (nuniques_<=0) calc_stats_(); min=minval_; max=maxval_; }

/// return number of unique data values occurring in the column:

  int nuniques() const //! \post nuniques>1
  { if (nuniques_<=0) calc_stats_(); return nuniques_; }

/// return successive unique values and its frequencies:

  void unique(int k,real &v,int &f) const //! \pre 0 <= k < nuniques
  { if (nuniques_<=0) calc_stats_(); const unique_rec_ &uni=uniques_[k]; v=uni.val; f=uni.frq; }

/// return successive unique values:

  real unival(int k) const //! \pre 0 <= k < nuniques
  { if (nuniques_<=0) calc_stats_(); return uniques_[k].val; }

/// return successive unique value frequencies:

  int unifrq(int k) const //! \pre 0 <= k < nuniques
  { if (nuniques_<=0) calc_stats_(); return uniques_[k].frq; }

//// calculate statistical function for data columns:

  /// return correlation coefficient of data values:
  friend real corrc(const datcolumn &c1,const datcolumn &c2);

  /// return f-score of data values:
  friend real fscor(const datcolumn &c1,const datcolumn &c2);

  /// return t-score of data values:
  friend real tscor(const datcolumn &c1,const datcolumn &c2);

/// perform partitioning of the column's data domain according with a given method:

  int partition(const partmeth_t &meth);
  
/// return current partitioning method:

  const partmeth_t &partmeth() const
  { return partmeth_; }
  
/// return number of parts in the partitioned column domain:

  int nparts() const //! \post 2 <= nparts <= nuniques
  { return nparts_; }

/// return norm of the column's domain partition:
//!
//! \note The norm is defined 
//! as average of all domain part 
//! sizes in the case of smoothed 
//! distribution or is defined as 
//! average of all nearest unique 
//! value differences in the case
//! of finest distribution. 
//!
  real partnorm() const //! \post partnorm>=0
  { return partnorm_; }

/// print info about statistics of the column to a given output stream:

  void print_stats(ostream &out) const;

/// print info about statistics of the column to a given output stream:

  void print_ident(ostream &out) const;

/// print info to output stream:

  friend ostream &operator<<(ostream &out,const datcolumn &c)
  {
   c.print_ident(out);
   return out;
  }
};

typedef datcolumn column; //!< alias name, used outside the namespace

typedef datcolumn datattr; // alias name for data attribute
typedef datcolumn attrib; //!< alias name, used outside the namespace

//// explicit friend function injections:

real corrc(const datcolumn &c1,const datcolumn &c2);
real fscor(const datcolumn &c1,const datcolumn &c2);
real tscor(const datcolumn &c1,const datcolumn &c2);

} // end of Data namespace

} // end of InfoSelCore namespace

#endif // end of DCOL_H_ header file

