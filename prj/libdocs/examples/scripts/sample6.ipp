#
#
# InfoSel++ Control Script
#
# Sample #6
#
#

#
# Deklaracje stalych i dzialania:
#

 output = "sample.rep";
 input = "sample.dat";
 
 precision = 0.001;
 classislast = true;       
 partition = equiwidth(24);  

 const beta = 0.9; 
 exec( b0(beta) );
 exec( b0(beta/2) );
 const b = 0.2; 
 exec( b0( 1.5*(beta/2+b) ) );

