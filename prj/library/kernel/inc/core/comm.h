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
/// Kernel Common Declarations 
/**
 * @file       comm.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file/namespace includes main standard headers 
 *             and contains basic elements commonly used in all
 *             layers of the library kernel.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef COMM_H_
#define COMM_H_

#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <cctype>
#include <cmath>
#include <cstring>
#include <ctime>
#include <limits>
#include <exception>
#include <typeinfo>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

namespace InfoSelCore /** \copybrief InfoSelCore.h \copydetails InfoSelCore.h */ { // namespace head

namespace Common /** \copybrief comm.h \copydetails comm.h */ { // namespace head

/////////////////////////////
////
//// Auxiliary Declarations
////

//@{
/// macros for syntax dependent on C++ implementation:

#if defined(_MSC_VER) && _MSC_VER<1300
# define instantiate template
#else /* ANSI C++ */
# define instantiate template class
#endif
//@}

//// basic number types:

const int iMin=numeric_limits<int>::min(); ///< the integer lower limit
const int iMax=numeric_limits<int>::max(); ///< the integer upper limit

typedef double real; ///< an alias name for the real type, parametrize it with caution
const real rMin=numeric_limits<real>::min(); ///< the smallest positive real value
const real rMax=numeric_limits<real>::max(); ///< the largest positive real value
const real rPrec=numeric_limits<real>::epsilon(); ///< the real-type arithmetic precision
const real rInf=numeric_limits<real>::infinity(); ///< the representation of positive real infinity

//@{
/// math constants:

const real mE         = 2.71828182845904523536;
const real mLOG2E     = 1.44269504088896340736;
const real mLOG10E    = 0.434294481903251827651;
const real mLN2       = 0.693147180559945309417;
const real mLN10      = 2.30258509299404568402;
const real mPI        = 3.14159265358979323846;
const real mPIr2      = 1.57079632679489661923;
const real mPIr4      = 0.785398163397448309616;
const real m1rPI      = 0.318309886183790671538;
const real m2rPI      = 0.636619772367581343076;
const real m2PI       = 2.0*mPI;
const real m1rSQRTPI  = 0.564189583547756286948;
const real m2rSQRTPI  = 1.12837916709551257390;
const real mSQRT2     = 1.41421356237309504880;
const real mSQRT2r2   = 0.707106781186547524401;
const real mSQRT3     = 1.732050807569;
const real mSQRT3r3   = 0.5773502691896;
//@}

//// object literal types:
//@{
/// Nil Object Literal
/// \details It means nothing, no-value.
struct nil_literal_ { nil_literal_() {} }; 
typedef const nil_literal_ &nil_literal;
//@}
//@{
/// First Object Literal
/// \details It means a first item.
struct first_literal_ { first_literal_() {} }; 
typedef const first_literal_ &first_literal;
//@}
//@{
/// Last Object Literal 
/// \details It means a last item.
struct last_literal_ { last_literal_() {} };
typedef const last_literal_ &last_literal;
//@}

//@{
/// object literal keywords:
///
/// They can be applied to extend 
/// the C++ syntax using the function
/// name overloading.
///
const nil_literal_ Nil;
const first_literal_ First;
const last_literal_ Last;
//@}

/////////////////////////////
////
//// Utility Functions
////

/// absolute value template:

template <typename T>
inline T abs(const T &x)
{ return x>=0 ? x : -x; }

/// value sign template:

template <typename T>
inline int sign(const T &x)
{ return x==0 ? 0 : x>0 ? 1 : -1; }

/// maximum value template for 2 arguments:

template <typename T>
inline T max2(const T &x1,const T &x2)
{ return x1>=x2 ? x1:x2; }

/// maximum value template for 3 arguments:

template <typename T>
inline T max3(const T &x1,const T &x2,const T &x3)
{ return max2(x1,max2(x2,x3)); }

/// minimum value template for 2 arguments:

template <typename T>
inline T min2(const T &x1,const T &x2)
{ return x1<=x2 ? x1:x2; }

/// minimum value template for 3 arguments:

template <typename T>
inline T min3(const T &x1,const T &x2,const T &x3)
{ return min2(x1,min2(x2,x3)); }

/// template for returning lower case of a character:

template <typename TChar>
inline TChar tolower(TChar x)
{ return std::tolower(x); }

/// template for making a character lower:

template <typename TChar>
inline void mklower(TChar &ret)
{ ret=std::tolower(ret); }

/// template for returning lower case of a string:

template <typename TChar>
inline basic_string< TChar, char_traits<TChar>, allocator<TChar> > tolower(const basic_string< TChar, char_traits<TChar>, allocator<TChar> > &x)
{ 
 basic_string< TChar, char_traits<TChar>, allocator<TChar> > ret=x;
 for_each(ret.begin(),ret.end(),(void(*)(TChar &))mklower);
 return ret; 
}

/// template for making a string lower:

template <typename TChar>
inline void mklower(basic_string< TChar, char_traits<TChar>, allocator<TChar> > &ret)
{ for_each(ret.begin(),ret.end(),(void(*)(TChar &))mklower); }

/// template for returning upper case of a character:

template <typename TChar>
inline TChar toupper(TChar x)
{ return std::toupper(x); }

/// template for making a character upper:

template <typename TChar>
inline void mkupper(TChar &ret)
{ ret=std::toupper(ret); }

/// template for returning upper case of a string:

template <typename TChar>
inline basic_string< TChar, char_traits<TChar>, allocator<TChar> > toupper(const basic_string< TChar, char_traits<TChar>, allocator<TChar> > &x)
{
 basic_string< TChar, char_traits<TChar>, allocator<TChar> > ret=x;
 for_each(ret.begin(),ret.end(),(void(*)(TChar &))mkupper);
 return ret; 
}

/// template for making a string upper:

template <typename TChar>
inline void mkupper(basic_string< TChar, char_traits<TChar>, allocator<TChar> > &ret)
{ for_each(ret.begin(),ret.end(),(void(*)(TChar &))mkupper); }

/// template for returning string representation of a value:

template <typename T>
inline string tostring(T x)
{
 ostringstream bout;
 bout << x;
 return !bout ? string("not-a-value") : bout.str();
}

/// template for converting a value to string and returning success flag:

template <typename T>
inline bool tostring(T x,string &ret)
{
 ostringstream bout;
 bout << x;
 return !bout ? false : (ret=bout.str(), true);
}

/// template for returning value converted from a string representation:

template <typename T>
inline T parse(const string &x)
{
 istringstream bin(x);
 T ret;
 bin >> ret;
 return ret;
}

/// template for converting a string to value and returning success flag:

template <typename T>
inline bool parse(const string &x,T &ret)
{
 istringstream bin(x);
 bin >> ret;
 return !bin ? false : true;
}

/// perform incresing for an integer type argument with arithmetic overflow checking:

inline bool chkinc(int &a)
{ return a>=iMax ? false : (++a, true); }

/// perform decreasing for an integer type argument with arithmetic overflow checking:

inline bool chkdec(int &a)
{ return a<=iMin ? false : (--a, true); }

/// perform addition for two integer type arguments with arithmetic overflow checking:

bool chkadd(int &ret,int a,int b); //!< \post (asserted) chkadd == iMin <= ret:=a+b <= iMax

/// perform subtraction operation for two integer type arguments with arithmetic overflow checking:

bool chksub(int &ret,int a,int b); //!< \post (asserted) chksub == iMin <= ret:=a-b <= iMax

/// perform multiplication operation for two integer type arguments with arithmetic overflow checking:

bool chkmul(int &ret,int a,int b); //!< \post (asserted) chkmul == iMin <= ret:=a*b <= iMax

/// perform divsion operation for two integer type arguments with arithmetic overflow checking:

bool chkdiv(int &ret,int a,int b); //!< \post (asserted) chksub == iMin <= ret:=a/b <= iMax

/// obtain monomial as integer power:

inline int mopow(int n,int k)
{ return k<=0 ? 1 : k==1 ? n : n*mopow(n,k-1); }

/// perform monomial calculation with arithmetic overflow checking:

bool chkmopow(int &ret,int n,int k); //!< \post (asserted) chkmopow == iMin <= ret:=mopow(n,k) <= iMax

/// obtain factorial for an integer number:

inline int fact(int n) //!< \post ret == n!:=1*2*...*(n-1)*n 
{ return n<2 ? 1 : n*fact(n-1); }

/// perform factorial calculation with arithmetic overflow checking:

bool chkfact(int &ret,int n); //!< \post (asserted) chkfact == iMin <= ret:=fact(n) <= iMax

/// obtain binomial coefficient ('choose' function):

inline int bicof(int n,int k) //!< \post ret == C(n,k):=n!/(k!(n-k)!) 
{ return n<k ? 0 : k<1 ? 1 : n*bicof(n-1,k-1)/k; }

/// perform binomial calculation with arithmetic overflow checking:

bool chkbicof(int &ret,int n,int k); //!< \post (asserted) chkbicof == iMin <= ret:=bicof(n,k) <= iMax

//@{
/// basic math functions for real type arguments:

inline real log2(const real &x) 
{ return std::log(x)/std::log(2.0); }

inline real sqr(const real &x)
{ return x*x; }

inline real cub(const real &x)
{ return x*x*x; }
//@}

//// return uniform random number at the interval [0,1):

inline real unirand()
{ return (real)std::rand()/RAND_MAX; }

//// return uniform random number at the interval [0,n):

inline real unirand(real n)
{ return ((real)std::rand()/RAND_MAX)*n; }

//// return uniform random number at the interval [a,b):

inline real unirand(real a,real b)
{ return a+((real)std::rand()/RAND_MAX)*(b-a); }

/// perform comparision of two integer numbers for decreasing order:

int intdecord(const void *x1,const void *x2);

/// perform comparision of two integer numbers for increasing order:

int intincord(const void *x1,const void *x2);

/// perform comparision of two real numbers for decreasing order:

int realdecord(const void *x1,const void *x2);

/// perform comparision of two real numbers for increasing order:

int realincord(const void *x1,const void *x2);

/////////////////////////////
///
/// Extension Declarations
/// for Input/Output Streams 
///

class iosex_base : public ios_base {
 public:
  typedef int ifmtflags; ///< integer type of extra stream format flag
 
//// extra flags for stream format: 

