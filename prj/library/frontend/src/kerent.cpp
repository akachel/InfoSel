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
/// Code Of Kernel Entry For Front-End Interfaces
/**
 * @file       kerent.cpp
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

int KernelEntry::apply_using_directive_(const char *pack,int &iuse)
{ return InfoSelUI::Algorithms::algs_collection_accessor_::apply_using_directive_(pack,iuse); }

int KernelEntry::partmethcount()
{ return InfoSelUI::Data::PartMethSet::count(); }

string KernelEntry::partmethname(int c)
{ return InfoSelUI::Data::PartMethSet::name(c); }

string KernelEntry::partmethdesc(int c)
{ return InfoSelUI::Data::PartMethSet::description(c); }

int KernelEntry::selalgcount()
{ return InfoSelUI::Algorithms::Assembly.count(); }

string KernelEntry::selalgpack(int i)
{ return InfoSelUI::Algorithms::Assembly[i].package(); }

string KernelEntry::selalgname(int i)
{ return InfoSelUI::Algorithms::Assembly[i].name(); }

string KernelEntry::selalgsymb(int i)
{ return InfoSelUI::Algorithms::Assembly[i].symbol(); }

string KernelEntry::selalgdesc(int i)
{ return InfoSelUI::Algorithms::Assembly[i].description(); }

bool KernelEntry::datfnames(const string &logs,list<string> &nams) 
{
 bool found=false;
 
 const string::size_type e=string::npos;
 string::size_type i=0;
 while (i!=e) {
  i=logs.find("input data file",i);
  if (i!=e) {
   i=logs.find(":",i);
   while (logs[++i]==' ');
   string fn=logs.substr(i,logs.find(" ",i)-i);
   nams.insert(nams.end(),fn);
   found=true;
  }
 } 
 nams.sort();
 nams.unique();

 return found;
}

bool KernelEntry::repfnames(const string &logs,list<string> &nams) 
{
 bool found=false;
 
 const string::size_type e=string::npos;
 string::size_type i=0;
 while (i!=e) {
  i=logs.find("output report file",i);
  if (i!=e) {
   i=logs.find(":",i);
   while (logs[++i]==' ');
   string nam=logs.substr(i,logs.find(" ",i)-i);
   nams.insert(nams.end(),nam);
   found=true;
  }
 } 
 nams.sort();
 nams.unique();

 return found;
}

} // end of InfoSelLib namespace

