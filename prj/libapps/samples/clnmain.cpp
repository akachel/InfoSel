// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
// //////                                        ////////
// //////                 InfoSel++              ////////
// //////           Command Execution Code       ////////
// //////                                        ////////
// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
///
/// Sample Application With Command Execution
/**
 * @file       clnmain.cpp
 * @version    1.00
 * @author     
 * @since      
 * @date       
 *
 * @contents  
 *             This is a sample application of the InfoSel++ library
 *             to execute any studied algorithm by processing system 
 *             command line.
 */

#include "InfoSelLib.h"
using namespace std;
USEPACK(xyz)

int main(int argc, char *argv[])
{
 using namespace InfoSelLib;

 CommandLine cln(argc,argv,cin,cout);
 CommandLine::procres res=cln.process(cout);

 return res.fail() ? -1 : 0;
}    

