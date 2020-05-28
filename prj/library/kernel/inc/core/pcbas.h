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
/// Kernel Processing Basic Elements
/**
 * @file       pcbas.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.08.04
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace contains basic elements for processing 
 *             performed by the library kernel which mainly concerns
 *             a preprocessing of studied input data and an execution 
 *             of all selection algorithms registered at the library kernel.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef PCBAS_H_
#define PCBAS_H_

namespace InfoSelCore {

namespace Processing /** \copybrief pcbas.h \copydetails pcbas.h */ { // namespace head
 
/////////////////////////////
///
/// Processing Specific Errors
///

struct xProcError : xError {};

/////////////////////////////
///
/// Output Report File 
///
/// It is used for reporting
/// results of library kernel 
/// processing.
//! 
//! \note The report
//! file if its object has
//! been created is also 
//! accessed widely through 
//! the objects cerr/clog
//! of standard I/O streams.
//!

class repfile {
  string fname_; //!< output file name for the report
  ofstream fout_, ferr_, flog_; //!< main output streams for writing text to the report file
  ios::fmtflags fout_flags_, ferr_flags_, flog_flags_; //!< initial format flags of main output streams
  streambuf *cerr_rdbuf_, *clog_rdbuf_; //!< initial buffers of standard output streams assigned to the report file
  ios::fmtflags cerr_flags_, clog_flags_; //!< initial format flags of standard output streams
  ostringstream bout_, clout_; //!< auxiliary output streams for the report clipboard
  bool is_buf_, is_cp_, is_cut_; //!< auxiliary flags for the report clipboard
  int seclevel_; //!< default number of section in the report file
  
//! get current output stream for the report file:
  ostream &get_output_(ostream &fout);

//! return default output stream for the report file:
  ostream &out_();
  
//! flush inner buffer to the report file and clipboard:
  void flush_buf_();
//@{
 private: //! <b> Not implemented/allowed. </b>
  repfile(repfile &);
  void operator=(repfile &);
//@}
 public:
/// create a report object and open its output file:

  repfile(const string &name,ios::openmode mode=ios::out);

/// close the report's output file and destroy its object:

  ~repfile();

/// return report file name:

  const string &name() const
  { return fname_; }

/// set up report file precision for cutting off real numbers:

  repfile &precision(real prec); //!< \pre (asserted) rPrec < prec < 1

/// return report file precision for cutting off real numbers:

  real prec() const;
  
/// set up report file position:

  repfile &seekp(streampos pos);

/// set up report file position:

  repfile &seekp(streamoff off,ios::seekdir dir);

/// return report file position:

  streampos tellp();

/// return nonbuffered output streams for writing text to the report file:

  ostream &err();

/// return buffered output streams for writing text to the report file:

  ostream &log();

/// make a copy of a following report text and append it to the report clipboard:

  repfile &cp()
  { is_cp_=true; return *this; }

/// cut a following report text and append it to the report clipboard:

  repfile &cut()
  { is_cut_=true; return *this; }

/// move the report clipboard contents into a given stream:

  bool move_clipboard(ostream &out);  

/// paste the report clipboard contents into a given stream:

  bool paste_clipboard(ostream &out);  

/// move the report clipboard contents into the report file:

  bool move_clipboard();  

/// paste the report clipboard contents into the report file:

  bool paste_clipboard();  

/// clear the report clipboard:

  bool clear_clipboard();
  
/// flush all output streams to the report file:

  repfile &flush();
  
/// create a new result section in the report file:

  repfile &section(const string &title,int level,bool newln=true); //!< \pre (asserted) -2 < level < +2

/// create a new result section in the report file:

  repfile &section(const string &title,bool newln=true)
  { return section(title,seclevel_,newln); }
  
/// create a new result section in the report file:

  repfile &sect(const string &title)
  { return section(title,seclevel_,false); }
  
/// set up default level for a next report section:

  repfile &seclevel(int level)
  { seclevel_=level; return *this; }

/// return default level for a next report section:

  int seclevel() const
  { return seclevel_; }

/// create a section of final result summary with content moved from the report clipboard:

  repfile &summary(int seclev,const string &hdr=""); //!< \pre (asserted) -2 < level < +2

/// create a section of final result summary with content moved from the report clipboard:

  repfile &summary(const string &hdr="")
  { return summary(seclevel_,hdr); }
};

typedef repfile report; //!< alias name, used outside the namespace

} // end of Processing namespace

using namespace Processing;

} // end of InfoSelCore namespace

#endif // end of PCBAS_H_ header file

