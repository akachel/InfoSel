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
/// Implementation of Kolmogorov-Smirnov Correlation-Based Filter algorithms
/**
 * @file       kscbf.cpp
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2008.11.15
 * @date       2009.03.20
 *
 * @contents  
 *             This fila contains an implementation of filter 
 *             algorithms based on the Kolmogorov-Smirnov correlation.
 *
 * @bibliography  
 *             - J. Biesiada, W. Duch, 
 *               Feature Selection for High-Dimensional Data: A Kolmogorov-Smirnov Correlation-Based Filter Solution 
 *               In Computer Recognition Systems. Proc. of the 4th International Conference on Computer Recognition Systems 
 *               (CORES 2005), Ed. M. Kurzynski, E. Puchala, M. Wozniak, A. Zolnierek, 
 *               Advances in Soft Computing, Springer Verlag, ISBN: 3-540-25054-9, pp. 95-104, 2005. 
 *             - J. Biesiada, W. Duch, 
 *               A Kolmogorov-Smirnov correlation-based filter solution for microarray gene expressions data.
 *               Springer Lecture Notes in Computer Science, Vol. 4985, pp. 285-294, 2008. 
 *               Presented at 14th Int. Conference on Neural Information Processing (ICONIP07), Kitakyushu, Japan, Nov.2007 
 *             - # Blachnik M., Duch W., Kachel A., Biesiada J., 
 *               Feature Selection for High-Dimensional Data: A Kolmogorov-Smirnov Class Correlation-Based Filter
 *               "Recent Developments of Artificial Intelligence Methods" AI-METH Series 2009, 
 *               Presented at Symposium On Methods of Artificial Intelligence, AI-METH 2009, 18-19 Nov. 2009.
 */

#include "InfoSelAPI.h"
REGPACK(kscbf)

