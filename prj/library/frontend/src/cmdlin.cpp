 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////       Library Front-End Interfaces     ////////
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
/// Code Of Command Line Front-End Interface
/**
 * @file       cmdlin.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2009.03.20
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains code of the InfoSel++ front-end 
 *             interfaces based on the low-level library user interface.
 */

#include "InfoSelLib.h"
#include "InfoSelUI.h"

namespace InfoSelLib {

/////////////////////////////
////
//// CommandLine class
//// definition
////

CommandLine::CommandLine(int argc,char *argv[],istream &a_stdin,ostream &a_stdout)
{
 rep_=new InfoSelUI::entpoint(argc,argv,a_stdin,a_stdout);
}

CommandLine::CommandLine(int argc,char *argv[],istream &a_stdin)
{
 rep_=new InfoSelUI::entpoint(argc,argv,a_stdin,InfoSelUI::nullstream::out);
}

CommandLine::CommandLine(int argc,char *argv[],ostream &a_stdout)
{
 rep_=new InfoSelUI::entpoint(argc,argv,InfoSelUI::nullstream::in,a_stdout);
}

CommandLine::CommandLine(int argc,char *argv[])
{
 rep_=new InfoSelUI::entpoint(argc,argv,InfoSelUI::nullstream::in,InfoSelUI::nullstream::out);
}

CommandLine::~CommandLine()
{
 InfoSelUI::entpoint *ent=reinterpret_cast<InfoSelUI::entpoint *>(rep_);
 delete ent;
}

void CommandLine::syntax(ostream &out)
{
 InfoSelUI::entpoint::print_syntax(out);
}

CommandLine::procres CommandLine::process(ostream &a_stdlog)
{
 InfoSelUI::entpoint &ent=*reinterpret_cast<InfoSelUI::entpoint *>(rep_);
 int ret=ent.perform_processing("command_line",a_stdlog);
 return ret;
}

CommandLine::procres CommandLine::process()
{
 return process(InfoSelUI::nullstream::out);
}

} // end of InfoSelLib namespace

