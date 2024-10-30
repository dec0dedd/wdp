// Michal Burzyński

#include "zbior_ary.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static long long Q;

long long min(long long a, long long b) {
    return a <= b ? a : b;
}

long long max(long long a, long long b) {
    return a >= b ? a : b;
}

// funkcja generujaca ciag arytmetyczny
zbior_ary ciag_arytmetyczny(int a, int q, int b) {
    Q = (long long)q;

    przedz prz;
    prz.lewy = (long long)a, prz.prawy = (long long)b;

    zbior zb;
    zb.przdz = malloc(sizeof(przedz));
    zb.przdz[0] = prz, zb.dlugosc = 1;
    zb.reszta = a % (int)Q;

    zbior_ary wynik;
    wynik.ciag = malloc(sizeof(zb));
    wynik.ciag[0] = zb;
    wynik.dlugosc = 1;

    return wynik;
}

// singleton to po prostu ciag arytmetyczny z jednym elementem
zbior_ary singleton(int a) {
    return ciag_arytmetyczny(a, (int)Q, a);
}

// funkcja zwracajaca moc zbioru A
unsigned moc(zbior_ary A) {
    unsigned ans = 0;

    // iterujemy sie po resztach z dzielenia przez Q podzbiorow ktore trzymamy
    // i dla kazdego przedzialu [L, R] dodajemy (R-L)/Q+1 do wynik poniewaz
    // w zbiorze {L, L+Q, L+2Q, ..., R} jest dokladnie (R-L)/Q+1 elementow
    for (int i = 0; i < A.dlugosc; ++i) {
        for (int j = 0; j < A.ciag[i].dlugosc; ++j) {
            ans += (unsigned)((A.ciag[i].przdz[j].prawy - A.ciag[i].przdz[j].lewy) / Q) + 1;
        }
    } 
    
    return ans;
}

// funkcja zwracajaca wartosc Ary_q(A)
unsigned ary(zbior_ary A) {
    unsigned ans = 0;

    for (int i = 0; i < A.dlugosc; ++i) {
        ans += (unsigned)(A.ciag[i].dlugosc);
    }

    return ans;
}

// funkcja pomocnicza zwracajaca sume dwoch zbiorow o elementach
// majacych te sama reszte z dzielenia przez Q
zbior _suma(zbior A, zbior B) {
    assert(A.reszta == B.reszta);

    zbior wynik;
    wynik.przdz = malloc((unsigned)(A.dlugosc + B.dlugosc) * sizeof(przedz));

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc || j < B.dlugosc) {
        przedz obecny;

        // wybieramy przedzial z lewym koncem bardziej wysunietym na lewo
        if (i < A.dlugosc && j < B.dlugosc) {
            przedz a_prz = A.przdz[i];
            przedz b_prz = B.przdz[j];

            if (a_prz.lewy < b_prz.lewy) {
                obecny = a_prz;
                ++i;
            } else {
                obecny = b_prz;
                ++j;
            }
        } else if (i < A.dlugosc) {
            obecny = A.przdz[i];
            ++i;
        } else {
            assert(j < B.dlugosc);
            obecny = B.przdz[j];
            ++j;
        }

        if (k == 0) {
            wynik.przdz[k] = obecny;
            ++k;
        } else {
            przedz *ostatni = &wynik.przdz[k-1];

            // jesli obecny.lewy <= ostatni->prawy+Q to możemy przedluzyc ostatni przedzial
            // zamiast dodawac nowy
            if (obecny.lewy - ostatni->prawy <= Q) {
                ostatni->lewy = min(ostatni->lewy, obecny.lewy);
                ostatni->prawy = max(ostatni->prawy, obecny.prawy);
            } else {
                wynik.przdz[k] = obecny;
                ++k;
            }
        }
    } assert(i == A.dlugosc && j == B.dlugosc);

    wynik.dlugosc = k;
    wynik.reszta = A.reszta;

    return wynik;
}

