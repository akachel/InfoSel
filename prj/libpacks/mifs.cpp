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
/// Implementation of Battiti's algorithm and other algorithms similar to mifs, like mifsu, amifs
/**
 * @file       mifs.cpp
 * @version    1.00
 * @author     Jacek Biesiada and Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             Implementation of Battiti's algorithm and other algorithms similar to mifs, like mifsu, amifs
 *
 * @bibliography  
 *             - Roberto Battiti, 
 *               Using mutual information for selecting features in supervised neural net learning," 
 *               IEEE Trans. Neural Networks, vol. 5, no. 4, July 1994.
 *             - Kwak N.,  Chong-Ho Choi,
 *               Input feature selection for classification problems,
 *               IEEE-EC, 12(1), pp. 143-159, Jan  2002.
 *             - M. Tesmer and P. A. Estevez.
 *               AMIFS: Adaptive Feature Selection by Using Mutual Information.
 *               In Proceedings of International Joint Conference on Neural Networks (IJCNN 2004), Budapest, IEEE Press.
 */

#include "InfoSelAPI.h"
REGPACK(mifs)

namespace _MIFS_ /** \copybrief mifs.cpp \copydetails mifs.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    MIFS
 *   \brief    Redundancy-shifting ranking algorithm - Battiti's algorithm 
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class MIFS : public Algorithm {
  real beta_;
 private: 
  /*! Member function which gives the name of criterion function, search strategy, and default parameters */
  string get_pars_decl_() const
  { return "real beta=0.5"; }
  string get_body_desc_() const
  { return "MIFS (Mutual Information Feature Selection) algorithm - Battiti's algorithm"; }
/*!
 * Two main functions:
 *  read_ - is responsible for reading all parameters needed by algorithm
 *  run_  - is responsible for running algorithm (datafile - represents input data, repfile - represents report file)
 */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  /*! Constructor which give the shorter name of algorithm and search strategies */
  MIFS(): Algorithm("mifs") {}
};
REGALGC(MIFS)

void MIFS::read_(parstream &pars,repfile &rep)
{
 pars("beta parameter",rep.cp().log())
     (0.5) >> beta_;
}

void MIFS::run_(datfile &file,repfile &rep)
{
 const real beta=beta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _S_(S,1);

 table P_FC = prob(_F_,C);
 table P_FF = prob(_F_,_F_);
 table P_F = prob(_F_);
 table P_C = prob(C);
 table Mi_FC = sum(sum( P_FC*log2(P_FC/(P_F^P_C)) ));
 table Mi_FF = sum(sum( P_FF*log2(P_FF/((P_F^P_F)*Per(4)(0,2,1,3))) ));
 vector MI_FC = Mi_FC;
 matrix MI_FF = Mi_FF;

 rep.log() << "Mutual information MI(F,C) = " << sorted << MI_FC << endl;
 rep.log() << "Mutual information MI(F,F) = " << MI_FF << endl;
  
 S.reversion(true);

 subdatset f = argmax(_F_, MI_FC );
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  vector MI_FFC = Mi_FC-beta*sum(_S_,Mi_FF);
  f = argmax(_F_, MI_FFC );
 }
}

/*!
 *   \class    MIFSU
 *   \brief    Redundancy-shifting ranking algorithm - Kwak's algorithm 
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class MIFSU : public Algorithm {
  real beta_;
 private: 
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  MIFSU(): Algorithm("mifsu") {}
};
REGALGC(MIFSU)

void MIFSU::read_(parstream &pars,repfile &rep)
{
 pars("beta parameter",rep.cp().log())
     (0.5) >> beta_;
}

void MIFSU::run_(datfile &file,repfile &rep)
{
 const real beta=beta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _S_(S,1);

 table P_FC = prob(_F_,C);
 table P_FF = prob(_F_,_F_);
 table P_F = prob(_F_);
 table P_C = prob(C);
 table H_C = -sum(P_C*log2(P_C));
 table Mi_FC = sum(sum( P_FC*log2(P_FC/(P_F^P_C)) ));
 table Mi_FF = sum(sum( P_FF*log2(P_FF/((P_F^P_F)*Per(4)(0,2,1,3))) ));
 table Gi_FF = (Mi_FC/H_C)*Mi_FF;
 vector MI_FC = Mi_FC;
 matrix MI_FF = Mi_FF;

 rep.log() << "Mutual information MI(F,C) = " << sorted << MI_FC << endl;
 rep.log() << "Mutual information MI(F,F) = " << MI_FF << endl;
  
 S.reversion(true);

 subdatset f = argmax(_F_, MI_FC);
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  vector MI_FFC = Mi_FC-beta*sum(_S_,Gi_FF);
  f = argmax(_F_, MI_FFC);
 }
}

/*!
 *   \class    AMIFS
 *   \brief    ????Redundancy-shifting ranking algorithm - Battiti's algorithm 
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class AMIFS : public Algorithm {
  real beta_;
 private: 
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  AMIFS(): Algorithm("amifs") {}
};
REGALGC(AMIFS)

void AMIFS::read_(parstream &pars,repfile &rep)
{
 pars("beta parameter",rep.cp().log())
     (0.5) >> beta_;
}

void AMIFS::run_(datfile &file,repfile &rep)
{
 const real beta=beta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _S_(S,1);

 table P_FC = prob(_F_,C);
 table P_FF = prob(_F_,_F_);
 table P_F = prob(_F_);
 table P_C = prob(C);
 table H_F = -sum(P_F*log2(P_F));
 table Mi_FC = sum(sum( P_FC*log2(P_FC/(P_F^P_C)) ));
 table Mi_FF = sum(sum( P_FF*log2(P_FF/((P_F^P_F)*Per(4)(0,2,1,3))) ));
 vector MI_FC = Mi_FC;
 matrix MI_FF = Mi_FF;

 rep.log() << "Entropy of features H(F) = " << H_F << endl;
 rep.log() << "Mutual information MI(F,C) = " << sorted << MI_FC << endl;
 rep.log() << "Mutual information MI(F,F) = " << MI_FF << endl;
  
 table minH_FF(_F_.card(),_F_.card());
 for (subdatset f1(_F_); f1!=Nil; ++f1)
 for (subdatset f2(_F_); f2!=Nil; ++f2) {
  real m12 = min2(H_F(f1),H_F(f2));
  minH_FF(f1,f2) = m12;
 }
 table NMi_FF = Mi_FF/minH_FF;
 matrix NMI_FF = NMi_FF;

 rep.log() << "Minimal entropy between two features H(i) vs. H(j) = " << minH_FF << endl;
 rep.log() << "Normalized mutual information between featurees = " << NMI_FF << endl;

 S.reversion(true);

 subdatset f = argmax(_F_, MI_FC);
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  vector MI_FFC = Mi_FC-beta*sum(_S_,NMi_FF);
  f = argmax(_F_, MI_FFC);
 }
}

} // end of _MIFS_ namespace

