#
#
# InfoSel++ Control Script
#
# This is an example script
# to execute any standard
# algorithm by the main 
# infosel++ application
#
#

 output = "output.rep"; # set up name of output report file      
 input = "input.dat";   # set up name of input data file       
 
 precision = 0.001;        # set up precision for real numbers           
 classislast = true;       # set up true if data-classes column is last inside the data file          
 partition = equiwidth(5); # set up partition method and its parameters   
 
 exec(pcc_1); # execute the standard algorithm (i.e. Correl::PearsonCC) with given variant '1' (i.e. the selection)
