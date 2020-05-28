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
/// Implementation of feature selection algorithms based on F-score.
/**
 * @file       fscore.cpp
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2004.03.30
 * @date       2009.07.12
 *
 * @contents  
 *             Implementation of feature selection algorithms based on F-score.
 *
 * @bibliography  
 *             - Chris Ding and Hanchuan Peng,
 *               Minimum redundancy feature selection from microarray gene expression data,
 *               Journal of Bioinformatics and Computational Biology, 3(2), pp. 185-205, 2005.
 */

#include "InfoSelAPI.h"
REGPACK(fscore)

namespace _FSCORE_ /** \copybrief fscore.cpp \copydetails fscore.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    Fscore
 *   \brief    Ranking algorithm with criterion function based on F-score statistics
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class Fscore : public Algorithm {
  real delta_;
 private: 
  enum { vRank, vCount };
  int get_numvariants_() const
  { return vCount; }
  string get_variantlabs_(int vnt) const
  {
   switch (vnt) {
    default: case vRank: return "ranking";
   }
  }
  /*! Member function which gives the name cirterion function, search strategy, and default parameters */
  string get_pars_decl_() const
  { return "real delta=0.001"; }
  string get_pars_desc_() const
  { return "delta - threshold for statistical predominant features"; }
  string get_body_desc_() const
  { return "F-score coefficient based feature selection"; }
 /*!
  * Two main function
  *  read_ - is responsible for reading all parameters needed by algorithm
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */
  Fscore(): Algorithm("fsc") {}
};
REGALGC(Fscore)

void Fscore::read_(parstream &pars,repfile &rep)
{
 pars("predominant feature threshold",rep.cp().log())
     (0.001) >> delta_;
}

void Fscore::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &G=file.auxset(1);
 datset &S=file.finset();

 datspace _F_(F,1);

 vector Fsc_FC = avg(fscor(_F_,C));
 
 rep.log() << "Correlation coefficient Fsc(F,C) = " << sorted << Fsc_FC << endl;

 G.reversion(true);

 subdatset f = argmax(_F_, Fsc_FC);
 while (f!=Nil) {
  F -= f;
  G += f;
  if (completed(G)) break;
  f = argmax(_F_, Fsc_FC);
  if (Fsc_FC(f) <= delta) break;
 }

 if (vnt==vRank) S <<= G;

}

} // end of _FSCORE_ namespace
