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
/// Gaussian Field Routines At Algorithm Implementation Tools
/**
 * @file       gauss.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.12.02
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains declarations of algorithm implementation tools.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef GAUSS_H_
#define GAUSS_H_

namespace InfoSelAPI {
 
namespace Algorithms {

namespace Tools {

/////////////////////////////
///
/// Routines For Formulating 
/// Gaussian Density Fields
///
/// The Gaussian field is formulated
/// for any input data arguments 
/// passed as different data structures. 
/// The result is a continuous 
/// field of probability distribution 
/// density of input data smoothed by 
/// gaussian function.
///

namespace gausses {
 field gauss(const Data::space &_F_,const Data::space &_S_,const Data::attrib &C,real sd=1.0); //!< \post y(xf,xs)=table{f,s,pC}
 field gauss(const Data::space &_F_,const Data::space &_S_,real sd=1.0); //!< \post y(xf,xs)=table{f,s}
 field gauss(const Data::space &_F_,const Data::attrib &C,real sd=1.0); //!< \post y(xf)=table{f,pC}
 field gauss(const Data::space &_F_,real sd=1.0); //!< \post y(xf)=table{f}
 field gauss(const Data::subset &f,const Data::subset &s,const Data::attrib &C,real sd=1.0); //!< \post y(xf,xs)=table{pC}
 field gauss(const Data::subset &f,const Data::subset &s,real sd=1.0); //!< \post y(xf,xs)=table{}
 field gauss(const Data::subset &f,const Data::attrib &C,real sd=1.0); //!< \post y(xf)=table{pC}
 field gauss(const Data::subset &f,real sd=1.0); //!< \post y(xf)=table{}
 field gauss(const Data::set &F,const Data::set &S,const Data::attrib &C,real sd=1.0); //!< \post y(xF,xS)=table{pC}
 field gauss(const Data::set &F,const Data::set &S,real sd=1.0); //!< \post y(xF,xS)=table{}
 field gauss(const Data::set &F,const Data::attrib &C,real sd=1.0); //!< \post y(xF)=table{pC}
 field gauss(const Data::set &F,real sd=1.0); //!< \post y(xF)=table{}
 field gauss(const Data::set &F,const Data::subset &s,const Data::attrib &C,real sd=1.0); //!< \post y(xF,xs)=table{pC}
 field gauss(const Data::set &F,const Data::subset &s,real sd=1.0); //!< \post y(xF,xs)=table{}
}
using namespace gausses;

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

#endif // end of GAUSS_H_ header file

