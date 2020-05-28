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
/// Algorithm Base Elements
/**
 * @file       albas.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.08.04
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains base, common elements 
 *             for both programming and using all studied feature 
 *             selection algorithms.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef ALBAS_H_
#define ALBAS_H_

namespace InfoSelCore {
 
namespace Algorithms /** \copybrief albas.h \copydetails albas.h */ { // namespace head

/////////////////////////////
///
/// Algorithms Specific Errors
///

struct xAlgsError : xError {};

/// Error Of Unknown Algorithm
struct xUnknownAlgorithm : xAlgsError { private: void printmsg_(ostream &out) const; };

/////////////////////////////
///
/// Root Of All Algorithm Classes 
///
/// This root of all algorithm
/// classes hierarchy. It declares 
/// methods for specification of 
/// studied selection algorithm.
/// Each final algorithm class must 
/// override some or all specification 
/// methods.
///

class AlgorithmBase {
 public:
  class parstream; //!< stream for reading algorithm parameters
  
 private:
  string package_; //!< package name as qualification for any tag of the algorithm
  mutable string name_; //!< named tag of the algorithm
  string symbol_; //!< short symbolic tag of the algorithm
  int curvnt_; //!< current number of variant of the algorithm code
  int prevnt_; //!< preset value of the algorithm's variant
  bool execing_; //!< flag of whether the algorithm is executing
  int inicard_; //!< cardinal number of features in the initial dataset at the beginning state of input data processing
  int fincard_; //!< cardinal number of features in the final dataset at the beginning state of input data processing
  int selmax_; //!< maximal number of features selected after input data processing
 
//! convert a given string to algorithm identifier:
  static void toident_(string &str);

//! call for reading parameters of the algorithm used at the run-time: 
  void call_read_(parstream &pars,const Data::file &file,Processing::report &rep);

//! call for running code of the algorithm to process input data, return computation results and time: 
  void call_run_(Data::file &file,Processing::report &rep,real &tim);
  
 private:
//! additional specification routine for specifying the algorithm variant count: 
  virtual int get_numvariants_() const; //!< \post numvariants>=0
//! additional specification routine for specifying the algorithm variant labels: 
  virtual string get_variantlabs_(int vnt) const; //!< \pre numvariants>0 && 0 <= vnt < numvariants

//! auxiliary description routine for describing declarations of the algorithm parameters: 
  virtual string get_pars_decl_() const;
//! auxiliary description routine for describing the algorithm parameters: 
  virtual string get_pars_desc_() const;
//! auxiliary description routine for describing the algorithm code-body: 
  virtual string get_body_desc_() const;

//! main execution routine for reading the algorithm parameters: 
  virtual void read_(parstream &pars,const Data::file &file,Processing::report &rep);
//! main execution routine for reading the algorithm parameters: 
  virtual void read_(parstream &pars,Processing::report &rep);
//! main execution routine for running the algorithm: 
  virtual void run_(Data::file &file,Processing::report &rep) = 0;
//@{
 private: //! <b> Not implemented/allowed. </b>
  AlgorithmBase(AlgorithmBase &);
  void operator=(AlgorithmBase &);
//@}
 public:
/// create algorithm instance with a given short symbol:

  AlgorithmBase(const string &symbol);

/// destroy algorithm instance:

  virtual ~AlgorithmBase();

/// set up package qualification for the algorithm tags:

  string qualification(const string &pack)
  { string packo=package_; package_=pack; toident_(package_); return packo; }
  
/// return package qualification:

  const string &package() const
  { return package_; }

//// return identification strings of the algorithm:

  /// return algorithm caption:
  string caption(bool shorten=false,bool novnt=false) const;

  /// return algorithm qualified name:
  string qname() const;

  /// return algorithm name:
  const string &name() const;

  /// return algorithm qualified symbol:
  string qsymbol() const;

  /// return algorithm symbol:
  const string &symbol() const
  { return symbol_; }
  
/// return number of specification variants for the algorithm:

  int nvariants() const //! \post nvariants>=0 
  { return get_numvariants_(); }
                       
/// set up current specification variant for the algorithm:

  int variant(int vnt) //! \post -1 <= variant < nvariants
  { int vnto=variant(); prevnt_=vnt; return vnto; }

/// return current specification variant for the algorithm:

