#
#
# InfoSel++ Control Script
#
# Sample #1
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test1.ipp
#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#

output = "sample0.rep";   # name of output file, by default: "default.rep"
input = "sample.dat";     # name of input file, by default: "default.dat"
classislast = true;       # setting the flags column location of classes, by default: true
precision = 0.001;        # setting precision of reading/writing data , default: 0.001
partition = exact;        # discretization method of choice and parameters, in this case we have exact discretization, where each element is recognized as uniq value 

exec( mi );               # execute "mi" ranking based on mutual information with default parameters
exec( suc );              # execute "suc" ranking based on symmetrical uncertainty coefficient

excluded(0,1);            # exclude from calculation rows with class 0 and 1
merged(nil);              # do not merge any class
exec( mi );               # execute "mi" ranking based on mutual information with default parameters
exec( suc );              # execute "suc" ranking based on symmetrical uncertainty coefficient

excluded(nil);            # reset set related to excluded classes. Now all classes will be considered in calculations
merged(0,1);              # merge class 0 and 1 into one class
exec( mi );               # execute "mi" ranking based on mutual information with default parameters
exec( suc );              # execute "suc" ranking based on symmetrical uncertainty coefficient

output = "sample1.out";   # In this line, we change the output file. From now on, all results will be stored in this file.

excluded(nil);            # reset set related to excluded classes. Now all classes will be considered in calculations. 
merged(nil);              # reset set related to merged classes. Now no class will be merged.
exec( mi );               # execute "mi" ranking based on mutual information with default parameters
exec( suc );              # execute "suc" ranking based on symmetrical uncertainty coefficient

notexcluded(0,1);         # do not exclude class 0,1. All other classes will be excluded. 
merged(nil);              # reset set related to merged classes. Now no class will be merged.
exec( mi );               # execute "mi" ranking based on mutual information with default parameters
exec( suc );              # execute "suc" ranking based on symmetrical uncertainty coefficient

excluded(nil);            # reset set related to excluded classes. Now all classes will be considered in calculations
notmerged(0,1);           # do not merge class 0,1. All other classes will be merged. 
exec( mi );               # execute "mi" ranking based on mutual information with default parameters
exec( suc );              # execute "suc" ranking based on symmetrical uncertainty coefficient


