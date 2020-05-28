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
/// Kernel Processing User Elements
/**
 * @file       pcuse.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2009.03.20
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains elements of library kernel 
 *             processing declared at the library user interface.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef PCUSE_H_
#define PCUSE_H_

namespace InfoSelUI {

namespace Processing /** \copybrief pcuse.h \copydetails pcuse.h */ { // namespace head

using namespace InfoSelCore::Processing;
 
/////////////////////////////
///
/// Input Control File 
///
/// It is used for controlling 
/// the course of library kernel 
/// processing.
///

class ctlfile {
  istream &in_; //!< reference to input stream
  ifstream fin_; //!< reference to input file stream
  string name_; //!< name to input file 
  ostringstream bout_; //!< auxiliary output string buffer
  
//! print complete script grammar to a given output stream:
  static void print_script_grammar_(ostream &out);

//! perform script processing for a given input script stream:
  static bool perform_script_processing_(istream &script,const string &header,ostream &ctllog) throw();
//@{
 private: //! <b> Not implemented/allowed. </b> 
  ctlfile(ctlfile &);
  void operator=(ctlfile &);
//@}
 public:
/// create a control object with given specification of its input file:

  explicit ctlfile(istream &in,const string &name="");

/// create a control object with given name of its input file:

  explicit ctlfile(const string &name); //!< \post (asserted) 'in' is ifstream

/// destroy the control file object:

  ~ctlfile();

/// return control file name:

  const string &name() const
  { return name_; }

/// print control file grammar:

  static void print_grammar(ostream &out)
  { print_script_grammar_(out); }

/// open the control's input file and perform its processing:
//!
//! \return
//!   \li +1 : success
//!   \li  0 : no action
//!   \li -1 : failure
//! 
  int perform_processing(ostream &log) throw();
};

typedef ctlfile control; //!< alias name, used outside the namespace

} // end of Processing namespace

using namespace Processing;

} // end of InfoSelUI namespace

#endif // end of PCUSE_H_ header file

