#
#
# InfoSel++ Control Script
#
# Sample #8
#
#

#
# Wykluczanie/laczenie klas:
#


output = "sample.rep";
input = "sample.dat";

classislast = true; # zamiast tego mozna stosowac instrukcje: classpos = 9;
precision = 0.001;
partition = equiwidth(24);

excluded(nil); # zbior klas wykluczonych ze pliku wejsciowego, domyslnie: excluded(nil) - zbior pusty
merged(nil); # zbior klas polaczonych we pliku wejsciowym, domyslnie: merged(nil) - zbior pusty
exec( b0 );

excluded(1,2,5); # wykluczono klasy: 1,2,5
merged(nil); # wyzeruj zbior
exec( b0 );

excluded(nil); # wyzeruj zbior
merged(1,2,5); # polaczono klasy: 1,2,5
exec( b0 );

notexcluded(1,2,5); # wykluczono wszystkie klasy poza: 1,2,5
merged(nil); # wyzeruj zbior
exec( b0 );

excluded(nil); # wyzeruj zbior
notmerged(1,2,5); # polaczono wszystkie klasy poza: 1,2,5
exec( b0 );

