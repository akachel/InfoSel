 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////  Algorithm Programming Interface Test  ////////
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
/// API Test Code
/**
 * @file       apitst.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2003.07.27
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains core code for testing the library 
 *             algorithm programming interface.
 */

#include "InfoSelAPI.h"
REGPACK(apitst)

namespace _APITST_ /** \copybrief apitst.cpp \copydetails apitst.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

class Test1 : public Algorithm {
 private: 
  void run_(datfile &file,repfile &rep);
 public:
  Test1(): Algorithm("t1") {}
};
REGALGC(Test1)

void Test1::run_(datfile &file,repfile &rep)
{
 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();
 
 {
  rep.cp().sect("Tests of operations on tables and sparses"); 

  datspace _F_(F,1);
  sparse Pspr = distrib(_F_);
  table Ptab = prob(_F_);
  sparse Qspr(Pspr,true); Qspr*=0.5;
  table Qtab = Qspr;
  
  rep.log() << endl << "testing arguments:" << endl;
  rep.log() << "_F_ := " << _F_ << endl;
  rep.log() << "Pspr := distrib(_F_) = " << Pspr << endl;
  rep.log() << "Ptab := prob(_F_) = " << Ptab << endl;
  rep.log() << "Qspr := 0.5*Pspr = " << Qspr << endl;
  rep.log() << "Qtab := Qspr = " << Qtab << endl;

  {
   rep.cp().log() << endl << "tested expression: Ptab==Pspr -->" << endl; 
   bool res = Ptab==Pspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Qtab==Qspr -->" << endl; 
   bool res = Qtab==Qspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: abs(Ptab)==abs(Pspr) -->" << endl; 
   table ytab=abs(Ptab);
   sparse yspr=abs(Pspr);
   rep.log() << "ytab := abs(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := abs(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: log(Ptab)==log(Pspr) -->" << endl; 
   table ytab=log(Ptab);
   sparse yspr=log(Pspr);
   rep.log() << "ytab := log(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := log(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: log2(Ptab)==log2(Pspr) -->" << endl; 
   table ytab=log2(Ptab);
   sparse yspr=log2(Pspr);
   rep.log() << "ytab := log2(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := log2(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: sqr(Ptab)==sqr(Pspr) -->" << endl; 
   table ytab=sqr(Ptab);
   sparse yspr=sqr(Pspr);
   rep.log() << "ytab := sqr(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := sqr(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: sqrt(Ptab)==sqrt(Pspr) -->" << endl; 
   table ytab=sqrt(Ptab);
   sparse yspr=sqrt(Pspr);
   rep.log() << "ytab := sqrt(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := sqrt(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: cub(Ptab)==cub(Pspr) -->" << endl; 
   table ytab=cub(Ptab);
   sparse yspr=cub(Pspr);
   rep.log() << "ytab := cub(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := cub(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: pow(Ptab,2.5)==pow(Pspr,2.5) -->" << endl; 
   table ytab=pow(Ptab,2.5);
   sparse yspr=pow(Pspr,2.5);
   rep.log() << "ytab := pow(Ptab,2.5) = " << ytab << endl; 
   rep.log() << "yspr := pow(Pspr,2.5) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: cum(Ptab)==cum(Pspr) -->" << endl; 
   table ytab=cum(Ptab);
   sparse yspr=cum(Pspr);
   rep.log() << "ytab := cum(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := cum(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: sum(Ptab)==sum(Pspr) -->" << endl; 
   table ytab=sum(Ptab);
   sparse yspr=sum(Pspr);
   rep.log() << "ytab := sum(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := sum(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: avg(Ptab)==avg(Pspr) -->" << endl; 
   table ytab=avg(Ptab);
   sparse yspr=avg(Pspr);
   rep.log() << "ytab := avg(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := avg(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: prod(Ptab)==prod(Pspr) -->" << endl; 
   table ytab=prod(Ptab);
   sparse yspr=prod(Pspr);
   rep.log() << "ytab := prod(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := prod(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: max(Ptab)==max(Pspr) -->" << endl; 
   table ytab=max(Ptab);
   sparse yspr=max(Pspr);
   rep.log() << "ytab := max(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := max(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: min(Ptab)==min(Pspr) -->" << endl; 
   table ytab=min(Ptab);
   sparse yspr=min(Pspr);
   rep.log() << "ytab := min(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := min(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: max2(Ptab,Qtab)==max2(Pspr,Qspr) -->" << endl; 
   table ytab=max2(Ptab,Qtab);
   sparse yspr=max2(Pspr,Qspr);
   rep.log() << "ytab := max2(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := max2(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: min2(Ptab,Qtab)==min2(Pspr,Qspr) -->" << endl; 
   table ytab=min2(Ptab,Qtab);
   sparse yspr=min2(Pspr,Qspr);
   rep.log() << "ytab := min2(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := min2(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diag(Ptab)==diag(Pspr) -->" << endl; 
   table ytab=diag(Ptab);
   sparse yspr=diag(Pspr);
   rep.log() << "ytab := diag(Ptab) = " << ytab << endl; 
   rep.log() << "yspr := diag(Pspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagadd(Ptab,Qtab)==diagadd(Pspr,Qspr) -->" << endl; 
   table ytab=diagadd(Ptab,Qtab);
   sparse yspr=diagadd(Pspr,Qspr);
   rep.log() << "ytab := diagadd(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := diagadd(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagsub(Ptab,Qtab)==diagsub(Pspr,Qspr) -->" << endl; 
   table ytab=diagsub(Ptab,Qtab);
   sparse yspr=diagsub(Pspr,Qspr);
   rep.log() << "ytab := diagsub(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := diagsub(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagmul(Ptab,Qtab)==diagmul(Pspr,Qspr) -->" << endl; 
   table ytab=diagmul(Ptab,Qtab);
   sparse yspr=diagmul(Pspr,Qspr);
   rep.log() << "ytab := diagmul(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := diagmul(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagdiv(Ptab,Qtab)==diagdiv(Pspr,Qspr) -->" << endl; 
   table ytab=diagdiv(Ptab,Qtab);
   sparse yspr=diagdiv(Pspr,Qspr);
   rep.log() << "ytab := diagdiv(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := diagdiv(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: dyadadd(Ptab,Qtab)==dyadadd(Pspr,Qspr) -->" << endl; 
   table ytab=dyadadd(Ptab,Qtab);
   sparse yspr=dyadadd(Pspr,Qspr);
   rep.log() << "ytab := dyadadd(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := dyadadd(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: dyadsub(Ptab,Qtab)==dyadsub(Pspr,Qspr) -->" << endl; 
   table ytab=dyadsub(Ptab,Qtab);
   sparse yspr=dyadsub(Pspr,Qspr);
   rep.log() << "ytab := dyadsub(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := dyadsub(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: dyadmul(Ptab,Qtab)==dyadmul(Pspr,Qspr) -->" << endl; 
   table ytab=dyadmul(Ptab,Qtab);
   sparse yspr=dyadmul(Pspr,Qspr);
   rep.log() << "ytab := dyadmul(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := dyadmul(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: dyaddiv(Ptab,Qtab)==dyaddiv(Pspr,Qspr) -->" << endl; 
   table ytab=dyaddiv(Ptab,Qtab);
   sparse yspr=dyaddiv(Pspr,Qspr);
   rep.log() << "ytab := dyaddiv(Ptab,Qtab) = " << ytab << endl; 
   rep.log() << "yspr := dyaddiv(Pspr,Qspr) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagadd(Ptab,Qtab)==diag( dyadadd(Ptab,Qtab)*Per(4)(0,2,1,3) ) -->" << endl; 
   table y1=diagadd(Ptab,Qtab);
   table y2=diag( dyadadd(Ptab,Qtab)*Per(4)(0,2,1,3) );
   rep.log() << "y1 := diagadd(Ptab,Qtab) = " << y1 << endl; 
   rep.log() << "y2 := diag( dyadadd(Ptab,Qtab)*Per(4)(0,2,1,3) ) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagadd(Ptab,Qtab)==diag( dyadadd(Ptab,Qtab)*Per(4,2)(2,1) ) -->" << endl; 
   table y1=diagadd(Ptab,Qtab);
   table y2=diag( dyadadd(Ptab,Qtab)*Per(4,2)(2,1) );
   rep.log() << "y1 := diagadd(Ptab,Qtab) = " << y1 << endl; 
   rep.log() << "y2 := diag( dyadadd(Ptab,Qtab)*Per(4,2)(2,1) ) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagadd(Pspr,Qspr)==diag( dyadadd(Pspr,Qspr)*Per(4)(0,2,1,3) ) -->" << endl; 
   table y1=diagadd(Pspr,Qspr);
   table y2=diag( dyadadd(Pspr,Qspr)*Per(4)(0,2,1,3) );
   rep.log() << "y1 := diagadd(Pspr,Qspr) = " << y1 << endl; 
   rep.log() << "y2 := diag( dyadadd(Pspr,Qspr)*Per(4)(0,2,1,3) ) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: diagadd(Pspr,Qspr)==diag( dyadadd(Pspr,Qspr)*Per(4,2)(2,1) ) -->" << endl; 
   table y1=diagadd(Pspr,Qspr);
   table y2=diag( dyadadd(Pspr,Qspr)*Per(4,2)(2,1) );
   rep.log() << "y1 := diagadd(Pspr,Qspr) = " << y1 << endl; 
   rep.log() << "y2 := diag( dyadadd(Pspr,Qspr)*Per(4,2)(2,1) ) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: (Ptab*Per(2)(1,0))==(Pspr*Per(2)(1,0)) -->" << endl; 
   table ytab=Ptab*Per(2)(1,0);
   sparse yspr=Pspr*Per(2)(1,0);
   rep.log() << "ytab := Ptab*Per(2)(1,0) = " << ytab << endl; 
   rep.log() << "yspr := Pspr*Per(2)(1,0) = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: (~Ptab)==(~Pspr) -->" << endl; 
   table ytab=~Ptab;
   sparse yspr=~Pspr;
   rep.log() << "ytab := ~Ptab = " << ytab << endl; 
   rep.log() << "yspr := ~Pspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab+Qtab==Pspr+Qspr -->" << endl; 
   table ytab=Ptab+Qtab;
   sparse yspr=Pspr+Qspr;
   rep.log() << "ytab := Ptab+Qtab = " << ytab << endl; 
   rep.log() << "yspr := Pspr+Qspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: 1.5+Ptab==1.5+Pspr -->" << endl; 
   table ytab=1.5+Ptab;
   sparse yspr=1.5+Pspr;
   rep.log() << "ytab := 1.5+Ptab = " << ytab << endl; 
   rep.log() << "yspr := 1.5+Pspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab+1.5==Pspr+1.5 -->" << endl; 
   table ytab=Ptab+1.5;
   sparse yspr=Pspr+1.5;
   rep.log() << "ytab := Ptab+1.5 = " << ytab << endl; 
   rep.log() << "yspr := Pspr+1.5 = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: (-Ptab)==(-Pspr) -->" << endl; 
   table ytab=-Ptab;
   sparse yspr=-Pspr;
   rep.log() << "ytab := -Ptab = " << ytab << endl; 
   rep.log() << "yspr := -Pspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab-Qtab==Pspr-Qspr -->" << endl; 
   table ytab=Ptab-Qtab;
   sparse yspr=Pspr-Qspr;
   rep.log() << "ytab := Ptab-Qtab = " << ytab << endl; 
   rep.log() << "yspr := Pspr-Qspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: 1.5-Ptab==1.5-Pspr -->" << endl; 
   table ytab=1.5-Ptab;
   sparse yspr=1.5-Pspr;
   rep.log() << "ytab := 1.5-Ptab = " << ytab << endl; 
   rep.log() << "yspr := 1.5-Pspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab-1.5==Pspr-1.5 -->" << endl; 
   table ytab=Ptab-1.5;
   sparse yspr=Pspr-1.5;
   rep.log() << "ytab := Ptab-1.5 = " << ytab << endl; 
   rep.log() << "yspr := Pspr-1.5 = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab*Qtab==Pspr*Qspr -->" << endl; 
   table ytab=Ptab*Qtab;
   sparse yspr=Pspr*Qspr;
   rep.log() << "ytab := Ptab*Qtab = " << ytab << endl; 
   rep.log() << "yspr := Pspr*Qspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: 1.5*Ptab==1.5*Pspr -->" << endl; 
   table ytab=1.5*Ptab;
   sparse yspr=1.5*Pspr;
   rep.log() << "ytab := 1.5*Ptab = " << ytab << endl; 
   rep.log() << "yspr := 1.5*Pspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab*1.5==Pspr*1.5 -->" << endl; 
   table ytab=Ptab*1.5;
   sparse yspr=Pspr*1.5;
   rep.log() << "ytab := Ptab*1.5 = " << ytab << endl; 
   rep.log() << "yspr := Pspr*1.5 = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab/Qtab==Pspr/Qspr -->" << endl; 
   table ytab=Ptab/Qtab;
   sparse yspr=Pspr/Qspr;
   rep.log() << "ytab := Ptab/Qtab = " << ytab << endl; 
   rep.log() << "yspr := Pspr/Qspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: 1.5/Ptab==1.5/Pspr -->" << endl; 
   table ytab=1.5/Ptab;
   sparse yspr=1.5/Pspr;
   rep.log() << "ytab := 1.5/Ptab = " << ytab << endl; 
   rep.log() << "yspr := 1.5/Pspr = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Ptab/1.5==Pspr/1.5 -->" << endl; 
   table ytab=Ptab/1.5;
   sparse yspr=Pspr/1.5;
   rep.log() << "ytab := Ptab/1.5 = " << ytab << endl; 
   rep.log() << "yspr := Pspr/1.5 = " << yspr << endl; 
   bool res = ytab==yspr;
   rep.cp().log() << "test result: " << res << endl; 
  }

  rep.cp().log() << endl;
 }
 
 { 
  rep.cp().sect("Tests of operations on tables and tensors"); 

  table Ptab(5,5); Ptab.rand();
  table Qtab(5,5); Qtab.rand();
  tensor Ptns = Ptab;
  tensor Qtns = Qtab;
  matrix Amtr = Ptns;
  matrix Bmtr = Qtns;
  matrix Imtr(5); Imtr.idnt();
  
  rep.log() << endl << "testing arguments:" << endl;
  rep.log() << "Ptab := table(5,5).rand() = " << Ptab << endl;
  rep.log() << "Qtab := table(5,5).rand() = " << Qtab << endl;
  rep.log() << "Ptns := Ptab = " << Ptns << endl;
  rep.log() << "Qtns := Qtab = " << Qtns << endl;
  rep.log() << "Amtr := Ptns = " << Amtr << endl;
  rep.log() << "Bmtr := Qtns = " << Bmtr << endl;
  rep.log() << "Imtr := matrix(5).idnt() = " << Imtr << endl;

  {
   rep.cp().log() << endl << "tested expression: Ptns==Ptab -->" << endl; 
   bool res = Ptns==Ptab;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Qtns==Qtab -->" << endl; 
   bool res = Qtns==Qtab;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Amtr==Ptns -->" << endl; 
   bool res = Amtr==Ptns;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Bmtr==Qtns -->" << endl; 
   bool res = Bmtr==Qtns;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: con(Ptns)==sum(diag(Ptab)) -->" << endl; 
   tensor ytns=con(Ptns);
   table ytab=sum(diag(Ptab));
   rep.log() << "ytns := con(Ptns) = " << ytns << endl; 
   rep.log() << "ytab := sum(diag(Ptab)) = " << ytab << endl; 
   bool res = ytns==ytab;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: con(Ptns,Qtns)==sum(diagmul(Ptab,~Qtab)) -->" << endl; 
   tensor ytns=con(Ptns,Qtns);
   table ytab=sum(diagmul(Ptab,~Qtab));
   rep.log() << "ytns := con(Ptns,Qtns) = " << ytns << endl; 
   rep.log() << "ytab := sum(diagmul(Ptab,~Qtab)) = " << ytab << endl; 
   bool res = ytns==ytab;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: det(Amtr*Bmtr)==det(Amtr)*det(Bmtr) -->" << endl; 
   real y1=det(Amtr*Bmtr);
   real y2=det(Amtr)*det(Bmtr);
   rep.log() << "y1 := det(Amtr*Bmtr) = " << y1 << endl; 
   rep.log() << "y2 := det(Amtr)*det(Bmtr) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: Amtr*inv(Amtr)==Imtr -->" << endl; 
   matrix y1=Amtr*inv(Amtr);
   matrix y2=Imtr;
   rep.log() << "y1 := Amtr*inv(Amtr) = " << y1 << endl; 
   rep.log() << "y2 := Imtr = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: det(inv(Bmtr))==1.0/det(Bmtr) -->" << endl; 
   real y1=det(inv(Bmtr));
   real y2=1.0/det(Bmtr);
   rep.log() << "y1 := det(inv(Bmtr)) = " << y1 << endl; 
   rep.log() << "y2 := 1.0/det(Bmtr) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }

  rep.cp().log() << endl;
 } 

 { 
  rep.cp().sect("Tests of operations on tables and fields"); 

  datspace _F_(F,1);
  table Ptab = prob(_F_);
  table Qtab = 0.5*Ptab;
  field Pfld = gauss(_F_);
  field Qfld = 0.5*Pfld;
  
  rep.log() << endl << "testing arguments:" << endl;
  rep.log() << "_F_ := " << _F_ << endl;
  rep.log() << "Ptab := prob(_F_) = " << Ptab << endl;
  rep.log() << "Qtab := 0.5*Ptab = " << Qtab << endl;
  rep.log() << "Pfld := gauss(_F_) = " << Pfld << endl;
  rep.log() << "Qfld := 0.5*Pfld = " << Qfld << endl;

  {
   rep.cp().log() << endl << "tested expression: integ()(sum(Pfld))==sum(sum(Ptab)) -->" << endl; 
   table y1=integ()(sum(Pfld))();
   table y2=sum(sum(Ptab));
   rep.log() << "y1 := integ()(sum(Pfld)) = " << y1 << endl; 
   rep.log() << "y2 := sum(sum(Ptab)) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }
  {
   rep.cp().log() << endl << "tested expression: sum(integ()(Pfld))==integ()(sum(Pfld)) -->" << endl; 
   table y1=sum(integ()(Pfld))();
   table y2=integ()(sum(Pfld))();
   rep.log() << "y1 := sum(integ()(Pfld)) = " << y1 << endl; 
   rep.log() << "y2 := integ()(sum(Pfld)) = " << y2 << endl; 
   bool res = y1==y2;
   rep.cp().log() << "test result: " << res << endl; 
  }

  rep.cut().log() << endl;
 } 
}

} // end of _APITST_ namespace