  static const ifmtflags sorted; ///< flag of sorted format
  static const ifmtflags ordered; ///< flag of ordered format
  static const ifmtflags weighted; ///< flag of weighted format
  static const ifmtflags shortened; ///< flag of shortened format
//@{ 
/// set up extra format flags for a given stream: 

  static bool setsorted(ios_base &s);
  static bool setordered(ios_base &s);
  static bool setweighted(ios_base &s);
  static bool setshortened(ios_base &s);
//@}
//@{ 
/// unset extra format flags for a given stream: 

  static bool unsetsorted(ios_base &s);
  static bool unsetordered(ios_base &s);
  static bool unsetweighted(ios_base &s);
  static bool unsetshortened(ios_base &s);
//@}
//@{ 
/// test whether extra format flags are set for a given stream: 

  static bool issorted(ios_base &s);
  static bool isordered(ios_base &s);
  static bool isweighted(ios_base &s);
  static bool isshortened(ios_base &s);
//@}
/// reset all extra format flags for a given stream: 

  static void resetallflags(ios_base &s);
};

//@{ 
/// function manipulators for extra stream format flags: 

inline ios_base &sorted(ios_base &s)
{ iosex_base::setsorted(s); return s; }

inline ios_base &ordered(ios_base &s)
{ iosex_base::setordered(s); return s; }

inline ios_base &weighted(ios_base &s)
{ iosex_base::setweighted(s); return s; }

inline ios_base &shortened(ios_base &s)
{ iosex_base::setshortened(s); return s; }

inline ios_base &nosorted(ios_base &s)
{ iosex_base::unsetsorted(s); return s; }

inline ios_base &noordered(ios_base &s)
{ iosex_base::unsetordered(s); return s; }

inline ios_base &noweighted(ios_base &s)
{ iosex_base::unsetweighted(s); return s; }

inline ios_base &noshortened(ios_base &s)
{ iosex_base::unsetshortened(s); return s; }
//@}

/////////////////////////////
///
/// Basic Stream Extension
///

template <typename TElem,typename TTraits>
class basic_iosex : public iosex_base {
};

/// character stream type:

typedef basic_iosex< char, char_traits<char> > iosex;

//@{ 
/// input/output stream comparison templates:

template <typename TElem,typename TTraits>
inline bool operator==(const basic_ios<TElem,TTraits> &s1,const basic_ios<TElem,TTraits> &s2)
{ return s1.rdbuf()==s2.rdbuf(); }

template <typename TElem,typename TTraits>
inline bool operator!=(const basic_ios<TElem,TTraits> &s1,const basic_ios<TElem,TTraits> &s2)
{ return s1.rdbuf()!=s2.rdbuf(); }
//@}

/////////////////////////////
///
/// Null Stream Buffer
///

template <typename TElem,typename TTraits>
class basic_nullbuf : public basic_streambuf<TElem,TTraits> {
  ios_base::openmode mode_; //!< open mode field
  enum { siz_=32*sizeof(TElem) }; //!< size of void memory storage
  TElem mem_[siz_]; //!< void memory storage
 public:
/// some auxiliary types:
  
