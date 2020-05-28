// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
// //////                                        ////////
// //////                 InfoSel++              ////////
// //////            Shell Execution Code        ////////
// //////                                        ////////
// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
///
/// Sample Application With Shell Execution
/**
 * @file       shlmain.cpp
 * @version    1.00
 * @author     
 * @since      
 * @date       
 *
 * @contents  
 *             This is a sample application of the InfoSel++ library 
*              to execute any studied algorithm by processing internal 
 *             program shell.
 */

#include "InfoSelLib.h"
using namespace std;
USEPACK(xyz) //// declare for using the algorithm package 'xyz' linked with this program

int main()
{
 using namespace InfoSelLib;
 typedef ProgramShell::Algorithm Algorithm;

 ProgramShell shl("Sample for feature selection"); ///< create a program shell object
 shl.setrepfn("abc.rep"); ///< set up name of output report file
 shl.setdatfn("abc.dat"); ///< set up name of input data file
 shl.setcpos_last(); ///< set up true if data-classes column is last inside the data file
 shl.setprec(0.001); ///< set up precision for real numbers
 shl.setexcl("10, 20"); ///< set up labels of data-classes to exclude from the data file
 shl.setmerg("5, 15"); ///< set up labels of data-classes to merge within the data file
 shl.setpartmth("equiwidth","5"); ///< set up partition method and its parameters
 
 shl.exec(Algorithm("x_0")("0.11,0.22,3")); ///< execute the algorithm with given variant '0' and for all given parameters
 shl.exec(Algorithm("y")("1,0.11,0.22,default")); ///< execute the algorithm for parameters preceded by choosen variant '1' and with last paramater as explicit default
 shl.exec(Algorithm("z")("missing,0.11,0.22")); ///< execute the algorithm for parameters preceded by missing (i.e. default) spec. of variant and with last paramater as implicit default
 shl.flush(); ///< perform processing and return its status
 ProgramShell::procres res=shl.status(cout);

 return res.fail() ? -1 : 0;
}    

