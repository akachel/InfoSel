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
/// Implementation of Fast Correlation-Based Filter algorithms.
/**
 * @file       fcbf.cpp
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2009.03.06
 * @date       2009.03.20
 *
 * @contents  
 *             Implementation of Fast Correlation-Based Filter algorithms.
 *
 * @bibliography  
 *             - L. Yu and H. Liu, 
 *               Feature selection for high-dimensional data: A fast correlation-based Filter Solution,  
 *               In Proceedings of the 12th International Conference on Machine Learning (ICML-03), Washington, D.C., 
 *               pp. 856-863, San Francisco, CA, 2003. Morgan Kaufmann. 
 *             - L. Yu and H. Liu,
 *               Efficient Feature Selection via Analysis of Relevance and Redundancy,
 *               Journal of Machine Learning Research, 4, pp. 1205-1224, 2004.
 */

#include "InfoSelAPI.h"
REGPACK(fcbf)

namespace _FCBF_ /** \copybrief fcbf.cpp \copydetails fcbf.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    FastCBF
 *   \brief    Ranking algorithm with criterion function based on SUC (Symmetrical Uncertainty Coefficient) plus 
 *             redundancy reduction (Huang Liu algorithm)
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class FastCBF : public Algorithm {
  real delta_;
 private: 
  /*! Member function which gives the name cirterion function, search strategy, and default parameters */
  string get_pars_decl_() const
  { return "real delta=0.005"; }
  string get_pars_desc_() const
  { return "delta - predefined threshold for relevant features"; }
  string get_body_desc_() const
  { return "Fast correlation-based filter, as proposed by L. Yu and H. Liu.\n"
           "A defnition of predominant correlation includes correlations\n"
           "beetwen feature and classes and between pairs of features.\n" 
           "The FastCBF algorithm does a typical ranking using SUC coeffcient\n"
           "to determine class-feature relevance, setting some threshold value\n"
           "SUC >= delta to decide how many features should be taken. In the second\n" 
           "part redundant features are removed by defining the predominant features.\n"; }
  /*!
   * Two main function
   *  read_ - is responsible for reading all parameters needed by algorithm
   *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
   */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  /*! Constructor which give the shorter name of algorithm and search strategies */
  FastCBF(): Algorithm("fcbf") {}
};
REGALGC(FastCBF)

void FastCBF::read_(parstream &pars,repfile &rep)
{
 pars("relevant feature threshold",rep.cp().log())
     (0.005) >> delta_;
}

void FastCBF::run_(datfile &file,repfile &rep)
{
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &Fp=file.auxset(1);
 datset &G=file.auxset(2);
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _G_(G,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);
 table H_F = -sum(P_F*log2(P_F));
 table H_C = -sum(P_C*log2(P_C));
 table MI_FC = sum(sum( P_FC*log2(P_FC/(P_F^P_C)) ));
 vector SU_FC = 2.0*(MI_FC/(H_F+H_C));
 table P_FF = prob(_F_,_F_);
 table MI_FF = sum(sum( P_FF*log2( P_FF/((P_F^P_F)*Per(4)(0,2,1,3)) ) ));
 matrix SU_FF = 2.0*(MI_FF/(H_F+H_F));

 rep.log() << "Mutual information, MI(F,C) = " << MI_FC << endl;
 rep.log() << "Symmetrical uncertainty coefficient, SUC(F,C) = " << sorted << SU_FC << endl;
 rep.log() << "Mutual information, MI(F,F) = " << MI_FF << endl;
 rep.log() << "Symmetrical uncertainty coefficient, SUC(F,F) = " << SU_FF << endl;

 G.reversion(true);

 subdatset f = argmax(_F_, SU_FC);
 while (f!=Nil) {
  F-=f;
  if (SU_FC(f)<delta) Fp+=f;  
  else {
   G+=f;
   if (completed(G)) break;
  } 
  f = argmax(_F_, SU_FC);
 }
 F <<= Fp;
 
 rep.log() << endl << ordered << G << endl;

 S.reversion(true);
 _G_.ordering(true);

 for (subdatset g(_G_,Last); g!=Nil; --g) {
  G-=g;
  S+=g;
  for (subdatset h(_G_,true); h!=Nil; ++h) 
  if (SU_FF(g,h)>=SU_FC(h)) {
   G-=h;
   F+=h;
  }
 }
}

} // end of _FCBF_ namespace