  typedef basic_streambuf<TElem,TTraits> base_type;
  typedef typename TTraits::int_type int_type;
  
/// create a buffer from mode:
  
  explicit basic_nullbuf(ios_base::openmode mode=ios_base::in|ios_base::out): mode_(mode)
  {
   if (!(mode_ & ios_base::out))
   base_type::setg(mem_,mem_,mem_+siz_);
   if (!(mode_ & ios_base::in))
   base_type::setp(mem_,mem_+siz_);
  }

/// destroy the buffer:
 
  virtual ~basic_nullbuf()
  {}

 protected:  
/// implementation of main base overflow method:

  virtual int_type overflow(int_type mch=TTraits::eof())
  {
   base_type::setp(mem_,mem_+siz_) ;
   if (TTraits::eq_int_type(TTraits::eof(),mch)) return TTraits::not_eof(mch);
   else return mch;
  }  

/// implementation of main base underflow method:

  virtual int_type underflow() 
  { 
   base_type::setg(mem_,mem_,mem_+siz_);
   return TTraits::eof(); 
  } 
};

/// character buffer type:

typedef basic_nullbuf< char, char_traits<char> > nullbuf;

/////////////////////////////
///
/// Null Input Streams
///

template <typename TElem,typename TTraits>
class basic_inullstream : public basic_istream<TElem,TTraits> {
 public:
  typedef basic_istream<TElem,TTraits> base_type; //!< auxiliary base type
  typedef basic_nullbuf<TElem,TTraits> buffer_type; //!< auxiliary buffer type

/// create a readable stream from mode:
  
  explicit basic_inullstream(ios_base::openmode mode=ios_base::in):
   basic_istream<TElem,TTraits>(&impl_), 
   impl_(mode|ios_base::in)
  {}

/// destroy the stream:
 
  virtual ~basic_inullstream()
  {}

/// return pointer to stream buffer:
     
  buffer_type *rdbuf() const
  { return const_cast<buffer_type *>(&impl_); }
  
 private:
  buffer_type impl_; //!< null stream implementaion
};

/// character stream type:

typedef basic_inullstream< char, char_traits<char> > inullstream;

/////////////////////////////
///
/// Null Output Streams
///

template <typename TElem,typename TTraits>
class basic_onullstream : public basic_ostream<TElem,TTraits> {
 public:
  typedef basic_ostream<TElem,TTraits> base_type; ///< auxiliary base type
  typedef basic_nullbuf<TElem,TTraits> buffer_type; ///< auxiliary buffer type

/// create a writable stream from mode:
  
  explicit basic_onullstream(ios_base::openmode mode=ios_base::out):
   basic_ostream<TElem,TTraits>(&impl_), 
   impl_(mode|ios_base::out)
  {}

/// destroy the stream:
 
  virtual ~basic_onullstream()
  {}

/// return pointer to stream buffer:
     
  buffer_type *rdbuf() const
  { return const_cast<buffer_type *>(&impl_); }
  
 private:
  buffer_type impl_; //!< null stream implementaion
};

/// character stream type:

typedef basic_onullstream< char, char_traits<char> > onullstream;

/////////////////////////////
///
/// Null Input/Output Streams
///

template <typename TElem,typename TTraits>
class basic_nullstream : public basic_iostream<TElem,TTraits> {
 public:
  typedef basic_iostream<TElem,TTraits> base_type; ///< auxiliary base type
  typedef basic_nullbuf<TElem,TTraits> buffer_type; ///< auxiliary buffer type

/// create a readable/writable stream from mode:
  
  explicit basic_nullstream(ios_base::openmode mode=ios_base::in|ios_base::out):
   basic_iostream<TElem,TTraits>(&impl_), 
   impl_(mode)
  {}

/// destroy the stream:
 
  virtual ~basic_nullstream()
  {}

/// return pointer to stream buffer:
     
  buffer_type *rdbuf() const
  { return const_cast<buffer_type *>(&impl_); }
  
 private:
  buffer_type impl_; //!< null stream implementaion
  
 public:
//// global I/O instances and their base-type references:

