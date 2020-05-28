#
#
# InfoSel++ Control Script
#
# Sample #3
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test3.ipp
#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#

#
# Hybrid algorithm in script mode:
#

 output = "sample.rep";        # name of output file, by default: "default.rep"
 input = "sample.dat";         # name of input file, by default: "default.dat"
 
 precision = 0.001;            # setting precision of reading/writing data , default: 0.001
 classislast = true;           # setting the flags column location of classes, by default: true
 partition = equiwidth(24);    
 
 exec( mi(10) > mifs(-1) );                # serial processing of algorithms. Ordered subset of 10 best features is 
                                           # passed from mi algorithm to mifs algorithm (sequentially)
 exec( mifs(0.5, 10) | suc(-1) );          # parallel processing of algorithms. From mifs algorithm we will bypass 
                                           # subset of 10 best features to suc ranking. (We are ignoring 10 best features.)
 exec( mifs(0.5, 10) | suc(-1) > gdd );    # mixed processing of algorithms (serial-parallel processing)