  int variant() const //! \post nvariants<=0 && variant==-1 || nvariants>0 && 0 <= variant < nvariants
  { int n=get_numvariants_(); return n<=0 ? -1 : prevnt_>=0 && prevnt_<n ? prevnt_ : curvnt_; }
                       
/// return all specification variant labels for the algorithm:

  string vlabels(int vnt) const; //!< \pre nvariants>0 && 0 <= vnt < nvariants

/// return current specification variant label for the algorithm:

  string vlabel() const
  { int v=variant(); return vlabels(v); }

/// cut off variant specification for a given algorithm tag:

  int cutvariant(string &tag) const;

/// return specification variant for a given algorithm tag:

  int retvariant(const string &tag) const
  { string t=tag; return cutvariant(t); }
  
/// test whether the algorithm is identified by a given qualified/named/symbolic tag with no variant specification:

  bool istagged(const string &tag) const;

/// return full description of the algorithm parameters and code:

  string description() const;

//// types of value stream for reading algorithm parameters of any type:

  /// Parameter Kinds 
  enum { 
   prNone=0x00, 
   prMain=0x01, 
   prOther=0x02 
  };
  
  /// Stream Modes for What Kind of Parameters to Read 
  enum {
   rdNonePars=prNone, 
   rdMainPars=prMain, 
   rdOtherPars=prOther, 
   rdAllPars=0xFF 
  };
  
//////  
/// Algorithm Parameter Stream 
///
/// It is used for reading values 
/// any algorithm parameters from 
/// a given input stream.   
///
  class parstream {
    ostream &rdprom_; //!< reference to prompt stream used in parameter reading
    istream &rdin_; //!< reference to main input stream used in parameter reading
    int rdmode_; //!< mode of parameter reading
    const char rdskip_; //!< symbol to skip at parameter readingr
    bool rdfail_; //!< flag of parameter reading failure
    string rdlog_; //!< buffer for storing logs used in parameter reading
    string parcap_; //!< parameter capture string
    int parkind_; //!< parameter kind code
    int i_defparval_; //!< default integer value for parameter
    real r_defparval_; //!< default real value for parameter
    ostream *parlog_; //!< pointer to log stream used in parameter reading
    
  //! initiate parameter reading  
    void initpar_()
    {
     parcap_="";
     parkind_=prNone;
     i_defparval_=0; 
     r_defparval_=0.0; 
     parlog_=0;    
    }

  //! perform parameter reading  
    template <typename T>
    bool readpar_(T &retpar,T defparval)
    {
     ostringstream bout;
     if (parcap_.empty()) parcap_="any parameter";
     bout << parcap_;
     retpar=defparval;
     if (!rdfail_ && (rdmode_ & parkind_)) { 
      rdprom_ << parcap_ << " (e.g. " << defparval << "): " << flush;
      char c=0;
      rdin_ >> ws >> c; 
      if (rdin_ && c!=rdskip_) {
       rdin_.putback(c);
       T par=defparval;
       rdin_ >> par;
       if (rdin_) retpar=par;
      }
      rdfail_ = !rdin_; 
     }
     if (retpar==defparval) bout << ", default";
     bout << ": " << retpar;
     rdlog_=bout.str();
     if (parlog_) *parlog_ << rdlog_ << endl;
     initpar_();
     return !rdfail_;
    }
   //@{
   private: //! <b> Not implemented/allowed. </b> 
    parstream(parstream &);
    void operator=(parstream &);
   //@} 
   public:
    static const char rdSkipPar='*'; ///< symbol to skip reading current parameter and to set default value
    
  /// create a parameter stream object:
   
    parstream(ostream &prompt,istream &in,int mode=rdAllPars,char skip=rdSkipPar): 
     rdprom_(prompt), rdin_(in), rdmode_(mode), rdskip_(skip), 
     rdfail_(false), rdlog_("")
    {
     initpar_();
    }
  //@{  
  /// return all construction arguments of the stream object:
  
    ostream &prompt()
    { return rdprom_; }
    
    const ostream &prompt() const
    { return rdprom_; }
    
    istream &in()
    { return rdin_; }
    
    const istream &in() const
    { return rdin_; }
    
    int mode() const
    { return rdmode_; }
    
