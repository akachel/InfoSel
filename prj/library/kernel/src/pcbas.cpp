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
/// Definitions Of Kernel Processing Basic Elements
/**
 * @file       pcbas.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains definitions of kernel processing related elements.
 */

#include "comm.h"
#include "pcbas.h"

namespace InfoSelCore {

namespace Processing {

/////////////////////////////
////
//// repfile class definition
////

repfile::repfile(const string &name,ios::openmode mode):
 fname_(name), fout_(name.c_str(),mode), seclevel_(0),
 bout_(string(500,'\0')), clout_(string(5000,'\0')), 
 is_buf_(false), is_cp_(false), is_cut_(false)
{
 if (!fout_) throw xFileOpenFail(fname_);

 streambuf *fbuf=fout_.rdbuf();
 ((ostream &)ferr_).rdbuf(fbuf);
 ((ostream &)flog_).rdbuf(fbuf);
 cerr_rdbuf_=cerr.rdbuf(); cerr.rdbuf(fbuf);
 clog_rdbuf_=clog.rdbuf(); clog.rdbuf(fbuf);

 fout_.setf(ios::fixed,ios::floatfield);
 ferr_.setf(ios::fixed,ios::floatfield);
 flog_.setf(ios::fixed,ios::floatfield);
 cerr_flags_=cerr.flags(); cerr.setf(ios::fixed,ios::floatfield);
 clog_flags_=clog.flags(); clog.setf(ios::fixed,ios::floatfield);

 fout_.setf(ios::boolalpha);
 ferr_.setf(ios::boolalpha);
 flog_.setf(ios::boolalpha);
 cerr.setf(ios::boolalpha);
 clog.setf(ios::boolalpha);

 fout_.unsetf(ios::unitbuf);
 ferr_.setf(ios::unitbuf);
 flog_.unsetf(ios::unitbuf);
 cerr.setf(ios::unitbuf);
 clog.unsetf(ios::unitbuf);

 fout_flags_=fout_.flags();
 ferr_flags_=ferr_.flags();
 flog_flags_=flog_.flags();
 
 precision(1e1*rPrec);
}

repfile::~repfile()
{
 flush_buf_();
 iosex::resetallflags(cerr);
 iosex::resetallflags(clog);
 cerr.flags(cerr_flags_);
 clog.flags(clog_flags_);
 cerr.rdbuf(cerr_rdbuf_);
 clog.rdbuf(clog_rdbuf_);
}

repfile &repfile::precision(real prec)
{
 prec = prec>rPrec && prec<1.0 ? prec : 0.001;
 int p = -log10(prec)+0.5;
 fout_.precision(p);
 ferr_.precision(p);
 flog_.precision(p);
 cerr.precision(p);
 clog.precision(p);
 return *this;
}

real repfile::prec() const
{
 int p = fout_.precision();
 real ret = pow(10.0,-p);
 return ret;
}

repfile &repfile::seekp(streampos pos)
{
 flush_buf_();
 fout_.seekp(pos);
 return *this;
}

repfile &repfile::seekp(streamoff off,ios::seekdir dir)
{
 flush_buf_();
 fout_.seekp(off,dir);
 return *this;
}

streampos repfile::tellp()
{
 flush_buf_();
 return fout_.tellp();
}

ostream &repfile::get_output_(ostream &fout)
{ 
 flush_buf_();
 bool is_cp=is_cp_;
 bool is_cut=is_cut_;
 is_cp_=is_cut_=false; 
 ostream &fret = is_cp ? (is_buf_=true, (ostream &)bout_) : is_cut ? (ostream &)clout_ : fout;
 fret.flags(fout.flags());
 iosex::resetallflags(fret);
 return fret;
}

ostream &repfile::out_()
{ 
 fout_.flags(fout_flags_);
 iosex::resetallflags(fout_);
 return get_output_(fout_);
}

ostream &repfile::err()
{ 
 ferr_.flags(ferr_flags_);
 iosex::resetallflags(ferr_);
 return get_output_(ferr_);
}

ostream &repfile::log()
{ 
 flog_.flags(flog_flags_);
 iosex::resetallflags(flog_);
 return get_output_(flog_);
}

bool repfile::move_clipboard(ostream &out)
{
 flush_buf_();
 clout_ << ends;
 clout_ << std::flush;
 string clp=clout_.str();
 clout_.seekp(0,ios::beg);
 
 const char *c=clp.c_str();
 if (c[0]=='\0') return false;
 else {
  out << c;
  return true;
 } 
}

bool repfile::paste_clipboard(ostream &out)
{
 ostringstream bout;
 if (!move_clipboard(bout)) return false;
 else {
  string buf=bout.str();
  out << buf;
  clout_ << buf;
  return true;
 }
}

bool repfile::move_clipboard()
{
 ostream &out=out_();
 if (!move_clipboard(out)) return false;
 else {
  out << std::flush;
  return true;
 } 
}

bool repfile::paste_clipboard()
{
 ostream &out=out_();
 if (!paste_clipboard(out)) return false;
 else {
  out << std::flush;
  return true;
 }
}

bool repfile::clear_clipboard()
{ 
 return move_clipboard(nullstream::out); 
}

repfile &repfile::flush()
{
 flush_buf_();
 fout_ << std::flush;
 clout_ << std::flush;
 is_cp_=is_cut_=false;
 return *this;
}

repfile &repfile::section(const string &title,int level,bool newln)
{
 const int nlev=5;
 const int msiz=50;
 static char lbr[nlev][msiz]={">>",">>>>>",">>>>>>>>>>",">>>>>>>>>>>>>>>",">>>>>>>>>>>>>>>>>>>>>>"};
 static char rbr[nlev][msiz]={"<<","<<<<<","<<<<<<<<<<","<<<<<<<<<<<<<<<","<<<<<<<<<<<<<<<<<<<<<<"};
 int i=nlev/2-level;
 if (i<0) i=0; else if(i>=nlev) i=nlev-1;
 ostream &out=out_();
 if (newln) out << endl;
 out << lbr[i] << ' ' << title << ' ' << rbr[i] << endl;
 return *this;
}

repfile &repfile::summary(int seclev,const string &hdr)
{
 flush_buf_();
 clout_ << ends;
 clout_ << std::flush;
 string clp=clout_.str();
 clout_.seekp(0,ios::beg);
 
 const char *c=clp.c_str();
 if (c[0]!='\0') {
  is_cp_=is_cut_=false;
  section("Final Result Summary",seclev);
  if (!hdr.empty())
  fout_ << endl << hdr << endl;
  fout_ << c << std::flush;
 }
 return *this;
} 

void repfile::flush_buf_()
{
 if (!is_buf_) return;
 is_buf_=false;
 
 bout_ << ends;
 bout_ << std::flush;
 string buf=bout_.str();
 bout_.seekp(0,ios::beg);

 const char *b=buf.c_str();
 if (b[0]!='\0') {
  fout_ << b;
  clout_ << b;
 } 
}

} // end of Common namespace

} // end of InfoSelCore namespace
