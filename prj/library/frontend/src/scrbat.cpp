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
/// Code Of Script Batch Front-End Interface
/**
 * @file       scrbat.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.07.27
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
//// ScriptBatch class
//// definition
////

ScriptBatch::ScriptBatch(istream &in,const string &name)
{
 rep_=new InfoSelUI::ctlfile(in,name.empty() ? string("script_batch") : name);
}

ScriptBatch::ScriptBatch(const string &name)
{
 rep_=new InfoSelUI::ctlfile(name);
}

ScriptBatch::~ScriptBatch()
{
 InfoSelUI::ctlfile *ctl=reinterpret_cast<InfoSelUI::ctlfile *>(rep_);
 delete ctl;
}

void ScriptBatch::grammar(ostream &out)
{
 InfoSelUI::ctlfile::print_grammar(out);
}

ScriptBatch::procres ScriptBatch::process(ostream &log) 
{
 log << endl << "loading InfoSel++ batch...." << endl;

 InfoSelUI::ctlfile &ctl=*reinterpret_cast<InfoSelUI::ctlfile *>(rep_);
 ostringstream ctllog;
 int ctlres=ctl.perform_processing(ctllog);
 string ctllogs=ctllog.str();

 if (ctlres>0) {
  datfns_.clear();
  KernelEntry::datfnames(ctllogs,datfns_);
  repfns_.clear();
  KernelEntry::repfnames(ctllogs,repfns_);
 }

 log << ctllogs;
 return ctlres;
}

ScriptBatch::procres ScriptBatch::process()
{
 return process(InfoSelUI::nullstream::out);
}

} // end of InfoSelLib namespace

