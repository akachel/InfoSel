 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////          Library Front-end Test        ////////
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
/// Front-End Test Code
/**
 * @file       fetest.cpp/ipp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.07.27
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains main code for driving
 *             a test of the library front-end interface.
 */

#include "InfoSelLib.h"
using namespace std;
USEPACK(fetst)

int main(int argc, char *argv[])
{
 using namespace InfoSelLib;
 
 cout << endl << "testing command-line user interface...." << endl;
 CommandLine cln(argc,argv,cin,cout);
 {
  CommandLine::procres res=cln.process(cout);
  cout << endl << "command-line processing result: " << res << endl;
  if (res.fail()) return -1;
 }

 cout << endl << "testing script-batch user interface...." << endl;
 ScriptBatch bat("fetest.ipp");
 {
  ScriptBatch::procres res=bat.process(cout);
  cout << endl << "script-batch processing result: " << res << endl;
  if (res.fail()) return -1;
 } 

 cout << endl << "testing program-shell user interface...." << endl;
 ProgramShell shl("Front-end Test");
 {
  shl.setrepfn("fetest.rep");
  shl.setdatfn("fetest.dat");
  shl.setprec(0.001);
  shl.setcpos_last();
  // shl.setexcl("2",true); // shl.setexcl("void",true); --> excludes all classes
  // shl.setmerg("1,2",false); // shl.setmerg("void",true); --> merges all classes
  shl.setpartmth("EquiWidth","5");
  
  ProgramShell::Algorithm a("a"); // or "TestA"
  ProgramShell::Algorithm b("b"); // or "TestB"
  ProgramShell::Algorithm c("c"); // or "TestC"

  shl.exec(a("0.6")); // or shl.exec(Algorithm("a")("0.6"));
  shl.exec(b("missing,7")); // missing --> sets up default value i.e. 0.5
  shl.exec(a("8") > b("2,4") | c);
  // shl.execall(); 
  
  shl.flush();
  ProgramShell::procres res=shl.status(cout);
  cout << endl << "program-shell processing result: " << res << endl;
  if (res.fail()) return -1;
 }

 return 0;
}    

