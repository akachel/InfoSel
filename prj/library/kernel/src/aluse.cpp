 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////          Library User Interface        ////////
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
/// Defintions Of Algorithm User Elements
/**
 * @file       aluse.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains defintions of selection algorithm related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dfile.h"
#include "albas.h"
#include "aluse.h"

namespace InfoSelUI {
 
namespace Algorithms {

/////////////////////////////
////
//// algs_collection_accessor_ 
//// class definition
////

char algs_collection_accessor_::usepacks_[maxnitems_][maxbuflen_];
int algs_collection_accessor_:: nusepacks_=0;
Algorithm *algs_collection_accessor_::useinsts_[maxnitems_];
int algs_collection_accessor_::nuseinsts_=0;

int algs_collection_accessor_::apply_using_directive_(const char *pack,int &iuse)
{
 if (iuse<=0) 
 if (!assemble_(pack)) return -1;
 return iuse++;
}

bool algs_collection_accessor_::assemble_(const char *pack)
{
 int packlen=strlen(pack);
 if (nusepacks_>=maxnitems_ || packlen<=0) return false;

 for (int k=0; k<nusepacks_; k++)
 if (strcmp(pack,usepacks_[k])==0) return false;

 int l=nusepacks_++;
 int m=min2(packlen,(int)maxbuflen_);
 strncpy(usepacks_[l],pack,m);
 usepacks_[l][m]='\0';
 isunstructed_=true;

 return true;
} 

void algs_collection_accessor_::construct_()
{
 algs_collection_base_::construct_();
 
 nuseinsts_=0;
 for (int k=0; k<nusepacks_; k++) {
  string usepack=usepacks_[k];
  
  int l=0;
  for (;;) {
   Algorithm *reginst=0;
   while (l<nreginsts_) {
    Algorithm &reg=*reginsts_[l++];
    if (usepack==reg.package()) {
     reginst=&reg;
     break;
    }
   }
   if (reginst==0) break;
   
   const string &regpack=reginst->package();
   const string &regname=reginst->name();
   int i=nuseinsts_; // i<maxnitems_
   while (i>=0) {
    string usep = i>0 ? string(useinsts_[i-1]->package()) : string("");
    if (regpack.compare(usep)>=0) {
     while (regpack==usep && i>=0) {
      string usen = i>0 ? string(useinsts_[i-1]->name()) : string("");
      if (regname.compare(usen)>=0) break;
      i--;
      usep = i>0 ? string(useinsts_[i-1]->package()) : string("");
     }
     for (int j=nuseinsts_; j>i; j--) useinsts_[j]=useinsts_[j-1];
     useinsts_[i]=reginst;
     nuseinsts_++;
     break;
    }
    i--;
   }
  } 
 } 
}

bool algs_collection_accessor_::contains(const string &tag) const
{
 if (isunstructed_) construct_(); 
 bool ret=false;
 int i=0;
 while (i<nuseinsts_) {
  const Algorithm &inst=*useinsts_[i];
  string t=tag;
  inst.cutvariant(t);
  if (inst.istagged(t)) {
   ret=true;
   break;
  }
  i++;
 }
 return ret;
}

Algorithm *algs_collection_accessor_::operator()(const string &tag)
{
 if (isunstructed_) construct_(); 
 Algorithm *ret=0;
 int i=0;
 while (i<nuseinsts_) {
  Algorithm &inst=*useinsts_[i];
  string t=tag;
  int v=inst.cutvariant(t);
  if (inst.istagged(t)) {
   inst.variant(v);
   ret=&inst;
   break;
  }
  i++;
 }
 return ret;
}

Algorithm &algs_collection_accessor_::operator[](const string &tag)
{ 
 Algorithm *inst=(*this)(tag);
 if (!inst) throw xUnknownAlgorithm();   
 return *inst;
}

algs_collection_accessor_ Algs;
algs_collection_accessor_ &SelAlgSet=Algs;
algs_collection_accessor_ &Assembly=Algs;

} // end of Algorithms namespace

} // end of InfoSelUI namespace

