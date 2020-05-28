// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
// //////                                        ////////
// //////                 InfoSel++              ////////
// //////            Algorithm Package           ////////
// //////                                        ////////
// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
///
/// Template Of User-Defined Algorithm Package
/**
 * @file       xyz.cpp
 * @version    1.00
 * @author     ...
 * @since      ...
 * @date       ...
 *
 * @contents  
 *             This is a template package for implementing studied 
 *             algorithms based on the InfoSel++ Library API.
 *
 * @bibliography  
 *             - first item 
 *             - second item
 *             ...
 */

#include "InfoSelAPI.h"
REGPACK(xyz) // 'xyz' - the common name of this algorithm package registered for using by the end-user; also this is the global qualification for names and symbols of all algorithms registered inside this package

namespace _XYZ_ /** \copybrief xyz.cpp \copydetails xyz.cpp */ {

using namespace InfoSelAPI;
using namespace InfoSelAPI::Data;
using namespace InfoSelAPI::Algorithms;

class Xxx : public Algorithm { // 'Xxx' - the main name of this algorithm
  real par1_; // any algorithm parameters
  real par2_;
 private: 
  int get_numvariants_() const
  { return 2; } // there are two variants of this algorithm implementation
  string get_variantlabs_(int vnt) const
  { // both name and symbol of this algorithm can be followed by the suffix '_0' or '_1' or '_variant_A' or '_variant_B' 
    // otherwise the variant 0 or 1 can be choosen from an input parameter stream given for this algorithm
   switch (vnt) {
    default: case 0: return "variant A";
    case 1: return "variant B";
   }
  }

  string get_pars_decl_() const
  { return "real par1=1.11, real par2=2.22"; } // prototype declaration of algorithm parameters with default values
  string get_pars_desc_() const
  { return "par1 - the first parameter ; par2 - the second parameter"; } // full description of algorithm parameters
  string get_body_desc_() const
  { return "the implementation of ..."; } // algorithm description

  void read_(parstream &pars,repfile &rep); // main routines of algorithm implementation
  void run_(datfile &file,repfile &rep);
 public:
  Xxx(): Algorithm("x") // 'x' - the shortcut symbol of this algorithm
  {} 
};
REGALGC(Xxx)

void Xxx::read_(parstream &pars,repfile &rep)
{
 pars("parameter #1",rep.cp().log()) // read algorithm parameters giving their short description and default value
     (0.11) >> par1_;
 pars("parameter #2",rep.cp().log())
     (0.22) >> par2_;
}

void Xxx::run_(datfile &file,repfile &rep)
{
 const int vnt=variant(); // value of algorithm variant
 const real par1=par1_; // values of algorithm parameters
 const real par2=par2_;

 const datattr &C=file.classes(); // main data declarations for algorithm code
 datset &F=file.iniset();
 datset &S=file.finset();
 //
 // TODO: Add any algorithm code here and report its results
 //
 rep.log() << "Warning: This is the template algorithm with no results !" << endl;
}

class Yyy : public Algorithm {
  real par1_;
  real par2_;
 private: 
  int get_numvariants_() const
  { return 2; }
  string get_variantlabs_(int vnt) const
  {
   switch (vnt) {
    default: case 0: return "variant A";
    case 1: return "variant B";
   }
  }

  string get_pars_decl_() const
  { return "real par1=1.11, real par2=2.22"; }
  string get_pars_desc_() const
  { return "par1 - the first parameter ; par2 - the second parameter"; }
  string get_body_desc_() const
  { return "the implementation of ..."; }

  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  Yyy(): Algorithm("y")
  {} 
};
REGALGC(Yyy)

void Yyy::read_(parstream &pars,repfile &rep)
{
 pars("parameter #1",rep.cp().log())
     (0.11) >> par1_;
 pars("parameter #2",rep.cp().log())
     (0.22) >> par2_;
}

void Yyy::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real par1=par1_;
 const real par2=par2_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();
 //
 // TODO: Add any algorithm code here and report its results
 //
 rep.log() << "Warning: This is the template algorithm with no results !" << endl;
}

class Zzz : public Algorithm {
  real par1_;
  real par2_;
 private: 
  int get_numvariants_() const
  { return 2; }
  string get_variantlabs_(int vnt) const
  {
   switch (vnt) {
    default: case 0: return "variant A";
    case 1: return "variant B";
   }
  }

  string get_pars_decl_() const
  { return "real par1=1.11, real par2=2.22"; }
  string get_pars_desc_() const
  { return "par1 - the first parameter ; par2 - the second parameter"; }
  string get_body_desc_() const
  { return "the implementation of ..."; }

  void read_(parstream &pars,repfile &rep);
  void run_(datfile &file,repfile &rep);
 public:
  Zzz(): Algorithm("z")
  {} 
};
REGALGC(Zzz)

void Zzz::read_(parstream &pars,repfile &rep)
{
 pars("parameter #1",rep.cp().log())
     (0.11) >> par1_;
 pars("parameter #2",rep.cp().log())
     (0.22) >> par2_;
}

void Zzz::run_(datfile &file,repfile &rep)
{
 const int vnt=variant();
 const real par1=par1_;
 const real par2=par2_;

 const datattr &C=file.classes();
 datset &F=file.iniset();
 datset &S=file.finset();
 //
 // TODO: Add any algorithm code here and report its results
 //
 rep.log() << "Warning: This is the template algorithm with no results !" << endl;
}

//
// TODO: Add further algorithm classes
//

} // end of _XYZ_ namespace

