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
/// Feature selection algorithms based on different statistical measures
/**
 * @file       measure.cpp
 * @version    1.00
 * @author     Jacek Biesiada and Adam Kachel
 * @since      2003.01.15
 * @date       2009.03.20
 *
 * @contents  
 *             Feature selection algorithms based on statistical measures, such as:
 *             Kolmogorov, Kullback, Bhattacharatyya, Matusita, and Sammona measures (distances)
 *             with ranking search strategy
 *
 * @bibliography  
 *             - T.R. Vilmansen, 
 *               "Feature Evalution with Measures of Probabilistic Dependence", 
 *               IEEE Transaction on Computers, 22(4), pp. 381- 388, April 1973.
 *             - W. Duch, 
 *               Filter Methods, In: Feature extraction, foundations and applications.
 *               Eds: I. Guyon, S. Gunn, M. Nikravesh, L. Zadeh, Studies in Fuzziness and Soft Computing,
 *               Physica-Verlag, Springer,pp. 89-118, 2006.
 */

#include "InfoSelAPI.h"
REGPACK(measure)

namespace _MEASURE_ /** \copybrief measure.cpp \copydetails measure.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    KD_Rank
 *   \brief    Ranking algorithm with criterion function based on Kolmogorov distance
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class KD_Rank : public Algorithm {
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
  string get_body_desc_() const
  { return "Ranking based on the Kolmogorow measure"; }
  string get_pars_decl_() const
  { return "real delta=0.001"; }
/*!
 * Two main function:
 *  read_ - is responsible for reading all parameters needed by algorithm
 *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
 */
  void run_(datfile &file,repfile &rep);
  void read_(parstream &pars,repfile &rep);
 public:
 /*! Constructor which give the shorter name of algorithm and search strategies */  
  KD_Rank(): Algorithm("kdr") {}
};
REGALGC(KD_Rank)

void KD_Rank::read_(parstream &pars,repfile &rep)
{
  pars("predominant feature threshold",rep.cp().log())
  (0.001) >> delta_;
}

void KD_Rank::run_(datfile &file,repfile &rep)
{

 const int vnt=variant(); 
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);

 vector KD_FC = sum(sum( abs(P_FC-(P_F^P_C)) ));
 rep.log() << "Kolmogorow Distances coefficient KD(F,C) = " << sorted << KD_FC <<endl;

 S.reversion(true);

if (vnt==vRank){
 subdatset f = argmax(_F_, KD_FC );
 while (!f.isnil()) {
  F-=f;
  S+=f;
  if (completed(file)) break;
  f = argmax(_F_, KD_FC );
  if (KD_FC(f) <= delta ) break;
 }
} 

}

/*!
 *   \class    KLD_Rank
 *   \brief    Ranking algorithm with criterion function based on Kullback distance
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class KLD_Rank : public Algorithm {
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
  string get_body_desc_() const
  { return "Ranking based on the Kullback measure"; }
  string get_pars_decl_() const
  { return "real delta=0.001"; }
/*!
 * Two main function:
 *  read_ - is responsible for reading all parameters needed by algorithm
 *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
 */
  void run_(datfile &file,repfile &rep);
  void read_(parstream &pars,repfile &rep);
 public:
/*! Constructor which give the shorter name of algorithm and search strategies */  
  KLD_Rank(): Algorithm("klr") {}
};
REGALGC(KLD_Rank)

void KLD_Rank::read_(parstream &pars,repfile &rep)
{
  pars("predominant feature threshold",rep.cp().log())
  (0.001) >> delta_;
}

void KLD_Rank::run_(datfile &file,repfile &rep)
{
 
 const int vnt=variant(); 
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);

 vector KLD_FC = sum(sum( (P_FC-(P_F^P_C))*log2(P_FC/(P_F^P_C)) ));
 rep.log() << "Kullback Distance coefficient KLD(F,C) = " << sorted << KLD_FC <<endl;

 S.reversion(true);

if (vnt==vRank){
  subdatset f = argmax(_F_, KLD_FC );
  while (!f.isnil()) {
   F-=f;
   S+=f;
   if (completed(file)) break;
   f = argmax(_F_, KLD_FC );
   if (KLD_FC(f) <= delta ) break;
  }
 }  
}