  static basic_inullstream<TElem,TTraits> nin; ///< inullstream global instance
  static basic_onullstream<TElem,TTraits> nout; ///< onullstream global instance
  static typename basic_inullstream<TElem,TTraits>::base_type &in; ///< refererence to inullstream global instance
  static typename basic_onullstream<TElem,TTraits>::base_type &out; ///< refererence to onullstream global instance
};

//// definitions of global I/O instances/references:

template <typename TElem,typename TTraits>
basic_inullstream<TElem,TTraits> basic_nullstream<TElem,TTraits>::nin;
template <typename TElem,typename TTraits>
basic_onullstream<TElem,TTraits> basic_nullstream<TElem,TTraits>::nout;
template <typename TElem,typename TTraits>
typename basic_inullstream<TElem,TTraits>::base_type &basic_nullstream<TElem,TTraits>::in=nin;
template <typename TElem,typename TTraits>
typename basic_onullstream<TElem,TTraits>::base_type &basic_nullstream<TElem,TTraits>::out=nout;

/// character stream type:

typedef basic_nullstream< char, char_traits<char> > nullstream;

/////////////////////////////
////
//// Source Debugging Support
////

/// main macro for dource code debugging: 
//! \note It must be used 
//! as an output stream and
//! is based on the cerr stream.
#if defined(SOURCE_DEBUG)
# define cdebug(ind) cerr << (ind<=0 ? "==> " : ind==1 ? "\t" : ind==2 ? "\t\t" : ind==3 ? "\t\t\t" : ind==4 ? "\t\t\t\t" : "\t\t\t\t\t")
#else
# define cdebug(ind) nullstream::out/*ind*/
#endif
/// header line macro for dource code debugging: 
#define cdbg cdebug(0)
/// one-indended subline macro for dource code debugging: 
#define cdeb cdebug(1)
/// two-indended subline macro for dource code debugging: 
#define cbug cdebug(2)

/////////////////////////////
///
/// Base Of All Error Exceptions
///

struct xError : public exception {
 xError() throw(); ///< create an error exception object
 xError(const xError &exn) throw(); ///< create a copy of an error exception object
 ~xError() throw(); ///< destroy error exception object
 xError &operator=(const xError &exn) throw(); ///< make a copy of an error exception object
 void cause(const exception &src) throw(); ///< set up a source exception as a cause for this exception
 const char *source() const throw(); ///< return description text of this exception source 
 const char *message() const throw(); ///< return message text of this exception
 const char *what() const throw(); ///< return full description text of this exception
 friend ostream &operator<<(ostream &out,const xError &exn) throw(); ///< print full description text of an exception to output stream
 private: char *src_; //!< pointer to source description text
 private: mutable char *msg_; //!< pointer to message text
 private: virtual void printmsg_(ostream &out) const = 0; //!< print message text of this exception to output stream
};

/////////////////////////////
///
/// Basic Exception Errors
///

struct xBasError : xError {};

/// Error Of Unexpected Exception
struct xUnexpctExn : xBasError { 
 xUnexpctExn(const exception &src) { cause(src); }
 xUnexpctExn(const xUnexpctExn &exn): xBasError(exn) {}
 private: void printmsg_(ostream &out) const; 
};

/// Error Of Unknown Exception
struct xUnknownExn : xBasError { private: void printmsg_(ostream &out) const; };

/////////////////////////////
///
/// Common Errors
///

struct xCommError : xError {};

/////////////////////////////
///
/// Internal Code Errors
///

struct xCodeError : xCommError {};

/// Error Of Routine Precondition Failed
struct xPrecondFail : xCodeError { private: void printmsg_(ostream &out) const; };
/// Error Of Routine Postcondition Failed
struct xPostcondFail : xCodeError { private: void printmsg_(ostream &out) const; };
/// Error Of Routine Invariant Failed
struct xInvarFail : xCodeError { private: void printmsg_(ostream &out) const; };
/// Error Of Unexpected Fault
struct xUnexpFault : xCodeError { private: void printmsg_(ostream &out) const; };
/// Error Of Not Allowed Operation
struct xNotAllowOper : xCodeError { private: void printmsg_(ostream &out) const; };
/// Error Of Not Implemented Operation
struct xNotImplemOper : xCodeError { private: void printmsg_(ostream &out) const; };
/// Error Of Invalid Operation
struct xInvalidOper : xCodeError { private: void printmsg_(ostream &out) const; };
/// Error Of Operation Failed
struct xOperFail : xCodeError { private: void printmsg_(ostream &out) const; };

/////////////////////////////
///
/// Input/Output Operation Errors
///

struct xInOutError : xCommError {
 string fnam; ///< name of failed file
 xInOutError(const string &fn) throw(): fnam(fn) {}
 ~xInOutError() throw() {}
};

/// Error Of File Open Failed
struct xFileOpenFail : xInOutError { xFileOpenFail(const string &fn): xInOutError(fn) {} private: void printmsg_(ostream &out) const; };
/// Error Of Wrong Input File
struct xWrongInputFile : xInOutError { xWrongInputFile(const string &fn): xInOutError(fn) {} private: void printmsg_(ostream &out) const; };

/////////////////////////////
///
/// Type Of Data Value Tuple
///
/// This is class template for 
/// manipulating any tuple of 
/// Ddata values of any type.
///

template <typename T>
class valtuple {
  T *vals_; //!< underlying array of values of type T
  int nvals_; //!< value array length 
 public:
/// create a null tuple object with no elements:

  valtuple():
   nvals_(0), vals_(0) 
  {}
  
/// create a singleton tuple object with one element of a given value:

  valtuple(T val): //!< \post count==1
   nvals_(1),
   vals_(new T[1])
  {
   *vals_=val;
  }
    
/// create a tuple object with cnt elements each one of a given value:

