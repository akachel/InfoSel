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
/// Definitions Of Kernel Common Elements
/**
 * @file       comm.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains outline definitions of kernel common elements.
 */

#include "comm.h"

namespace InfoSelCore {

namespace Common {

/////////////////////////////
////
//// utility function 
//// definitions
////

bool chkadd(int &ret,int a,int b)
{
 const real imin=iMin+1;
 const real imax=iMax-1;
 double x=a, y=b;
 double z=x+y;
 if (z<imin || z>imax) return false;
 return ret=a+b, true;
}

bool chksub(int &ret,int a,int b)
{
 const real imin=iMin+1;
 const real imax=iMax-1;
 double x=a, y=b;
 double z=x-y;
 if (z<imin || z>imax) return false;
 return ret=a-b, true;
}

bool chkmul(int &ret,int a,int b)
{
 if (a==0 || b==0) return ret=0, true; 
 if (a<0 || b<0) {
  int res; 
  bool chk;
  if (b>0) return chk=chkmul(res,-a,b), ret = chk ? -res : ret, chk;
  else if (a>0) return chk=chkmul(res,a,-b), ret = chk ? -res : ret, chk;
  else return chkmul(ret,-a,-b);
 } 
 const real imax=iMax-1;
 double x=a, y=b;
 double z=log(x)+log(y);
 if (z>log(imax)) return false;
 return ret=a*b, true;
}

bool chkdiv(int &ret,int a,int b)
{
 if (b==0) return false; 
 return ret=a/b, true;
}

bool chkmopow(int &ret,int n,int k)
{
 if (n<0) {
  int res; 
  bool chk;
  return chk=chkmopow(res,-n,k), ret = chk ? -res : ret, chk;
 } 
 if (n<=1) return ret = k<=0 ? 1 : n, true;
 const real imax=iMax-1;
 const real rmax=1.0e100;
 real x=0.0;
 for (int i=1; i<=k; i++) {
  real y=n;
  x+=log(y);
  if (x>rmax) return false;
 }
 if (x>log(imax)) return false;
 real z=exp(x);
 return ret=z+0.5, true;
}

bool chkfact(int &ret,int n)
{
 const real imax=iMax-1;
 const real rmax=1.0e100;
 real x=0.0;
 for (int i=2; i<=n; i++) {
  real y=i;
  x+=log(y);
  if (x>rmax) return false;
 }
 if (x>log(imax)) return false;
 real z=exp(x);
 return ret=z+0.5, true;
}

bool chkbicof(int &ret,int n,int k)
{
 if (n<k) return ret=0, true;
 const real imax=iMax-1;
 const real rmax=1.0e100;
 real x=0.0;
 for (int i=1; i<=k; i++) {
  real y=n-k+i;
  y/=i;
  x+=log(y);
  if (x>rmax) return false;
 }
 if (x>log(imax)) return false;
 real z=exp(x);
 return ret=z+0.5, true;
}

int intdecord(const void *x1,const void *x2)
{
 int r1=*reinterpret_cast<const real *>(x1);
 int r2=*reinterpret_cast<const real *>(x2);
 return r1<r2 ? +1 : r1>r2 ? -1 : 0;
}

int intincord(const void *x1,const void *x2)
{
 int r1=*reinterpret_cast<const real *>(x1);
 int r2=*reinterpret_cast<const real *>(x2);
 return r1<r2 ? -1 : r1>r2 ? +1 : 0;
}

int realdecord(const void *x1,const void *x2)
{
 real r1=*reinterpret_cast<const real *>(x1);
 real r2=*reinterpret_cast<const real *>(x2);
 return r1<r2 ? +1 : r1>r2 ? -1 : 0;
}

int realincord(const void *x1,const void *x2)
{
 real r1=*reinterpret_cast<const real *>(x1);
 real r2=*reinterpret_cast<const real *>(x2);
 return r1<r2 ? -1 : r1>r2 ? +1 : 0;
}

/////////////////////////////
////
//// iosex_base class 
//// definition
////

const iosex_base::ifmtflags iosex_base::sorted=iosex_base::xalloc();
const iosex_base::ifmtflags iosex_base::ordered=iosex_base::xalloc();
const iosex_base::ifmtflags iosex_base::weighted=iosex_base::xalloc();
const iosex_base::ifmtflags iosex_base::shortened=iosex_base::xalloc();

bool iosex_base::setsorted(ios_base &s)
{ bool ret=(bool)s.iword(sorted); s.iword(sorted)=(long)true; return ret; }

bool iosex_base::setordered(ios_base &s)
{ bool ret=(bool)s.iword(ordered); s.iword(ordered)=(long)true; return ret; }

bool iosex_base::setweighted(ios_base &s)
{ bool ret=(bool)s.iword(weighted); s.iword(weighted)=(long)true; return ret; }

bool iosex_base::setshortened(ios_base &s)
{ bool ret=(bool)s.iword(shortened); s.iword(shortened)=(long)true; return ret; }

bool iosex_base::unsetsorted(ios_base &s)
{ bool ret=(bool)s.iword(sorted); s.iword(sorted)=(long)false; return ret; }

bool iosex_base::unsetordered(ios_base &s)
{ bool ret=(bool)s.iword(ordered); s.iword(ordered)=(long)false; return ret; }

bool iosex_base::unsetweighted(ios_base &s)
{ bool ret=(bool)s.iword(weighted); s.iword(weighted)=(long)false; return ret; }

bool iosex_base::unsetshortened(ios_base &s)
{ bool ret=(bool)s.iword(shortened); s.iword(shortened)=(long)false; return ret; }

bool iosex_base::issorted(ios_base &s)
{ return (bool)s.iword(sorted); }

bool iosex_base::isordered(ios_base &s)
{ return (bool)s.iword(ordered); }

bool iosex_base::isweighted(ios_base &s)
{ return (bool)s.iword(weighted); }

bool iosex_base::isshortened(ios_base &s)
{ return (bool)s.iword(shortened); }

void iosex_base::resetallflags(ios_base &s)
{ unsetsorted(s); unsetordered(s); unsetweighted(s); unsetshortened(s); }

/////////////////////////////
////
//// xError structure 
//// definitions
////

xError::xError() throw():
 src_(0), msg_(0)
{}

xError::xError(const xError &exn) throw():
 exception(exn),
 src_(0), msg_(0)
{
 if (exn.src_!=0) {
  size_t siz=strlen(exn.src_)+1; 
  src_=(char *)malloc(siz);
  if (src_!=0) strcpy(src_,exn.src_);
 } 
 if (exn.msg_!=0) {
  size_t siz=strlen(exn.msg_)+1; 
  msg_=(char *)malloc(siz);
  if (msg_!=0) strcpy(msg_,exn.msg_);
 } 
}

xError::~xError() throw()
{
 if (msg_!=0) free(msg_);
 if (src_!=0) free(src_);
}

xError &xError::operator=(const xError &exn) throw()
{
 if (src_!=0) free(src_);
 if (exn.src_==0) src_=0; 
 else {
  size_t siz=strlen(exn.src_)+1; 
  src_=(char *)malloc(siz);
  if (src_!=0) strcpy(src_,exn.src_);
 } 
 if (msg_!=0) free(msg_);
 if (exn.msg_==0) msg_=0;
 else {
  size_t siz=strlen(exn.msg_)+1; 
  msg_=(char *)malloc(siz);
  if (msg_!=0) strcpy(msg_,exn.msg_);
 } 
 (exception &)*this=exn;
 return *this;
}

void xError::cause(const exception &src) throw()
{
 if (src_!=0) free(src_);
 size_t siz=strlen(src.what())+1; 
 src_=(char *)malloc(siz);
 if (src_!=0) strcpy(src_,src.what());
}

const char *xError::source() const throw()
{
 return src_!=0 ? src_ : "";
}

const char *xError::message() const throw()
{
 if (msg_==0) {
  ostringstream bout;
  printmsg_(bout);
  string buf=bout.str();
  size_t siz=strlen(buf.c_str())+1; 
  msg_=(char *)malloc(siz);
  if (msg_!=0) strcpy(msg_,buf.c_str());
 } 
 return msg_!=0 ? msg_ : "";
}

const char *xError::what() const throw()
{
 const size_t bufsiz=511;
 static char buf[bufsiz+1];
 int n;
 if (src_==0) buf[0]='\0';
 else {
  n=min2(strlen(src_),bufsiz);
  strncpy(buf,src_,n);
  buf[n]='\0';
  n=bufsiz-strlen(buf);
  strncat(buf," => ",n);
 } 
 n=bufsiz-strlen(buf);
 strncat(buf,message(),n);
 return buf;
}

ostream &operator<<(ostream &out,const xError &exn) throw()
{
 out << "!!! ";
 exn.printmsg_(out);
 if (exn.src_!=0) out << " ," << endl << "!!! caused by : " << endl << "!!! " << exn.src_;
 out << " !";
 return out;
}

/////////////////////////////
////
//// exception type 
//// definitions
////

void xUnexpctExn::printmsg_(ostream &out) const
{
 out << "Fatal error: Unexpected exception";
}

void xUnknownExn::printmsg_(ostream &out) const
{
 out << "Fatal error: Unknown exception";
}

void xPrecondFail::printmsg_(ostream &out) const
{
 out << "Fatal error: Procedure preconditions failed";
}

void xPostcondFail::printmsg_(ostream &out) const
{
 out << "Fatal error: Procedure postconditions failed";
}

void xInvarFail::printmsg_(ostream &out) const
{
 out << "Fatal error: Procedure invariants failed";
}

void xUnexpFault::printmsg_(ostream &out) const
{
 out << "Fatal error: Unexpected code fault";
}

void xNotAllowOper::printmsg_(ostream &out) const
{
 out << "Error: Not allowed operation use";
}

void xNotImplemOper::printmsg_(ostream &out) const
{
 out << "Error: Not yet implemented operation use";
}

void xInvalidOper::printmsg_(ostream &out) const
{
 out << "Error: Invalid operation";
}

void xOperFail::printmsg_(ostream &out) const
{
 out << "Error: Operation failed";
}

void xFileOpenFail::printmsg_(ostream &out) const
{
 out << "Error: File open failed or there is no input file: " << fnam << "";
}

void xWrongInputFile::printmsg_(ostream &out) const
{
 out << "Error: Wrong input file: " << fnam << "";
}

} // end of Common namespace

} // end of InfoSelCore namespace

