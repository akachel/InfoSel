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
/// Implementation of Pearson Redundancy-Based Filter algorithms
/**
 * @file       prbf.cpp
 * @version    1.00
 * @author     Jacek Biesiada and Adam Kachel
 * @since      2008.11.15
 * @date       2009.03.20
 *
 * @contents  
 *             Implementation of Pearson Redundancy-Based Filter algorithms
 *
 * @bibliography  
 *             - J. Biesiada, W. Duch, 
 *               Feature Selection for High-Dimensional Data: A Pearson Redundancy Based Filter 
 *               Advances in Soft Computing Vol. 45, 242-249, 2008.
 */

#include "InfoSelAPI.h"
REGPACK(prbf)

// ALGLIB function declarations:
double chisquareprobdens(double x, double k);
double invchisquarecdistribution(double y, double k);

namespace _PRBF_ /** \copybrief prbf.cpp \copydetails prbf.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *      \class          PearsonCBF_Base_
 *      \brief          Basic class for all algorithms based on Pearson chi^2 statistics
 *      \author         Jacek Biesiada & Adam Kachel
 *      \version        1.0.0
 */

class PearsonCBF_Base_ : public Algorithm {
 protected:
  enum { vProbDensTest, vCumDistrTest, vCount };
  real alpha_;
  real delta_;
  real chsq_probdens_(real x, int n);
  real chsq_cumdistr_(real a, int n);
  string get_P_desc_() const
  { return "Pearson Redundancy Based Filter\n"
           "Feature ranking is performed first, requiring\n"
           "selection of the ranking index. In the second\n"
           "step redundant features are removed using\n"
           "the Pearson test. The Pearson chi^2 test measures the difference\n"
           "between the probability distribution of two binned random variables."; }

 private: 
  int get_numvariants_() const
  { return vCount; }
  string get_variantlabs_(int vnt) const
  {
   switch (vnt) {
    case vProbDensTest: return "probability density based test";
    default: case vCumDistrTest: return "cumulative distribution based test";
   }
  }
  /*! Member function which gives the name cirterion function, search strategy, and default parameters */
  string get_pars_decl_() const
  { return "real alpha=0.05, real delta=0.005"; }
  string get_pars_desc_() const
  { return "alpha - predefined level of statistical significance ; delta - predefined threshold for relevant features"; }
  /*!
   * Main function:
   *  read_ - is responsible for reading all parameters needed by algorithm
   */
  void read_(parstream &pars,repfile &rep);
 public:
  /*! Constructor */
  PearsonCBF_Base_(const string &symbol): Algorithm(symbol) {}
};

void PearsonCBF_Base_::read_(parstream &pars,repfile &rep)
{
 pars("statistical significance level",rep.cp().log())
     (0.05) >> alpha_;
 pars("relevant feature threshold",rep.cp().log())
     (0.005) >> delta_;
}

real PearsonCBF_Base_::chsq_probdens_(real x, int n)
{                         
 return chisquareprobdens(x, n);
} 

real PearsonCBF_Base_::chsq_cumdistr_(real a, int n)
{                         
 return invchisquarecdistribution(a, n);
} 

 /*!
 *   \class    PearsonCBF
 *   \brief    Redundancy removing ranking -- Pearson Redundancy Based Filter algorithms
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class PearsonCBF : public PearsonCBF_Base_ {
 private: 
 /*! Member function which gives the name cirterion function, search strategy, and default parameters */  
  string get_body_desc_() const
  { return get_P_desc_(); }
 /*!
  * Main function:
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */ 
  PearsonCBF(): PearsonCBF_Base_("prbf") {}
};
REGALGC(PearsonCBF)

