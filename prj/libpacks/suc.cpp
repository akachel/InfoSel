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
/// Algorithm based on Symmetrical Uncertainty Coefficient with four search strategies
/**
 * @file       suc.cpp
 * @version    1.00
 * @author     Jacek Biesiada and Adam Kachel
 * @since      2006.11.18
 * @date       2009.03.20
 *
 * @contents  
 *             Implementation of algorithims based on Symmetrical Uncertainty Coefficient 
 *             (normalized mutual information) with four search strategies (ranking, forward, 
 *             backward, and greedy )
 *
 * @bibliography  
 *             - J. Biesiada , W. Duch, 
 *               Feature Selection for High-Dimensional Data: A Kolmogorov-Smirnov Correlation-Based Filter Solution" 
 *               CORES 2005, 4th International Conference on Computer Recognition Systems, 22-25 May 2005. Rydzyna. 
 *               Advances in Soft Computing, Computer Recognition Systems. pp. 95-105. 
 *             - Pudil, P. and Novovivcov'a, J. and Kittler, J. 
 *               Floating search methods in feature selection. 
 *               Pattern Recognition Letters, 1994, 15(-), 1119-1125.
 *             - W.H. Press, S.A. Teukolsky, W.T. Vetterling, and B.P. Flannery,
 *               Numerical recipes in C. The art of scientific computing. 
 *               Cambridge University Press, Cambridge, UK, 1988.
 */

#include "InfoSelAPI.h"
REGPACK(suc)

namespace _SUC_ /** \copybrief suc.cpp \copydetails suc.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *      \class          SUCcoeff
 *      \brief          The  multi-dimensional feature selection algorithm with Symetrical Uncertainty Coefficient as criterial
 *                      with ranking, forward, backward, and greedy asearch strategy
 *      \author         Jacek Biesiada & Adam Kachel
 *      \version        1.0.0
 */

class SUCcoeff : public Algorithm {
  real delta_;
  int minsubcard_, maxsubcard_;
 private:
 /*! Member function which gives the name cirterial function, search strategy, and default parameters */ 
  enum { vRank, vfSel, vbSel, vGreedy, vCount };
  int get_numvariants_() const
  { return vCount; }
  string get_variantlabs_(int vnt) const
  {
   switch (vnt) {
    default: case vRank: return "ranking";
    case vfSel: return "forward selection";
    case vbSel: return "backward selection";
    case vGreedy: return "multi-dimensional ranking";
   }
  }

  string get_pars_decl_() const
  { return "real delta=0.001, int minsubcard=1, int maxsubcard=0"; }
  string get_pars_desc_() const
  { return "delta - threshold for statistical predominant features ; minsubcard - minimal cardinal number of subsets ; maxsubcard - maximal cardinal number of subsets"; }
  string get_body_desc_() const
  { return "Symetrical Uncertainly Coefficient based feature selection"; }
 /*!
  * Two main function
  *  read_ - is responsible for reading all parameters needed by algorithm
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */

  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */
  SUCcoeff(): Algorithm("suc") {}
};
REGALGC(SUCcoeff)

void SUCcoeff::read_(parstream &pars,repfile &rep)
{
 pars("predominant feature threshold",rep.cp().log())
     (0.001) >> delta_;
 if (variant()==vGreedy) {
  pars("minimal sub-cardinal number",rep.cp().log())
      (1) >> minsubcard_;
  pars("maximal sub-cardinal number",rep.cp().log())
      (0) >> maxsubcard_;
 }    
}

