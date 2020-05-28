#
#
# InfoSel++ Control Script
#
# Sample #2
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test2.ipp
#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#


 output = "sample.rep";       # name of output file, by default: "default.rep"
 input = "sample.dat";        # name of input file, by default: "default.dat"
 
 precision = 0.001;           # setting precision of reading/writing data , default: 0.001
 classislast = true;          # setting the flags column location of classes, by default: true
 partition = equiwidth(24);   # discretization method of choice and parameters (here: number of bins), by default : equiwidth(24)
 
 exec( MIFS(0.7, 10) );       # execute algorithm mifs by calling a class name MIFS (Battiti's algorithm) with two parameters, beta equal to 0.7 and algorithm returns only 
                              # 10 important features. These two approaches are equivalent. 

 exec( mifs(0.7, 10) );       # the same command as before, but algorithm is executed by its acronym from Infosel++ menu
 exec( mifs(0.7) );           # this command does not specify number of selected features by default (-1). Algorithm will return all features.
 exec( mifs );                # this command uses default parameters for mifs algorithm. Beta = 0.5 and will return all features.
 
 execall;                     # process all algorithms with default parameters 

 echoln("");                  # this command writes text "" (empty text) in output file, in this script "sample.rep".
 echo("-------");
 echo("koniec");
 echoln("------");
