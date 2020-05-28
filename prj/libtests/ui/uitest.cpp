 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////           User Interface Test          ////////
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
/// UI Test Code
/**
 * @file       uitest.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.07.27
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains main code for driving
 *             a test of the library user interface.
 */

#include "InfoSelUI.h"
USEPACK(uitst)

int main(int argc, char *argv[])
{
 using namespace InfoSelUI;
 using namespace InfoSelUI::Data;
 using namespace InfoSelUI::Algorithms;

 entpoint ent(argc,argv,cin,cout); // uses ctlfile internally
 int res=ent.perform_processing("test_main",cout);
 if (res<0) return -1;
 else if (res>0) return 0;

 try {
  cout << endl << "opening the output report file...." << endl;
  repfile rep("uitest.rep",ios::out|ios::app);

  cout << endl << "opening the input data file...." << endl;
  datfile::parsrec dpars;
  dpars.prec=0.001;
  dpars.lastc = true;
  dpars.excl="";
  dpars.merg="";
  dpars.pmth=partmeth_t::EquiWidth("5");
  datfile dat("uitest.dat",dpars);
  
  dat.initiate(rep);
  
  Algs["a"].execute(Algorithm::pars("0.1",Algorithm::rdMainPars),dat,rep,cout);
  dat.reinit(rep);

  Algs["b"].execute(Algorithm::pars("0.2 *"),dat,rep,cout);
  dat.reinit(rep);

  Algs["c"].execute(Algorithm::pars("0.3"),dat,rep,cout);
  dat.reinit(rep);

  dat.finalize(rep);
 }
 
 catch (const xError &exn)
 {
  cout << endl << exn << endl;
  return -1;
 }
 
 catch (const exception &e)
 {
  xUnexpctExn exn(e);
  cout << endl << exn << endl;
  return -1;
 }
 
 catch (...)
 {
  xUnknownExn exn;
  cout << endl << exn << endl;
  return -1;
 }
 
 return 0;
}    

