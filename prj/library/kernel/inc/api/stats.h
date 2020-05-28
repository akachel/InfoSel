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
/// Statistical Function Routines At Algorithm Implementation Tools
/**
 * @file       stats.h
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2009.07.12
 * @date       2009.11.22
 *
 * @contents  
 *             This file contains declarations of algorithm implementation tools.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef STATS_H_
#define STATS_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Routines for Calculating
/// Statistical Functions 
///
/// The routines are applied 
/// to any input data arguments 
/// passed as different data 
/// structures. The calculation 
/// results are stored in dense 
/// tables with small number of 
/// dimensions, which is depended 
/// on dimensionality of routine 
/// arguments.
///

namespace statses {
 table corrc(const Data::space &_F_,const Data::space &_S_); //!< \post y=rho(f,s,k,l)==Data::corrc(f(k),s(l))
 table corrc(const Data::space &_F_,const Data::attrib &C); //!< \post y=rho(f,k)==Data::corrc(f(k),C)
 table corrc(const Data::subset &f,const Data::subset &s); //!< \post y=rho(k,l)==Data::corrc(f(k),s(l))
 table corrc(const Data::subset &f,const Data::attrib &C); //!< \post y=rho(k)==Data::corrc(f(k),C)
 table corrc(const Data::attrib &C,const Data::subset &f); //!< \post y=rho(k)==Data::corrc(C,f(k))
 table corrc(const Data::set &F,const Data::set &S); //!< \post y=rho(k,l)==Data::corrc(F(k),S(l))
 table corrc(const Data::set &F,const Data::attrib &C); //!< \post y=rho(k)==Data::corrc(F(k),C)
 table corrc(const Data::attrib &C,const Data::set &F); //!< \post y=rho(k)==Data::corrc(C,F(k))
 table corrc(const Data::set &F,const Data::subset &s); //!< \post y=rho(k,l)==Data::corrc(F(k),s(l))

 table fscor(const Data::space &_F_,const Data::space &_S_); //!< \post y=rho(f,s,k,l)==Data::fscor(f(k),s(l))
 table fscor(const Data::space &_F_,const Data::attrib &C); //!< \post y=rho(f,k)==Data::fscor(f(k),C)
 table fscor(const Data::subset &f,const Data::subset &s); //!< \post y=rho(k,l)==Data::fscor(f(k),s(l))
 table fscor(const Data::subset &f,const Data::attrib &C); //!< \post y=rho(k)==Data::fscor(f(k),C)
 table fscor(const Data::attrib &C,const Data::subset &f); //!< \post y=rho(k)==Data::fscor(C,f(k))
 table fscor(const Data::set &F,const Data::set &S); //!< \post y=rho(k,l)==Data::fscor(F(k),S(l))
 table fscor(const Data::set &F,const Data::attrib &C); //!< \post y=rho(k)==Data::fscor(F(k),C)
 table fscor(const Data::attrib &C,const Data::set &F); //!< \post y=rho(k)==Data::fscor(C,F(k))
 table fscor(const Data::set &F,const Data::subset &s); //!< \post y=rho(k,l)==Data::fscor(F(k),s(l))

 table tscor(const Data::space &_F_,const Data::space &_S_); //!< \post y=rho(f,s,k,l)==Data::tscor(f(k),s(l))
 table tscor(const Data::space &_F_,const Data::attrib &C); //!< \post y=rho(f,k)==Data::tscor(f(k),C)
 table tscor(const Data::subset &f,const Data::subset &s); //!< \post y=rho(k,l)==Data::tscor(f(k),s(l))
 table tscor(const Data::subset &f,const Data::attrib &C); //!< \post y=rho(k)==Data::tscor(f(k),C)
 table tscor(const Data::attrib &C,const Data::subset &f); //!< \post y=rho(k)==Data::tscor(C,f(k))
 table tscor(const Data::set &F,const Data::set &S); //!< \post y=rho(k,l)==Data::tscor(F(k),S(l))
 table tscor(const Data::set &F,const Data::attrib &C); //!< \post y=rho(k)==Data::tscor(F(k),C)
 table tscor(const Data::attrib &C,const Data::set &F); //!< \post y=rho(k)==Data::tscor(C,F(k))
 table tscor(const Data::set &F,const Data::subset &s); //!< \post y=rho(k,l)==Data::tscor(F(k),s(l))
}
using namespace statses;

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of STATS_H_ header file