  valtuple(int cnt,T val):
   nvals_(cnt<0 ? 0 : cnt)
  {
   vals_ = nvals_>0 ? new T[nvals_] : 0;
   for (int k=0; k<nvals_; k++) vals_[k]=val;
  }
  
/// create a tuple object with cnt elements each one as the nil of undefined value:

  valtuple(int cnt,nil_literal):
   nvals_(cnt<0 ? 0 : cnt)
  {
   vals_ = nvals_>0 ? new T[nvals_] : 0;
  }

/// create a tuple object with cnt elements copied from a given array:

  static valtuple create(int cnt,const T vals[]) //! \pre count(vals)==cnt
  {
   valtuple ret(cnt,Nil);
   for (int k=0; k<ret.nvals_; k++) ret.vals_[k]=vals[k];
   return ret;
  }

/// create a tuple object with cnt elements copied from a given list:

  static valtuple create(int cnt,...) //! \pre count(...)==cnt
  {
   valtuple ret(cnt,Nil);
   va_list ap;
   va_start(ap,cnt);
   for (int k=0; k<ret.nvals_; k++) ret.vals_[k]=va_arg(ap,T);
   va_end(ap);
   return ret;
  }

/// create a tuple object and assign to it a given tuple by copying inner structure:

  valtuple(const valtuple &tup):
   nvals_(tup.nvals_)
  {
   vals_ = nvals_>0 ? new T[nvals_] : 0;
   for (int k=0; k<nvals_; k++) vals_[k]=tup.vals_[k];
  }

/// destroy the tuple object:

  ~valtuple()
  {
   delete[] vals_;
  }

/// clear contents of this tuple object:

  void clear() //! \post count==0
  {
   if (nvals_!=0) {
    nvals_=0;
    delete[] vals_;
    vals_=0;
   }
  }
  
/// set up this tuple object with one element of a given value:

  void setup(T val) //! \post count==1
  {
   if (nvals_!=1) {
    nvals_=1;
    delete[] vals_;
    vals_=new T[1];
   }
   *vals_=val;
  }
  
/// set up this tuple object with cnt elements each one of a given value:

  void setup(int cnt,T val)
  {
   if (cnt<0) cnt=0;
   if (nvals_!=cnt) {
    nvals_=cnt;
    delete[] vals_;
    vals_ = nvals_>0 ? new T[nvals_] : 0;
   }
   for (int k=0; k<nvals_; k++) vals_[k]=val;
  }
  
/// set up this tuple object with cnt elements each one as the nil of undefined value:

  void setup(int cnt,nil_literal)
  {
   if (cnt<0) cnt=0;
   if (nvals_!=cnt) {
    nvals_=cnt;
    delete[] vals_;
    vals_ = nvals_>0 ? new T[nvals_] : 0;
   }
  }
  
/// recreate this tuple object and set up its cnt elements to values from a given table:

  void recreate(int cnt,const T vals[]) //! \pre count(vals)==cnt
  {
   if (cnt<0) cnt=0;
   if (nvals_!=cnt) {
    nvals_=cnt;
    delete[] vals_;
    vals_ = nvals_>0 ? new T[nvals_] : 0;
   }
   for (int k=0; k<nvals_; k++) vals_[k]=vals[k];
  }

/// recreate this tuple object and set up its cnt elements to values from a given list:

  void recreate(int cnt,...) //! \pre count(...)==cnt
  {
   if (cnt<0) cnt=0;
   if (nvals_!=cnt) {
    nvals_=cnt;
    delete[] vals_;
    vals_ = nvals_>0 ? new T[nvals_] : 0;
   }
   va_list ap;
   va_start(ap,cnt);
   for (int k=0; k<nvals_; k++) vals_[k]=va_arg(ap,T);
   va_end(ap);
  }

/// assign a given tuple to the tuple object by copying inner structure:

  valtuple &operator=(const valtuple &tup)
  {
   if (vals_!=tup.vals_) {
    if (nvals_!=tup.nvals_) {
     nvals_=tup.nvals_;
     delete[] vals_;
     vals_ = nvals_>0 ? new T[nvals_] : 0;
    }
    for (int k=0; k<nvals_; k++) vals_[k]=tup.vals_[k];
   }
   return *this;
  }

/// return number of elements in the tuple:

  int count() const //! \post count>=0
  { return nvals_; }

/// return k-th element of the tuple:

  T &operator[](int k) //! \pre 0 <= k < count
  { return vals_[k]; }
  
/// return k-th element of the tuple:

  T operator[](int k) const //! \pre 0 <= k < count
  { return vals_[k]; }

/// return first element of the tuple:

  T &operator*() //! \pre count>0
  { return *vals_; }
  
/// return first element of the tuple:

  T operator*() const //! \pre count>0
  { return *vals_; }

/// return C-style pointer to first element of the tuple:

  T *c_ptr() //! \post c_ptr!=0 <=> count>0
  { return vals_; }
  
/// return C-style pointer to first element of the tuple:

  const T *c_ptr() const //! \post c_ptr!=0 <=> count>0
  { return vals_; }

/// set up a given value for all elements of the tuple:

  valtuple &operator=(T val) //! \invariant count
  {
   for (int k=0; k<nvals_; k++) 
   vals_[k]=val;
   return *this;
  }

/// perform this relational operation for two tuples:

  friend bool operator==(const valtuple &x1,const valtuple &x2)
  { 
   if (x1.vals_==x2.vals_) return true;
   if (x1.nvals_!=x2.nvals_) return false;
   for (int k=0; k<x1.nvals_; k++) 
   if (x1.vals_[k]!=x2.vals_[k]) return false;
   return true;
  }   

/// perform this relational operation for two tuples:

  friend bool operator!=(const valtuple &x1,const valtuple &x2)
  { return !(x1==x2); }   

/// return minimal value of the tuple:

  T minval() const //! \pre count>0
  {
   T ret;
   for (int k=0; k<nvals_; k++) 
   if (k==0 || vals_[k]<ret) ret=vals_[k];
   return ret;
  }
  
/// return maximal value of the tuple:

