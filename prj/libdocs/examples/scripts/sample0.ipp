#
#
# InfoSel++ Control Script
#
# Sample #0
#
#

#
# Plik skrypu jest wykonywany przez
# program po podaniu polecenia:
#
#          infosel++ -s test1.npp
#
# Wiecej informacji o opcjach
# linii polecen programu oraz
# o nazwach metod partycjonowania
# i algorytmow mozna uzyskac
# poprzez opcje -h
#

#
# Krotki opis jezyka skryptu:
#


# komentarz liniowy

\# 
  
 komentarz blokowy

#\


 output = "sample.rep";      # nazwa pliku wyjsciwego, domyslnie: "default.rep"
 input = "sample.dat";       # nazwa pliku danych, domyslnie: "default.dat"
 
 precision = 0.001;           # ustawienie precyzji odczytu/zapisu danych, domyslnie: 0.001
 classislast = true;          # ustawienie flagi polozenia kolumny klas, domyslnie: true
 partition = equiwidth(24);   # wybor metody partycjonowania i podanie jej parametrow (tu: liczby przedzialow), domyslnie: equiwidth(24)
 
 exec( battiti_0(0.7, 10) );

 