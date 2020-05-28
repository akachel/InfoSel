#
#
# InfoSel++ Control Script
#
# Sample #6
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test6.ipp
#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#

#
# Constant value declaration and arithmetic operations:
#

 output = "sample.rep";            # name of output file, by default: "default.rep"
 input = "sample.dat";             # name of input file, by default: "default.dat"
 
 precision = 0.001;                # setting precision of reading/writing data , default: 0.001
 classislast = true;               # setting the flags column location of classes, by default: true
 partition = equiwidth(24);        # discretization method of choice and parameters (here: number of bins), by default : equiwidth(24)

 const beta = 0.9;                 # declaration of beta 
 exec( mifs(beta) );               # passing beta into mifs algorithm
 exec( mifs(beta/2) );             # passing beta divided by 2 into mifs algorithm
 const b = 0.2;                    # declaration of new b constant 
 exec( mifs( 1.5*(beta/2+b) ) );   # passing complex expression into mifs algorithm