void SUCcoeff::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _S_(S,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);
 
 table H_FC = -sum(sum(P_FC*log2(P_FC)));
 table H_F = -sum(P_F*log2(P_F));
 table H_C = -sum(P_C*log2(P_C));

 table MI_FC = H_C + H_F - H_FC;
 vector SUC_FC = 2.0 * ( MI_FC / (H_C + H_F) );

 rep.log() << "Entropy H(F,C) = " << H_FC << endl;
 rep.log() << "Entropy H(F) = " << H_F << endl;
 rep.log() << "Entropy H(C) = " << H_C << endl;
 rep.log() << "MI-coefficient U(F,C) = " << sorted << MI_FC << endl;
 rep.log() << "SUC-coefficient SUC(F,C) = " << sorted << SUC_FC << endl;

 S.reversion(true);
 
 if (vnt==vRank) { 
  subdatset f = argmax(_F_, SUC_FC);
  while (!f.isnil()) {
  F-=f;
  S+=f;
   if (completed(file)) break;
   f = argmax(_F_, SUC_FC);
   if (SUC_FC(f) <= delta) break;
  }
 } else
 if (vnt==vfSel) {
  //rep.log() << ordered << G << endl;

  subdatset f = argmax(_F_, SUC_FC);
  real maxu = SUC_FC(f);
  while (!f.isnil()) {
   rep.log() << "Maximal SUC-coefficient Umax=U( " << f << " ) = " << maxu << endl;
   F-=f;
   S+=f;
   if (completed(file)) break;

   maxu=-rInf;
   subdatset maxg;
   for (subdatset g(_F_); !g.isnil(); ++g) {
    sparse P_SgC = distrib(S,g,C);
    sparse P_Sg = distrib(S,g);

    table H_SgC = -sum(sum(sum(P_SgC*log2(P_SgC))));
    table H_Sg = -sum(sum(P_Sg*log2(P_Sg)));

    real u_SgC = 2.0 * (H_C() + H_Sg() - H_SgC())/(H_C() + H_Sg());
    rep.log() << "SUC-coefficient U( " << S << " , " << g << " , C ) = " << u_SgC << endl;

    if (u_SgC>maxu) {
     maxu=u_SgC;
     maxg=g;
    }
   }
   f=maxg;
  }
 } else
 if (vnt==vbSel) {
  while (true) {    
   real maxu=-rInf;
   subdatset maxg(_F_);
   for (subdatset g=maxg; !g.isnil(); ++g) {
    F-=g;
    real u_FgC;
    if (F.isempty()) u_FgC=0.0;
    else {
     sparse P_FgC = distrib(F,C);
     sparse P_Fg = distrib(F);

     table H_FgC = -sum(sum(P_FgC*log2(P_FgC)));
     table H_Fg = -sum(P_Fg*log2(P_Fg));

     u_FgC = 2.0 * ( H_C() + H_Fg() - H_FgC())/(H_C()+H_Fg());
     rep.log() << "SUC-coefficient u( " << F << " -> " << g << " , C ) = " << u_FgC << endl;
    } 
    if (u_FgC>maxu) {
     maxu=u_FgC;
     maxg=g;
    }
    F+=g;
   } 
   if (maxg.isnil()) break;
   F-=maxg;
   S+=maxg;
   rep.log() << "Maximal SUC-coefficient U = " << maxu << " found for feature f = " << maxg << endl;
   if (completed(file)) break;
  }
 } else
 if (vnt==vGreedy) {
  const int minsubcard = minsubcard_<1 ? 1 : minsubcard_;
  const int maxsubcard = maxsubcard_<minsubcard ? F.card()-1 : maxsubcard_;
  for (int subcard=minsubcard; subcard<=maxsubcard; subcard++) {
   if (subcard>minsubcard) {
    rep.log() << endl;
    S.print_order(rep.cp().log());
    rep.cp().log() << endl;
    F <<= S;
   }
   rep.cp().log() << endl << "Cardinal number of subset: " << subcard << endl;
   F.reversion(false);
   datspace _G_(F,subcard);
   while (true) {    
    real maxu=-rInf;
    subdatset maxg(_G_);
    for (subdatset g=maxg; !g.isnil(); ++g) {
     sparse P_gC = distrib(g,C);
     sparse P_g = distrib(g);

     table H_gC = -sum(sum(P_gC*log2(P_gC)));
     table H_g = -sum(P_g*log2(P_g));

     real u_gC = 2.0 * ( H_C() + H_g() - H_gC())/(H_C()+H_g());
     rep.log() << "SUC-coefficient u( " << g << " , C ) = " << u_gC << endl;

     if (u_gC>maxu) {
      maxu=u_gC;
      maxg=g;
     }
    } 
    if (maxg.isnil()) break;
    F-=maxg;
    S+=maxg;
    rep.log() << "Maximal SUC-coefficient U = " << maxu << " found for subset f = " << maxg << endl;
    if (completed(file)) break;
   }
   if (!F.isempty()) {
    rep.log() << "Remaining subset F = " << F << endl;
    S <<= F;
   }
  } 
 }
}

} // end of _SUC_ namespace

