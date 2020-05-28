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
/// Implementation of t-score based feature selection algorithm
/**
 * @file       tscore.cpp
 * @version    1.00
 * @author     Jacek Biesiada, Adam Kachel
 * @since      2004.03.30
 * @date       2009.07.12
 *
 * @contents  
 *             Implementation of t-score based feature selection algorithm
 *
 * @bibliography  
 *             - Yang Su, T.M. Murali, Vladimir Pavlovic, Michael Schaffer and Simon Kasif . 
 *               RankGene: identification of diagnostic genes based on expression data. 
 *               2003. Bioinformatics, 19(12):1578-1579
 */

#include "InfoSelAPI.h"
REGPACK(tscore)

namespace _TSCORE_ /** \copybrief tscore.cpp \copydetails tscore.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    Tscore
 *   \brief    Ranking algorithm with criterial funcion based on t-score statistics
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */
 
class Tscore : public Algorithm {
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
 /*! Member function which gives the name cirterial function, search strategy, and default parameters */
  string get_pars_decl_() const
  { return "real delta=0.001"; }
  string get_pars_desc_() const
  { return "delta - threshold for statistical predominant features"; }
  string get_body_desc_() const
  { return "T-score coefficient based feature selection"; }
 /*!
  * Two main function
  *  read_ - is responsible for reading all parameters needed by algorithm
  *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
  */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */ 
  Tscore(): Algorithm("tsc") {}
};
REGALGC(Tscore)

void Tscore::read_(parstream &pars,repfile &rep)
{
 pars("predominant feature threshold",rep.cp().log())
     (0.001) >> delta_;
}

void Tscore::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &G=file.auxset(1);
 datset &S=file.finset();

 datspace _F_(F,1);

 vector Tsc_FC = avg(tscor(_F_,C));
 
 rep.log() << "Correlation coefficient Tsc(F,C) = " << sorted << Tsc_FC << endl;

 G.reversion(true);

 subdatset f = argmax(_F_, Tsc_FC);
 while (f!=Nil) {
  F -= f;
  G += f;
  if (completed(G)) break;
  f = argmax(_F_, Tsc_FC);
  if (Tsc_FC(f) <= delta) break;
 }

 if (vnt==vRank) S <<= G;
}

} // end of _TSCORE_ namespace
