#
#
# InfoSel++ Control Script
#
# Sample #3
#
#

#
# Algorytmy hybrydowe:
#

 output = "sample.rep";
 input = "sample.dat"; 
 
 precision = 0.001;
 classislast = true;       
 partition = equiwidth(24);
 
 exec( ig(10) > ic3(-1) );  # przetwarzanie szeregowe
 exec( k(2, 10) | ic3(-1) );  # przetwarzanie rownolegle
 exec( k(2, 10) | ig(5) > ic3(-1) );  # przetwarzanie mieszane