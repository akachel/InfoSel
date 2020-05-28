// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
// //////                                        ////////
// //////                 InfoSel++              ////////
// //////            Batch Execution Code        ////////
// //////                                        ////////
// //////////////////////////////////////////////////////
// //////////////////////////////////////////////////////
///
/// Sample Application With Batch Execution
/**
 * @file       batmain.cpp
 * @version    1.00
 * @author     
 * @since      
 * @date       
 *
 * @contents  
 *             This is a sample application of the InfoSel++ library
 *             to execute any studied algorithm by processing given 
 *             script batch.
 */

#include "InfoSelLib.h"
using namespace std;
USEPACK(xyz)

int main(int argc, char *argv[])
{
 using namespace InfoSelLib;

 if (argc<2) {
  ScriptBatch bat(cin);
  ScriptBatch::procres res=bat.process(); //!< process(cout) --> displays processing logs
  return res.fail() ? -1 : 0;
 } else {
  string fn = argv[1];
  ScriptBatch bat(fn);
  ScriptBatch::procres res=bat.process(); //!< process(cout) --> displays processing logs
  return res.fail() ? -1 : 0;
 } 
}    

