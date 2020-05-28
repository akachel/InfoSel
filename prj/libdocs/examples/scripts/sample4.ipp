#
#
# InfoSel++ Control Script
#
# Sample #4
#
#

#
# Wielowartosciowe wyrazenia:
#

 output = "sample.rep";
 input = "sample.dat";
 
 precision = 0.001;
 classislast = true;       
 partition = { equiwidth(24) , equidepth(16) , maxvdiff(24) };  # zbior wartosci
 
 exec( b0( [0.5..0.9:0.1] ) );  # zakres wartosci od 0.5 do 0.9 ze skokiem 0.1

#
# program zostanie wykonany dla wszystkich kombinacji 
# wartosci p/w wielowartosciowych wyrazen, a wiec dla:
#
#  partition=equiwidth(24) , beta=0.5  
#  partition=equiwidth(24) , beta=0.6  
#     ...........
#  partition=equiwidth(24) , beta=0.9
#  partition=equidepth(16) , beta=0.5  
#  partition=equidepth(16) , beta=0.6
#     ...........
#     ...........
#  partition=maxvdiff(24) , beta=0.9
#
