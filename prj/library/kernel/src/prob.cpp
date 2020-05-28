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
/// Definitions Of Dense Probability Routines
/**
 * @file       prob.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
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
#include "prob.h"

namespace InfoSelAPI {
               
namespace Algorithms {

namespace Tools {

namespace probs {

/////////////////////////////
////
//// dense probability
//// routine definitions
////

table prob(const Data::space &_F_,const Data::space &_S_,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(_F_,_S_,C):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "_S_ = " << _S_ << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(_F_.card(),_S_.card(),_F_.nparts(),_S_.nparts(),C.nparts());
 int n=min3(_F_.ninsts(),_S_.ninsts(),C.ninsts());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (Data::subset s(_S_); !s.isnil(); ++s)
 for (int i=0; i<n; i++) 
 y(f,s,f[i],s[i],C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::space &_F_,const Data::space &_S_)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(_F_,_S_):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "_S_ = " << _S_ << endl;
#endif
 table y(_F_.card(),_S_.card(),_F_.nparts(),_S_.nparts());
 int n=Common::min2(_F_.ninsts(),_S_.ninsts());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (Data::subset s(_S_); !s.isnil(); ++s)
 for (int i=0; i<n; i++) 
 y(f,s,f[i],s[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::space &_F_,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(_F_,C):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(_F_.card(),_F_.nparts(),C.nparts());
 int n=Common::min2(_F_.ninsts(),C.ninsts());
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (int i=0; i<n; i++) 
 y(f,f[i],C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::space &_F_)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(_F_):" << endl;
 cdeb << "_F_ = " << _F_ << endl;
#endif
 table y(_F_.card(),_F_.nparts());
 int n=_F_.ninsts();
 for (Data::subset f(_F_); !f.isnil(); ++f)
 for (int i=0; i<n; i++) 
 y(f,f[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::subset &f,const Data::subset &s,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(f,s,C):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(f.nparts(),s.nparts(),C.nparts());
 int n=min3(f.ninsts(),s.ninsts(),C.ninsts());
 for (int i=0; i<n; i++) 
 y(f[i],s[i],C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::subset &f,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(f,s):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(f.nparts(),s.nparts());
 int n=Common::min2(f.ninsts(),s.ninsts());
 for (int i=0; i<n; i++) 
 y(f[i],s[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::subset &f,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(f,C):" << endl;
 cdeb << "f = " << f << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(f.nparts(),C.nparts());
 int n=Common::min2(f.ninsts(),C.ninsts());
 for (int i=0; i<n; i++) 
 y(f[i],C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::subset &f)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(f):" << endl;
 cdeb << "f = " << f << endl;
#endif
 table y(f.nparts());
 int n=f.ninsts();
 for (int i=0; i<n; i++) 
 y(f[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::attrib &C,const Data::subset &f,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(C,f,s):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "f = " << f << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(C.nparts(),f.nparts(),s.nparts());
 int n=min3(C.ninsts(),f.ninsts(),s.ninsts());
 for (int i=0; i<n; i++) 
 y(C[i],f[i],s[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::attrib &C,const Data::subset &f)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(C,f):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "f = " << f << endl;
#endif
 table y(C.nparts(),f.nparts());
 int n=Common::min2(C.ninsts(),f.ninsts());
 for (int i=0; i<n; i++) 
 y(C[i],f[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::set &F,const Data::set &S,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(F,S,C):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(F.nparts(),S.nparts(),C.nparts());
 int n=min3(F.ninsts(),S.ninsts(),C.ninsts());
 for (int i=0; i<n; i++) 
 y(F[i],S[i],C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::set &F,const Data::set &S)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(F,S):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
#endif
 table y(F.nparts(),S.nparts());
 int n=Common::min2(F.ninsts(),S.ninsts());
 for (int i=0; i<n; i++) 
 y(F[i],S[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::set &F,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(F,C):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(F.nparts(),C.nparts());
 int n=Common::min2(F.ninsts(),C.ninsts());
 for (int i=0; i<n; i++) 
 y(F[i],C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::set &F)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(F):" << endl;
 cdeb << "F = " << F << endl;
#endif
 table y(F.nparts());
 int n=F.ninsts();
 for (int i=0; i<n; i++) 
 y(F[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::attrib &C,const Data::set &F,const Data::set &S)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(C,F,S):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "F = " << F << endl;
 cdeb << "S = " << S << endl;
#endif
 table y(C.nparts(),F.nparts(),S.nparts());
 int n=min3(C.ninsts(),F.ninsts(),S.ninsts());
 for (int i=0; i<n; i++) 
 y(C[i],F[i],S[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::attrib &C,const Data::set &F)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(C,F):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "F = " << F << endl;
#endif
 table y(C.nparts(),F.nparts());
 int n=Common::min2(C.ninsts(),F.ninsts());
 for (int i=0; i<n; i++) 
 y(C[i],F[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::set &F,const Data::subset &s,const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(F,s,C):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(F.nparts(),s.nparts(),C.nparts());
 int n=min3(F.ninsts(),s.ninsts(),C.ninsts());
 for (int i=0; i<n; i++) 
 y(F[i],s[i],C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::set &F,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(F,s):" << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(F.nparts(),s.nparts());
 int n=Common::min2(F.ninsts(),s.ninsts());
 for (int i=0; i<n; i++) 
 y(F[i],s[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::attrib &C,const Data::set &F,const Data::subset &s)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(C,F,s):" << endl;
 cdeb << "C = " << C << endl;
 cdeb << "F = " << F << endl;
 cdeb << "s = " << s << endl;
#endif
 table y(C.nparts(),F.nparts(),s.nparts());
 int n=min3(C.ninsts(),F.ninsts(),s.ninsts());
 for (int i=0; i<n; i++) 
 y(C[i],F[i],s[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

table prob(const Data::attrib &C)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=prob(C):" << endl;
 cdeb << "C = " << C << endl;
#endif
 table y(C.nparts());
 int n=C.ninsts();
 for (int i=0; i<n; i++) 
 y(C[i])++;
 y/=n;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return table(y);
}

} // end of probs namespace

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

