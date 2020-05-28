 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////            Algorithm Package           ////////
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
/// Implementation feature selection based on Markov Blanket
/**
 * @file       mblank.cpp
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             Implementation feature selection based on Markov Blanket
 *
 * @bibliography  
 *             - Daphne Koller and Mehran Sahami
 *               Toward Optimal Feature Selection, International Conference on Machine Learning
 *               pp. 284--292, 1996
 *             - Eric P. Xing and Michael I. Jordan and Richard M. Karp
 *               Feature selection for high-dimensional genomic microarray data,
 *               ICML '01: Proceedings of the Eighteenth International Conference on Machine Learning,
 *               pp. 601-608, 2001, Morgan Kaufmann Publishers Inc. San Francisco, CA, USA
 */

#include "InfoSelAPI.h"
REGPACK(mblank)

namespace _MBLANK_ /** \copybrief mblank.cpp \copydetails mblank.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    MarkovBlanket
 *   \brief    Feature selection algorithm based on Markov Blanket
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class MarkovBlanket : public Algorithm {
  int nblanks_;
 private: 
  /*! Member function which gives the name cirterion function, search strategy, and default parameters */
 string get_body_desc_() const
    { return "Feature selection algorithm based on Markov Blanket"; }
  /*!
   * Two main function:
   *  read_ - is responsible for reading all parameters needed by algorithm
   *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
   */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  /*! Constructor which give the shorter name of algorithm and search strategies */
  MarkovBlanket(): Algorithm("mbr") {}
};
REGALGC(MarkovBlanket)

void MarkovBlanket::read_(parstream &pars,repfile &rep)
{
 pars("cardinal number of Markov blanket",rep.cp().log())
     (1) >> nblanks_;
}

void MarkovBlanket::run_(datfile &file,repfile &rep)
{
 int nblanks=nblanks_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 if (nblanks>=F.card()) nblanks=F.card()-1;
 datspace _F_(F,1);
 datspace _M_(F,nblanks);
 nblanks=_M_.subcard();

 table g_FF(_F_.card(),_F_.card());
 for (subdatset h(_F_); !h.isnil(); ++h) {
  table P_hC = prob(h,C);
  table P_h = prob(h);
  table P_C_h = P_hC/P_h;
  for (subdatset f(_F_); !f.isnil(); ++f) {
   table P_fhC = prob(f,h,C);
   table P_fh = prob(f,h);
   table P_C_fh = P_fhC/P_fh;
   table g_fh = sum(sum(sum( P_C_fh*log2(((P_C_fh*Per(3)(1,2,0))/P_C_h)*Per(3)(2,0,1)) )));
   g_FF(f,h) = g_fh();
  }
 }

 rep.log() << "Cross-entropy gamma(F,F) = " << g_FF << endl;

 for (;;) {
  real mind=+rInf;
  subdatset f(_F_);
  for (subdatset h=f; !h.isnil(); ++h) {
   F-=h;
   real ming=+rInf;
   subdatset m(_M_);
   for (subdatset n=m; !n.isnil(); ++n) {
    real maxg=-rInf;
    for (int k=0; k<n.card(); k++) {
     real g=g_FF(h,n(k));
     if (g>maxg) maxg=g;
    }
    if (maxg<ming) {
     ming=maxg;
     m=n;
    }
   }
   F+=h;
   rep.log() << "Markov blanket m( " << h << " ) = " << m << endl;
   real d;
   if (m.isnil()) d=0.0;
   else {
    rep.log() << "   minimal cross-entropy gamma( " << h << " , mk ) = " << ming << endl;
    sparse P_mC = distrib(m,C);
    sparse P_m = distrib(m);
    sparse P_C_m = P_mC/P_m;
    sparse P_mhC = distrib(m,h,C);
    sparse P_mh = distrib(m,h);
    sparse P_C_mh = P_mhC/P_mh;
    sparse d_mh = sum(sum( P_mh*sum( P_C_mh*log2(((P_C_mh*Per(3)(0,2,1))/P_C_m)*Per(3)(0,2,1)) ) ));
    d=d_mh();
   }
   rep.log() << "Expected cross-entropy deltaG( " << h << " | " << m << " ) = " << d << endl;
   if (d<mind) {
    mind=d;
    f=h;
   }
  }
  if (f.isnil()) break;
  rep.log() << "Minimal expected cross-entropy deltaGmin = " << mind << " found for feature f = " << f << endl;
  F-=f;
  S+=f;
  if (completed(file)) break;
 }
}

} // end of _MBLANK_ namespace