  T maxval() const //! \pre count>0
  {
   T ret;
   for (int k=0; k<nvals_; k++) 
   if (k==0 || vals_[k]>ret) ret=vals_[k];
   return ret;
  }
  
/// sort elements of the tuple in the decreasing order:

  void sortdec() //! \invariant count
  {
   int n=nvals_-1;
   bool dosort;
   do {
    dosort=false;
    for (int k=0; k<n; k++) {
     T &a=vals_[k];
     T &b=vals_[k+1];
     if(a<b) {
      swap(a,b);
      dosort=true;
     }
    } 
   } while (dosort);
  }
  
/// sort elements of the tuple in the increasing order:

  void sortinc() //! \invariant count
  {
   int n=nvals_-1;
   bool dosort;
   do {
    dosort=false;
    for (int k=0; k<n; k++) {
     T &a=vals_[k];
     T &b=vals_[k+1];
     if(a>b) {
      swap(a,b);
      dosort=true;
     }
    } 
   } while (dosort);
  }
  
//////
/// Auxiliary Type Of Tuple Map
///
/// It is used for supporting 
/// implicit conversions for 
/// tuples. The converion is 
/// done from the source vector 
/// value to a destination scalar 
/// value.
///             
  template <typename TSclEval,typename TVecEval>
  class map {
    typedef map<TSclEval,TVecEval> TMap; //!< type of this class
    typedef T TScalar; //!< type of scalar data
    typedef TScalar TSclRet; //!< type of scalar returned value
    typedef valtuple<T> TVector; //!< type of vector data as scalar data tuple
    typedef const TVector &TVecRet; //!< type of vector returned value
   private: 
    TSclEval dsteval_; //!< destination evaluator object for scalar data
    mutable bool no_dstval_; //!< flag of no evaluation for destination scalar value
    mutable TScalar dstval_; //!< destination scalar value
    TVecEval srceval_; //!< source evaluator object for vector data
    mutable bool no_srcval_; //!< flag of no evaluation for source vector value
    mutable TVector srcval_; //!< source vector value
    
  //! Manipulator For Scalar-Vector Returned Value
    class TSclVecRet {
      const TMap &that_; //!< manipulated object reference
     public:
      TSclVecRet(const TMap &that): that_(that) {}
      operator TSclRet() const { return that_.getdst(); }
      operator TVecRet() const { return that_.getsrc(); }
    };
    friend class TSclVecRet;
   public:
    typedef TSclRet dstret_t; ///< type of scalar returned value
    typedef TVecRet srcret_t; ///< type of vector returned value
    typedef TSclVecRet ret_t; ///< type of scalar-vector returned Value
    
  /// create null map object:

    map():
     no_dstval_(true), 
     no_srcval_(true) 
    {}
    
  /// create map object with a default source/destination value:

    map(T defval):
     no_dstval_(true), dstval_(defval),
     no_srcval_(true), srcval_(defval) 
    {}

  /// create map object with given destination & source evaluators:

    map(TSclEval dsteval,TVecEval srceval):
     dsteval_(dsteval), no_dstval_(true), 
     srceval_(srceval), no_srcval_(true) 
    {}

  /// create map object with given destination & source evaluators and a default value:

    map(TSclEval dsteval,TVecEval srceval,T defval):
     dsteval_(dsteval), no_dstval_(true), dstval_(defval),
     srceval_(srceval), no_srcval_(true), srcval_(defval) 
    {}

  /// test whether there is no evaluation for destination value:

    void nuldst() const
    { no_dstval_=true; }

  /// return destination value:

    TSclRet getdst() const
    { if (no_dstval_) no_dstval_=!dsteval_(dstval_); return dstval_; }

  /// test whether there is no evaluation for source value:

    void nulsrc() const
    { no_srcval_=true; }

  /// return source value:

    TVecRet getsrc() const
    { if (no_srcval_) no_srcval_=!srceval_(srcval_); return srcval_; }

  /// reset evaluation for destination/source values:

    void nul() const
    { no_dstval_=no_srcval_=true; }

  /// return unified scalar-vector value:

    TSclVecRet get() const
    { return TSclVecRet(*this); }
  };
//||||

/// print contents of the tuple to a given output stream:
  
  friend ostream &operator<<(ostream &out,const valtuple &tup)
  {
   const int nshr=10;
   bool isshr = iosex::isshortened(out) && tup.nvals_>nshr;
   out << '(';
   int n = isshr ? nshr : tup.nvals_;
   if (n>0) {
    int k;
    for (k=0; k<n-1; k++)
    out << tup.vals_[k] << ',';
    out << tup.vals_[k];
    if (isshr) out << ",...";
   }
   out << ')';
   return out;
  }
};

typedef valtuple<int> inttuple; ///< integer tuple type
typedef valtuple<real> realtuple; ///< real tuple type

/////////////////////////////
///
/// Type Of Value Combination 
///
/// This is a class template for 
/// generating and manipulating 
/// any combination of values
/// from integer-typed value set.
///

template <typename INT>
class valcomb {
 public:
  typedef valtuple<INT> tuple_t; //!< type of associated value tuple
 private: 
  int lim_; //!< limit of values as upper bound of the value set
  tuple_t tup_; //!< current value tuple
  int num_; //!< number of all tuples or -1 in the overflow case
  int idx_; //!< index of current tuple
 public:
/// create a null combination object:

  valcomb():
   lim_(0), tup_(0,Nil),
   num_(0), idx_(-1)
  {}

/// create a combination object as the infimum of a specified combination space:

  valcomb(int n,int k): //!< \pre (asserted) 0 <= k <= n+1
   lim_(k<0 ? (n<-1 ? -1 : n) : n<k-1 ? k-1 : n), 
   tup_(k<0 ? 0 : k,Nil),
   num_(0), idx_(-1)
  { 
   bool res=infcomb(lim_,tup_); 
   if (res) {
    num_=ncombs(lim_,tup_.count());
    idx_ = num_>=0 ? -1 : num_-1;
   } 
  }

/// create a combination object and assign to it a given combination by copying inner structure:

  valcomb(const valcomb &cmb):
   lim_(cmb.lim_), tup_(cmb.tup_), 
   num_(cmb.num_), idx_(cmb.idx_)
  {}

/// assign a given combination to the combination object by copying inner structure:

  valcomb &operator=(const valcomb &cmb)
  {
   lim_=cmb.lim_; 
   tup_=cmb.tup_;
   num_=cmb.num_; 
   idx_=cmb.idx_;
   return *this;
  }

/// return upper bound of values for all elements of the combination:

  int limit() const //! \post limit>=-1
  { return lim_; }
  
/// return number of elements in the combination:

  int length() const //! \post 0 <= length <= limit+1
  { return tup_.count(); }
  
/// return value of the l-th element of the combination:

  int operator[](int l) const //! \pre 0 <= l < length  \post !isnil => (0 <= tup[0] < ... < tup[length-1] < limit)
  { return tup_[l]; }

/// return tuple of all elements of the combination:

  const tuple_t &tuple() const //! \post !isnil => (0 <= tuple[0] < ... < tuple[length-1] < limit)
  { return tup_; }

/// return number of all combinations in the combination space:

  int card() const //! \post card>=-1
  { return num_; }
  
/// return index of this combination at the combination space:

  int index() const //! \post -2 <= index < card
  { return idx_; }
  
/// return ordinal number of this combination at the combination space:

  int ord() const //! \post ord!=index , -2 <= ord < card
  { return num_>=0 ? combord(lim_,tup_) : idx_; }
  
/// make this combination as the infimum of the combination space:
 
  bool asinf() 
  { bool res=infcomb(lim_,tup_); if (res && num_>=0) idx_=-1; return res; }
  
/// make this combination as the minimum of the combination space:
 
  bool asmin()
  { bool res=mincomb(lim_,tup_); if (res && num_>=0) idx_=0; return res; }
  
/// make this combination as the maximum of the combination space:
 
  bool asmax()
  { bool res=maxcomb(lim_,tup_); if (res && num_>=0) idx_=num_-1; return res; }
  
/// make this combination as the supremum of the combination space:
 
  bool assup()
  { bool res=supcomb(lim_,tup_); if (res && num_>=0) idx_=num_; return res; }
  
/// make this combination as the nil of the combination space:
 
  bool asnil()
  { return asinf(); }
  
/// test whether the non-null combination is equal to the infimum of the combination space:

  bool isinf() const
  { return isinfcomb(lim_,tup_); }    

/// test whether the non-null combination is equal to the minimum of the combination space:

  bool ismin() const
  { return ismincomb(lim_,tup_); }    

/// test whether the non-null combination is equal to the maximum of the combination space:

  bool ismax() const
  { return ismaxcomb(lim_,tup_); }    

/// test whether the non-null combination is equal to the supremum of the combination space:

  bool issup() const
  { return issupcomb(lim_,tup_); }    

/// test whether the combination is equal to the nil of the combination space:

  bool isnil() const
  { return isinf() || issup(); }    

/// make this combination as the previous one at the combination space:
  
  bool asprev()
  { 
   if (isinf()) return false;
   if (num_>=0) idx_--; 
   return prevcomb(lim_,tup_); 
  }
  
/// make this combination as the next one at the combination space:
  
  bool asnext()
  { 
   if (issup()) return false;
   if (num_>=0) idx_++; 
   return nextcomb(lim_,tup_); 
  }
  
/// make this combination as the previous one at the combination space:
  
  bool operator--()
  { 
   if (isinf() && !assup()) return false;
   if (num_>=0) idx_--; 
   return prevcomb(lim_,tup_); 
  }

/// make this combination as the next one at the combination space:
  
  bool operator++()
  { 
   if (issup() && !asinf()) return false;
   if (num_>=0) idx_++; 
   return nextcomb(lim_,tup_); 
  }

/// make this combination as the behind one at the combination space:
  
  bool operator-=(int inc)
  {
   if (inc<0) return (*this)+=-inc;
   while (--inc>=0)
   if (!(--(*this))) return false;
   return true;
  }
  
/// make this combination as the after one at the combination space:
  
  bool operator+=(int inc)
  {
   if (inc<0) return (*this)-=-inc;
   while (--inc>=0)
   if (!(++(*this))) return false;
   return true;
  }
//@{  
/// perform this relational operations for two combinations:

  friend bool operator==(const valcomb &x1,const valcomb &x2)
  { return x1.lim_==x2.lim_ && x1.tup_==x2.tup_; }   

  friend bool operator!=(const valcomb &x1,const valcomb &x2)
  { return !(x1==x2); }   

  friend bool operator<(const valcomb &x1,const valcomb &x2)
  { 
   if (&x1==&x2) return false;
   if (x1.lim_!=x2.lim_) return false;
   if (x1.tup_.count()!=x2.tup_.count()) return false;
   for (int l=0; l<x1.tup_.count(); l++) 
   if (x1.tup_[l]!=x2.tup_[l]) return x1.tup_[l]<x2.tup_[l];
   return false;
  }   

  friend bool operator>(const valcomb &x1,const valcomb &x2)
  { 
   if (&x1==&x2) return false;
   if (x1.lim_!=x2.lim_) return false;
   if (x1.tup_.count()!=x2.tup_.count()) return false;
   for (int l=0; l<x1.tup_.count(); l++) 
   if (x1.tup_[l]!=x2.tup_[l]) return x1.tup_[l]>x2.tup_[l];
   return false;
  }   
  
  friend bool operator<=(const valcomb &x1,const valcomb &x2)
  { return x1==x2 && x1<x2; }   

