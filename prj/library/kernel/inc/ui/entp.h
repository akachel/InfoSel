 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////          Library User Interface        ////////
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
/// Main Entry Point At Kernel Processing
/**
 * @file       entp.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2009.03.20
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains declarations of kernel processing elements. 
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef ENTP_H_
#define ENTP_H_

namespace InfoSelUI {

namespace Processing {
 
/////////////////////////////
///
/// Main Entry Point 
///
/// It is use for entering 
/// the code of library kernel 
/// processing.
///

class entpoint {
  int mode_; //!< current processing mode
  int ncmds_; //!< number of command line items
  std::string *cmds_; //!< table of command line items
  istream &stdin_; //!< reference to standard input stream
  ostream &stdout_; //!< reference to standard output stream
  ostream *stdlog_; //!< reference to standard log stream
//@{
 private: //! <b> Not implemented/allowed. </b> 
  entpoint(entpoint &);
  void operator=(entpoint &);
//@}
 public:
/// create an entry object with given specification of program's main point:

  entpoint(int argc,char *argv[],istream &a_stdin,ostream &a_stdout);

/// destroy the entry point object:

  ~entpoint();

//////
/// Entry Point Processing Modes
///
  enum {
   pmNull=0x00,    ///< no processing performed
   pmHelp=0x01,    ///< help text provided (displayed or written)
   pmScript=0x02,  ///< control script loaded
   pmCmds=0x04,    ///< control commands executed
   pmFull=0xFF     ///< full processing performed
  };
//||||

/// return current processing mode:  

  int mode() const
  { return mode_; }

/// print entry point banner with short info about this library:

  static void print_banner(ostream &out);

/// print syntax of entry point commands:

  static void print_syntax(ostream &out);

/// display entry point help with full description of this library:

  static void display_help(ostream &out,ostream &prompt,istream &pause);

/// perform entry point processing: 
//!
//! \return
//!   \li +1 : success
//!   \li  0 : no action
//!   \li -1 : failure
//! 
  int perform_processing(const string &cap,ostream &a_stdlog) throw();
};

typedef entpoint entry; //!< alias name, used outside the namespace

} // end of Processing namespace

} // end of InfoSelUI namespace

#endif // end of ENTP_H_ header file