    char skip() const
    { return rdskip_; }
  //@}
  //@{
  /// set up properties of current parameter to read from the stream:
    
    parstream &operator()(const string &caption,int kind,ostream &log) 
    { parcap_=caption; parkind_=kind; parlog_=&log; return *this; }
    
    parstream &operator()(const string &caption,int kind) 
    { parcap_=caption; parkind_=kind; return *this; }
    
    parstream &operator()(const string &caption,ostream &log) 
    { parcap_=caption; parkind_=prMain; parlog_=&log; return *this; }
    
    parstream &operator()(const string &caption) 
    { parcap_=caption; parkind_=prMain; return *this; }
  //@}
  //@{
  /// set up / read value of current parameter of a given type:
    
    parstream &operator()(int defval) 
    { i_defparval_=defval; r_defparval_=defval; return *this; }
    
    bool operator>>(int &ret)
    { return readpar_(ret,i_defparval_); }

    parstream &operator()(real defval) 
    { r_defparval_=defval; return *this; }
    
    bool operator>>(real &ret)
    { return readpar_(ret,r_defparval_); }
  //@}
  //@{
  /// return current status of the stream:
    
    bool fail() const
    { return rdfail_; }

    bool good() const
    { return !rdfail_; }

    const string &log() const
    { return rdlog_; }
  //@}
  };
//||||  

//////
/// Algorithm Parameter Stream for Input String Stream
///
  class parstrstream : public parstream {
    onullstream nout_;
    istringstream srdin_;
   public:
    explicit parstrstream(const string &str,int mode=rdAllPars,char skip=rdSkipPar): 
     parstream(nout_,srdin_,mode,skip),
     srdin_(str)
    {}
  };
//||||  

//////
/// Algorithm Parameter Stream for Null Input Stream
///
  class parnulstream : public parstream {
    onullstream nout_;
    inullstream nin_;
   public:
    parnulstream(): 
     parstream(nout_,nin_,rdNonePars)
    {}
  };
//||||  

/// set up algorithm parameters for the execution method:

  static parstrstream &pars(const string &str,int mode=rdAllPars)
  { static parstrstream ret(str,mode); return ret; }
  
/// set up algorithm parameters for the execution method:

  static parnulstream &pars()
  { static parnulstream ret; return ret; }
  
/// main execution method for the algorithm to read its parameters, run input data processing and print computation results:

  void execute(parstream &pars,Data::file &file,Processing::report &rep,ostream &log);
  
/// return beginning state of input data processed by the algorithm for the initial set:

  int inicard() const
  { return inicard_; }
  
/// return beginning state of input data processed by the algorithm for the final set:

  int fincard() const
  { return fincard_; }
  
/// return maximal number of features selected by the algorithm during data processing:

  int selmax() const
  { return selmax_; }
};

/////////////////////////////
///
/// Collection Of All Algorithms
///
/// It is used for storing single 
/// instances representing classes 
/// with algorithm specification.
//!
//! \internal This class is
//! used by the API and UI 
//! interfaces. 
//!

class algs_collection_base_ {
 protected:
  enum {
   maxnitems_=1000, //!< maximal number of items included by the collection
   maxbuflen_=100 //!< maximal length of auxiliary buffers used in the collection
  }; 
  static AlgorithmBase *reginsts_[maxnitems_]; //!< underlying array of all algorithm instances registered in the collection
  static int nreginsts_; //!< current number of registered algorithm instances
  static bool isunstructed_; //!< flag of whether the collection's inner structure is not fully built
  
 //! do register new algorithm instance of a singleton class with a given package qualification:
  static int register_(const char *pack,AlgorithmBase *inst);

 //! do unregister the algorithm for a given index of its register:
  static void unregister_(int &ireg);

 //! end up building the collection's inner structure:
  static void construct_(); 
//@{
 private: //! <b> Not implemented/allowed. </b>
  algs_collection_base_(algs_collection_base_ &);
  void operator=(algs_collection_base_ &);
//@}
 protected:
/// create algorithm collection base:

  algs_collection_base_()
  {}

/// destroy algorithm collection base:

  ~algs_collection_base_()
  {}
};

} // end of Algorithms namespace

} // end of InfoSelCore namespace

#endif // end of ALBAS_H_ header file

