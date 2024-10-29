# Zbiory arytmetyczne

**Uwaga 1:** -2 % 5 == -2.

**Uwaga 2:** Plik ocen.c został uzupełniony o przykładowe wywołania funkcji nalezy.

W tym zadaniu powinieneś zaimplementować w języku C podstawowe operacje teoriomnogościowe na zbiorach liczb całkowitych. Zbiory w zadaniu konstruujemy za pomocą specyficznych ciągów arytmetycznych.

Zbiory będziemy reprezentować za pomocą typu struct zbior_ary.

Do dyspozycji są dwa konstruktory zbiorów:

- `zbior_ary ciag_arytmetyczny(int a, int q, int b);`
Tworzy zbiór będący skończonym ciągiem arytmetycznym o różnicy q>0, o zadanym elemencie początkowym a i końcowym b. (Mamy a mod q = b mod q.) Różnica q będzie taka sama we wszystkich wywołaniach konstruktora ciag_arytmetyczny w danym teście.

- `zbior_ary singleton(int a);`
Tworzy zbiór złożony z tylko jednej liczby całkowitej a.

Operacje na zbiorach, jakie należy zaimplementować, to:
- `zbior_ary suma(zbior_ary A, zbior_ary B);`
Wynikiem funkcji jest suma teoriomnogościowa zbiorów A i B.
-`zbior_ary iloczyn(zbior_ary A, zbior_ary B);`
Wynikiem funkcji jest część wspólna zbiorów A i B.
-`zbior_ary roznica(zbior_ary A, zbior_ary B);`
Wynikiem funkcji jest różnica zbiorów A i B.


Ponadto należy zaimplementować następujące funkcje:


- `bool nalezy(zbior_ary A, int x);`
Funkcja sprawdza, czy liczba x należy do zbioru A.
-`unsigned moc(zbior_ary A);`
Funkcja daje w wyniku liczbę elementów zbioru A.

Dla danego zbioru A oraz liczby q, przez `Ary_q(A)` oznaczamy minimalną liczbę parami rozłącznych ciągów arytmetycznych o różnicy q, których sumą jest zbiór A. Np. `Ary_5({1,2,4,7,9,12,19})=4` jako że zbiór {1,2,4,7,9,12,17,19} jest sumą czterech ciągów arytmetycznych o różnicy 5, tj. {1}, {2,7,12,17}, {4,9} i {19}.

Powinieneś zaimplementować jeszcze jedną funkcję:
- `unsigned ary(zbior_ary A);`
Funkcja danego zbioru A daje w wyniku liczbę `Ary_q(A)`, dla zadanego w teście parametru q.


W Twojej implementacji złożoność czasowa funkcji suma(A, B), iloczyn(A, B) i roznica(A, B) powinna być proporcjonalna do `Ary_q(A)+Ary_q(B)`, złożoność funkcji nalezy(A, x) powinna być `O(log(Ary_q(A)))`, a złożoność funkcji moc(A) i ary(A) powinna być w najgorszym razie `O(Ary_q(A))`. Do przechowywania zbioru A należy użyć `O(Ary_q(A))` pamięci. Rozwiązania nieoptymalne czasowo lub pamięciowo, ale poprawne, mogą liczyć na punkty częściowe.

Argumentami powyższych funkcji będą liczby typu int. Parametr q będzie liczbą dodatnią i we wszystkich wywołaniach funkcji ciag_arytmetyczny w danym teście będzie taki sam. Ponadto funkcja ciag_arytmetyczny zostanie wywołana jako pierwsza, dzięki czemu Twoja biblioteka będzie mogła od razu poznać parametr q.

Wyżej opisane funkcje znajdują się w pliku `zbior_ary.h`. Twoim zadaniem jest zaproponować reprezentację zbiorów, jako struct `zbior_ary`, która umożliwia efektywne wykonywanie operacji jak napisano powyżej. Twoje rozwiązanie ma być umieszczone w pliku o nazwie `zbior_ary.c` i pasować do specyfikacji interfejsu `zbior_ary.h`. Należy również załączyć plik `zbior_ary.h` uzupełniony o definicję typu struct zbior_ary.

Kilka przykładowych operacji na zbiorach możesz znaleźć w pliku `ocen.c`. Komenda kompilacji (pliki `zbior_ary.c` i zbior_ary.h muszą być w katalogu):

`gcc @opcje ocen.c zbior_ary.c -o ary -lm`

Opcja `-lm` załącza `math.h`. Koniecznie musi znajdować się na końcu komendy kompilacji. W rozwiązaniu nie musisz używać `math.h`, ale możesz.