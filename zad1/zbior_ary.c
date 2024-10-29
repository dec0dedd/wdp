// Michał Burzyński

#include "zbior_ary.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static int Q;

int min(int a, int b) {
    return a <= b ? a : b;
}

int max(int a, int b) {
    return a >= b ? a : b;
}

// funkcja generująca ciąg arytmetyczny
zbior_ary ciag_arytmetyczny(int a, int q, int b) {
    Q = q;

    przedz it;
    it.lewy = a, it.prawy = b;

    zbior zb;
    zb.przdz = malloc(sizeof(przedz));
    zb.przdz[0] = it, zb.dlugosc = 1;
    zb.reszta = a % Q;

    zbior_ary wynik;
    wynik.ciag = malloc(sizeof(zb));
    wynik.ciag[0] = zb;
    wynik.dlugosc = 1;

    return wynik;
}

// singleton to po prostu ciąg arytmetyczny z jednym elementem
zbior_ary singleton(int a) {
    return ciag_arytmetyczny(a, Q, a);
}

// funkcja zwracająca moc zbioru A
unsigned moc(zbior_ary A) {
    unsigned ans = 0;

    for (int i=0; i<A.dlugosc; ++i) {
        for (int j=0; j<A.ciag[i].dlugosc; ++j) {
            ans += (unsigned)((A.ciag[i].przdz[j].prawy - A.ciag[i].przdz[j].lewy) / Q) + 1;
        }
    } 
    
    return ans;
}

// funkcja zwracająca wartość "ary" zbioru A
unsigned ary(zbior_ary A) {
    unsigned ans = 0;
    for (int i=0; i<A.dlugosc; ++i) {
        ans += (unsigned)(A.ciag[i].dlugosc);
    }

    return ans;
}

// funkcja pomocnicza zwracająca sumę dwóch zbiorów o elementach
// mających tę samą redlugosctę z dzielenia przez Q
zbior _suma(zbior A, zbior B) {
    assert(A.reszta == B.reszta);

    zbior wynik;
    wynik.przdz = malloc((unsigned)(A.dlugosc+B.dlugosc)*sizeof(przedz));

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {
        przedz a_prz = A.przdz[i];
        przedz b_prz = B.przdz[j];

        przedz obecny;

        // wybieramy przedział z lewym końcem bardziej wysuniętym na lewo
        if (a_prz.lewy < b_prz.lewy) obecny = a_prz, ++i;
        else obecny = b_prz, ++j;

        if (k == 0) {
            wynik.przdz[k++] = obecny;
        } else {
            przedz *ostatni = &wynik.przdz[k-1];

            // jeśli obecny.lewy <= last->prawy+Q to możemy przedłużyć ostatni przedział
            // zamiast dodawać nowy
            if (obecny.lewy <= ostatni->prawy + Q) {
                ostatni->lewy = min(ostatni->lewy, obecny.lewy);
                ostatni->prawy = max(ostatni->prawy, obecny.prawy);
            } else {
                wynik.przdz[k++] = obecny;
            }
        }
    }

    // dodajemy pozostałe przedziały ze zbioru A
    while (i < A.dlugosc) {
        przedz obecny = A.przdz[i];
        ++i;

        if (k == 0) {
            wynik.przdz[k++] = obecny;
        } else {
            przedz *ostatni = &wynik.przdz[k-1];

            // jeśli obecny.lewy <= last->prawy+Q to możemy przedłużyć ostatni przedział
            // zamiast dodawać nowy
            if (obecny.lewy <= ostatni->prawy + Q) {
                ostatni->lewy = min(ostatni->lewy, obecny.lewy);
                ostatni->prawy = max(ostatni->prawy, obecny.prawy);
            } else {
                wynik.przdz[k++] = obecny;
            }
        }
    }

    // dodajemy pozostałe przedziały z B
    while (j < B.dlugosc) {
        przedz obecny = B.przdz[j];
        ++j;

        if (k == 0) {
            wynik.przdz[k++] = obecny;
        } else {
            przedz *last = &wynik.przdz[k-1];

            // jeśli obecny.lewy <= last->prawy+Q to możemy przedłużyć ostatni przedział
            // zamiast dodawać nowy
            if (obecny.lewy <= last->prawy + Q) {
                last->lewy = min(last->lewy, obecny.lewy);
                last->prawy = max(last->prawy, obecny.prawy);
            } else {
                wynik.przdz[k++] = obecny;
            }
        }
    }

    wynik.dlugosc = k;
    wynik.reszta = A.reszta;

    return wynik;
}

