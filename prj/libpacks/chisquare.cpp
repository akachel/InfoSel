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
/// Selection ranking based on chi-square statistics
/**
 * @file       chisquare.cpp
 * @version    1.00
 * @author     Jacek Biesiada and Adam Kacel
 * @since      2002.11.18
 * @date       2009.03.20
 *
 * @contents  
 *             Selection ranking based on chi-square statistics
 *
 * @bibliography  
 *             - Duch W., Wieczorek T., Biesiada J., Blachnik M.,
 *               Comparision of feature ranking methods based on information entropy,
 *               Proc. of International Joint Conference on Neural Networks (IJCNN'04), 2004, IEEE Pres
 */

#include "InfoSelAPI.h"
REGPACK(chisquare)

namespace _CHISQUARE_ /** \copybrief chisquare.cpp \copydetails chisquare.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    Chinfo
 *   \brief    Ranking algorithm based on chi-square statistics 
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class Chinfo : public Algorithm {
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
  /*! Member function which gives the name of criterion function, search strategy, and default parameters */
  string get_pars_decl_() const
  { return "real delta=0.001"; }
  string get_pars_desc_() const
  { return "deltaI - threshold for statistical predominant features"; }
  string get_body_desc_() const
  { return "Chi-square coefficient based feature selection"; }

  /*!
   * Two main functions: 
   *  read_ - is responsible for reading all parameters needed by algorithm
   *  run_  - is responsible for running algorithm (datafile - represents input data, repfile - represents report file)
   */
  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  /*! Constructor which give the shorter name of algorithm and search strategies */
  Chinfo(): Algorithm("chq") {}
};
REGALGC(Chinfo)

void Chinfo::read_(parstream &pars,repfile &rep)
{
 pars("predominant feature threshold",rep.cp().log())
     (0.001) >> delta_;
}

void Chinfo::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &G=file.auxset(1);
 datset &S=file.finset();

 datspace _F_(F,1);
 datspace _S_(S,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);
 
 
 table num_dem = ((sqr(P_FC-(P_F^P_C)))/(P_F^P_C));

 vector CHI_FC = sum(sum( num_dem ));

 rep.log() << "CHI-square statistics CHI(F,C) = " << sorted << CHI_FC << endl;
  
 S.reversion(true);

 
 if (vnt==vRank){ 

   subdatset f = argmax(_F_, CHI_FC );
    while (!f.isnil()) {
    F-=f;
    S+=f;
     if (completed(file)) break; // here is a inconsistenct in case of file
     f = argmax(_F_, CHI_FC);
     if (CHI_FC(f) <= delta ) break;
    }
   S <<= G;
 }
}

} // end of _CHISQUARE_ namespace

