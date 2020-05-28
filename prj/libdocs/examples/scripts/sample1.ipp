#
#
# InfoSel++ Control Script
#
# Sample #1
#
#

output = "sample1.rep";

input = "sample.dat";
classislast = true;
precision = 0.001;
partition = exact;

exec( b0 );
exec( c );

excluded(0,1);
merged(nil);
exec( b0 );
exec( c );

excluded(nil);
merged(0,1);
exec( b0 );
exec( c );

output = "sample2.out";

excluded(nil);
merged(nil);
exec( b0 );
exec( c );

notexcluded(0,1);
merged(nil);
exec( b0 );
exec( c );

excluded(nil);
notmerged(0,1);
exec( b0 );
exec( c );