namespace _KSCBF_ /** \copybrief kscbf.cpp \copydetails kscbf.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    KolmogorovSmirnovCBF_Base_
 *   \brief    Basic class for all algorithms based on Kolmogorov-Smirnov statistics
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class KolmogorovSmirnovCBF_Base_ : public Algorithm {
 protected:
  real alpha_;
  real delta_;
  real max_KS_distance_(real a, int n=-1);
  string get_KS_desc_() const
  { return "Kolmogorov-Smirnov Correlation-Based Filter\n"
           "Feature ranking is performed first, requiring\n"
           "selection of the ranking index. In the second\n"
           "step redundant features are removed using\n"
           "the K-S test. The K-S test measures the maximum\n"
           "difference between cummulative distribution\n" 
           "of two random variables."; }

 private: 
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
  KolmogorovSmirnovCBF_Base_(const string &symbol): Algorithm(symbol) {}
};

void KolmogorovSmirnovCBF_Base_::read_(parstream &pars,repfile &rep)
{
 pars("statistical significance level",rep.cp().log())
     (0.05) >> alpha_;
 pars("relevant feature threshold",rep.cp().log())
     (0.005) >> delta_;
}

real KolmogorovSmirnovCBF_Base_::max_KS_distance_(real a, int n)
{
 const int nmax = 171;
 static const real alpha_tab[nmax] = {0.001, 0.00108, 0.00116, 0.00126, 0.00136, 0.00146, 0.00158, 0.0017, 0.00186, 0.00198, 0.00213, 0.00229, 0.00247, 0.00265, 0.00285, 0.00307, 0.0033, 0.00354, 0.0038, 0.00408, 0.00438, 0.00469, 0.005, 0.00503, 0.00539, 0.00577, 0.00617, 0.00661, 0.00707, 0.00756, 0.00808, 0.00864, 0.00922, 0.00985, 0.01, 0.01051, 0.01121, 0.01195, 0.01274, 0.01357, 0.01446, 0.01539, 0.01638, 0.01742, 0.01852, 0.01969, 0.02, 0.02092, 0.02222, 0.02359, 0.02503, 0.02655, 0.02815, 0.02984, 0.03162, 0.03348, 0.03545, 0.03751, 0.03968, 0.04196, 0.04435, 0.04686, 0.04949, 0.05, 0.05224, 0.05513, 0.05815, 0.06132, 0.06463, 0.06809, 0.07171, 0.0755, 0.07944, 0.08357, 0.08787, 0.09235, 0.09703, 0.1, 0.1019, 0.10697, 0.11225, 0.11774, 0.12345, 0.12939, 0.13556, 0.14196, 0.14861, 0.1555, 0.16264, 0.17005, 0.17772, 0.18566, 0.19387, 0.20236, 0.21114, 0.22021, 0.22957, 0.23922, 0.24917, 0.25943, 0.27, 0.28087, 0.29206, 0.30356, 0.32748, 0.32748, 0.33992, 0.35266, 0.36571, 0.37907, 0.39273, 0.40668, 0.42093, 0.43545, 0.45026, 0.46532, 0.48063, 0.49619, 0.51197, 0.52796, 0.54414, 0.5605, 0.577, 0.59363, 0.61036, 0.62717, 0.64402, 0.66089, 0.67774, 0.69453, 0.71124, 0.72781, 0.74422, 0.76042, 0.77636, 0.79201, 0.80732, 0.82225, 0.83678, 0.85077, 0.86428, 0.87724, 0.88961, 0.90134, 0.91242, 0.92282, 0.9325, 0.94147, 0.94969, 0.95719, 0.96395, 0.96998, 0.97532, 0.97998, 0.984, 0.98741, 0.99027, 0.99262, 0.99452, 0.99603, 0.99719, 0.99807, 0.99872, 0.99917, 0.99949, 0.9997, 0.99983, 0.99991, 0.99995, 0.99998, 0.99999};
 static const real lambda_tab[nmax] = {1.95, 1.94, 1.93, 1.92, 1.91, 1.9, 1.89, 1.88, 1.87, 1.86, 1.85, 1.84, 1.83, 1.82, 1.81, 1.8, 1.79, 1.78, 1.77, 1.76, 1.75, 1.74, 1.731, 1.73, 1.72, 1.71, 1.7, 1.69, 1.68, 1.67, 1.66, 1.65, 1.64, 1.63, 1.627, 1.62, 1.61, 1.6, 1.59, 1.58, 1.57, 1.56, 1.55, 1.54, 1.53, 1.52, 1.518, 1.51, 1.5, 1.49, 1.48, 1.47, 1.46, 1.45, 1.44, 1.43, 1.42, 1.41, 1.4, 1.39, 1.38, 1.37, 1.36, 1.358, 1.35, 1.34, 1.33, 1.32, 1.31, 1.3, 1.29, 1.28, 1.27, 1.26, 1.25, 1.24, 1.23, 1.224, 1.22, 1.21, 1.2, 1.19, 1.18, 1.17, 1.16, 1.15, 1.14, 1.13, 1.12, 1.11, 1.1, 1.09, 1.08, 1.07, 1.06, 1.05, 1.04, 1.03, 1.02, 1.01, 1, 0.99, 0.98, 0.97, 0.95, 0.96, 0.94, 0.93, 0.92, 0.91, 0.9, 0.89, 0.88, 0.87, 0.86, 0.85, 0.84, 0.83, 0.82, 0.81, 0.8, 0.79, 0.78, 0.77, 0.76, 0.75, 0.74, 0.73, 0.72, 0.71, 0.7, 0.69, 0.68, 0.67, 0.66, 0.65, 0.64, 0.63, 0.62, 0.61, 0.6, 0.59, 0.58, 0.57, 0.56, 0.55, 0.54, 0.53, 0.52, 0.51, 0.5, 0.49, 0.48, 0.47, 0.46, 0.45, 0.44, 0.43, 0.42, 0.41, 0.4, 0.39, 0.38, 0.37, 0.36, 0.35, 0.34, 0.33, 0.32, 0.31, 0.3};
 if (n<0 || n>nmax) n=nmax;
 
 for (int i=0; i<n; i++) 
 if (a<=alpha_tab[i]) return lambda_tab[i];
 
 return 1.0;
} 

/*!
 *   \class    KolmogorovSmirnovCBF
 *   \brief    Redundancy removing ranking -- Kolmogorov-Smirnov Correlation-Based Filter
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */
class KolmogorovSmirnovCBF : public KolmogorovSmirnovCBF_Base_ {
 private: 
 /*! Member function which gives the name cirterion function, search strategy, and default parameters */
  string get_body_desc_() const
  { return get_KS_desc_(); }
 /*!
  * Main function:
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */
  KolmogorovSmirnovCBF(): KolmogorovSmirnovCBF_Base_("ks_cbf") {}
};
REGALGC(KolmogorovSmirnovCBF)

