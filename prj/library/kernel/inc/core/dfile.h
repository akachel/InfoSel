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
/// Data File At Input Data Structures
/**
 * @file       dfile.h
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

#ifndef DFILE_H_
#define DFILE_H_

namespace InfoSelCore {
 
namespace Data {

/////////////////////////////
///
/// Input Data File
///
/// It represents and preprocesses 
/// any studied input data file. 
/// This is a root object of input 
/// data structures, storing and
/// exposing data sets. 
///

class datfile {
 public: 
//// forward declaration of preprocessing parameters record:
  struct parsrec;
  
 private: 
  string name_; //!< file system name of the file
  real prec_; //!< resultant precision of all data values in the file 
  int nrows_; //!< number of data rows in the file
  int ncols_; //!< number of data columns in the file
  int ninsts_; //!< number of processed data instances in the file
  int clspos_; //!< zero-based positional number of the data classes column in the input file 
  string exclstr_, mergstr_; //!< set of data classes labels to exclude/merge at the file 
  datset *orgset_; //!< object of the original set of columns with preprocessed feature data
  datcolumn *orgcls_; //!< object of the original column with preprocessed data classes
  datset *iniset_, *finset_; //!< objects of the initial & final sets of all feature columns
  datset *iniset_ptr_, *finset_ptr_; //!< current pointers to the initial & final sets
  datset **imdsets_, **auxsets_; //!< array of objects of intermediate & auxiliary feature sets
  int nimdsets_, nauxsets_; //!< intermediate & auxiliary sets array length
  int imdset_idx_; //!< current index of an unused intermediate set
  datset *iexset_ptr_; //!< persistent pointer to the extra intermediate set
  int pipestep_; //!< current step of pipeline processing of the file
  bool islastpipes_; //!< flag of whether there is the last pipe-step
  bool serlpipe_; //!< flag of whether the pipeline processing is performed serially
  int procescnt_; //!< number of ended successive processing runs of the file
  
//! return given pointer to an auxiliary set: 
  datset *auxset_ptr_(int n) const;

//! read file and translate its format according with given parameters:
  bool rdtranslate_(const parsrec &pars,istream &in,ostream &out);
  
//! read file and preprocess its data according with given parameters:
  void rdpreprocess_(const parsrec &pars,istream &in);
//@{
 private: //! <b> Not implemented/allowed. </b>
  datfile(datfile &);
  void operator=(datfile &);
//@}
 public:
//////
/// Record of Data Preprocessing & File Format Parameters
///
  struct parsrec {
   real prec; //!< data precision for reading real numbers 
   partmeth_t pmth; //!< descriptor of data partition method
   int cpos; //!< classes column position
   bool lastc; //!< flag to indicate whether the classes column is last in the data file
   string excl; //!< space-separated label set of excluded/not-excluded classes
   bool note; //!< negation flag for above exclusion set
   string merg; //!< space-separated label set of merged/not-merged classes
   bool notm; //!< negation flag for above mergence set
   
  /// create a record:
   parsrec(); 

  /// destroy record:
   ~parsrec(); 
   
  /// reinitiate the record:
   void operator()()
   { *this=parsrec(); }
  };
//||||

/// create a data file object, read data from its input file and perform preprocessing:
  
  datfile(const string &name,const parsrec &pars,int nimdsets=10,int nauxsets=10); //!< \pre (asserted) pars.cpos>=0 , rPrec < pars.prec < 1

/// destroy the data file object:

  ~datfile();

/// initiate the data file object for successive processing runs:

  void initiate(Processing::report &rep,bool more_info=false);

/// reinitiate the data file object for successive processing runs:

  void reinit(Processing::report &rep,bool more_info=false);

/// finalize the data file object for successive processing runs:

  void finalize(Processing::report &rep,bool less_info=false);
  
//// perform one processing run in a pipeline form with multiple processing subruns:

  /// test whether pipeline processing is running:
  bool pipeprocessing() const
  { return pipestep_>=0; }

  /// test whether pipeline processing is at first step:
  bool atfirstpipestep() const
  { return pipestep_==0; }

  /// test whether pipeline processing is at last step:
  bool atlastpipestep() const
  { return islastpipes_; }

  /// test whether pipeline processing is serial:
  bool serialpipe() const
  { return serlpipe_; }

  /// test whether pipeline processing is parallel:
  bool parallpipe() const
  { return pipestep_>0 && !serlpipe_; }

  /// return pipeline processing step:
  int pipestep() const //! \post pipestep>=-1
  { return pipestep_; }

  /// make first pipeline processing step:
  void mkfirstpipestep();

  /// make next pipeline processing step:
  void mknextpipestep(bool serially=true);

  /// make last pipeline processing step:
  void mklastpipestep(bool serially=true);

/// return count of ended successive runs with distinct data file processing:

  int procescnt() const //! \post procescnt>=0
  { return procescnt_; }
  
/// return data file name:
  
  const string &name() const
  { return name_; }
  
/// return data file precision:
  
  real prec() const //! \post rPrec < prec < 1
  { return prec_; }

/// return number of all data rows in the file:

  int nrows() const //! \post nrows>0
  { return nrows_; }

/// return number of all data columns in the file:

  int ncols() const //! \post ncols>0
  { return ncols_; }

/// return number of file rows with data instances selected for further processing:

  int ninsts() const //! \post 0 < ninsts <= nrows
  { return ninsts_; }

/// return positional number of the data classes column at the original layout of input file:

  int clspos() const //! \post clspos>=0
  { return clspos_; }

/// return the set of all data feature attributes:

  const datset &features() const
  { return *orgset_; }

/// return the data classes attribute:

  const datattr &classes() const
  { return *orgcls_; }
  
/// return initial copy of the feature set:

  datset &iniset()
  { return *iniset_ptr_; }

/// return initial copy of the feature set:

  const datset &iniset() const
  { return *iniset_ptr_; }

/// return the final feature set as a result of processing the initial one:

  datset &finset()
  { return *finset_ptr_; }

/// return the final feature set as a result of processing the initial one:

  const datset &finset() const
  { return *finset_ptr_; }

/// return an auxiliary feature set with a given ordinal number:

  datset &auxset(int n) //! \pre n >= 1
  { return *auxset_ptr_(n); }
  
/// return an auxiliary feature set with a given ordinal number:

  const datset &auxset(int n) const //! \pre n >= 1
  { return *auxset_ptr_(n); }

/// print info about original contents of the input file to a given output stream:

  void print_conts(ostream &out) const;

/// print contents to output stream:

  friend ostream &operator<<(ostream &out,const datfile &f)
  {
   f.print_conts(out);
   return out;
  }
};

typedef datfile file; //!< alias name, used outside the namespace

} // end of Data namespace

} // end of InfoSelCore namespace

#endif // end of DFILE_H_ header file

