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
/// Definitions Of Basic Elements For Input Data Structures
/**
 * @file       data.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains defintions of input data related elements.
 */

#include "comm.h"
#include "data.h"

namespace InfoSelCore {

namespace Data {

/////////////////////////////
////
//// exception type definitions
////
                            
void xDataError::printmsg_(ostream &out) const
{
 print_msg_(out);
 if (!dnam.empty()) out << " for the " << dnam;
}

void xOneUniqDistrb::print_msg_(ostream &out) const
{
 out << "Error: Data distribution with only one unique value is not allowed";
}

void xNumUniqDistrbReq::print_msg_(ostream &out) const
{
 out << "Error: Data distribution with " << numu << " unique values required";
}

void xNoPartMeth::print_msg_(ostream &out) const
{
 out << "Error: No data partition method specified";
}

void xColumnLinkFail::print_msg_(ostream &out) const
{
 out << "Fatal error: Data column object linkage failure";
}

void xSetLinkFail::print_msg_(ostream &out) const
{
 out << "Fatal error: Data set object linkage failure";
}

void xSpaceLinkFail::print_msg_(ostream &out) const
{
 out << "Fatal error: Data space object linkage failure";
}

void xSubsetLinkFail::print_msg_(ostream &out) const
{
 out << "Fatal error: Data subset object linkage failure";
}
                    
void xSpaceCardOverflow::print_msg_(ostream &out) const
{
 out << "Error: Integer overflow of data space cardinality";
}

void xSubsetIdnOverflow::print_msg_(ostream &out) const
{
 out << "Error: Integer overflow of data subset identifical number";
}

void xDomainNPartsOverflow::print_msg_(ostream &out) const
{
 out << "Error: Integer overflow of total number of data domain partition cells";
}

void xDomainPartOverflow::print_msg_(ostream &out) const
{
 out << "Error: Integer overflow of total index of data domain partition cell";
}

void xTooManyImdSets::print_msg_(ostream &out) const
{
 out << "Error: Too many intermediate data sets required in pipeline processing of data file";
}

void xTooManyAuxSets::print_msg_(ostream &out) const
{
 out << "Error: Too many auxiliary data sets required in data file processing";
}

void xFileProcFail::print_msg_(ostream &out) const
{
 out << "Fatal error: Data file processing failure";
}

/////////////////////////////
////
//// datum class definition
////

datum::datum(): 
 val_(0.0), val_tup_(0.0),
 part_(-1), part_tup_(-1)
{}
  
datum::~datum()
{}
          
void datum::read_val_(istream &in)
{
 real v;
 in >> ws >> v;
 if (in) {
  val_=v;
  *val_tup_=v; 
 } 
}

void datum::print_instn(ostream &out) const
{
 out << '(';
 out << val_ << ':' << part_;
 out << ')';
}

} // end of Data namespace

} // end of InfoSelCore namespace

