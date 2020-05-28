#
#
# InfoSel++ Control Script
#
# Sample #5
#
#

#
# Zlozone wielowartosciowe wyrazenia:
#

 output = "sample.rep";
 input = "sample.dat";
 
 precision = 0.001;
 classislast = true;       
 partition = { equiwidth( {16,24} ) , equidepth( [16..24:2] ) , maxvdiff(24) };  
  # p/w to zlozony zbior wartosci,
  # jest to rownowazne prostemu zbiorowi:
  # partition = { equiwidth(16) , equiwidth(24) , equidepth(16) , equidepth(18) , equidepth(20) , equidepth(22) , equidepth(24) , maxvdiff(24) };
 
 exec( b0( [0.5..0.9:0.1] ) );