void KolmogorovSmirnovCBF::run_(datfile &file,repfile &rep)
{
 const real alpha=alpha_;
 const real delta=delta_;
 const real lambda_alpha=max_KS_distance_(alpha);

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
 table D_F = cum(prob(_F_));
 matrix lambda_FF = sqrt(F.ninsts()/2.0) * max(abs(diagsub(D_F, D_F)));

 rep.log() << "Mutual information, MI(F,C) = " << MI_FC << endl;
 rep.log() << "Symmetrical uncertainty coefficient, SUC(F,C) = " << sorted << SU_FC << endl;
 rep.log() << "statistical significance value, lambda(alpha) = " << lambda_alpha << " for predefined level, alpha = " << alpha << endl;
 rep.log() << "K-S statistics, lambda(F,F) = " << lambda_FF << endl;

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

 for (subdatset g(_G_,Last); g!=Nil; --g) {
  G -= g;
  S += g | SU_FC(g);
  for (subdatset h(_G_,true); h!=Nil; ++h) 
  if (lambda_FF(g,h)<lambda_alpha) {
   G-=h;
   F+=h;
  }
 }
}

/*!
 *   \class    KolmogorovSmirnovClassCBF
 *   \brief    Redundancy removing ranking -- Kolmogorov-Smirnov Class Correlation-Based Filter
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */
class KolmogorovSmirnovClassCBF : public KolmogorovSmirnovCBF_Base_ {
 private:
 /*! Member function which gives the name cirterion function, search strategy, and default parameters */  
  string get_body_desc_() const
  { return "Class-Consistent "+get_KS_desc_(); }
 /*!
  * Main function:
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */ 
  KolmogorovSmirnovClassCBF(): KolmogorovSmirnovCBF_Base_("ksc_cbf") {}
};
REGALGC(KolmogorovSmirnovClassCBF)

void KolmogorovSmirnovClassCBF::run_(datfile &file,repfile &rep)
{
 const real alpha=alpha_;
 const real delta=delta_;
 const real lambda_alpha=max_KS_distance_(alpha);

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
 table D_F_C = cum( (~prob(_F_,C)/prob(C))*Per(3)(2,0,1) );
 matrix lambda_FF = sqrt(F.ninsts()/2.0) * max(max(abs( diag(diagsub(D_F_C, D_F_C)*Per(5)(0,2,4,1,3))*Per(4)(0,1,3,2) )));

 rep.log() << "Mutual information, MI(F,C) = " << MI_FC << endl;
 rep.log() << "Symmetrical uncertainty coefficient, SUC(F,C) = " << sorted << SU_FC << endl;
 rep.log() << "statistical significance value, lambda(alpha) = " << lambda_alpha << " for predefined level, alpha = " << alpha << endl;
 rep.log() << "K-S class-consistent statistics, lambda(F,F) = " << lambda_FF << endl;

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

 for (subdatset g(_G_,Last); g!=Nil; --g) {
  G -= g;
  S += g | SU_FC(g);
  for (subdatset h(_G_,true); h!=Nil; ++h) 
  if (lambda_FF(g,h)<lambda_alpha) {
   G-=h;
   F+=h;
  }
 }
}

} // end of _KSCBF_ namespace