// funkcja zwracająca sumę zbiorów A i B
zbior_ary suma(zbior_ary A, zbior_ary B) {
    zbior_ary wynik;
    wynik.ciag = malloc((unsigned)(A.dlugosc + B.dlugosc)*sizeof(zbior));

    int i=0, j=0, k=0;
    while (i < A.dlugosc && j < B.dlugosc) {
        if (A.ciag[i].reszta < B.ciag[j].reszta) {
            wynik.ciag[k++] = A.ciag[i++];
        } else if (A.ciag[i].reszta > B.ciag[j].reszta) {
            wynik.ciag[k++] = B.ciag[j++];
        } else {
            wynik.ciag[k++] = _suma(A.ciag[i++], B.ciag[j++]);
        }
    }

    // dodajemy pozostałe podzbiory z A i B
    while (i < A.dlugosc) wynik.ciag[k++] = A.ciag[i++];
    while (j < B.dlugosc) wynik.ciag[k++] = B.ciag[j++];

    wynik.dlugosc = k;
    return wynik;
}


// funkcja pomocnicza zwracająca różnicę zbiórów A-B
// zakładamy, że zbiór A oraz zbiór B trzymają elementy
// o tych samych redlugosctach z dzielenia przez Q
zbior _roznica(zbior A, zbior B) {
    assert(A.reszta == B.reszta);

    zbior wynik;
    wynik.przdz = malloc((unsigned)(A.dlugosc+B.dlugosc)*sizeof(zbior));

    przedz *a_przedzialy = malloc((unsigned)(A.dlugosc)*sizeof(przedz));
    memcpy(a_przedzialy, A.przdz, (unsigned)(A.dlugosc)*sizeof(przedz));

    przedz *b_przedzialy = malloc((unsigned)(B.dlugosc)*sizeof(przedz));
    memcpy(b_przedzialy, B.przdz, (unsigned)(B.dlugosc)*sizeof(przedz));

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {
        przedz *a_prz = &a_przedzialy[i];
        przedz *b_prz = &b_przedzialy[j];

        if (a_prz->prawy < b_prz->lewy) {
            wynik.przdz[k++] = *a_prz;
            ++i;
        } else if (a_prz->lewy > b_prz->prawy) {
            ++j;
        } else {

            // jeśli a_prz->lewy < b_prz->lewy to musimy "uciąć"
            // powstały przedział do (a_prz->lewy, b_prz->lewy-Q)
            if (a_prz->lewy < b_prz->lewy) {
                wynik.przdz[k].lewy = a_prz->lewy;
                wynik.przdz[k].prawy = b_prz->lewy-Q;

                // jeśli powstały przedział jest poprawny tzn. jego lewy koniec
                // jest przed jego prawym to dodajemy go do zbioru
                if (wynik.przdz[k].lewy <= wynik.przdz[k].prawy) ++k;
            }

            // jeśli b_prz->prawy < a_prz->prawy to musimy
            // przesunąć lewy koniec przedziału a_prz
            if (b_prz->prawy < a_prz->prawy) {
                a_prz->lewy = b_prz->prawy + Q;
                ++j;
            } else ++i;
        }
    }

    // dodajemy pozostałe przedziały ze zbioru A
    while (i < A.dlugosc) wynik.przdz[k++] = a_przedzialy[i++];

    wynik.dlugosc = k;
    wynik.reszta = A.reszta;
    return wynik;
}

