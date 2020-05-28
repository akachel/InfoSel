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
     // ////// Copyright (C) 2010, 2010
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
/// Mutual Information based algorithms with different search strategies
/**
 * @file       mi.cpp
 * @version    1.00
 * @author     Jacek Biesiada and Adam Kachel
 * @since      2002.11.18
 * @date       2010.03.20
 *
 * @contents  
 *             Mutual information based algorithms with four search strategies 
 *             (ranking, forward, backward, and greedy) 
 *
 * @bibliography  
 *             - Peng, H.C., Long, F., and Ding, C., 
 *               "Feature selection based on mutual information: criteria of max-dependency, max-relevance, and min-redundancy," 
 *               IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 27, No. 8, pp. 1226-1238, 2005.
 *             - Yao, Y. Y.
 *               "Information-theoretic measures for knowledge discovery and data mining", 
 *               in Entropy Measures, Maximum Entropy Principle and Emerging Applications , 
 *               Karmeshu (ed.), Springer, pp. 115-136, 2003.
 *             - Duch W., Wieczorek T., Biesiada J., Blachnik M. 
 *               Comparision of feature ranking methods based on information entropy. 
 *               in Proc. of International Joint Conference on Neural Networks (IJCNN'04). IEEE Press. 2004
 */

#include "InfoSelAPI.h"
REGPACK(mi)

namespace _MI_ /** \copybrief mi.cpp \copydetails mi.cpp */ { 

using namespace InfoSelAPI; 
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *      \class          MInfo
 *      \brief          The  multidimensional feature selection algorithm with mutual information as criterion function
 *                      with ranking, forward, backward, and greedy search strategies
 *      \author         Jacek Biesiada & Adam Kachel
 *      \version        1.0.0
 */

class MInfo : public Algorithm {
  real delta_;
  int minsubcard_, maxsubcard_;
 private:
 /*! Member function which gives the name cirterion function, search strategy, and default parameters */ 
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
  { return "Mutual Information coefficient based feature selection"; }
/*!
 * Two main function:
 *  read_ - is responsible for reading all parameters needed by algorithm
 *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
 */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */  
  MInfo(): Algorithm("mi") {}
};
REGALGC(MInfo)

void MInfo::read_(parstream &pars,repfile &rep)
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

void MInfo::run_(datfile &file,repfile &rep)
{
 const int vnt=variant(); /*! vnt - represents a search method */
 const real delta=delta_; /*! cut-off value for finishing a ranking */

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset(); 

 datspace _F_(F,1);
 datspace _S_(S,1);

 table P_FC = prob(_F_,C); /*! table of joint probability between features and class */
 table P_F = prob(_F_);    /*! table of probability for features */
 table P_C = prob(C);      /*! table of probability for class */
 
 table H_FC = -sum(sum(P_FC*log2(P_FC))); /*! table of entropies between features and class */
 table H_F = -sum(P_F*log2(P_F));         /*! table of entropies for features */
 table H_C = -sum(P_C*log2(P_C));         /*! table of entropy for class */

 vector MI_FC = H_C + H_F - H_FC;         /*! table of mutual information between features and class */

 rep.log() << "Entropy H(F,C) = " << H_FC << endl; /* Initial reporting lines */ 
 rep.log() << "Entropy H(F) = " << H_F << endl;
 rep.log() << "Entropy H(C) = " << H_C << endl;
 rep.log() << "MI-coefficient U(F,C) = " << sorted << MI_FC << endl;

 // vector I_FC = sum(sum( P_FC*log2(P_FC/(P_F^P_C)) ));
 // rep.log() << "Mutual information I(F,C) = " << sorted << I_FC << endl;
  
 S.reversion(true);
 
 if (vnt==vRank) { 
  subdatset f = argmax(_F_, MI_FC ); /*! Main loop in case of ranking with cut-off value delta */
  while (!f.isnil()) {
   F-=f;
   S+=f;
   if (completed(file)) break;
   f = argmax(_F_, MI_FC);
   if (MI_FC(f) <= delta) break;
  }
 } else
 if (vnt==vfSel) { /*! Main loop in case of forward search strategy */
  //rep.log() << ordered << G << endl;

  subdatset f = argmax(_F_, MI_FC); 
  real maxu = MI_FC(f);
  while (!f.isnil()) {
   rep.log() << "Maximal MI-coefficient Umax=U( " << f << " ) = " << maxu << endl;
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

    real u_SgC = H_C() + H_Sg() - H_SgC();
    rep.log() << "MI-coefficient U( " << S << " , " << g << " , C ) = " << u_SgC << endl;

    if (u_SgC>maxu) {
     maxu=u_SgC;
     maxg=g;
    }
   }
   f=maxg;
  }
 } else
 if (vnt==vbSel) { /*! Main loop in case of backward search strategy */
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

     u_FgC = H_C() + H_Fg() - H_FgC();
     rep.log() << "MI-coefficient u( " << F << " -> " << g << " , C ) = " << u_FgC << endl;
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
   rep.log() << "Maximal MI-coefficient U = " << maxu << " found for feature f = " << maxg << endl;
   if (completed(file)) break;
  }
 } else
 if (vnt==vGreedy) { /*! Main loop in case of greedy (exhausted) search strategy */
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

     real u_gC = H_C() + H_g() - H_gC();
     rep.log() << "MI-coefficient u( " << g << " , C ) = " << u_gC << endl;

     if (u_gC>maxu) {
      maxu=u_gC;
      maxg=g;
     }
    } 
    if (maxg.isnil()) break;
    F-=maxg;
    S+=maxg;
    rep.log() << "Maximal MI-coefficient U = " << maxu << " found for subset f = " << maxg << endl;
    if (completed(file)) break;
   }
   if (!F.isempty()) {
    rep.log() << "Remaining subset F = " << F << endl;
    S <<= F;
   }
  } 
 }
}

} // end of _MI_ namespace

