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
/// Implementation of algorithms based on max-relevance min-redundancy heuristic
/**
 * @file       mrmr.cpp
 * @version    1.00
 * @author     Jacek Biesiada and Adam Kachel
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             Implementation of algorithms based on max-relevance min-redundancy heuristic, such as:
 *             Mutual Information Difference, Mutual Information Quotient, F-score Correlation Difference,
 *             and F-score Correlation Quotient.
 *
 * @bibliography  
 *             - Hanchuan Peng, Fuhui Long, and Chris Ding
 *               Feature selection based on mutual information: criteria of max-dependency, max-relevance, and min-redundancy
 *               IEEE Transactions on Pattern Analysis and Machine Intelligence, 27(8) pp. 1226-1238, 2005.
 *             - Chris Ding and Hanchuan Peng
 *               Minimum redundancy feature selection from microarray gene expression data
 *               Journal of Bioinformatics and Computational Biology, 3(2), pp. 185-205, 2005.
 */

#include "InfoSelAPI.h"
REGPACK(mrmr)

namespace _MRMR_ /** \copybrief mrmr.cpp \copydetails mrmr.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    MID
 *   \brief    Mutual Information Difference - algorithm based on max-relevance-min-redundancy heuristic
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */
class MID : public Algorithm {
 private: 
  /*! Member function which give the name cirterial function, search strategy, and default parameters */  
 string get_body_desc_() const
  { return "Mutual Information Difference - algorithm based on max-relevance-min-redundancy heuristic"; }  
  /*!
   * Main function
   *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
   */
  void run_(datfile &file,repfile &rep);
 public:
  /*! Constructor which give the shorter name of algorithm and search strategies */
  MID(): Algorithm("mid") {}
};
REGALGC(MID)

void MID::run_(datfile &file,repfile &rep)
{
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

 subdatset f = argmax(_F_, MI_FC);
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  vector MI_FFC = Mi_FC-(1.0/S.card())*sum(_S_,Mi_FF);
  f = argmax(_F_, MI_FFC );
 }
}

/*!
 *   \class    MIQ
 *   \brief    Mutual Information Quotient - algorithm based on max-relevance-min-redundancy heuristic
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */
 
class MIQ : public Algorithm {
 private: 
 /*! Member function which give the name cirterial function, search strategy, and default parameters */
 string get_body_desc_() const
   { return "Mutual Information Quotient - algorithm based on max-relevance-min-redundancy heuristic"; }
 /*!
  * Main function
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */ 
  MIQ(): Algorithm("miq") {}
};
REGALGC(MIQ)

void MIQ::run_(datfile &file,repfile &rep)
{
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

 subdatset f = argmax(_F_, MI_FC);
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  vector MI_FFC = Mi_FC/((1.0/S.card())*sum(_S_,Mi_FF));
  f = argmax(_F_, MI_FFC);
 }
}

/*!
 *   \class    FID
 *   \brief    F-score Information Difference - algorithm based on max-relevance-min-redundancy heuristic
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */
 
class FID : public Algorithm {
 private: 
 /*! Member function which give the name cirterial function, search strategy, and default parameters */
 string get_body_desc_() const
 { return "F-score Information Difference - algorithm based on max-relevance-min-redundancy heuristic"; }
 /*!
  * Main function
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */ 
  FID(): Algorithm("fid") {}
};
REGALGC(FID)

void FID::run_(datfile &file,repfile &rep)
{
 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _S_(S,1);

 table Fsc_FC = avg(fscor(_F_,C));
 table Rcc_FF = abs(avg(avg(corrc(_F_,_F_))));
 vector FSC_FC = Fsc_FC;
 matrix RCC_FF = Rcc_FF;

 rep.log() << "F-score F(F,C) = " << sorted << FSC_FC << endl;
 rep.log() << "Correlation Coefficient between features R(F,F) = " << RCC_FF << endl;
  
 S.reversion(true);

 subdatset f = argmax(_F_, FSC_FC);
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  vector FSC_FFC = Fsc_FC-(1.0/S.card())*sum(_S_,Rcc_FF);
  f = argmax(_F_, FSC_FFC);
 }
}

/*!
 *   \class    FIQ
 *   \brief    Mutual Information Quotient - algorithm based on max-relevance-min-redundancy heuristic
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */
 
class FIQ : public Algorithm {
 private:
 /*! Member function which give the name cirterial function, search strategy, and default parameters */
 string get_body_desc_() const
   { return "F-score Information Quotient - algorithm based on max-relevance-min-redundancy heuristic"; }
 /*!
  * Main function
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
 private: 
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */ 
  FIQ(): Algorithm("fiq") {}
};
REGALGC(FIQ)

void FIQ::run_(datfile &file,repfile &rep)
{
 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _S_(S,1);

 table Fsc_FC = avg(fscor(_F_,C));
 table Rcc_FF = abs(avg(avg(corrc(_F_,_F_))));
 vector FSC_FC = Fsc_FC;
 matrix RCC_FF = Rcc_FF;

 rep.log() << "F-score F(F,C) = " << sorted << FSC_FC << endl;
 rep.log() << "Correlation Coefficient between features R(F,F) = " << RCC_FF << endl;
  
 S.reversion(true);

 subdatset f = argmax(_F_, FSC_FC);
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  vector FSC_FFC = Fsc_FC-((1.0/S.card())/sum(_S_,Rcc_FF));
  f = argmax(_F_, FSC_FFC);
 }
}

} // end of _MRMR_ namespace