  friend bool operator>=(const valcomb &x1,const valcomb &x2)
  { return x1==x2 && x1>x2; }   
//@}
/// print info about indexing, contents and setting of a combination to a given output stream:
  
  friend ostream &operator<<(ostream &out,const valcomb &cmb)
  {
   bool isshr = iosex::isshortened(out);
   if (cmb.num_>=0) out << cmb.idx_ << ") ";
   out << cmb.tup_; 
   if (!isshr) {
    out << " in comb(" << cmb.lim_ << ',' << cmb.tup_.count() << ')';
    if (cmb.num_>=0) out << " of " << cmb.num_;
   } 
   return out;
  }

//// implementation of combination generator:
  
  /// return integer number of all combinations:
  static int ncombs(int n,int k) //! \post n!/k!(n-k)!>iMax => ncombs=-1 , n<k => ncombs=0 , (k<1 || k==n) => ncombs=1
  {
   int N=-1;
   chkbicof(N,n,k);
   return N;
  }
  
  /// return integer index of a given tuple at non-ordered combination list:
  static int combidx(int n,const tuple_t &m) //! \pre 0 <= m[0] < ... < m[m.count-1] < n  \post -1 <= combidx(n,m) < ncombs(n,m.count)
  {
   int k=m.count();
   valcomb cmb(n,k);
   if (cmb.card()<0) return -1;
   while (++cmb && cmb.tuple()!=m);
   return cmb.index();
  }
  
  /// return integer index of a given tuple at ordered combination list:
  static int combord(int n,const tuple_t &m) //! \pre ncombs(n,m)>=0 , 0 <= m[0] < ... < m[m.count-1] < n  \post -1 <= combord(n,m) < ncombs(n,m.count)
  {
   int k=m.count();
   if (n<k) return -1;
   int i=0;
   for (int l=0; l<k; l++) i+=bicof(m[l],l+1);
   return i;
  }
  
  /// test whether a tuple is infimum of combination space:
  static bool isinfcomb(int n,const tuple_t &m) 
  { int l=m.count()-1; return l<=n && (l<0 || m[l]<l); }    

  /// test whether a tuple is minimum of combination space:
  static bool ismincomb(int n,const tuple_t &m) 
  { int l=m.count()-1; return l<n && (l<0 || m[l]==l); }    

  /// test whether a tuple is maximum of combination space:
  static bool ismaxcomb(int n,const tuple_t &m) 
  { int l=m.count()-1; return l<n && (l<0 || m[l]==n-1); }    

  /// test whether a tuple is supremum of combination space:
  static bool issupcomb(int n,const tuple_t &m) 
  { int l=m.count()-1; return l<=n && (l<0 || m[l]>=n); }    

  /// return tuple for infimum of combination space:
  static bool infcomb(int n,tuple_t &m) //! \post 0 <= m[0] < ... < m[m.count-2] == m[m.count-1] , ret <=> (m[m.count-1]<n && m.count<=n+1)
  {
   int k=m.count();
   int a=-2;
   for (int l=0; l<k; l++) m[l] = (l<=0 ? (l<k-1 ? (a+=2) : ++a) : l<k-1 ? ++a : a);
   return a<n;
  }
  
  /// return tuple for minimum of combination space:
  static bool mincomb(int n,tuple_t &m) //! \post 0 <= m[0] < ... < m[m.count-1] , ret <=> (m[m.count-1]<n && m.count<=n)
  {
   int k=m.count();
   int a=-1;
   for (int l=0; l<k; l++) m[l]=++a;
   return a<n;
  }
  
  /// return tuple for maximum of combination space:
  static bool maxcomb(int n,tuple_t &m) //! \post m[0] < ... < m[m.count-1] < n , ret <=> (m[0]>=0 && m.count<=n)
  {
   int k=m.count();
   int a=n-k-1;
   for (int l=0; l<k; l++) m[l]=++a;
   return a>=k-1;
  }
  
  /// return tuple for supremum of combination space:
  static bool supcomb(int n,tuple_t &m) //! \post m[0] < ... < m[m.count-1] == n , ret <=> (m[0]>=-1 && m.count<=n+1)
  {
   int k=m.count();
   int a=n-k;
   for (int l=0; l<k; l++) m[l] = (l<=0 ? (l<k-1 ? a : ++a) : l<k-1 ? ++a : (a+=2));
   return a>=k-1;
  }
  
  /// return next tuple at combination space:
  static bool nextcomb(int n,tuple_t &m) //! \pre 0 <= m[0] < ... < m[m.count-2] <= m[m.count-1]  \post 0 <= m[0] < ... < m[m.count-1] <= n , !ret <=> (m.count<0 || m[m.count-1]==n)
  {
   int k=m.count();
   int h=k, a;
   while (--h>=0 && (a=m[h])>=n-k+h);
   if (h<0) { if (k>0) m[k-1]=n; return false; }
   for (int l=h; l<k; l++) m[l]=++a;
   return true;
  }
  
  /// return previous tuple at combination space:
  static bool prevcomb(int n,tuple_t &m) //! \pre 0 <= m[0] < ... < m[m.count-1] <= n  \post 0 <= m[0] < ... < m[m.count-2] <= m[m.count-1] , !ret <=> (m.count<0 || m[m.count-1]==m.count-2)
  {
   int k=m.count();
   int h=k, a=iMin, b=iMin;
   while (--h>=0 && (a=m[h])>=b) b=a-1;
   if (h<0 && a<=0) { if (k>0) m[k-1]=k-2; return false; }
   for (int l=h+1; l<k;) m[l]=b, l++, b=n-k+l;
   return true;
  }
};

typedef valcomb<int> intcomb; ///< integer combination type

} // end of Common namespace

using namespace Common;

} // end of InfoSelCore namespace

#endif // end of COMM_H_ header file

