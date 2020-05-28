 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////       Standard Console Application     ////////
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
/// Code Of Interactive User Interface For Standard Console Application
/**
 * @file       intcon.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains master code for running and controlling 
 *             jobs with the InfoSel++ library by processing textual 
 *             interaction-console user interface based on the standard 
 *             program input/output streams.
 */

#include "InfoSelUI.h"
#include <strstream> //// warning: this header is deprecated, must be replaced by <sstream> in the future

namespace {

using namespace InfoSelUI;
using namespace InfoSelUI::Data;
using namespace InfoSelUI::Algorithms;

int Algorithm_Execution_(char *algitems,int algrdmode,datfile &datf,repfile &repf)
{
 int nalgs;
 try {
  char *c=algitems;
  if (*c=='\0') {
   nalgs=0;
   for (int i=0; i<Algs.count(); i++) {
    Algorithm::parstream algpars(cout,cin,algrdmode);
    Algs[i].execute(algpars,datf,repf,cout);
    nalgs++;
    datf.reinit(repf);
   }
  } else {
   nalgs=0;
   while (*c!='\0') {
    while (*c==' ') c++;
    if (*c=='\0') break;
    else 
    if (*c=='(') {
     while (*c!='\0') {
      while (*c==' ') c++;
      if (*c==')') break;
      enum {ppBegin,ppSequent,ppParall} pipemode;
      if (*c=='(') pipemode=ppBegin;
      else
      if (*c=='>') pipemode=ppSequent;
      else
      if (*c=='|') pipemode=ppParall;
      else break;
      c++;
      while (*c==' ') c++;
      if (*c=='\0' || *c==')') break;
      char *d=c;
      while (*d!='\0' && *d!=' ' && *d!='>' && *d!='|' && *d!=')') d++;
      char *e=d;
      while (*e==' ') e++;
      bool islast = *e=='\0' || *e==')';
      char s=*d;
      *d='\0';
      Algorithm &alg=Algs[c];
      if (pipemode==ppBegin) { if (!islast) datf.mkfirstpipestep(); }
      else if (!islast) datf.mknextpipestep(pipemode==ppSequent);
      else datf.mklastpipestep(pipemode==ppSequent);
      Algorithm::parstream algpars(cout,cin,algrdmode);
      alg.execute(algpars,datf,repf,cout);
      if (islast) {
       nalgs++;
       datf.reinit(repf);
      }
      *d=s;
      c=d;
     }
     c++;
    } else {
     char *d=c;
     while (*d!='\0' && *d!=' ' && *d!='(') d++;
     char s=*d;
     *d='\0';
     Algorithm &alg=Algs[c];
     Algorithm::parstream algpars(cout,cin,algrdmode);
     alg.execute(algpars,datf,repf,cout);
     nalgs++;
     datf.reinit(repf);
     *d=s;
     c=d;
    }
   }
  }
 }
 
 catch (...)
 {
  datf.reinit(repf);
  throw;
 }

 return nalgs;
}

int Algorithm_Choice_(datfile &datf,repfile &repf) throw()
{
 int Nalgs=0;
 do {
  char ans;
  int nalgs;
  try {
   static char items[150]; memset(items,'\0',sizeof items);
   int wid;
   int rdmode;
   
   cout << endl << "Selection algorithms: " << endl << endl;
   nalgs=0;
   wid=0;
   for (int i=0; i<Algs.count(); i++) {
    const Algorithm &alg=Algs[i];
    int n=alg.symbol().length();
    if (n>wid) wid=n;
   }
   {
    const int ncols=3;
    const int cnlen=26;
    const int lnlen=ncols*cnlen;
    const int N=Algs.count();
    static char line[lnlen];
    line[lnlen-1]='\0';
    int nlins=N/ncols;
    int nllcols=N%ncols;
    if (nllcols>0) nlins++;
    else nllcols=ncols;
    for (int l=0; l<nlins; l++) {
     memset(line,' ',lnlen-1);
     int ic=0;
     for (int c=0; c<(l<nlins-1?ncols:nllcols); c++) {
      const Algorithm &alg=Algs[ic+l];
      ostrstream bout(line+c*cnlen,cnlen-2);
      bout.setf(ios::left,ios::adjustfield);
      bout << '[' << setw(wid) << alg.symbol() << "] " << alg.caption(false,true);
      ic+=nlins-(c<nllcols?0:1);
     }
     cout << line << endl;
    }
   }
   cout << endl << ">> choose space-separated items (none = all): ";
   cin.ignore(1);
   cin.getline(items,sizeof items-1);
   cout << "Do you want to set up all/main/no algorithm parameters (a/m/n) ? "; cin >> ans;
   mkupper(ans);
   rdmode = ans=='A' ? Algorithm::rdAllPars : ans=='M' ? Algorithm::rdMainPars : Algorithm::rdNonePars;

   nalgs=Algorithm_Execution_(items,rdmode,datf,repf);
  }
  
  catch (const xAlgsError &exn)
  {
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  catch (const xDataError &exn)
  {
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  catch (const xProcError &exn)
  {
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  catch (const xCommError &exn)
  {
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  catch (const exception &e)
  {
   xUnexpctExn exn(e);
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  catch (...)
  {
   xUnknownExn exn;
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  Nalgs+=nalgs;
  cout << endl << "Executed successfully " << nalgs << " algorithm(s)" << endl;
  cout << endl << "Do you want to choose another algorithms (y/n) ? "; cin >> ans;
  if (toupper(ans)=='N') break;
 } while (true);
 
 return Nalgs; 
}    

} // end of namespace

void InteractionConsole() throw()
{
 using namespace InfoSelUI;
 using namespace InfoSelUI::Data;
 using namespace InfoSelUI::Algorithms;

 string repfn;
 cout << endl << "Output report file name: "; cin >> repfn;

 cout << endl << "opening the output report file...." << endl;
 repfile repf(repfn,ios::out|ios::app);

 do {
  char ans;
  try {
   static char line[150]; memset(line,'\0',sizeof line);
   string datfn;
   datfile::parsrec dpars;
   int itm;
   
   cout << endl;
   cout << "Input data file name: "; cin >> datfn;
   cout << "Is the classes column last (y/n) ? "; cin >> ans;
   dpars.lastc = toupper(ans)=='Y';
   if (!dpars.lastc)
   { cout << "Position of the classes column: "; cin >> dpars.cpos; }
   cout << "Exclusion classes set (space-separated): "; 
   cin.ignore(1);
   cin.getline(line,sizeof line-1);
   dpars.excl=line;
   if (!dpars.excl.empty()) {
    cout << "Are the above classes not excluded (y/n) ? "; cin >> ans;
    dpars.note = toupper(ans)=='Y';
    cin.ignore(1);
   }
   cout << "Mergence classes set (space-separated): ";
   cin.getline(line,sizeof line-1);
   dpars.merg=line;
   if (!dpars.merg.empty()) {
    cout << "Are the above classes not merged (y/n) ? "; cin >> ans;
    dpars.notm = toupper(ans)=='Y';
   }
   cout << "Real data precision (e.g. 0.001): "; cin >> dpars.prec;
   cout << endl << "Partition methods: " << endl;
   for (int c=0; c<PartMethSet::count(); c++)
   cout << "[" << c << "] - " << PartMethSet::name(c) << endl;
   cout << ">> choose item: "; cin >> itm;
   dpars.pmth=partmeth_t::create(itm,cout,cin);

   cout << endl << "opening the input data file...." << endl;
   datfile datf(datfn,dpars);

   datf.initiate(repf);

   int Nalgs=Algorithm_Choice_(datf,repf);
   if (Nalgs>0) repf.log() << endl << "Executed successfully " << Nalgs << " algorithm(s) ";
   else repf.log() << endl << "Warning: No algorithms have been executed ";
   repf.log() << "for the input data file: " << datfn;
   if (Nalgs>0) repf.log() << " ." << endl;
   else repf.log() << " !" << endl;

   datf.finalize(repf);
  }
  
  catch (const xInOutError &exn)
  {
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  catch (const exception &e)
  {
   xUnexpctExn exn(e);
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  catch (...)
  {
   xUnknownExn exn;
   cout << endl << exn << endl;
   repf.err() << endl << exn << endl;
  }
  
  cout << endl << "Do you want to process another data file (y/n) ? "; cin >> ans;
  if (toupper(ans)=='N') break;
 } while (true); 
}

