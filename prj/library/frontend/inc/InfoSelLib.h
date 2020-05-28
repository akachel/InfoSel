 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////       Library Front-End Interfaces     ////////
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
/// The Library Front-End Interfaces
/**
 * @file       InfoSelLib.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.02.02
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains all top-level, front-end interfaces  
 *             for the InfoSel++ library kernel. Any front-end interface wraps 
 *             the kernel hence all low-level kernel elements are not accessible
 *             in this namespace.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef InfoSelLIB_H_
#define InfoSelLIB_H_

#include <string>
#include <list>
#include <iostream>

/////////////////////////////
///
/// Macro-Directive for Using 
/// Algorithm Packages
/// 
/// It must be applied for declaring 
/// use of any package which contains
/// registered classes with studied 
/// algorithm implementation. 
//!
//! \see The _packuse.cxx file has 
//! several "USEPACK" directives for 
//! using standard algorithm packages. 
//! \note Used packages must be first 
//! itself registered at the library 
//! kernel.
//!

#ifndef USEPACK
#define USEPACK(name) \
 extern "C" int infosel_##name##_package_; \
 static int using_infosel_##name##_package_=InfoSelLib::KernelEntry::apply_using_directive_(#name,infosel_##name##_package_);
#endif

namespace InfoSelLib /** \copybrief InfoSelLib.h \copydetails InfoSelLib.h */ {

/////////////////////////////
///
/// Kernel Entry For User Interfaces  
///
/// This is an entry point to the InfoSel++ 
/// library kernel. It provides a basic 
/// set of auxiliary elements common 
/// for all derivative front-end interfaces.
///

class KernelEntry {
 public:
//! \internal Apply the using directive for a given algorithm package.
  static int apply_using_directive_(const char *pack,int &iuse);
//@{
 private: //! <b> Not implemented/allowed. </b>
  KernelEntry(KernelEntry &);
  void operator=(KernelEntry &);
//@}  
 protected: 
//! create an entry object:
  KernelEntry()
  {}
 public:
//// browse partition method set:

  /// return count of partition methods:
  static int partmethcount();

  /// return name of partition method of a given index:
  static std::string partmethname(int c);

  /// return description of partition method of a given index:
  static std::string partmethdesc(int c);

//// browse selection algorithm set:

  /// return count of selection algorithms:
  static int selalgcount();

  /// return package name for selection algorithm of a given index:
  static std::string selalgpack(int i);

  /// return name of selection algorithm of a given index:
  static std::string selalgname(int i);

  /// return symbol of selection algorithm of a given index:
  static std::string selalgsymb(int i);

  /// return description of selection algorithm of a given index:
  static std::string selalgdesc(int i);

//////
/// Type Of Kernel Processing Result
//!
//! Result codes:
//!   \li +1 : processing succeeded
//!   \li  0 : no action performed
//!   \li -1 : processing failed
//! 
  class procres {
    int cod_; //!< result code field
   public: 
    procres(int c) //! create result record
     : cod_(c)
    {}
    operator int() const //! return code of result
    { return cod_; }
    bool good() const //! test for good result
    { return cod_>0; }
    bool none() const //! test for no result
    { return cod_==0; }
    bool fail() const //! test for fail result
    { return cod_<0; }
    friend std::ostream &operator<<(std::ostream &out,const procres &r) //! print result to output stream
    {
     out << (r.cod_>0 ? "succeeded" : r.cod_==0 ? "nullified" : "failed");
     return out;
    }
  }; 
//||||

/// extract input data file names from a given control logs of kernel processing:

  static bool datfnames(const std::string &logs,std::list<std::string> &nams); 

/// extract output report file names from a given control logs of kernel processing:

  static bool repfnames(const std::string &logs,std::list<std::string> &nams); 
};

/////////////////////////////
///
/// Command-Line User Interface
///
/// Used for running and controlling 
/// jobs with the InfoSel++ library 
/// by processing command arguments 
/// of the main entry point of 
/// application program.
///

class CommandLine : public KernelEntry {
  void *rep_; //!< pointer to data representation
 public:
/// create a line object, giving the main command arguments and standard I/O streams:

  CommandLine(int argc,char *argv[],std::istream &a_stdin,std::ostream &a_stdout);

/// create a line object, giving the main command arguments and standard input streams:

  CommandLine(int argc,char *argv[],std::istream &a_stdin); //!< \post (asserted) 'stdout' is onullstream

/// create a line object, giving the main command arguments and standard output streams:

  CommandLine(int argc,char *argv[],std::ostream &a_stdout); //!< \post (asserted) 'stdin' is inullstream

/// create a line object, giving the main command arguments:

  CommandLine(int argc,char *argv[]); //!< \post (asserted) 'stdin/out' are i/onullstream

/// destroy the line object:

  ~CommandLine();

/// return command syntax text:

  static void syntax(std::ostream &out);

/// perform processing of the command line, returning control logs & processing result:

  procres process(std::ostream &a_stdlog);

/// perform processing of the command line, returning processing result:

  procres process(); //!< \post (asserted) 'stdlog' is onullstream
};

/////////////////////////////
///
/// Scripting Batch User Interface
///
/// Used for running and controlling 
/// jobs with the InfoSel++ library 
/// by processing input script files. 
///

class ScriptBatch : public KernelEntry {
 public:
/// type of file name list:
  typedef std::list<std::string> fnlist_t;
  
 private:
  void *rep_; //!< pointer to data representation
  fnlist_t datfns_, repfns_; //!< stored file names
 public:
/// create a batch object, giving an input script file and its name:

  explicit ScriptBatch(std::istream &in,const std::string &name="");

/// create a batch object, giving an input script file name:

  explicit ScriptBatch(const std::string &name); //!< \post (asserted) 'in' is nullstream::in

/// destroy the batch object:

  ~ScriptBatch();

/// return script grammar text:

  static void grammar(std::ostream &out);

/// perform processing of the script batch, returning control logs & processing result:
                                     
  procres process(std::ostream &log);

/// perform processing of the script batch, returning processing result:
                                     
  procres process(); //!< \post (asserted) 'log' is onullstream
  
/// return names of all data files involved in the latest processing:

  const fnlist_t &datfnames() const
  { return datfns_; }

/// return names of all report files involved in the latest processing:

  const fnlist_t &repfnames() const
  { return repfns_; }
};

/////////////////////////////
///
/// Programmatic Shell User Interface 
///
/// Used for binding the InfoSel++ Library 
/// with any application source code.
///

class ProgramShell : public KernelEntry {
  void *rep_; //!< pointer to data representation
 public:
/// create a null object, no library binding performed:

  ProgramShell();

/// create a shell object, giving its processing caption:

  explicit ProgramShell(const std::string &caption);

/// destroy the shell object:

  ~ProgramShell();

/// create a copy of a given shell object:

  ProgramShell(const ProgramShell &shl);

/// make a copy of a given shell object:

  ProgramShell &operator=(const ProgramShell &shl);

/// set up input file names for reading studied data:

  void setdatfn(const std::string &n);

/// return input file names for reading studied data:

  const std::string &datfn() const;
             
/// set up output file names for printing process report:

  void setrepfn(const std::string &n);

/// return output file names for printing process report:

  const std::string &repfn() const;
            
//// access all data preprocessing parameters:

  /// set up real data precision:
  void setprec(double p);                    

  /// return real data precision:
  double prec() const; 
  
  /// set up data partition method:
  void setpartmth(const std::string &name,const std::string &pars);

  /// return name of data partition method:
  const std::string &pmname() const;         

  /// return parameters of data partition method:
  const std::string &pmpars() const;
  
  /// set up classes column position:
  void setcpos(int p);

  /// symbol of last position for classes column:
  enum { cpLast=-1 };

  /// set up last position for classes column:
  void setcpos_last() { setcpos(cpLast); }                       

  /// return classes column position:
  int cpos() const;

  /// test whether position for classes column is last:
  bool cpos_last() const { return cpos()==cpLast; }
                            
  /// set up excluded classes (e=="void" for an empty classes set):
  void setexcl(const std::string &e,bool note=false);

  /// return excluded classes:
  const std::string &excl() const;            

  /// return not-excluded classes flag:
  bool note() const; 

  /// set up merged classes (m=="void" for an empty classes set):
  void setmerg(const std::string &m,bool notm=false);

  /// return merged classes:
  const std::string &merg() const;           

  /// set up not-merged classes flags:
  bool notm() const; 
  
//////
/// Proxy Class Of Studied Selection Algorithms
///
  class Algorithm {
    friend class ProgramShell;
   private: 
    void *rep_; //!< pointer to data representation
   public:
  /// create an algorithm proxy, giving its identification tag (name or symbol):

    explicit Algorithm(const std::string &tag);

  /// destroy the algorithm proxy:

    ~Algorithm();
    
  /// create a copy of a given algorithm proxy:

    Algorithm(const Algorithm &A);

  /// make a copy of a given algorithm proxy:

    Algorithm &operator=(const Algorithm &A);
    
  /// pass on algorithm invocation parameters:

    Algorithm &operator()(const std::string &pars);
    
  /// connect serially two algorithms to perform pipeline processing:

    Algorithm operator>(const Algorithm &A) const;
    
  /// connect parallely two algorithms to perform pipeline processing:

    Algorithm operator|(const Algorithm &A) const; 
    
  /// return the algorithm identification tag:

    const std::string &tag() const;

  /// return the algorithm identification invocation parameters:

    const std::string &pars() const;
  };
//||||
  
/// pass a given algorithm to execution:

  void exec(const Algorithm &A);

/// pass all registered algorithms to execution:

  void execall();
  
/// advance processing of the shell object:

  void flush();

/// reinitiate processing of the shell object:

  void reset();

/// inspect status of the shell object processing, returning control logs & processing result:

  procres status(std::ostream &log) const;

/// inspect status of the shell object processing, returning processing result:

  procres status() const; //!< \post (asserted) 'log' is onullstream
};

} // end of InfoSelLib namespace

#endif // end of InfoSelLIB_H_ header file

