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
/// Defintions Of Algorithm Base Elements
/**
 * @file       albas.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains defintions of selection algorithm related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dfile.h"
#include "albas.h"

namespace InfoSelCore {
 
namespace Algorithms {

/////////////////////////////
////
//// exception type definitions
////

void xUnknownAlgorithm::printmsg_(ostream &out) const
{
 out << "Error: Unknown feature selection algorithm, no registering or using directive was applied";
}

/////////////////////////////
////
//// AlgorithmBase class 
//// definition
////

AlgorithmBase::AlgorithmBase(const string &symbol):
 symbol_(symbol), execing_(false), 
 curvnt_(0), prevnt_(-1),
 inicard_(0), fincard_(0), selmax_(0)
{
 toident_(symbol_);
 if (symbol_.empty()) symbol_="xyz";
}
  
void AlgorithmBase::toident_(string &str)
{ 
 string::iterator b=str.begin(); 
 string::iterator e=str.end(); 
 string::iterator d=b; 
 while (d!=e) {
  string::iterator c=d;
  d++;
  if (*c==' ' || *c=='-') *c='_';
  else if (*c!='_' && !isalnum(*c)) str.erase(c);
 }
}

AlgorithmBase::~AlgorithmBase()
{}

string AlgorithmBase::caption(bool shorten,bool novnt) const
{
 string cap;
 string pck=package();
 if (!pck.empty()) mkupper(pck[0]);
 string nam=name();
 if (!nam.empty()) mkupper(nam[0]);
 if (!pck.empty()) 
 if (!shorten) cap+=pck+":"; 
 else {
  string x=tolower(pck);
  string y=tolower(nam);
  if (y.find(x)==string::npos) cap+=pck+":";
 } 
 cap+=nam;
 replace(cap.begin(),cap.end(),'_','-');
 int nvnts=nvariants();
 if (nvnts>0)
 if (shorten) {
  int vnt = novnt ? prevnt_ : curvnt_;    
  if (vnt>=0 && vnt<nvnts) cap+="-"+tostring(vnt);
 } else
 if (!novnt) {
  string vntlab=vlabel();    
  cap+="-"+vntlab;
 } 
 return cap;
}

string AlgorithmBase::qname() const
{
 string n=name();
 if (package_.empty()) return n;
 else return package_+"_"+n;
}

const string &AlgorithmBase::name() const
{
//
// Instead of defining an extra virtual
// method we extract an algorithm name
// from the mangled class name returned
// by the typeid operator. To avoid considering
// different mangling formats we assume a common
// class name format as follows: 
//        AAAADDDD
// where AAAA is a sequence of at least three
// alpha or underscore chars and DDDD is an optional 
// digit sequence. We just try to find this format
// at the end of the mangled name. Unfortunately,
// this method is not quit general, so some corrections 
// should be added in the special cases.
//
 if (name_.empty()) {
  const int minalp=3;
  string tnam=typeid(*this).name();
  string::iterator c=tnam.end();
  string::iterator b=tnam.begin();
  while (c>b) {
   c--;
   while (c>=b && !(*c=='_' || isalpha(*c) || isdigit(*c))) c--;
   string::iterator d=c+1;
   while (c>=b && isdigit(*c)) c--;
   string::iterator e=c+1;
   while (c>=b && (*c=='_' || isalpha(*c))) c--;
   c++;
   if (e-c < minalp) continue;
   else {
#ifdef __GNUC__
    if (*(d-1)=='E') d--;
#endif
    name_.assign(c,d);
    break;
   }
  }
  toident_(name_);
  if (name_.empty()) name_="XxxYyyZzz";
 }
 return name_;
}

string AlgorithmBase::qsymbol() const
{
 string s=symbol();
 if (package_.empty()) return s;
 else return package_+"_"+s;
}

string AlgorithmBase::vlabels(int vnt) const
{
 string lab=get_variantlabs_(vnt);
 toident_(lab);
 if (lab.empty()) lab="var_"+tostring(vnt+1);
 return lab;
}

int AlgorithmBase::cutvariant(string &tag) const
{
 int vnt=-1;
 if (tag.empty()) return vnt;
 string::size_type pos=tag.find_last_of('_');
 if (pos==string::npos) return vnt;
 string suffix=tag.substr(pos+1);
 if (suffix.empty()) return vnt;
 if (!parse(suffix,vnt)) {
  mklower(suffix);
  int nvnts=nvariants();
  for (int v=0; v<nvnts; v++) {
   string vlab=vlabels(v);
   mklower(vlab);
   if (suffix==vlab) {
    vnt=v;
    break;
   }
  } 
 }
 if (vnt>=0) tag.erase(pos);
 return vnt;
}

bool AlgorithmBase::istagged(const string &tag) const
{ 
 string t=tag;
 toident_(t);
 return t==symbol() || t==qsymbol() || t==name() || t==qname(); 
} 

int AlgorithmBase::get_numvariants_() const
{ 
 return 0;
}

string AlgorithmBase::get_variantlabs_(int) const
{
 return "";
}

string AlgorithmBase::get_pars_decl_() const
{
 return "";
}

string AlgorithmBase::get_pars_desc_() const
{
 return "";
}

string AlgorithmBase::get_body_desc_() const
{
 return "";
}

string AlgorithmBase::description() const
{
 ostringstream bout;

 int nvnts=nvariants();
 bout << qsymbol() << " / " << name() << " (";
 if (nvnts>0)
 bout << "int variant=0, ";
 string pardecl=get_pars_decl_();
 if (!pardecl.empty()) bout << pardecl << ", ";
 bout << "int selmax=-1)";
 string boddesc=get_body_desc_();
 if (!boddesc.empty()) {
  mkupper(boddesc[0]);
  bout << "\n    " << boddesc;
  if (boddesc[boddesc.length()-1]!='.') bout << '.';
 } 
 if (nvnts>0) {
  bout << "\n    Variants:";
  for (int v=0; v<nvnts; v++) {
   string vlab=vlabels(v);
   if (!vlab.empty())
   bout << "\n\t [" << v << "] - " << vlab;
  } 
 }
 bout << "\n    Parameters:";
 if (nvnts>0)
 bout << "\n\t variant - zero-based number for the algorithm's variant";
 string pardesc=get_pars_desc_();
 if (!pardesc.empty()) {
  string::iterator b=pardesc.begin();
  string::iterator e=pardesc.end();
  string::iterator c=b, d=e-1;
  while (c!=e) {
   if (*c==';' && c>b && c<d) *(c-1)='\n', *c='\t', *(c+1)=' ';
   c++;
  }
  bout << "\n\t " << pardesc;
 }
 bout << "\n\t selmax - max. num. of selected features (-1 = total)";

 return bout.str();
}

void AlgorithmBase::read_(parstream &pars,const Data::file &,Processing::report &rep)
{
 read_(pars,rep);
}

void AlgorithmBase::read_(parstream &,Processing::report &)
{}

void AlgorithmBase::call_read_(parstream &pars,const Data::file &file,Processing::report &rep)
{
 rep.flush();

 inicard_=file.iniset().card();
 fincard_=file.finset().card();

 int nvnts=nvariants();
 if (nvnts>0)
 if (prevnt_<0) {
  ostringstream bout;
  bout << "all variants:" << endl;
  for (int v=0; v<nvnts; v++) {
   string vlab=vlabels(v);
   if (!vlab.empty())
   bout << " [" << v << "] - " << vlab << endl;
  } 
  bout << "choosen item";
  pars(bout.str(),prMain,rep.log())
      (0) >> prevnt_;
  int vnt=variant();    
  rep.cut().log() << "choosen variant: " << vnt << " (" << vlabels(vnt) << ')' << endl;
 } else {
  int vnt=variant();    
  rep.cp().log() << "given variant: " << vnt << " (" << vlabels(vnt) << ')' << endl;
 }
 
 read_(pars,file,rep);
 
 pars("maximal number of selected features (-1 = total)",prOther,rep.log())
     (-1) >> selmax_;
     
 if (nvnts>0) {
  if (prevnt_>=0 && prevnt_<nvnts) curvnt_=prevnt_;
  prevnt_=-1;
 } 
 int nfeas=file.features().card();
 if (selmax_<0 || selmax_>nfeas) selmax_=nfeas;
}

void AlgorithmBase::call_run_(Data::file &file,Processing::report &rep,real &tim)
{
 clock_t t0=clock();
 
 run_(file,rep);
 
 clock_t t1=clock();
 tim=(real)(t1-t0)/CLOCKS_PER_SEC;
}

void AlgorithmBase::execute(parstream &pars,Data::file &file,Processing::report &rep,ostream &log)
{
 if (execing_) return;
 execing_=true;

 rep.flush();
 if (file.procescnt()<=0) {
  rep.section("Selection Algorithms");
  rep.seclevel(rep.seclevel()+1);
 } 
 
 bool logisprom = log==pars.prompt();
 log << endl; 
 if (!logisprom)
 log << "executing " << flush;
 
 string cap=caption(true,true);
 if (!file.pipeprocessing()) {
  const char *s=" algorithm";
  rep.cp().section(cap+s);
  log << cap << s; 
  if (logisprom) log << ':' << endl;
  else log << "...." << endl;
 } else {
  const char *s1=" , hybrid-algorithm ";
  int n=file.pipestep()+1;
  const char *s2 = file.parallpipe() ? "parallel component # " : "component # ";
  rep.cp().section(cap+s1+s2+tostring(n));
  log << cap << s1 << s2 << n;
  if (logisprom) log << " :" << endl;
  else log << " ...." << endl;
 }
 {
  rep.cp().log() << endl;
  if (logisprom) log << endl;
  streampos rp=rep.tellp();
  streampos lp=log.tellp();
  call_read_(pars,file,rep);
  if (rep.tellp()!=rp)
  rep.log() << endl;
  if (log.tellp()!=lp || pars.mode()!=rdNonePars)
  if (logisprom) log << endl;
 }
 
 rep.seclevel(rep.seclevel()+1);
 if (logisprom) 
 log << "running" << "...." << endl;
 
 real t;
 try {
  t=0.0;
  call_run_(file,rep,t);
 }
 catch (...)
 {
  rep.log() << endl;
  file.finset().print_order(rep.cp().log()); 
  rep.cp().log() << endl;
  rep.log() << "(run was broken)" << endl;
  rep.seclevel(rep.seclevel()-1);
  execing_=false;
  throw;
 }

 rep.log() << endl;
 file.finset().print_order(rep.cp().log());
 rep.cp().log() << endl;
 rep.log() << "(run used: " << t << " s)" << endl;
 rep.seclevel(rep.seclevel()-1);

 execing_=false;
}

/////////////////////////////
////
//// algs_collection_base_ 
//// class definition
////

AlgorithmBase *algs_collection_base_::reginsts_[maxnitems_]={0};
int algs_collection_base_::nreginsts_=0;
bool algs_collection_base_::isunstructed_=true;

int algs_collection_base_::register_(const char *pack,AlgorithmBase *inst)
{
 int ireg;

 if (strlen(pack)<=0 || inst==0) ireg=-1;
 else {
  bool doreg=true;
  if (nreginsts_>=maxnitems_) doreg=false;
  else 
  for (int i=0; i<maxnitems_; i++) {
   AlgorithmBase *reg=reginsts_[i];
   if (reg!=0 && (inst==reg || typeid(*inst)==typeid(*reg))) doreg=false;
  } 

  if (doreg) {
   ireg=nreginsts_++;
   reginsts_[ireg]=inst;
   inst->qualification(pack);
   isunstructed_=true; 
  } else {
   delete inst;
   ireg=-1;
  }
 }
 
 return ireg;
}

void algs_collection_base_::unregister_(int &ireg)
{
 if (ireg>=0 && ireg<maxnitems_) {
  delete reginsts_[ireg];
  reginsts_[ireg]=0;
  nreginsts_--;
  ireg=-1;
  isunstructed_=true;
 }
}

void algs_collection_base_::construct_()
{
 isunstructed_=false; 
 int k=0;
 for (int i=0; i<maxnitems_ && k<nreginsts_; i++)
 if (reginsts_[i]!=0) k++;
 else {
  int j;
  for (j=i; j<maxnitems_-1; j++) reginsts_[j]=reginsts_[j+1];
  reginsts_[j]=0;
 } 
}

} // end of Algorithms namespace

} // end of InfoSelCore namespace
