 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////  Algorithm Programming Interface Test  ////////
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
/// Entry Point For API Test
/**
 * @file       apitest.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.07.27
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains entry and master code for driving
 *             a test of the library algorithm programming interface.
 */

#include "InfoSelUI.h"
USEPACK(apitst)

int main(int argc, char *argv[])
{
 using namespace InfoSelUI;
 using namespace InfoSelUI::Data;
 using namespace InfoSelUI::Algorithms;

 entpoint ent(argc,argv,cin,cout);
 int res=ent.perform_processing("test_main",cout);
 if (res<0) return -1;
 else if (res>0) return 0;

 try {
  cout << endl << "opening the output report file...." << endl;
  repfile rep("apitest.rep",ios::out|ios::app);

  cout << endl << "opening the input data file...." << endl;
  datfile::parsrec dpars;
  dpars.prec=1e-8;
  dpars.lastc = true;
  dpars.pmth=partmeth_t::EquiWidth("5");
  datfile dat("apitest.dat",dpars);

  dat.initiate(rep);
  
  Algs["Test1"].execute(Algorithm::pars(),dat,rep,cout);
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

