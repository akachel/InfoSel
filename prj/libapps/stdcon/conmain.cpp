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
/// Entry Code For Standard Console Application
/**
 * @file       conmain.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains entry code for driving the InfoSel++ 
 *             library application with standard console user interface.
 */

#include "InfoSelUI.h"
extern void InteractionConsole() throw();

int main(int argc,char *argv[])
{
 using namespace InfoSelUI;

 entpoint ent(argc,argv,cin,cout);
 int res=ent.perform_processing("console_main",cout);
 
 if (res<0) return -1;
 else if (res==0) InteractionConsole();

 return 0;
}    

