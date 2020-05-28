#
#
# InfoSel++ Control Script
#
# Sample #0
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test0.ipp
#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#

#
# Brief description of Infosel++ script language:
#


# line comment

\# 
  
 block comments

#\


 output = "sample.rep";      # name of output file, by default: "default.rep"
 input = "sample.dat";       # name of input file, by default: "default.dat"
 
 precision = 0.001;           # setting precision of reading/writing data , default: 0.001
 classislast = true;          # setting the flags column location of classes, by default: true
 partition = equiwidth(24);   # discretization method of choice and parameters (here: number of bins), by default : equiwidth(24)
 
 exec( mifs(0.7, 10) );

 
