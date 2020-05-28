#
#
# InfoSel++ Control Script
#
# Sample #8
#
#

#
# Infosel++ is used in batch mode using command:
#
#          infosel++.g.exe -s test8.ipp

#
# We can find more informations about options and method of discretization 
# by using an Infosel application with option -h.
#

#
# Merging/excluding of rows, another example:
#


output = "sample0.rep";   # name of output file, by default: "default.rep"
input = "sample.dat";     # name of input file, by default: "default.dat"
classislast = true;       # setting the flags column location of classes, by default: true
precision = 0.001;        # setting precision of reading/writing data , default: 0.001
partition = exact;        # discretization method of choice and parameters. In this case, we have exact discretization, where each element is recognized as unique value .

excluded(nil);            # set of classes exclude from input file, by default: excluded(nil) -- empty set
merged(nil);              # set of classes merge in input file, by default: merged(nil) -- empty set
exec( mi );               # execute "mi" ranking based on mutual information with default parameters

excluded(1,2,5);          # exclude from calculation rows with class 1,2 and 5
merged(nil);              # do not merge any class
exec( mi );               # execute "mi" ranking based on mutual information with default parameters

excluded(nil);            # reset set related to excluded classes. Now all classes will be considered in calculations.
merged(1,2,5);            # merge class 0,1, and 5 into one class
exec( mi );               # execute "mi" ranking based on mutual information with default parameters

excluded(nil);            # reset set related to excluded classes. Now all classes will be considered in calculations.
merged(nil);              # reset set related to merged classes. Now no class will be merged.
exec( mi );               # execute "mi" ranking based on mutual information with default parameters.

notexcluded(1,2,5);       # do not exclude class 1,2 and 5. All others classes will be excluded. 
merged(nil);              # reset set related to merged classes. Now no class will be merged.
exec( mi );               # execute "mi" ranking based on mutual information with default parameters

excluded(nil);            # reset set related to excluded classes. Now all classes will be considered in calculations.
notmerged(1,2,5);         # do not merge class 1,2, and 5. All other classes will me merged into one class.
exec( mi );               # execute "mi" ranking based on mutual information with default parameters

