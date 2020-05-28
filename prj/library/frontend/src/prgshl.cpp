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
/// Code Of Program Shell Front-End Interface
/**
 * @file       prgshl.cpp
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
//// ProgramShell::Algorithm 
//// class definition
////

namespace { namespace ProgramShell_ { namespace Algorithm_ {

const char *is_hybrid_="$hybrid$";

struct proxy_representation_ {
 string tag;
 string pars;
 string invo;

 void init(const string &t)
 {
  tag=t;
  pars="";
  invo=t;
 }
 
 void copy(const proxy_representation_ &x)
 {
  tag=x.tag;
  pars=x.pars;
  invo=x.invo;
 }
};

}}}

ProgramShell::Algorithm::Algorithm(const string &tag)
{
 using namespace ProgramShell_::Algorithm_;
 rep_=new proxy_representation_;
 proxy_representation_ *that=reinterpret_cast<proxy_representation_ *>(rep_);
 that->init(tag);
}

ProgramShell::Algorithm::~Algorithm()
{
 using namespace ProgramShell_::Algorithm_;
 proxy_representation_ *that=reinterpret_cast<proxy_representation_ *>(rep_);
 delete that;
}

ProgramShell::Algorithm::Algorithm(const ProgramShell::Algorithm &A)
{
 using namespace ProgramShell_::Algorithm_;
 rep_=new proxy_representation_;
 proxy_representation_ *that=reinterpret_cast<proxy_representation_ *>(rep_);
 const proxy_representation_ *A_that=reinterpret_cast<const proxy_representation_ *>(A.rep_);
 that->copy(*A_that);
}

ProgramShell::Algorithm &ProgramShell::Algorithm::operator=(const ProgramShell::Algorithm &A)
{
 using namespace ProgramShell_::Algorithm_;
 {
  proxy_representation_ *that=reinterpret_cast<proxy_representation_ *>(rep_);
  delete that;
 }
 { 
  rep_=new proxy_representation_;
  proxy_representation_ *that=reinterpret_cast<proxy_representation_ *>(rep_);
  const proxy_representation_ *A_that=reinterpret_cast<const proxy_representation_ *>(A.rep_);
  that->copy(*A_that);
 } 
 return *this;
}

ProgramShell::Algorithm &ProgramShell::Algorithm::operator()(const string &pars)
{
 using namespace ProgramShell_::Algorithm_;
 proxy_representation_ *that=reinterpret_cast<proxy_representation_ *>(rep_);
 if (that->tag.find(is_hybrid_)!=string::npos) return *this; // no parameter setup provided for hybrids
 that->pars=pars;
 if (pars.empty()) that->invo=that->tag;
 else that->invo=that->tag+"("+pars+")";
 return *this;
}
  
ProgramShell::Algorithm ProgramShell::Algorithm::operator>(const ProgramShell::Algorithm &A) const
{
 using namespace ProgramShell_::Algorithm_;
 const proxy_representation_ *that=reinterpret_cast<const proxy_representation_ *>(rep_);
 const proxy_representation_ *A_that=reinterpret_cast<const proxy_representation_ *>(A.rep_);
 ProgramShell::Algorithm res(is_hybrid_+that->tag+">"+A_that->tag);
 proxy_representation_ *res_that=reinterpret_cast<proxy_representation_ *>(res.rep_);
 res_that->invo=that->invo+" > "+A_that->invo;
 res_that->pars=that->pars+" , "+A_that->pars; // all hybrid parameter list
 return res;
}

ProgramShell::Algorithm ProgramShell::Algorithm::operator|(const ProgramShell::Algorithm &A) const
{
 using namespace ProgramShell_::Algorithm_;
 const proxy_representation_ *that=reinterpret_cast<const proxy_representation_ *>(rep_);
 const proxy_representation_ *A_that=reinterpret_cast<const proxy_representation_ *>(A.rep_);
 ProgramShell::Algorithm res(is_hybrid_+that->tag+"|"+A_that->tag);
 proxy_representation_ *res_that=reinterpret_cast<proxy_representation_ *>(res.rep_);
 res_that->invo=that->invo+" | "+A_that->invo;
 res_that->pars=that->pars+" , "+A_that->pars; // all hybrid parameter list
 return res;
}

const string &ProgramShell::Algorithm::tag() const
{ 
 using namespace ProgramShell_::Algorithm_;
 const proxy_representation_ *that=reinterpret_cast<const proxy_representation_ *>(rep_);
 return that->tag; 
}

const string &ProgramShell::Algorithm::pars() const
{ 
 using namespace ProgramShell_::Algorithm_;
 const proxy_representation_ *that=reinterpret_cast<const proxy_representation_ *>(rep_);
 return that->pars; 
}

/////////////////////////////
////
//// ProgramShell class 
//// definition
////

namespace { namespace ProgramShell_ {

const string sEmpty_="";

struct object_representation_ {
 string ctlcap;
 ostringstream ctlout;
 string ctllogs;
 int ctlres;
 bool flushed;
 
 string datfn;
 string repfn;
 double prec;
 string pmname;
 string pmpars;
 int cpos;
 string excl;
 bool note;
 string merg;
 bool notm;

 void print_ctlcap()
 { 
  if (ctlcap.empty()) return;
  
  ctlout << '#' << endl << '#' << endl << "\\#" << endl 
         << endl << ctlcap << endl 
         << endl << "#\\" << endl << '#' << endl << '#' << endl << endl; 
 }
 
 void init(const string &cap);
 void reinit() { init(ctlcap); }
 void copy(const object_representation_ &x);
 void flush();
};

void object_representation_::init(const string &caption)
{
 ctlcap=caption;
 ctlout.str("");
 print_ctlcap();
 ctllogs="initialized InfoSel++ shell: no action performed";
 ctlres=0;
 flushed=true;
 
 datfn="default.dat";
 repfn="default.rep";
 prec=0.001;
 pmname="EquiWidth";
 pmpars="24";
 cpos=ProgramShell::cpLast;
 excl="";
 note=false;
 merg="";
 notm=false;
}

void object_representation_::copy(const object_representation_ &x)
{
 ctlout.str(x.ctlout.str());
 ctllogs=x.ctllogs;
 ctlres=x.ctlres;
 flushed=x.flushed;

 datfn=x.datfn;
 repfn=x.repfn;
 prec=x.prec;
 pmname=x.pmname;
 pmpars=x.pmpars;
 cpos=x.cpos;
 excl=x.excl;
 note=x.note;
 merg=x.merg;
 notm=x.notm;
}

void object_representation_::flush()
{
 if (flushed) return;
 
 string ctltxt=ctlout.str();
 if (ctltxt.empty()) return;

 string ctlnam="program_shell";
 istringstream ctlin(ctltxt);
 ostringstream ctllog;
 
 ctllog << endl << "flushing InfoSel++ shell...." << endl;
#ifdef SOURCE_DEBUG
 ctllog << endl << "invoked control operations:" << endl;
 ctllog << ctltxt;
#endif
 InfoSelUI::ctlfile ctl(ctlin,ctlnam);
 ctlres=ctl.perform_processing(ctllog);
 ctllogs=ctllog.str();

 ctlout.str("");
 print_ctlcap();
 flushed=true;
}

}} // end of ProgramShell_ namespace 

ProgramShell::ProgramShell(): rep_(0)
{}

ProgramShell::ProgramShell(const string &caption)
{
 using namespace ProgramShell_;
 rep_=new object_representation_;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->init(caption);
}

ProgramShell::~ProgramShell()
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->flush();
 delete that;
}

ProgramShell::ProgramShell(const ProgramShell &shl): rep_(0)
{
 using namespace ProgramShell_;
 if (!shl.rep_) return;
 rep_=new object_representation_;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 const object_representation_ *shl_that=reinterpret_cast<const object_representation_ *>(shl.rep_);
 that->copy(*shl_that);
}

ProgramShell &ProgramShell::operator=(const ProgramShell &shl)
{
 using namespace ProgramShell_;
 {
  object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
  that->flush();
  delete that;
  rep_=0;
 }
 { 
  if (!shl.rep_) return *this;
  rep_=new object_representation_;
  object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
  const object_representation_ *shl_that=reinterpret_cast<const object_representation_ *>(shl.rep_);
  that->copy(*shl_that);
 }
 return *this;
}

void ProgramShell::setdatfn(const string &n)
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->datfn=n;
 if (!n.empty()) {
  that->ctlout << "input=\"" << n << "\";" << endl;
  that->flushed=false;
 } 
}

const string &ProgramShell::datfn() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return sEmpty_;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->datfn; 
}

void ProgramShell::setrepfn(const string &n)
{ 
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->repfn=n;
 if (!n.empty()) {
  that->ctlout << "output=\"" << n << "\";" << endl;
  that->flushed=false;
 } 
}

const string &ProgramShell::repfn() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return sEmpty_;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->repfn; 
}

void ProgramShell::setprec(double p)
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->prec=p;
 if (p>0.0) {
  that->ctlout << "precision=" << p << ';' << endl;
  that->flushed=false;
 } 
}

double ProgramShell::prec() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return 0.0;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->prec; 
}

void ProgramShell::setpartmth(const string &n,const string &p)
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->pmname=n;
 that->pmpars=p;
 if (!n.empty()) {
  if (p.empty()) that->ctlout << "partition=" << n << ';' << endl;
  else that->ctlout << "partition=" << n << '(' << p << ");" << endl;
  that->flushed=false;
 }  
}

const string &ProgramShell::pmname() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return sEmpty_;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->pmname; 
}

const string &ProgramShell::pmpars() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return sEmpty_;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->pmpars; 
}

void ProgramShell::setcpos(int p)
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->cpos=p;
 if (p>=0) that->ctlout << "classpos=" << p << ';' << endl;
 else that->ctlout << "classislast=true;" << endl;
 that->flushed=false;
}

int ProgramShell::cpos() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return 0;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->cpos; 
}

void ProgramShell::setexcl(const string &e,bool ne)
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->excl=e;
 that->note=ne;
 if (!e.empty()) {
  if (ne) that->ctlout << "notexcluded=" << e << ';' << endl;
  else that->ctlout << "excluded=" << e << ';' << endl;
  that->flushed=false;
 } 
}

const string &ProgramShell::excl() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return sEmpty_;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->excl; 
}

bool ProgramShell::note() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return false;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->note; 
}

void ProgramShell::setmerg(const string &m,bool nm)
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->merg=m;
 that->notm=nm;
 if (!m.empty()) {
  if (nm) that->ctlout << "notmerged=" << m << ';' << endl;
  else that->ctlout << "merged=" << m << ';' << endl;
  that->flushed=false;
 } 
}

const string &ProgramShell::merg() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return sEmpty_;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->merg; 
}

bool ProgramShell::notm() const
{ 
 using namespace ProgramShell_;
 if (!rep_) return false;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 return that->notm; 
}

void ProgramShell::exec(const ProgramShell::Algorithm &A)
{
 using namespace ProgramShell_;
 using namespace ProgramShell_::Algorithm_;
 if (!rep_) return;
 const proxy_representation_ *A_that=reinterpret_cast<const proxy_representation_ *>(A.rep_);
 if (A_that->invo.empty()) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->ctlout << "exec(" << A_that->invo << ");" << endl;
 that->flushed=false;
}

void ProgramShell::execall()
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->ctlout << "execall;" << endl;
 that->flushed=false;
}

void ProgramShell::flush()
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->flush();
 setdatfn(that->datfn);
 setrepfn(that->repfn);
 setprec(that->prec);
 setpartmth(that->pmname,that->pmpars);
 setcpos(that->cpos);
 setexcl(that->excl,that->note);
 setmerg(that->merg,that->notm);
 that->flushed=true;
}

void ProgramShell::reset()
{
 using namespace ProgramShell_;
 if (!rep_) return;
 object_representation_ *that=reinterpret_cast<object_representation_ *>(rep_);
 that->flush();
 that->reinit();
}

ProgramShell::procres ProgramShell::status(ostream &log) const
{
 using namespace ProgramShell_;
 if (!rep_) return false;
 const object_representation_ *that=reinterpret_cast<const object_representation_ *>(rep_);
 log << that->ctllogs;
 return that->ctlres;
}

ProgramShell::procres ProgramShell::status() const
{
 using namespace ProgramShell_;
 return status(InfoSelUI::nullstream::out);
}

} // end of InfoSelLib namespace

