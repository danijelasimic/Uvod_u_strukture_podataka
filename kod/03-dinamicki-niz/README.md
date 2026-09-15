# Glava 3 — Dinamički niz

Kod koji prati glavu 3 knjige *Uvod u strukture podataka*.

| Fajl | Sadržaj |
|---|---|
| `dinamicki_niz.h` | Šablon klase `DinamickiNiz<T>` — kompletna implementacija iz glave. |
| `testovi.cpp` | Testovi svih operacija, uključujući ivične slučajeve i kopiranje. |
| `primer_ucitavanje.cpp` | Primer iz uvoda glave: učitavanje nepoznatog broja podataka. |
| `cena_dodavanja.cpp` | Eksperimentalna provera amortizovane analize: broj upisa pri aritmetičkoj i geometrijskoj strategiji rasta. |
| `rast_vektora.cpp` | Ispisuje kojim činiocem raste kapacitet `std::vector` u vašoj standardnoj biblioteci. |

## Operacije klase

Pored operacija opisanih u glavi (`velicina`, `kapacitet`, `operator[]`,
`dodaj`, `umetni`, `obrisi`, konstruktor kopije i operator dodele), klasa ima
i nekoliko pomoćnih funkcija koje se u tekstu ne navode, ali se lako izvode iz
njega:

| Funkcija | Značenje | Složenost | `std::vector` |
|---|---|---|---|
| `prazan()` | da li je niz bez elemenata | $O(1)$ | `empty()` |
| `poslednji()` | referenca na poslednji element | $O(1)$ | `back()` |
| `obrisi_poslednji()` | uklanjanje sa kraja | $O(1)$ | `pop_back()` |
| `pronadji(x)` | indeks prvog elementa jednakog `x`, odnosno `velicina()` ako ga nema | $O(n)$ | `std::find` |

## Prevođenje

Potreban je prevodilac koji podržava C++17, na primer:

```
g++ -std=c++17 -Wall -Wextra -pedantic testovi.cpp -o testovi
g++ -std=c++17 -Wall -Wextra -pedantic primer_ucitavanje.cpp -o primer_ucitavanje
g++ -std=c++17 -Wall -Wextra -pedantic cena_dodavanja.cpp -o cena_dodavanja
g++ -std=c++17 -Wall -Wextra -pedantic rast_vektora.cpp -o rast_vektora
```

Testovi se oslanjaju na `assert`, pa se **ne smeju prevoditi sa `-DNDEBUG`** —
tada se sve provere uklanjaju i program „prolazi“ bez ikakve provere.

Greške u radu sa memorijom (curenje, pristup oslobođenoj memoriji, dvostruko
oslobađanje) mogu se otkriti prevođenjem sa sanitizerom (GCC i Clang):

```
g++ -std=c++17 -g -fsanitize=address,undefined testovi.cpp -o testovi
```

Dobra vežba: iz `dinamicki_niz.h` uklonite konstruktor kopije i operator
dodele. Prevodilac tada sam generiše plitko kopiranje, pa `test_kopiranja`
pada već na proveri nezavisnosti kopije i originala. Ako se program prevede
sa `-DNDEBUG -fsanitize=address`, provere nestaju i sanitizer prijavljuje i
drugu posledicu plitkog kopiranja opisanu u glavi: dvostruko oslobađanje
istog bloka.

## Zašto nema `dinamicki_niz.cpp`

`DinamickiNiz` je šablon klase. Prevodilac pravi klasu `DinamickiNiz<int>`
tek na mestu gde se ona upotrebi, i za to mora da vidi definicije svih
funkcija članica. Zato se čitava implementacija šablona nalazi u zaglavlju.
Kada bi se definicije premestile u poseban `.cpp` fajl, program bi se preveo,
ali bi povezivanje prijavilo nedefinisane funkcije.
