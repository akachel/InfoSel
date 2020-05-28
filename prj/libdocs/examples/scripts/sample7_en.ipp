#
#
# InfoSel++ Control Script
#
# Sample #7
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test7.ipp
#
#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#

#
# Example of complex script.
#


output = "sample0.rep";   # name of output file, by default: "default.rep"
input = "sample.dat";     # name of input file, by default: "default.dat"
classislast = true;       # setting the flags column location of classes, by default: true
precision = 0.001;        # setting precision of reading/writing data , default: 0.001
partition = exact;        # discretization method of choice and parameters. In this case, we have exact discretization, where each element is recognized as a unique value 


 exec( mifs(0.7) );       # this command execute algorithm mifs (Battiti's algorithm) with beta equal to 0.7

# other file:

 output = "sample2.rep";  # name of output file, by default: "default.rep"
 input = "sample2.dat";   # name of input file, by default: "default.dat"
 precision = 0.001;       # setting precision of reading/writing data , default: 0.001
 classislast = true;      # setting the flags column location of classes, by default: true
 partition = equidepth(24);  # discretization method of choice and parameters (here: number of bins), by default : equiwidth(24)

 exec( suc );             # execute "suc" ranking based on symmetrical uncertainty coefficient

#
# Because algorithms are processed sequentially, now we can change some parameters 
# and the data set selected previously will be processed with new parameters:
#

 partition = maxvdiff(24);  

 exec(  amifs); 
