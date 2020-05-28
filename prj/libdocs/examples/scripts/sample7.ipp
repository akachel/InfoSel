#
#
# InfoSel++ Control Script
#
# Sample #7
#
#

#
# Zlozone skrypty:
#

 output = "sample1.rep";
 input = "sample1.dat";
 
 precision = 0.001;
 classislast = true;       
 partition = equiwidth(24);  

 exec( b0(0.7) );


# nastepny plik:

 output = "sample2.rep";
 input = "sample2.dat";
 
 precision = 0.001;
 classislast = true;       
 partition = equidepth(24);  

 exec( ig );


# zmiana parametrow dla tego samego pliku:
#  jest to rownowazne zastosowaniu wielowartosciowych wyrazen
#

 partition = maxvdiff(24);  

 exec( ig );