void PearsonCBF::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real alpha=alpha_;
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
 real a = F.nsglparts()<3 ? -0.5 : 0.0;
 matrix chsq_FF = F.ninsts()*sum( sqr(diagsub(P_F, P_F)+a) / (_F_.card()| P_F) );
 real ndegs_F = F.nsglparts()-1;

 rep.log() << "Mutual information, MI(F,C) = " << MI_FC << endl;
 rep.log() << "Symmetrical uncertainty coefficient, SUC(F,C) = " << sorted << SU_FC << endl;
 rep.log() << "Pearson statistics, chi^2(F,F) = " << chsq_FF << endl;

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
 
 _G_.ordering(true);

 if (vnt==vProbDensTest) {
  rep.log() << "statistical significance level, alpha = " << alpha << endl;
  for (subdatset g(_G_,Last); g!=Nil; --g) {
   G -= g;
   S += g | SU_FC(g);
   for (subdatset h(_G_,true); h!=Nil; ++h) 
   if (chsq_probdens_(chsq_FF(g,h),ndegs_F)>alpha) {
    G-=h;
    F+=h;
   }
  }
 } else 
 if (vnt==vCumDistrTest) {
  const real chsq_alpha=chsq_cumdistr_(alpha,ndegs_F);
  rep.log() << "statistical significance value, chi^2(alpha) = " << chsq_alpha << " for predefined level, alpha = " << alpha << endl;
  for (subdatset g(_G_,Last); g!=Nil; --g) {
   G -= g;
   S += g | SU_FC(g);
   for (subdatset h(_G_,true); h!=Nil; ++h) 
   if (chsq_FF(g,h)<chsq_alpha) {
    G-=h;
    F+=h;
   }
  }
 } 
}

/*!
 *      \class          PearsonClassCBF
 *      \brief          Redundancy removing ranking -- Pearson Redundancy Class Based Filter algorithms
 *      \author         Jacek Biesiada & Adam Kachel
 *      \version        1.0.0
 */

class PearsonClassCBF : public PearsonCBF_Base_ {
 private:
  string get_body_desc_() const
  { return "Class-Consistent "+get_P_desc_(); }

  void run_(datfile &file,repfile &rep);
 public:
  PearsonClassCBF(): PearsonCBF_Base_("pc_rbf") {}
};
REGALGC(PearsonClassCBF)

void PearsonClassCBF::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real alpha=alpha_;
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
 table D_F_C = (~prob(_F_,C)/prob(C))*Per(3)(2,0,1);
 real a = F.nsglparts()<3 ? -0.5 : 0.0;
 matrix chsq_FF = F.ninsts() * sum(sum( sqr( diag(diagsub(D_F_C, D_F_C)*Per(5)(0,2,4,1,3))*Per(4)(0,1,3,2)+a ) / (_F_.card()| D_F_C) ));
 real ndegs_F = F.nsglparts()-1;

 rep.log() << "Mutual information, MI(F,C) = " << MI_FC << endl;
 rep.log() << "Symmetrical uncertainty coefficient, SUC(F,C) = " << sorted << SU_FC << endl;
 rep.log() << "Pearson class-consistent statistics, chi^2(F,F) = " << chsq_FF << endl;

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
 
 _G_.ordering(true);

 if (vnt==vProbDensTest) {
  rep.log() << "statistical significance level, alpha = " << alpha << endl;
  for (subdatset g(_G_,Last); g!=Nil; --g) {
   G -= g;
   S += g | SU_FC(g);
   for (subdatset h(_G_,true); h!=Nil; ++h) 
   if (chsq_probdens_(chsq_FF(g,h),ndegs_F)>alpha) {
    G-=h;
    F+=h;
   }
  }
 } else 
 if (vnt==vCumDistrTest) {
  const real chsq_alpha=chsq_cumdistr_(alpha,ndegs_F);
  rep.log() << "statistical significance value, chi^2(alpha) = " << chsq_alpha << " for predefined level, alpha = " << alpha << endl;
  for (subdatset g(_G_,Last); g!=Nil; --g) {
   G -= g;
   S += g | SU_FC(g);
   for (subdatset h(_G_,true); h!=Nil; ++h) 
   if (chsq_FF(g,h)<chsq_alpha) {
    G-=h;
    F+=h;
   }
  }
 } 
}

} // end of _PRBF_ namespace
