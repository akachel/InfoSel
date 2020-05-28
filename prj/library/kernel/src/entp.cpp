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
/// Definitions For Main Entry Point
/**
 * @file       entp.cpp
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
#include "entp.h"

namespace InfoSelUI {

namespace Processing {

/////////////////////////////
////
//// entpoint class
//// definition
////

entpoint::entpoint(int argc,char *argv[],istream &a_stdin,ostream &a_stdout):
 mode_(pmNull), 
 ncmds_(0), cmds_(0), 
 stdin_(a_stdin), stdout_(a_stdout), stdlog_(0)
{
 if (argc>1) {
  ncmds_=argc;
  cmds_=new string[ncmds_];
  int i;
  for (i=1; i<argc; i++)
  cmds_[i-1]=argv[i];
  cmds_[i-1]="-break";
 } 
}

entpoint::~entpoint()
{
 delete[] cmds_;
 if (stdlog_!=0) {
  ostream &out=*stdlog_;
  out << endl << endl << "   <<<< END OF PROGRAM >>>>" << endl << endl;
 }
}

void entpoint::print_banner(ostream &out)
{
 out << endl;
 out << "  |" << endl;
 out << "  | InfoSel++" << endl;
 out << "  | Information Theoretic Feature Selection Program" << endl;
 out << "  | by Adam Kachel & Jacek Biesiada" << endl;
 out << "  |" << endl;
 out << endl;
}

void entpoint::print_syntax(ostream &out)
{
 out << endl << "Syntax of entry point commands:" << endl << endl;
 out << "  -h|help [file_name]" << endl;
 out << "    Displays this help info or writes it to a given file." << endl << endl;
 out << "  -s|scr|script file_name" << endl;
 out << "    Performs processing a given script file." << endl << endl;
 out << "  -in|input|data file_name" << endl;
 out << "    Sets up input data file." << endl << endl;
 out << "  -out|output|report file_name" << endl;
 out << "    Sets up output report file." << endl << endl;
 out << "  -p|prec|precision real_val" << endl;
 out << "    Sets up real data precision." << endl << endl;
 out << "  -pm|pmeth|partition meth_name|meth_name(val_list)" << endl;
 out << "    Sets up data partition method." << endl << endl;
 out << "  -cp|cpos|classpos int_val" << endl;
 out << "    Sets up classes column position." << endl << endl;
 out << "  -lc|lastc|classislast bool_val" << endl;
 out << "    Sets up a flag whether classes column is last." << endl << endl;
 out << "  -e|excl|excluded str_list" << endl;
 out << "    Sets up labels for excluded classes." << endl << endl;
 out << "  -ne|note|notexcluded str_list" << endl;
 out << "    Sets up labels for not-excluded classes." << endl << endl;
 out << "  -m|merg|merged str_list" << endl;
 out << "    Sets up labels for merged classes." << endl << endl;
 out << "  -nm|notm|notmerged str_list" << endl;
 out << "    Sets up labels for not-merged classes." << endl << endl;
 out << "  -exe|exec|exeselalg alg_tag|alg_tag(val_list)" << endl;
 out << "    Executes a given selection algorithm." << endl << endl;
 out << "  -execall" << endl;
 out << "    Executes all selection algorithms." << endl << endl;
 out << "  -silent" << endl;
 out << "    Perform processing without displaying output info." << endl << endl;
 out << "  -verbose" << endl;
 out << "    Perform processing displaying more output info." << endl << endl;
 out << "  -break" << endl;
 out << "    Breaks processing commands." << endl << endl;
}

namespace { namespace entpoint_ {

class helpviewer_ {
  ostream &out_;
  ostream &prompt_;
  istream &pause_;
  string hlptxt_;
  int pgn0_, pgn1_, pgn_;
  int cnt_;
  bool println_(const string &line);
 public: 
  helpviewer_(ostream &out,ostream &prompt,istream &pause);
  void display(int pgn0,int pgn1); 
};  

helpviewer_::helpviewer_(ostream &out,ostream &prompt,istream &pause):
 out_(out), prompt_(prompt), pause_(pause)
{
 ostringstream bout;

 Processing::entry::print_syntax(bout);

 bout << endl << "Partition methods: " << endl << endl;
 for (int c=0; c<Data::PartMethSet::count(); c++)
 bout << "  "+Data::PartMethSet::description(c) << endl << endl;

 bout << endl << "Selection algorithms: " << endl << endl;
 for (int i=0; i<Algorithms::Assembly.count(); i++)
 bout << "  "+Algorithms::Assembly[i].description() << endl << endl;

 Processing::control::print_grammar(bout);
 
 hlptxt_=bout.str();
}

bool helpviewer_::println_(const string &line)
{
 out_ << line << endl;

 cnt_+=1+line.length()/80;
 if (cnt_<pgn_) return true;
 else {
  prompt_ << endl;
  prompt_ << "press any key to continue or 'q' to quit.... " << flush;
  char key;
  key = pause_.get();
  prompt_ << endl;
  cnt_=0;
  pgn_=pgn1_;
  return toupper(key)!='Q';
 } 
}

void helpviewer_::display(int pgn0,int pgn1)
{
 pgn0_=pgn0;
 pgn1_=pgn1;
 pgn_=pgn0;
 cnt_=0;
 
 istringstream bin(hlptxt_);
 string ln;
 bool quit=false;
 while (!bin.eof() && !quit) {
  getline(bin,ln);
  quit = !println_(ln);
 }
}

}} // end of entpoint_ namespace 

void entpoint::display_help(ostream &out,ostream &prompt,istream &pause)
{
 using namespace entpoint_;
 helpviewer_ help(out,prompt,pause);
 help.display(15,20);
}

int entpoint::perform_processing(const string &cap,ostream &a_stdlog) throw()
{
 mode_=pmNull;

 bool silent=false, verbose=false;
 int i=0;
 while (i<ncmds_) {
  string ins=cmds_[i++];
  if (ins=="-break") break;
  else
  if (ins=="-silent") {
   silent=true;
   break;
  } else
  if (ins=="-verbose") {
   verbose=true;
   break;
  }
 } 
 
 ostream &verylog = a_stdlog; // very important logs
 ostream &normlog = silent ? nullstream::out : a_stdlog; // normal logs
 ostream &lesslog = verbose ? a_stdlog : nullstream::out; // less important logs

 print_banner(normlog);
 stdlog_=&normlog;
 if (ncmds_==0) return 0;

 ostringstream ctlout;
 i=0;
 while (i<ncmds_) {
  string ins=cmds_[i++];
  if (ins=="-break") break;
  else
  if (ins=="-silent" || ins=="-verbose") continue;
  else
  if (ins=="-h" || ins=="-help") {
   string opr;
   if (cmds_[i][0]!='-') opr=cmds_[i++];
   string &fn=opr;
   if (fn.empty()) display_help(verylog,stdout_,stdin_);
   else {
    ofstream fout(fn.c_str());
    inullstream nin;
    onullstream nout;
    normlog << endl << "writing InfoSel++ help...." << endl;
    entpoint::print_banner(fout);
    display_help(fout,nout,nin);
   }
   mode_|=pmHelp;
  } else
  if (ins=="-s" || ins=="-scr" || ins=="-script") {
   string opr=cmds_[i++];
   string &fn=opr;
   normlog << endl << "loading InfoSel++ script...." << endl;
   ctlfile scr(fn);
   int res=scr.perform_processing(lesslog);
   normlog << endl << "script processing result: " << (res>0 ? "succeeded" : res==0 ? "nullified" : "failed") << endl;
   mode_|=pmScript;
   if (res<0) return res;
  } else
  if (ins=="-in" || ins=="-input" || ins=="-data") {
   string opr=cmds_[i++];
   ctlout << "input=\"" << opr << "\";" << endl;
  } else
  if (ins=="-out" || ins=="-output" || ins=="-report") {
   string opr=cmds_[i++];
   ctlout << "output=\"" << opr << "\";" << endl;
  } else
  if (ins=="-p" || ins=="-prec" || ins=="-precision") {
   string opr=cmds_[i++];
   ctlout << "precision=" << opr << ";" << endl;
  } else
  if (ins=="-pm" || ins=="-pmeth" || ins=="-partition") {
   string opr=cmds_[i++];
   ctlout << "partition=" << opr << ";" << endl;
  } else
  if (ins=="-cp" || ins=="-cpos" || ins=="-classpos") {
   string opr=cmds_[i++];
   ctlout << "classpos=" << opr << ";" << endl;
  } else
  if (ins=="-lc" || ins=="-lastc" || ins=="-classislast") {
   string opr=cmds_[i++];
   ctlout << "classislast=" << opr << ";" << endl;
  } else
  if (ins=="-e" || ins=="-excl" || ins=="-excluded") {
   string opr=cmds_[i++];
   ctlout << "excluded=" << opr << ";" << endl;
  } else
  if (ins=="-ne" || ins=="-note" || ins=="-notexcluded") {
   string opr=cmds_[i++];
   ctlout << "notexcluded=" << opr << ";" << endl;
  } else
  if (ins=="-m" || ins=="-merg" || ins=="-merged") {
   string opr=cmds_[i++];
   ctlout << "merged=" << opr << ";" << endl;
  } else
  if (ins=="-nm" || ins=="-notm" || ins=="-notmerged") {
   string opr=cmds_[i++];
   ctlout << "notmerged=" << opr << ";" << endl;
  } else
  if (ins=="-exe" || ins=="-exec" || ins=="-exeselalg") {
   string opr=cmds_[i++];
   ctlout << "exec(" << opr << ");" << endl;
  } else
  if (ins=="-execall") {
   string opr="()";
   ctlout << "execall" << opr << ";" << endl;
  } else {
   verylog << endl << "Entry error: unknown command: " << ins << " !" << endl;
   return -1;
  }
 }
 
 string ctltxt=ctlout.str();
 if (ctltxt.empty()) return mode_==pmNull ? 0 : +1;
 
 string ctlnam = cap.empty() ? string("entry_point") : cap;
 istringstream ctlin(ctltxt);
 
 normlog << endl << "executing InfoSel++ commands...." << endl;
#ifdef SOURCE_DEBUG
 lesslog << endl << "entered control commands:" << endl;
 lesslog << ctltxt;
#endif
 Processing::control ctl(ctlin,ctlnam);
 int ret=ctl.perform_processing(lesslog);

 mode_|=pmCmds;
 return ret; 
}

} // end of Processing namespace

} // end of InfoSelUI namespace

