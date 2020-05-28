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
/// Sparse Distribution Routines At Algorithm Implementation Tools
/**
 * @file       distrib.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains declarations of algorithm implementation tools.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef DISTRIB_H_
#define DISTRIB_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Routines for Calculating
/// Sparse Probability Distribution
///
/// The distribution is calculated
/// for any input data arguments 
/// passed as different data structures. 
/// The calculation results are stored
/// in sparse collections, which can have 
/// any number of dimensions depending on
/// dimensionality of routine arguments.
///

namespace distribs {
 sparse distrib(const Data::space &_F_,const Data::space &_S_,const Data::attrib &C); //!< \post y=P(f,s,pf,ps,pC)
 sparse distrib(const Data::space &_F_,const Data::space &_S_); //!< \post y=P(f,s,pf,ps)
 sparse distrib(const Data::space &_F_,const Data::attrib &C); //!< \post y=P(f,pf,pC)
 sparse distrib(const Data::space &_F_); //!< \post y=P(f,pf)
 sparse distrib(const Data::subset &f,const Data::subset &s,const Data::attrib &C); //!< \post y=P(pf,ps,pC)
 sparse distrib(const Data::subset &f,const Data::subset &s); //!< \post y=P(pf,ps)
 sparse distrib(const Data::subset &f,const Data::attrib &C); //!< \post y=P(pf,pC)
 sparse distrib(const Data::subset &f); //!< \post y=P(pf)
 sparse distrib(const Data::attrib &C,const Data::subset &f,const Data::subset &s); //!< \post y=P(pC,pf,ps)
 sparse distrib(const Data::attrib &C,const Data::subset &f); //!< \post y=P(pC,pf)
 sparse distrib(const Data::set &F,const Data::set &S,const Data::attrib &C); //!< \post y=P(pF,pS,pC)
 sparse distrib(const Data::set &F,const Data::set &S); //!< \post y=P(pF,pS)
 sparse distrib(const Data::set &F,const Data::attrib &C); //!< \post y=P(pF,pC)
 sparse distrib(const Data::set &F); //!< \post y=P(pF)
 sparse distrib(const Data::attrib &C,const Data::set &F,const Data::set &S); //!< \post y=P(pC,pF,pS)
 sparse distrib(const Data::attrib &C,const Data::set &F); //!< \post y=P(pC,pF)
 sparse distrib(const Data::set &F,const Data::subset &s,const Data::attrib &C); //!< \post y=P(pF,ps,pC)
 sparse distrib(const Data::set &F,const Data::subset &s); //!< \post y=P(pF,ps)
 sparse distrib(const Data::attrib &C,const Data::set &F,const Data::subset &s); //!< \post y=P(pC,pF,ps)
 sparse distrib(const Data::attrib &C); //!< \post y=P(pC)
}
using namespace distribs;

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of DISTRIB_H_ header file