/*!
 *   \class    BD_Rank
 *   \brief    Ranking algorithm with criterion function based on Bhattacharatyya distance
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class BD_Rank : public Algorithm {
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
  string get_body_desc_() const
  { return "Ranking based on the Bhattacharatyya measure"; }
  string get_pars_decl_() const
  { return "real delta=0.001"; }
/*!
 * Two main function:
 *  read_ - is responsible for reading all parameters needed by algorithm
 *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
 */
  void run_(datfile &file,repfile &rep);
  void read_(parstream &pars,repfile &rep);
 public:
/*! Constructor which give the shorter name of algorithm and search strategies */  
  BD_Rank(): Algorithm("bdr") {}
};
REGALGC(BD_Rank)

void BD_Rank::read_(parstream &pars,repfile &rep)
{
  pars("predominant feature threshold",rep.cp().log())
  (0.001) >> delta_;
}

void BD_Rank::run_(datfile &file,repfile &rep)
{
 const int vnt=variant(); 
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);

 vector BD_FC = -log2( sum(sum( sqrt(P_FC*(P_F^P_C)) )) );
 rep.log() << "Bhattacharatyya Distance coefficient U(F,C) = " << sorted << BD_FC <<endl;

 S.reversion(true);

 if (vnt==vRank){ 
  subdatset f = argmax(_F_, BD_FC );
  while (!f.isnil()) {
   F-=f;
   S+=f;
   if (completed(file)) break;
   f = argmax(_F_, BD_FC );
  }
 } 
}

/*!
 *   \class    MD_Rank
 *   \brief    Ranking algorithm with criterion function based on Matusita distance
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class MD_Rank : public Algorithm {
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
  string get_body_desc_() const
  { return "Ranking based on the Matusita measure"; }
  string get_pars_decl_() const
  { return "real delta=0.001"; }
/*!
 * Two main function:
 *  read_ - is responsible for reading all parameters needed by algorithm
 *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
 */
  void run_(datfile &file,repfile &rep);
  void read_(parstream &pars,repfile &rep);
 public:
/*! Constructor which give the shorter name of algorithm and search strategies */  
  MD_Rank(): Algorithm("mdr") {}
};
REGALGC(MD_Rank)

void MD_Rank::read_(parstream &pars,repfile &rep)
{
  pars("predominant feature threshold",rep.cp().log())
  (0.001) >> delta_;
}

void MD_Rank::run_(datfile &file,repfile &rep)
{
 const int vnt=variant(); 
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);

 vector MD_FC = sqrt( sum(sum( sqr(sqrt(P_FC)-sqrt(P_F^P_C)) )) );
 rep.log() << "Matusita Distance coefficient MD(F,C) = " << sorted << MD_FC <<endl;

 S.reversion(true);

 if (vnt==vRank){
  subdatset f = argmax(_F_, MD_FC );
  while (!f.isnil()) {
   F-=f;
   S+=f;
   if (completed(file)) break;
   f = argmax(_F_, MD_FC );
  }
 } 
}

/*!
 *   \class    SD_Rank
 *   \brief    Ranking algorithm with criterion function based on Sammon distance
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class SD_Rank : public Algorithm {
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
  string get_body_desc_() const
  { return "Ranking based on the Sammona measure"; }
  string get_pars_decl_() const
  { return "real delta=0.001"; }
/*!
 * Two main function:
 *  read_ - is responsible for reading all parameters needed by algorithm
 *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
 */
  void run_(datfile &file,repfile &rep);
  void read_(parstream &pars,repfile &rep);
 public:
/*! Constructor which give the shorter name of algorithm and search strategies */  
  SD_Rank(): Algorithm("sdr") {}
};
REGALGC(SD_Rank)

void SD_Rank::read_(parstream &pars,repfile &rep)
{
  pars("predominant feature threshold",rep.cp().log())
  (0.001) >> delta_;
}

void SD_Rank::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real delta=delta_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);

 table P_FC = prob(_F_,C);
 table P_F = prob(_F_);
 table P_C = prob(C);

 vector SD_FC = 1.0 - sum(sum( min2(P_FC,(P_F^P_C)) ));
 rep.log() << "Sammona Distance coefficient SD(F,C) = " << sorted << SD_FC <<endl;

 S.reversion(true);

 if (vnt==vRank){
  subdatset f = argmax(_F_, SD_FC );
  while (!f.isnil()) {
   F-=f;
   S+=f;
   if (completed(file)) break;
   f = argmax(_F_, SD_FC );
  }
 }

}

} // end of _MEASURE_ namespace
