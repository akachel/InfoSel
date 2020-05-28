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
/// The Library Algorithm Programming Interface
/**
 * @file       InfoSelAPI.h
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.08.04
 * @date       2009.07.12
 *
 * @contents  
 *             This file/namespace contains the kernel's layer with a back-end 
 *             interface of the InfoSel++ library. The interface is intended for 
 *             implementing information theoretic feature selection algorithms 
 *             in the object oriented programming style.
 *             This file must be included to every C++ source file representing 
 *             an algorithm implementation package.
 */

#ifndef __cplusplus
# error C++ compiler required
#endif

#ifndef INFOSELAPI_H_
#define INFOSELAPI_H_

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
#include "field.h"
#include "stats.h"
#include "distrib.h"
#include "prob.h"
#include "gauss.h"
#include "tensor.h"
#include "alprg.h"

#endif // end of INFOSELAPI_H_ header file

