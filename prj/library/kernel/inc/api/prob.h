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
/// Dense Probability Routines At Algorithm Implementation Tools 
/**
 * @file       prob.h
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

#ifndef PROB_H_
#define PROB_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Routines for Calculating
/// Dense Probability Distribution
///
/// The distribution is calculated
/// for any input data arguments 
/// passed as different data structures. 
/// The calculation results are stored in
/// dense tables with small number of 
/// dimensions, which is depended 
/// on dimensionality of routine 
/// arguments.
///

namespace probs {
 table prob(const Data::space &_F_,const Data::space &_S_,const Data::attrib &C); //!< \post y=P(f,s,pf,ps,pC)
 table prob(const Data::space &_F_,const Data::space &_S_); //!< \post y=P(f,s,pf,ps)
 table prob(const Data::space &_F_,const Data::attrib &C); //!< \post y=P(f,pf,pC)
 table prob(const Data::space &_F_); //!< \post y=P(f,pf)
 table prob(const Data::subset &f,const Data::subset &s,const Data::attrib &C); //!< \post y=P(pf,ps,pC)
 table prob(const Data::subset &f,const Data::subset &s); //!< \post y=P(pf,ps)
 table prob(const Data::subset &f,const Data::attrib &C); //!< \post y=P(pf,pC)
 table prob(const Data::subset &f); //!< \post y=P(pf)
 table prob(const Data::attrib &C,const Data::subset &f,const Data::subset &s); //!< \post y=P(pC,pf,ps)
 table prob(const Data::attrib &C,const Data::subset &f); //!< \post y=P(pC,pf)
 table prob(const Data::set &F,const Data::set &S,const Data::attrib &C); //!< \post y=P(pF,pS,pC)
 table prob(const Data::set &F,const Data::set &S); //!< \post y=P(pF,pS)
 table prob(const Data::set &F,const Data::attrib &C); //!< \post y=P(pF,pC)
 table prob(const Data::set &F); //!< \post y=P(pF)
 table prob(const Data::attrib &C,const Data::set &F,const Data::set &S); //!< \post y=P(pC,pF,pS)
 table prob(const Data::attrib &C,const Data::set &F); //!< \post y=P(pC,pF)
 table prob(const Data::set &F,const Data::subset &s,const Data::attrib &C); //!< \post y=P(pF,ps,pC)
 table prob(const Data::set &F,const Data::subset &s); //!< \post y=P(pF,ps)
 table prob(const Data::attrib &C,const Data::set &F,const Data::subset &s); //!< \post y=P(pC,pF,ps)
 table prob(const Data::attrib &C); //!< \post y=P(pC)
}
using namespace probs;

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of PROB_H_ header file