// funkcja zwracajaca sume zbiorow A i B
zbior_ary suma(zbior_ary A, zbior_ary B) {
    zbior_ary wynik;
    wynik.ciag = malloc((unsigned)(A.dlugosc + B.dlugosc) * sizeof(zbior));

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {

        // jesli zbior A.ciag[i] trzyma zbiory o reszcie mniejszej niz zbior B.ciag[j]
        // to wystarczy go po prostu dodac do wynikowego zbioru i iterowac sie dalej
        // poniewaz przedzialy [L, R] oraz [X, Y] takie ze L%Q != X%Q mozemy traktowac niezaleznie
        // (analogicznie w druga strone)
        if (A.ciag[i].reszta < B.ciag[j].reszta) {
            wynik.ciag[k] = A.ciag[i];
            ++k, ++i;
        } else if (A.ciag[i].reszta > B.ciag[j].reszta) {
            wynik.ciag[k] = B.ciag[j];
            ++k, ++j;
        } else {
            wynik.ciag[k] = _suma(A.ciag[i], B.ciag[j]);
            ++k, ++i, ++j;
        }
    }

    // dodajemy pozostale podzbiory z A i B
    while (i < A.dlugosc) {
        wynik.ciag[k] = A.ciag[i];
        ++k, ++i;
    }

    while (j < B.dlugosc) {
        wynik.ciag[k] = B.ciag[j];
        ++k, ++j;
    }

    wynik.dlugosc = k;
    return wynik;
}


// funkcja pomocnicza zwracajaca roznice zbiorow A i B
// zakladamy, ze zbior A oraz zbior B trzymaja elementy
// o tych samych resztach z dzielenia przez Q
zbior _roznica(zbior A, zbior B) {
    assert(A.reszta == B.reszta);

    zbior wynik;
    wynik.przdz = malloc((unsigned)(A.dlugosc + B.dlugosc) * sizeof(zbior));

    przedz *a_przedzialy = malloc((unsigned)(A.dlugosc) * sizeof(przedz));
    memcpy(a_przedzialy, A.przdz, (unsigned)(A.dlugosc) * sizeof(przedz));

    przedz *b_przedzialy = malloc((unsigned)(B.dlugosc) * sizeof(przedz));
    memcpy(b_przedzialy, B.przdz, (unsigned)(B.dlugosc) * sizeof(przedz));

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {
        przedz *a_prz = &a_przedzialy[i];
        przedz *b_prz = &b_przedzialy[j];

        if (a_prz->prawy < b_prz->lewy) {
            // obecny przedzial z b_przedzialy zaczyna sie pozniej nie konczy sie a_prz
            // wiec nie moga sie przecinac
            wynik.przdz[k] = *a_prz;
            ++k, ++i;
        } else if (a_prz->lewy > b_prz->prawy) {
            // obecny przedzial b_prz konczy sie wczesniej niz zaczyna sie a_prz
            // wiec mozemy go pominac
            ++j;
        } else {
            // jesli a_prz->lewy < b_prz->lewy to musimy "uciac"
            // powstaly przedzial do (a_prz->lewy, b_prz->lewy-Q)
            if (a_prz->lewy < b_prz->lewy) {
                wynik.przdz[k].lewy = a_prz->lewy;
                wynik.przdz[k].prawy = b_prz->lewy-Q;

                // jesli powstaly przedzial jest poprawny tzn. jego lewy koniec
                // jest przed jego prawym to dodajemy go do zbioru
                if (wynik.przdz[k].lewy <= wynik.przdz[k].prawy) ++k;
            }
            // jesli b_prz->prawy < a_prz->prawy to musimy
            // przesunac lewy koniec przedzialu a_prz
            if (b_prz->prawy < a_prz->prawy) {
                a_prz->lewy = b_prz->prawy + Q;
                ++j;
            } else {
                ++i;
            }
        }
    }

    // dodajemy pozostale przedzialy ze zbioru A
    while (i < A.dlugosc) {
        wynik.przdz[k] = a_przedzialy[i];
        ++k, ++i;
    }

    wynik.dlugosc = k;
    wynik.reszta = A.reszta;
    return wynik;
}

