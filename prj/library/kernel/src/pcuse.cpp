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
/// Definitions Of Kernel Processing User Elements
/**
 * @file       pcuse.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2009.03.20
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains definitions of kernel processing related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dfile.h"
#include "albas.h"
#include "aluse.h"
#include "pcuse.h"

namespace InfoSelUI {

namespace Processing {

/////////////////////////////
////
//// ctlfile class
//// definition
////

ctlfile::ctlfile(istream &in,const string &name): 
 in_(in), 
 name_(name.empty() ? string("control_file") : name)
{}

ctlfile::ctlfile(const string &name): 
 in_(fin_), 
 name_(name.empty() ? string("default.ctl") : name)
{}

ctlfile::~ctlfile()
{}

int ctlfile::perform_processing(ostream &log) throw()
{
 try {
  string tim;
  {
   time_t t=time(0);
   tim=ctime(&t);
   tim.erase(tim.find('\n'));
  } 
  
  string buf=bout_.str();
  bool isinit = buf.empty();
  istringstream bin_(buf);
  istream &in = isinit ? in_ : bin_;
  if (in==fin_) {
   log << endl << "opening the input control file...." << endl;
   fin_.open(name_.c_str());
   if (!fin_) throw xFileOpenFail(name_);
  }

  string hdr;
  {
   ostringstream sout;
   sout << "File name: " << name_ << endl;
   sout << "Load time: " << tim << endl;
   sout << endl;
   if (!in.eof()) {
    char c;
    in >> skipws >> c;
    in.putback(c);
   } 
   int i=0;
   while (!in.eof()) {
    string ln;
    getline(in,ln);
    if (isinit) bout_ << ln << endl;
    if (++i==1 && (ln.empty() || ln[0]!='#' || ln.length()<=1 || ln[1]!='#')) {
     sout << "##############" << endl;
     if (ln.empty() || ln[0]!='#') {
      sout << '#' << endl;
      sout << '#' << endl << endl;
     } 
    }
    sout << ln << endl;
   }
   sout << "                   #" << endl;
   sout << "                   #" << endl;
   sout << "      ##############";
   if (i>0) hdr=sout.str();
  } 

  if (in==fin_) fin_.close();
  log << endl;
  log << "input control file: " << name_ << endl;
  if (!hdr.empty()) log << "control load time: " << tim << endl;
  else {
   log << "loaded control content: empty" << endl;
   return 0;
  } 

  istringstream bin(bout_.str());
  bool res=perform_script_processing_(bin,hdr,log);
  return res ? +1 : -1;
 }

 catch (const xInOutError &exn)
 {
  log << endl << exn << endl;
 }
 
 catch (const exception &e)
 {
  xUnexpctExn exn(e);
  log << endl << exn << endl;
 }

 catch (...)
 {
  xUnknownExn exn;
  log << endl << exn << endl;
 }

 return -1;
}

} // end of Processing namespace

} // end of InfoSelUI namespace

