 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////     Algorithm Programming Interface    ////////
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
/// Definitions For Math Tensor
/**
 * @file       tensor.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.05.29
 * @date       2009.11.22
 *
 * @contents  
 *             This file contains definitions of algorithm tool related elements.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dspac.h"
#include "dsubs.h"
#include "dfile.h"
#include "albas.h"
#include "altls.h"
#include "sparse.h"
#include "table.h"
#include "tensor.h"

namespace InfoSelAPI {
             
namespace Algorithms {

namespace Tools {

/////////////////////////////
////
//// tensor class definition
////

int *tensor::sizes1_;
int *tensor::sizes2_;

int *tensor::new_sizes_(int *&sizes,int ndims,int size)
{
 sizes = ndims>0 ? new int[ndims] : 0; 
 for (int k=0; k<ndims; k++) sizes[k]=size;
 return sizes;
}

const table &tensor::chk_sizes_(const table &tab)
{
 int s=0;
 for (int k=0; k<tab.ndims(); k++)
 if (s<=0) s=tab.size(k);
 else if (tab.size(k)!=s) throw xTensorEqSizReq();
 return tab;
}

tensor con(const tensor &x1,const tensor &x2)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=con(x1,x2):" << endl;
 cdeb << "x1 = " << x1 << endl;
 cdeb << "x2 = " << x2 << endl;
#endif
 int s1 = x1.rep_.ndims()>0 ? x1.rep_.size() : 1;
 int s2 = x2.rep_.ndims()>0 ? x2.rep_.size() : 1;
 if (s1!=s2) throw xPrecondFail();
 int so=s1;
 tensor y(x1.rep_.ndims()-1,x2.rep_.ndims()-1,so);
 int bo=1;
 for (int k=1; k<x2.rep_.ndims(); k++) bo*=so;
 int n1=so;
 int n2=bo;
 int i=0;
 for (int j1=0; j1<n1; j1++)
 for (int j2=0; j2<n2; j2++) {
  real uo=0.0;
  for (int io=0; io<so; io++) uo+=x1.rep_[j1*bo+io]*x2.rep_[io*bo+j2];
  y.rep_[i++]=uo;
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return tensor(y);
}

/////////////////////////////
////
//// tensor0R class definition
////

const table &tensor0R::chk_ndims_(const table &tab)
{
 if (tab.ndims()!=0) throw xTensor0RankReq();
 return tab;
}

/////////////////////////////
////
//// tensor1R class definition
////

const table &tensor1R::chk_ndims_(const table &tab)
{
 if (tab.ndims()!=1) throw xTensor1RankReq();
 return tab;
}

real norm(const tensor1R &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=norm(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.dimn();
 real y=0.0;
 for (int i=0; i<n; i++) {
  real xi=x(i);
  y+=xi*xi;
 } 
 y=std::sqrt(y);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return y;
}

Data::subset argmax(const Data::space &_F_,const tensor1R &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "f=argmax(_F_,x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 if (x.dimn()!=_F_.card()) throw xPrecondFail();
 const real eps=1e5*rPrec; // eps<1.0
 real uo=-rInf;
 Data::subset f(_F_,true);
 for (Data::subset g=f; !g.isnil(); ++g) {
  real xg=x(g);
  if (xg>uo+eps) {
   uo=xg;
   f=g;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "f = " << f << endl;
#endif
 return Data::subset(f);
}

Data::subset argmin(const Data::space &_F_,const tensor1R &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "f=argmin(_F_,x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 if (x.dimn()!=_F_.card()) throw xPrecondFail();
 const real eps=1e5*rPrec; // eps<1.0
 real uo=+rInf;
 Data::subset f(_F_,true);
 for (Data::subset g=f; !g.isnil(); ++g) {
  real xg=x(g);
  if (xg<uo-eps) {
   uo=xg;
   f=g;
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "f = " << f << endl;
#endif
 return Data::subset(f);
}

/////////////////////////////
////
//// tensor2R class definition
////

const table &tensor2R::chk_ndims_(const table &tab)
{
 if (tab.ndims()!=2) throw xTensor2RankReq();
 return tab;
}

tensor2R &tensor2R::idnt()
{
 rep_.clone();
 const tensor2R &x=*this;
#ifdef SOURCE_DEBUG
 cdbg << "y=idnt(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.dimn();
 tensor2R &y=*this;
 for (int i=0; i<n; i++)
 for (int j=0; j<n; j++)
 if (i==j) y(i,i)=1.0;
 else y(i,j)=0.0;
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return y;
}

real det(const tensor2R &x)
{
 static int cnt=0;
 static bool *xrow, *xcol;
#ifdef SOURCE_DEBUG
 if (cnt==0) {
  cdbg << "y=det(x):" << endl;
  cdeb << "x = " << x << endl;
 }
#endif
 int n=x.dimn(); // n>0
 if (++cnt==1) {
  xrow=new bool[n];
  memset(xrow,0,n*sizeof(bool));
  xcol=new bool[n];
  memset(xcol,0,n*sizeof(bool));
 } 
 real d=0.0;
 int k=0;
 for (int i=0; i<n; i++) 
 if (!xrow[i]) {
  k++;
  xrow[i]=true;
  int l=0;
  for (int j=0; j<n; j++) 
  if (!xcol[j]) {
   l++;
   xcol[j]=true;
   d+=x(i,j)*( (k+l)%2==0 ? +1.0 : -1.0 )*det(x);
   xcol[j]=false;
  }
  xrow[i]=false;
  break;
 }
 if (--cnt==0) delete[] xrow, delete[] xcol;
 real y = k>0 ? d : 1.0;
#ifdef SOURCE_DEBUG
 if (cnt==0)
 cdeb << "y = " << y << endl;
#endif
 return y;
}

real tr(const tensor2R &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=tr(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 int n=x.dimn();
 real y=0.0;
 for (int i=0; i<n; i++)
 y+=x(i,i);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return y;
}

tensor2R inv(const tensor2R &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=inv(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 const real eps=1e5*rPrec; // eps<1.0
 int n=x.dimn();
 int in=n-1;
 tensor2R X(x,true);
 tensor2R Y(x,false), &y=Y;
 for (int i=0; i<n; i++) 
 Y(i,i)=1.0;
 for (int ia=0; ia<in; ia++) {
  real xmax=0.0;
  int io=ia;
  for (int ib=ia; ib<n; ib++){
   real xval=Common::abs(X(ib,ia));
   if (xmax<xval){
    xmax=xval;
    io=ib;
   }
  }
  if (xmax<eps) throw xOperFail();
  if (io!=ia) {
   for (int ib=ia; ib<n; ib++) {
    real &Xiaib=X(ia,ib);
    real &Xioib=X(io,ib);
    real xval=Xiaib;
    Xiaib=Xioib;
    Xioib=xval;
   }
   for (int ib=0; ib<n; ib++) {
    real &Yiaib=Y(ia,ib);
    real &Yioib=Y(io,ib);
    real yval=Yiaib;
    Yiaib=Yioib;
    Yioib=yval;
   }
  }
  real &Xiaia=X(ia,ia);
  for (int ib=ia+1; ib<n; ib++) {
   real &Xibia=X(ib,ia);
   for (int ic=ia+1; ic<n; ic++)
   X(ib,ic)-=X(ia,ic)*Xibia/Xiaia;
   for (int ic=0; ic<n; ic++)
   Y(ib,ic)-=Y(ia,ic)*Xibia/Xiaia;
  }
 }
 real &Xinin=X(in,in);
 for (int ib=0; ib<n; ib++) {
  Y(in,ib)/=Xinin;
  for (int ia=in-1; ia>=0; ia--) {
   real &Yiaib=Y(ia,ib);
   for (int ic=ia+1; ic<n; ic++)
   Yiaib-=X(ia,ic)*Y(ic,ib);
   Yiaib/=X(ia,ia);
  }
 }
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return tensor2R(y);
}

tensor2R npow(const tensor2R &x,int n)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=npow(x,n):" << endl;
 cdeb << "x = " << x << endl;
 cdeb << "n = " << n << endl;
#endif
 tensor2R y(x.dimn());
 if (n==0) y.idnt();
 else
 if (n==1) y=tensor2R(x,true);
 else {
  y.idnt();
  int m=Common::abs(n);
  for (int i=0; i<m; i++)
  y=x*y;
  if (n<0) y=inv(y);
 } 
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return tensor2R(y);
}

tensor2R operator~(const tensor2R &x)
{
#ifdef SOURCE_DEBUG
 cdbg << "y=op~(x):" << endl;
 cdeb << "x = " << x << endl;
#endif
 tensor2R y(x,false);
 int n=x.dimn();
 for (int i=0; i<n; i++)
 for (int j=0; j<n; j++)
 y(i,j)=x(j,i);
#ifdef SOURCE_DEBUG
 cdeb << "y = " << y << endl;
#endif
 return tensor2R(y);
}

} // end of Tools namespace

} // end of Algorithms namespace

} // end of InfoSelAPI namespace