zbior_ary roznica(zbior_ary A, zbior_ary B) {
    zbior_ary wynik;
    wynik.ciag = malloc((unsigned)(A.dlugosc+B.dlugosc)*sizeof(zbior));

    int i = 0, j = 0, k = 0; 

    // iterujemy sie po podzbiorach A oraz B
    // idac rosnaco po resztach z dzielenia wartosci elementow przez Q
    while (i < A.dlugosc && j < B.dlugosc) {
        if (A.ciag[i].reszta < B.ciag[j].reszta) {
            wynik.ciag[k] = A.ciag[i];
            ++k, ++i;
        } else if (A.ciag[i].reszta > B.ciag[j].reszta) {
            ++j;
        } else {
            wynik.ciag[k] = _roznica(A.ciag[i], B.ciag[j]);
            ++k, ++i, ++j;
        }
    }

    // dodajemy pozostale podzbiory z A
    while (i < A.dlugosc) {
        wynik.ciag[k] = A.ciag[i];
        ++k, ++i;
    }

    wynik.dlugosc = k;

    return wynik;
}

// funkcja pomocnicza zwracajaca iloczyn zbiorow A i B,
// zakladamy, ze elementy zbiorow A oraz B zwracaja ta sama
// reszte z dzielenia przez Q
zbior _iloczyn(zbior A, zbior B) {
    zbior wynik;
    wynik.przdz = malloc((unsigned)(A.dlugosc + B.dlugosc) * sizeof(przedz));
    wynik.reszta = A.reszta;

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {
        przedz a_prz = A.przdz[i];
        przedz b_prz = B.przdz[j];

        // jesli prawy koniec a_prz jest mniejszy niz lewy b_prz
        // to przechodzimy dalej (analogicznie w odwrotnym przypadku)
        if (a_prz.prawy < b_prz.lewy) {
            ++i;
        } else if (b_prz.prawy < a_prz.lewy) {
            ++j;
        } else {
            wynik.przdz[k].lewy = max(a_prz.lewy, b_prz.lewy);
            wynik.przdz[k].prawy = min(a_prz.prawy, b_prz.prawy);

            // jesli przedzial jest poprawny tzn. jego lewy koniec jest
            // mniejszy niz prawy to dodajemy go do zbioru
            if (wynik.przdz[k].lewy <= wynik.przdz[k].prawy) {
                ++k;
            }

            if (A.przdz[i].prawy < B.przdz[j].prawy) {
                ++i;
            } else {
                ++j;
            }
        }
    }

    wynik.dlugosc = k;
    return wynik;
}

// funkcja zwracajaca iloczyn zbiorów A i B
zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
    zbior_ary wynik;
    wynik.ciag = malloc((unsigned)(A.dlugosc + B.dlugosc) * sizeof(zbior));

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {
        if (A.ciag[i].reszta < B.ciag[j].reszta) {
            ++i;
        } else if (A.ciag[i].reszta > B.ciag[j].reszta) {
            ++j;
        } else {
            wynik.ciag[k] = _iloczyn(A.ciag[i], B.ciag[j]);
            ++k, ++i, ++j;
        }
    }

    wynik.dlugosc = k;
    return wynik;
}

// funkcja zwracajaca true wtedy i tylko wtedy gdy element
// x nalezy do zbioru A
bool nalezy(zbior_ary A, int x) {
    int x_reszta = x % (int)Q, l = 0, r = A.dlugosc - 1;

    // podzbiory A sa posortowane rosnaco po resztach z dzielenia
    // elementow, które trzymaja wiec mozemy wykonac wyszukiwanie binarne
    while (l <= r) {
        int mid = (l + r) / 2;

        if (A.ciag[mid].reszta < x_reszta) {
            l = mid + 1;
        } else if (A.ciag[mid].reszta > x_reszta) {
            r = mid - 1;
        } else {
            int l_intv = 0, r_intv = A.ciag[mid].dlugosc - 1, ans = -1;

            // gdy znalezlismy juz indeks odpowiadajacy za podzbior z reszta x_reszta
            // musimy teraz znalezc przedzial [L, R] z najwiekszym L
            // mniejszym lub rownym x co wykonamy przez wyszukiwanie binarne
            while (l_intv <= r_intv) {
                int mid_intv = (l_intv + r_intv) / 2;

                if (A.ciag[mid].przdz[mid_intv].lewy <= x) ans = mid_intv, l_intv = mid_intv + 1;
                else r_intv = mid_intv - 1;
            }

            // jesli ans = -1 to nie znalezlismy takiego przedzialu
            // wiec zwracamy false
            if (ans == -1) return false;
            return A.ciag[mid].przdz[ans].lewy <= x && x <= A.ciag[mid].przdz[ans].prawy;
        }
    } return false;
}
