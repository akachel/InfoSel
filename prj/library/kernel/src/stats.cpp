 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////     Algorithm Programming Interface    ////////
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
/// Definitions Of Statistical Function Routines
/**
 * @file       stats.cpp
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2009.07.12
 * @date       2009.07.12
 *
 * @contents  
 *             This file contains definitions of algorithm tool related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dspac.h"
#include "dsubs.h"
#include "dfile.h"
#include "albas.h"
#include "altls.h"
#include "sparse.h"
#include "table.h"
#include "stats.h"

namespace InfoSelAPI {
               
namespace Algorithms {

namespace Tools {

namespace statses {

/////////////////////////////
////
//// statistical routine
//// definitions - corrc
////

table corrc(const Data::space &_F_,const Data::space &_S_)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(_F_,_S_):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "_S_ = " << _S_ << endl;
#endif
 table y(_F_.card(),_S_.card(),_F_.subcard(),_S_.subcard());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (Data::subset s(_S_); !s.isnil(); ++s)
 for (int k=0; k<f.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(f,s,k,l)=Data::corrc(f(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::space &_F_,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(_F_,C):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(_F_.card(),_F_.subcard());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (int k=0; k<f.card(); k++) 
 y(f,k)=Data::corrc(f(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::subset &f,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(f,s):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(f.card(),s.card());
 for (int k=0; k<f.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(k,l)=Data::corrc(f(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::subset &f,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(f,C):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(f.card());
 for (int k=0; k<f.card(); k++) 
 y(k)=Data::corrc(f(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::attrib &C,const Data::subset &f)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(C,f):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "f = " << f << endl;
#endif
 table y(f.card());
 for (int k=0; k<f.card(); k++) 
 y(k)=Data::corrc(C,f(k));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::set &F,const Data::set &S)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(F,S):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
#endif
 table y(F.card(),S.card());
 for (int k=0; k<F.card(); k++) 
 for (int l=0; l<S.card(); l++) 
 y(k,l)=Data::corrc(F(k),S(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::set &F,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(F,C):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(F.card());
 for (int k=0; k<F.card(); k++) 
 y(k)=Data::corrc(F(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::attrib &C,const Data::set &F)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(C,F):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "F = " << F << endl;
#endif
 table y(F.card());
 for (int k=0; k<F.card(); k++) 
 y(k)=Data::corrc(C,F(k));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table corrc(const Data::set &F,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=corrc(F,s):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(F.card(),s.card());
 for (int k=0; k<F.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(k,l)=Data::corrc(F(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

/////////////////////////////
////
//// statistical routine
//// definitions - fscor
////

table fscor(const Data::space &_F_,const Data::space &_S_)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(_F_,_S_):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "_S_ = " << _S_ << endl;
#endif
 table y(_F_.card(),_S_.card(),_F_.subcard(),_S_.subcard());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (Data::subset s(_S_); !s.isnil(); ++s)
 for (int k=0; k<f.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(f,s,k,l)=Data::fscor(f(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::space &_F_,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(_F_,C):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(_F_.card(),_F_.subcard());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (int k=0; k<f.card(); k++) 
 y(f,k)=Data::fscor(f(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::subset &f,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(f,s):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(f.card(),s.card());
 for (int k=0; k<f.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(k,l)=Data::fscor(f(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::subset &f,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(f,C):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(f.card());
 for (int k=0; k<f.card(); k++) 
 y(k)=Data::fscor(f(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::attrib &C,const Data::subset &f)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(C,f):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "f = " << f << endl;
#endif
 table y(f.card());
 for (int k=0; k<f.card(); k++) 
 y(k)=Data::fscor(C,f(k));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::set &F,const Data::set &S)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(F,S):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
#endif
 table y(F.card(),S.card());
 for (int k=0; k<F.card(); k++) 
 for (int l=0; l<S.card(); l++) 
 y(k,l)=Data::fscor(F(k),S(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::set &F,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(F,C):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(F.card());
 for (int k=0; k<F.card(); k++) 
 y(k)=Data::fscor(F(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::attrib &C,const Data::set &F)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(C,F):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "F = " << F << endl;
#endif
 table y(F.card());
 for (int k=0; k<F.card(); k++) 
 y(k)=Data::fscor(C,F(k));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table fscor(const Data::set &F,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=fscor(F,s):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(F.card(),s.card());
 for (int k=0; k<F.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(k,l)=Data::fscor(F(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

/////////////////////////////
////
//// statistical routine
//// definitions - tscor
////

table tscor(const Data::space &_F_,const Data::space &_S_)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(_F_,_S_):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "_S_ = " << _S_ << endl;
#endif
 table y(_F_.card(),_S_.card(),_F_.subcard(),_S_.subcard());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (Data::subset s(_S_); !s.isnil(); ++s)
 for (int k=0; k<f.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(f,s,k,l)=Data::tscor(f(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::space &_F_,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(_F_,C):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(_F_.card(),_F_.subcard());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (int k=0; k<f.card(); k++) 
 y(f,k)=Data::tscor(f(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::subset &f,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(f,s):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(f.card(),s.card());
 for (int k=0; k<f.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(k,l)=Data::tscor(f(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::subset &f,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(f,C):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(f.card());
 for (int k=0; k<f.card(); k++) 
 y(k)=Data::tscor(f(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::attrib &C,const Data::subset &f)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(C,f):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "f = " << f << endl;
#endif
 table y(f.card());
 for (int k=0; k<f.card(); k++) 
 y(k)=Data::tscor(C,f(k));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::set &F,const Data::set &S)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(F,S):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
#endif
 table y(F.card(),S.card());
 for (int k=0; k<F.card(); k++) 
 for (int l=0; l<S.card(); l++) 
 y(k,l)=Data::tscor(F(k),S(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::set &F,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(F,C):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(F.card());
 for (int k=0; k<F.card(); k++) 
 y(k)=Data::tscor(F(k),C);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::attrib &C,const Data::set &F)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(C,F):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "F = " << F << endl;
#endif
 table y(F.card());
 for (int k=0; k<F.card(); k++) 
 y(k)=Data::tscor(C,F(k));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table tscor(const Data::set &F,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tscor(F,s):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(F.card(),s.card());
 for (int k=0; k<F.card(); k++) 
 for (int l=0; l<s.card(); l++) 
 y(k,l)=Data::tscor(F(k),s(l));
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

} // end of statses namespace

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

