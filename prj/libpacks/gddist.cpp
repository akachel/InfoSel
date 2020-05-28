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
/// Implementation of feature selection algorithm based on GD-distance.
/**
 * @file       gddist.cpp
 * @version    1.00
 * @author     Jacek Biesiada
 * @since      2002.05.29
 * @date       2009.03.20
 *
 * @contents  
 *             Implementation of feature selection algorithm based on GD-distance
 *
 * @bibliography  
 *             - Lorenzo J., Hernandez M., Mendez J., 
 *               GD: A Measure based on Information Theory for Attribute Selection, 
 *               Proc. International Conf. IBERANIA, LNAI 1484(77) 1998.
 */

#include "InfoSelAPI.h"
REGPACK(gddist)

namespace _GDDIST_ /** \copybrief gddist.cpp \copydetails gddist.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

/*!
 *   \class    GDdist
 *   \brief    Ranking algorithm with criterion function based on GD-distance
 *   \author   Jacek Biesiada & Adam Kachel
 *   \version  1.0.0
 */

class GDdist : public Algorithm {
 private: 
 /*! Member function which gives the name cirterion function, search strategy, and default parameters */  
  string get_body_desc_() const
  { return "Ranking algorithm based on GD-distance"; }

  /*!
   * Main function:
   *  run_  - is responsible for runing algorithm (datafile - represents imput data, repfile - represents report file)
   */
  void run_(datfile &file,repfile &rep);
 public:
  /*! Constructor which give the shorter name of algorithm and search strategies */
  GDdist(): Algorithm("gdd") {}
};
REGALGC(GDdist)

void GDdist::run_(datfile &file,repfile &rep)
{
 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();

 datspace _F_(F,1);

 table P_FC = prob(_F_,C);
 table P_FF = prob(_F_,_F_);
 table P_F = prob(_F_);
 table P_C = prob(C);
 table I_FC = sum(sum( P_FC*log2(P_FC/(P_F^P_C)) ));
 table H_FC = -sum(sum( P_FC*log2(P_FC) ));
 vector D_FC = H_FC-I_FC;
 matrix I_FF = sum(sum( P_FF*log2(P_FF/((P_F^P_F)*Per(4)(0,2,1,3))) ));
 matrix r_I_FF = inv(I_FF);

 rep.log() << "Mataras distances D(F,C) = " << sorted << D_FC << endl;
 rep.log() << "Mutual information I(F,F) = " << I_FF << endl;

 for(;;) {
  real mind=+rInf;
  subdatset f(_F_);
  for (subdatset g=f; !g.isnil(); ++g) {
   F-=g;
   real dGD_FC=0.0;
   for (subdatset f1(_F_); !f1.isnil(); ++f1) {
    real u=0.0;
    for (subdatset f2(_F_); !f2.isnil(); ++f2)
    u += r_I_FF(f1,f2)*D_FC(f2);
    dGD_FC += D_FC(f1)*u;
   }
   F+=g;
   rep.log() << "GD-distance dGD(F-" << g << ",C) = " << dGD_FC << endl;
   if (dGD_FC<mind) {
    mind=dGD_FC;
    f=g;
   }
  }
  if (f.isnil()) break;
  rep.log() << "Minimal GD-distance dGDmin = " << mind << " found for feature f = " << f << endl;
  F-=f;
  S+=f;
  if (completed(file)) break;
 }
}

} // end of _GDDIST_ namespace

