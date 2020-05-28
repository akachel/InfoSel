#
#
# InfoSel++ Control Script
#
# Sample #4
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test4.ipp
#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#

#
# Multi-valued expressions:
#

 output = "sample.rep";    # name of output file, by default: "default.rep"
 input = "sample.dat";     # name of input file, by default: "default.dat"
 
 precision = 0.001;        # setting precision of reading/writing data , default: 0.001
 classislast = true;       # setting the flags column location of classes, by default: true
 partition = { equiwidth(24) , equidepth(16) , maxvdiff(24) };  
                           # discretization method of choice and parameters. Here we selected set of tree diffrent discretization: 
                           # equal-width discretization with 24 bins, equal-frequency discretization with 16 bins, and equal-variance discretization with 24 bins
 
 exec( mifs( [0.5..0.9:0.1] ) );  # parameter beta is defined in range 0.5 to 0.9 with interval equal to 0.1

#
# In this case, the infosel++ application will be processed for all combinations of multi-valued expressions 
# which can be expressed as follows: 
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
