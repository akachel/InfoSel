#
#
# InfoSel++ Control Script
#
# This is a template script 
# to execute any studied
# algorithm in batch mode
#
#

 output = "abc.rep"; # set up name of output report file
 input = "abc.dat";  # set up name of input data file
 
 precision = 0.001;        # set up precision for real numbers
 classislast = true;       # set up true if data-classes column is last inside the data file
 excluded = 10, 20;        # set up labels of data-classes to exclude from the data file
 merged = 5, 15;           # set up labels of data-classes to merge within the data file
 partition = equiwidth(5); # set up partition method and its parameters
 
 echoln; # write message text to the report file and standard output
 echoln("processing the input data file...");
 
 exec( x_0(0.11, 0.22, 3) ); # execute the algorithm with given variant '0' and for all given parameters
 exec( y(1, 0.11, 0.22, default) ); # execute the algorithm for parameters preceded by choosen variant '1' and with last paramater as explicit default
 exec( z(missing, 0.11, 0.22) ); # execute the algorithm for parameters preceded by missing (i.e. default) spec. of variant and with last paramater as implicit default
