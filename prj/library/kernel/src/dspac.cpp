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
/// Definitions For Input Data Space
/**
 * @file       dspac.cpp
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
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dspac.h"

namespace InfoSelCore {

namespace Data {

/////////////////////////////
////
//// datspace class definition
////

datspace::datspace(const datset &s,int subcard):
 nlinks_(0), refset_(const_cast<datset *>(&s)), isordered_(false),
 nsubelms_(subcard<1 ? 1 : subcard>s.nptrcols_ ? s.nptrcols_ : subcard),
 card_rep_(totalcard_proxy_(*this),cardtuple_proxy_(*this)),
 nparts_rep_(totalnparts_proxy_(*this),npartstuple_proxy_(*this))
{
 refset_->nlinks_++;
 intcomb cmb(s.nptrcols_,nsubelms_);
 nsubs_=cmb.card();
 if (cmb.asinf()) infloc_=cmb;
 if (cmb.asmin()) minloc_=cmb;
 if (cmb.asmax()) maxloc_=cmb;
 if (cmb.assup()) suploc_=cmb;
 if (cmb.asnil()) nilloc_=cmb;
}

datspace::~datspace()
{ 
 if (nlinks_>0) throw xSpaceLinkFail();
 refset_->nlinks_--;
}

bool datspace::cardtuple_proxy_::operator()(inttuple &ret) const //! \pre that_.nsubelms_>0
{
 int m=that_.nsubelms_;
 int n=that_.nsuperelms_();
 ret.setup(m,n);
 return true;
}

bool datspace::totalcard_proxy_::operator()(int &ret) const //! \pre that_.nsubelms_>0
{
 ret=0;
 int N=that_.nsubs_;
 if (N<0) throw xSpaceCardOverflow();
 ret=N;
 return true;
}

bool datspace::npartstuple_proxy_::operator()(inttuple &ret) const //! \pre that_.nsubelms_>0
{
 int m=that_.nsubelms_;
 int n=that_.nsglparts();
 ret.setup(m,n);
 return true;
}

bool datspace::totalnparts_proxy_::operator()(int &ret) const //! \pre that_.nsubelms_>0
{
 ret=0;
 int m=that_.nsubelms_;
 int n=that_.nsglparts();
 int N;
 if (!chkmopow(N,n,m)) throw xDomainNPartsOverflow();
 ret=N;
 return true;
}

void datspace::print_conts(ostream &out) const
{
 out << *refset_ << " ^ " << nsubelms_;
}

} // end of Data namespace

} // end of InfoSelCore namespace