zbior_ary roznica(zbior_ary A, zbior_ary B) {
    zbior_ary wynik;
    wynik.ciag = malloc((unsigned)(A.dlugosc+B.dlugosc)*sizeof(zbior));

    int i = 0, j = 0, k = 0; 

    // iterujemy się po podzbiorach A oraz B
    // idąc rosnąco po redlugosctach z dzielenia wartości elementów przez Q
    while (i < A.dlugosc && j < B.dlugosc) {
        if (A.ciag[i].reszta < B.ciag[j].reszta) {
            wynik.ciag[k++] = A.ciag[i++];
        } else if (A.ciag[i].reszta > B.ciag[j].reszta) {
            ++j;
        } else {
            wynik.ciag[k++] = _roznica(A.ciag[i++], B.ciag[j++]);
        }
    }

    // dodajemy pozostałe podzbiory z A
    while (i < A.dlugosc) wynik.ciag[k++] = A.ciag[i++];
    wynik.dlugosc = k;

    return wynik;
}

// funkcja pomocnicza zwracająca iloczyn zbiorów A i B,
// zakładamy, że elementy zbiorów A oraz B zwracają tą samą
// redlugosctę z dzielenia przez Q
zbior _iloczyn(zbior A, zbior B) {
    zbior wynik;
    wynik.przdz = malloc((unsigned)(A.dlugosc+B.dlugosc)*sizeof(przedz));
    wynik.reszta = A.reszta;

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {
        przedz a_prz = A.przdz[i];
        przedz b_prz = B.przdz[j];

        // jeśli prawy koniec a_prz jest mniejdlugoscy niż lewy b_prz
        // to przechodzimy dalej (analogiczny w odwrotnym przypadku)
        if (a_prz.prawy < b_prz.lewy) ++i;
        else if (b_prz.prawy < a_prz.lewy) ++j;
        else {
            wynik.przdz[k].lewy = max(a_prz.lewy, b_prz.lewy);
            wynik.przdz[k].prawy = min(a_prz.prawy, b_prz.prawy);

            // jeśli przedział jest poprawny tzn. jego lewy koniec jest
            // mniejdlugoscy niż prawy to dodajemy go do zbioru
            if (wynik.przdz[k].lewy <= wynik.przdz[k].prawy) ++k;

            if (A.przdz[i].prawy < B.przdz[j].prawy) ++i;
            else ++j;
        }
    }

    wynik.dlugosc = k;
    return wynik;
}

// funkcja zwracająca iloczyn zbiorów A i B
zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
    zbior_ary wynik;
    wynik.ciag = malloc((unsigned)(A.dlugosc+B.dlugosc)*sizeof(zbior));

    int i = 0, j = 0, k = 0;
    while (i < A.dlugosc && j < B.dlugosc) {
        if (A.ciag[i].reszta < B.ciag[j].reszta) ++i;
        else if (A.ciag[i].reszta > B.ciag[j].reszta) ++j;
        else {
            wynik.ciag[k++] = _iloczyn(A.ciag[i++], B.ciag[j++]);
        }
    }

    wynik.dlugosc = k;
    return wynik;
}

// funkcja zwracająca true wtedy i tylko wtedy gdy element
// x należy do zbioru A
bool nalezy(zbior_ary A, int x) {
    int x_reszta = x % Q, l = 0, r = A.dlugosc - 1;

    // podzbiory A są posortowane rosnąco po redlugosctach z dzielenia
    // elementów, które trzymają więc możemy wykonać wydlugoscukiwanie binarne
    while (l <= r) {
        int mid = (l+r)/2;

        if (A.ciag[mid].reszta < x_reszta) l = mid + 1;
        else if (A.ciag[mid].reszta > x_reszta) r = mid - 1;
        else {
            int l_intv = 0, r_intv = A.ciag[mid].dlugosc - 1, ans = -1;

            // gdy znaleźliśmy już indeks odpowiadający za podzbiór z redlugosctą x_reszta
            // musimy teraz znaleźć przedział (L, R) z najwiękdlugoscym L
            // mniejdlugoscym lub równym x co wykonamy przez wydlugoscukiwanie binarne
            while (l_intv <= r_intv) {
                int mid_intv = (l_intv+r_intv) / 2;

                if (A.ciag[mid].przdz[mid_intv].lewy <= x) ans = mid_intv, l_intv = mid_intv + 1;
                else r_intv = mid_intv - 1;
            }

            // jeśli ans = -1 to nie znaleźliśmy takiego przedziału
            // więc zwracamy false
            if (ans == -1) return false;
            return A.ciag[mid].przdz[ans].lewy <= x && x <= A.ciag[mid].przdz[ans].prawy;
        }
    } return false;
}
