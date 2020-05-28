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
/// Definitions For Input Data File
/**
 * @file       dfile.cpp
 * @version    1.00
 * @author     Adam Kachel, Jacek Biesiada
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains defintions of input data related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dfile.h"

namespace InfoSelCore {

namespace Data {

/////////////////////////////
////
//// datfile class definition
////

datfile::parsrec::parsrec(): 
 prec(0.001), pmth(partmeth_t::EquiWidth("24")), 
 cpos(0), lastc(true), 
 excl(""), note(false),
 merg(""), notm(false)
{}

datfile::parsrec::~parsrec()
{}

datfile::datfile(const string &name,const parsrec &pars,int nimdsets,int nauxsets):
 name_(name), prec_(0), 
 nrows_(0), ncols_(0), ninsts_(0), clspos_(0), exclstr_(""), mergstr_(""), 
 orgset_(0), orgcls_(0), iniset_(0), finset_(0), imdsets_(0), auxsets_(0),
 nimdsets_(nimdsets<2 ? 2 : nimdsets), nauxsets_(nauxsets<1 ? 1 : nauxsets), 
 iniset_ptr_(0), finset_ptr_(0), imdset_idx_(-1), iexset_ptr_(0),
 pipestep_(-1), islastpipes_(false), serlpipe_(false), procescnt_(0)
{
 ifstream fin(name_.c_str());
 if (!fin) throw xFileOpenFail(name_);
 ostringstream bout;
 if (!rdtranslate_(pars,fin,bout)) throw xWrongInputFile(name_);
 istringstream bin(bout.str());
 rdpreprocess_(pars,bin);

 iniset_=new datset("initial (F)",*orgset_);
 iniset_ptr_=iniset_;
 iniset_->nlinks_++;
 finset_=new datset("final (S)",*orgset_);
 finset_ptr_=finset_;
 finset_->nlinks_++;
 imdsets_=new datset*[nimdsets_];
 for (int k=0; k<nimdsets_; k++) {
  datset *s=new datset(string("intermediate (FS")+tostring(k+1)+")",*orgset_);
  imdsets_[k]=s;
  s->nlinks_++;
 } 
 iexset_ptr_=imdsets_[nimdsets_-1];
 auxsets_=new datset*[nauxsets_];
 for (int k=0; k<nauxsets_; k++) {
  datset *s=new datset(string("auxiliary (G")+tostring(k+1)+")",*orgset_);
  auxsets_[k]=s;
  s->nlinks_++;
 } 

 *iniset_ <<= *orgset_;
 *orgset_ += *iniset_;
}

bool datfile::rdtranslate_(const parsrec &pars,istream &in,ostream &out)
{
 int cpos = pars.lastc || pars.cpos<0 ? iMax : pars.cpos;
 const string &excl=pars.excl;
 string excls=excl+" ";
 bool note=pars.note;
 const string &merg=pars.merg;
 string mergs=merg+" ";
 bool notm=pars.notm;

 if (!excl.empty()) exclstr_ = (note ? "~{ " : "{ ")+excl+" }";
 else if (note) exclstr_ = "{ * }";
 if (!merg.empty()) mergstr_ = (notm ? "~{ " : "{ ")+merg+" }";
 else if (notm) mergstr_ = "{ * }";

 string iln;
 getline(in,iln);
 string mergcls;
 while (!in.eof()) {
  if (!iln.empty()) {
   const string::iterator ilnb=iln.begin();
   const string::iterator ilne=iln.end();
   string::iterator c=ilnb;
   string::iterator clsb=ilnb, clse=ilne;
   if (*c==',' || *(ilne-1)==',') return false;
   int ncols=1;
   while (c!=ilne) {
    if (*c==',') {
     *c=' '; 
     ncols++;
     if (cpos>=ncols-1) clsb=c+1; 
     else if (cpos==ncols-2) clse=c;
    }
    c++;
   }
   while (*clsb==' ' && clsb<clse) clsb++;
   while (clse>clsb && *(clse-1)==' ') clse--;
   if (clsb>=clse) return false;
   if (ncols_<=0) ncols_=ncols;
   else if (ncols!=ncols_) return false;
   nrows_++;
   
   string cls=iln.substr(clsb-ilnb,clse-clsb);
   iln.erase(clsb,clse);
   bool isclsex = !excl.empty() && excls.find(cls+" ")!=string::npos;
   if (isclsex && note || !isclsex && !note) {
    ninsts_++;
    bool isclsmr = !merg.empty() && mergs.find(cls+" ")!=string::npos;
    if (!(isclsmr && notm || !isclsmr && !notm)) 
    if (mergcls.empty()) mergcls=cls;
    else cls=mergcls;
    out << iln << ' ' << cls << endl;
   }
  }
  getline(in,iln);
 }
 if (ncols_<=0 || ninsts_<=0) return false;
 int cidx=ncols_-1;
 clspos_ = cpos>cidx ? cidx : cpos;

 return true;
}

void datfile::rdpreprocess_(const parsrec &pars,istream &in)
{
 int nfeas=ncols_-1;
 int clsid=ncols_-1;
 
 orgset_=new datset("original (F)","feature",nfeas,nfeas,ninsts_);
 orgcls_=new datcolumn("classes (C)",clsid,ninsts_);

 for (int i=0; i<ninsts_; i++) {
  string iln;
  getline(in,iln);
  istringstream bin(iln);
  (*orgset_)[i].read_val_(bin);
  (*orgcls_)[i].read_val_(bin);
 }

 real prec = pars.prec>rPrec && pars.prec<1.0 ? pars.prec : 0.001;
 real r_prec=1.0/prec;
 orgset_->precision(prec,r_prec);
 orgcls_->precision(prec,r_prec);
 prec_=prec;

 orgset_->partition(pars.pmth);
 int np=orgcls_->partition(partmeth_t::FineGrain());
 if (np>10*orgset_->nsglparts()) orgcls_->partition(pars.pmth);
 
 orgset_->nlinks_++;
 orgcls_->nlinks_++;
}

datfile::~datfile()
{
 for (int k=0; k<nauxsets_; k++) auxsets_[k]->nlinks_--, delete auxsets_[k];
 delete[] auxsets_;
 for (int k=0; k<nimdsets_; k++) imdsets_[k]->nlinks_--, delete imdsets_[k];
 delete[] imdsets_;
 finset_->nlinks_--; delete finset_;
 iniset_->nlinks_--; delete iniset_;
 orgcls_->nlinks_--; delete orgcls_;
 orgset_->nlinks_--; delete orgset_;
}

void datfile::initiate(Processing::report &rep,bool more_info) 
{
 if (procescnt_>0) throw xFileProcFail();
 if (rep.prec()<prec_) rep.precision(prec_);

 ostream &out=rep.flush().seclevel(0).section("Input Data").log();
 
 out << endl;
 out << "File name: " << name_ << endl;
 out << "Data precision: " << prec_ << endl;
 if (ninsts_!=nrows_)
 out << "Number of all data rows: " << nrows_ << endl;
 out << "Classes column position: " << clspos_ << endl;
 if (!exclstr_.empty())
 out << "Exclusion classes set: " << exclstr_ << endl;
 if (!mergstr_.empty())
 out << "Mergence classes set: " << mergstr_ << endl;
 
 if (more_info) {
  out << endl;
  out << "File contents:" << endl;
  print_conts(out);
 }
 
 out << endl;
 iniset_->print_stats(out);
 out << endl;
 finset_->print_stats(out);
 out << endl;
 orgcls_->print_stats(out);
}

void datfile::reinit(Processing::report &rep,bool more_info)
{
 ostream &out=rep.flush().log();

 if (more_info) {
  out << endl;
  out << "Input file processing results:" << endl;
  iniset_ptr_->print_order(out); 
  out << endl;
  finset_ptr_->print_order(out); 
  out << endl;
 }

 do {
  *iniset_ <<= *finset_ptr_;
  finset_ptr_->reinitiate();
  if (imdset_idx_<=0) break;
  finset_ptr_=imdsets_[--imdset_idx_];
 } while (true);
 for (int k=0; k<nauxsets_; k++) {
  datset &A=*auxsets_[k];
  if (!A.isempty()) *iniset_ <<= A;
  A.reinitiate();
 }
 *iniset_ += *orgset_;

 iniset_ptr_=iniset_;
 finset_ptr_=finset_;
 imdset_idx_=-1;
 pipestep_=-1;
 islastpipes_=false;
 serlpipe_=false;
 procescnt_++;

 orgset_->reinitiate();
 orgcls_->reinitiate();
 finset_->reinitiate();
 iniset_->reinitiate();
}

void datfile::finalize(Processing::report &rep,bool less_info)
{
 ostream &out=rep.flush().seclevel(0).log(); 

 if (!less_info) {
  rep.summary(); 
  if (procescnt_>0) out << endl << "Input data file was " << procescnt_ << " times processed." << endl;
  else out << endl << "Warning: Input data file was preprocessed only !" << endl;
 } 

 out << endl;
 out << endl;
 out << endl;
}

void datfile::mkfirstpipestep()
{
 if (pipestep_>=0) return;
 pipestep_=0;
 imdset_idx_=0;
 iniset_ptr_=iniset_;
 finset_ptr_=imdsets_[imdset_idx_++];
}
  
void datfile::mknextpipestep(bool serially)
{
 if (pipestep_<0 || islastpipes_) return;
 if (imdset_idx_>=nimdsets_-1) throw xTooManyImdSets();
 for (int k=0; k<nauxsets_; k++) {
  datset &A=*auxsets_[k];
  if (!A.isempty()) *iexset_ptr_ <<= A;
  A.reinitiate();
 }
 pipestep_++;
 serlpipe_=serially;
 if (serially) iniset_ptr_=finset_ptr_;
 finset_ptr_=imdsets_[imdset_idx_++];
 orgset_->reinitiate();
 orgcls_->reinitiate();
 iniset_ptr_->reinitiate();
}
  
void datfile::mklastpipestep(bool serially)
{
 if (pipestep_<0 || islastpipes_) return;
 for (int k=0; k<nauxsets_; k++) {
  datset &A=*auxsets_[k];
  if (!A.isempty()) *iexset_ptr_ <<= A;
  A.reinitiate();
 }
 pipestep_++;
 islastpipes_=true;
 serlpipe_=serially;
 if (serially) iniset_ptr_=finset_ptr_;
 finset_ptr_=finset_;
 orgset_->reinitiate();
 orgcls_->reinitiate();
 iniset_ptr_->reinitiate();
}

datset *datfile::auxset_ptr_(int n) const
{ 
 int k=n-1;
 if (k>=nauxsets_) throw xTooManyAuxSets();
 return auxsets_[k]; 
}

void datfile::print_conts(ostream &out) const
{
 const int nshr=10;
 bool isshr = iosex::isshortened(out) && ninsts_>nshr;
 int n = isshr ? nshr : ninsts_;
 for (int i=0; i<n; i++)
 out << (*orgset_)[i] << "  -->  " << (*orgcls_)[i] << endl;
 if (isshr) out << "..." << endl;
}

} // end of Data namespace

} // end of InfoSelCore namespace

