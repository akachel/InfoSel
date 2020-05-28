#
#
# InfoSel++ Control Script
#
# Sample #2
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
 
 exec( battiti_0(0.7, 10) ); \# wykonanie podanego algorytmu, 
                                 nazwa algorytmu powinna byc podana malymi literami, 
                                 spacje zastapione znakiem '_' ,
                                 przekazane parametry to: 
                                  wspolczynnik beta, domyslnie: 0.5
                                  liczba cech w zbiorze finalnym, domyslnie: -1 (wszystkie cechy)  #\

 exec( b0(0.7, 10) );         # zamiast nazwy mozna podac symbol algorytmu
 exec( b0(0.7) );             # doymslnie: b0(0.7, -1)
 exec( b0 );                  # domyslnie: b0(0.5, -1)
 
 execall;                     # wykonanie wszystkich algorytmow z parametrami domyslnymi

 echoln("");                  # zapisanie tekstu w pliku wyjsciowym output             
 echo("-------");
 echo("koniec");
 echoln("------");
