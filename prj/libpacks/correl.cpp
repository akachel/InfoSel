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
/// Implementation of feature selection algorithms based on correlation coefficient.
/**
 * @file       correl.cpp
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2004.03.30
 * @date       2009.07.12
 *
 * @contents  
 *             Implementation of feature selection algorithms based on correlation coefficient.
 *              
 * @bibliography  
 *             - M.A. Hall,
 *               Correlation-based Feature Subset Selection for Machine Learning. PhD thesis, 
 *               Department of Computer Science, University of Waikato, Waikato, N.Z., 1999.
 *             - W.H. Press, S.A. Teukolsky, W.T. Vetterling, and B.P. Flannery,
 *               Numerical recipes in C. 
 *               The art of scientific computing, Cambridge, University Press, Cambridge, UK, 1988.
 *             - J. Biesiada, W. Duch, 
 *               Feature Selection for High-Dimensional Data: A Kolmogorov-Smirnov Correlation-Based Filter
 *               CORES 2005, Proc. 4 th International Conference on Computer Recognition Systems, Advanced in Soft Computing"
 *               1, 2005, 22-25 May, pp. 95-99, Springer Verlag,
 */

#include "InfoSelAPI.h"
REGPACK(correl)

namespace _CORREL_ /** \copybrief correl.cpp \copydetails correl.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    PearsonCC
 *   \brief    Ranking algorithm with criterion function based on Pearson correlation coefficient
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class PearsonCC : public Algorithm {
  real delta_;
 private: 
  enum { vRank, vSel, vCount };
  int get_numvariants_() const
  { return vCount; }
  string get_variantlabs_(int vnt) const
  {
   switch (vnt) {
    default: case vRank: return "ranking";
    case vSel: return "selection";
   }
  }
  /*! Member function which gives the name of cirterion function, search strategy, and default parameters */
  string get_pars_decl_() const
  { return "real delta=0.001"; }
  string get_pars_desc_() const
  { return "delta - threshold for statistical predominant features"; }
  string get_body_desc_() const
  { return "Pearson correlation coefficient based feature selection"; }
 /*!
  * Two main function
  *  read_ - is responsible for reading all parameters needed by algorithm
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
   /*! Constructor which give the shorter name of algorithm and search strategies */
  PearsonCC(): Algorithm("pcc") {}
};
REGALGC(PearsonCC)

void PearsonCC::read_(parstream &pars,repfile &rep)
{
 pars("predominant feature threshold",rep.cp().log())
     (0.001) >> delta_;
}

void PearsonCC::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &G=file.auxset(1);
 datset &S=file.finset();

 datspace _F_(F,1);

 vector R_FC = abs(avg(corrc(_F_,C)));
 
 rep.log() << "Correlation coefficient R(F,C) = " << sorted << R_FC << endl;

 G.reversion(true);

 subdatset f = argmax(_F_, R_FC);
 while (f!=Nil) {
  F -= f;
  G += f;
  if (completed(G)) break;
  f = argmax(_F_, R_FC);
  if (R_FC(f) <= delta) break;
 }

 if (vnt==vRank) S <<= G;
 else
 if (vnt==vSel) {
  rep.log() << ordered << G << endl;

  datspace _G_(G,1);
  datspace _S_(S,1);
  
  matrix R_GG = abs(avg(avg(corrc(_G_,_G_))));

  rep.log() << "Correlation coefficient R(G,G) = " << R_GG << endl;

  S.reversion(true);
  _G_.ordering(true);

  for (subdatset g(_G_,Last); g!=Nil; --g) {
   G -= g;
   S += g;
   real r_gC = R_FC(g);
   subdatset s = argmin(_S_, R_FC);
   for (subdatset h(_G_,true); h!=Nil; ++h) 
   if (R_GG(s,h)>r_gC) {
    G-=h;
    F+=h;
   }
  }
 /**
  S.reversion(true);
  _G_.ordering(false);

  subdatset g = argmax(_G_, R_FC);
  while (g!=Nil) {
   G -= g;
   S += g;
   real r_gC = R_FC(g);
   subdatset s = argmin(_S_, R_FC);
   for (subdatset h(_G_); h!=Nil; ++h) 
   if (R_GG(s,h)>r_gC) {
    G-=h;
    F+=h;
   }
   g = argmax(_G_, SU_GC);
  }
 */
 } 
}

} // end of _CORREL_ namespace
