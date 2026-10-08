# Glava 4 — Povezane liste

Kod koji prati glavu 4 knjige *Uvod u strukture podataka*.

| Fajl | Sadržaj |
|---|---|
| [`lista.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/lista.h) | Struktura `Cvor<T>` i šablon klase `Lista<T>` — jednostruko povezana lista iz glave, sa učešljavanjem. |
| [`dvostruka_lista.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/dvostruka_lista.h) | Šablon klase `DvostrukaLista<T>` — dvostruko povezana lista sa sentinel-čvorom, dopunjena operacijama i iteratorom. |
| [`testovi_lista.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/testovi_lista.cpp) | Testovi svih operacija klase `Lista`, uključujući granične slučajeve i listu od dva miliona elemenata. |
| [`testovi_dvostruka_lista.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/testovi_dvostruka_lista.cpp) | Testovi klase `DvostrukaLista`; svaki test proverava i invarijantu. |
| [`plesaci.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/plesaci.cpp) | Zadatak o plesačima u krugu, rešen kružnom listom. |
| [`ciklus.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/ciklus.cpp) | Flojdov algoritam za otkrivanje ciklusa, sa primerima lista sa ciklusom i bez njega. |
| [`linijski_editor.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/linijski_editor.cpp) | Linijski editor iz glave, napisan nad `std::list` i nad klasom `DvostrukaLista`, radi poređenja. |
| [`obilazak_niza_i_liste.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/04-povezane-liste/obilazak_niza_i_liste.cpp) | Merenje: obilazak niza i liste iste dužine, kao ilustracija keš lokalnosti. |

## Operacije koje nisu u knjizi

Klasa `Lista` ima sve operacije iz glave, a pored njih i funkciju
`ucesljaj`, čija je ideja u glavi opisana, a kod izostavljen:

| Funkcija | Značenje | Složenost | `std::forward_list` |
|---|---|---|---|
| `ucesljaj(druga)` | učešljavanje dve sortirane liste; `druga` ostaje prazna | $O(n + m)$ | `merge(druga)` |

Klasa `DvostrukaLista` u glavi ima samo umetanje i brisanje. Ostale operacije
izvode se iz njih:

| Funkcija | Značenje | Složenost | `std::list` |
|---|---|---|---|
| `umetni_iza(c, x)` | umetanje iza čvora `c` | $O(1)$ | — |
| `obrisi_sa_pocetka()` | brisanje prvog elementa | $O(1)$ | `pop_front()` |
| `obrisi_sa_kraja()` | brisanje poslednjeg elementa | $O(1)$ | `pop_back()` |
| `isprazni()` | brisanje svih elemenata | $O(n)$ | `clear()` |
| `pronadji(x)` | pokazivač na prvi čvor sa podatkom `x`, odnosno `nullptr` ako ga nema | $O(n)$ | `std::find` |
| `begin()`, `end()` | iteratori za obilazak, kao u odeljku o iteratorima | $O(1)$ | `begin()`, `end()` |

Iterator je ugnežđena klasa `DvostrukaLista<T>::Iterator`. Čuva pokazivač na
čvor i nudi samo operatore `*`, `++`, `--`, `==` i `!=`, pa korisnik preko
njega ne može da pokvari vezu među čvorovima. Pozicija `end()` je
sentinel-čvor, kao kod `std::list`. Zahvaljujući funkcijama `begin()` i
`end()`, lista se može obilaziti i petljom `for (int x : lista)`.

## Prevođenje

Potreban je prevodilac koji podržava C++17, na primer:

```
g++ -std=c++17 -Wall -Wextra -pedantic testovi_lista.cpp -o testovi_lista
g++ -std=c++17 -Wall -Wextra -pedantic testovi_dvostruka_lista.cpp -o testovi_dvostruka_lista
g++ -std=c++17 -Wall -Wextra -pedantic plesaci.cpp -o plesaci
g++ -std=c++17 -Wall -Wextra -pedantic ciklus.cpp -o ciklus
g++ -std=c++17 -Wall -Wextra -pedantic linijski_editor.cpp -o linijski_editor
g++ -std=c++17 -O2 obilazak_niza_i_liste.cpp -o obilazak_niza_i_liste
```

Merenje u `obilazak_niza_i_liste.cpp` ima smisla samo uz optimizaciju
(`-O2`). Vremena zavise od računara, ali je odnos svuda sličan: obilazak
niza je najbrži, obilazak liste čiji čvorovi leže redom u memoriji primetno
sporiji, a obilazak liste sa nasumično raspoređenim čvorovima sporiji za red
veličine ili više.

Testovi se oslanjaju na `assert`, pa se treba biti pažljiv ako se **prevodi
sa `-DNDEBUG`** — tada se sve provere uklanjaju i program „prolazi“ bez
ikakve provere.

Greške u radu sa memorijom mogu se otkriti prevođenjem sa sanitizerom (GCC i
Clang):

```
g++ -std=c++17 -g -fsanitize=address,undefined testovi_lista.cpp -o testovi_lista
```

## Vežbe

1. U funkciji `obrisi_sa_pocetka` (fajl `lista.h`) zamenite redosled naredbi
   tako da se `delete stari;` izvrši pre `pocetak_ = stari->sledeci;`.
   Program se često i dalje ponaša ispravno, jer oslobođena memorija još
   nije prepisana. Prevedite testove sa `-fsanitize=address`: sanitizer
   prijavljuje čitanje oslobođene memorije, o kome je reč u glavi 2.
2. U funkciji `ucesljaj` izbrišite red `druga.pocetak_ = nullptr;`. Tada
   obe liste smatraju da su vlasnici istih čvorova. Prevedite testove sa
   `-DNDEBUG -fsanitize=address` i pogledajte šta sanitizer prijavljuje pri
   uništavanju lista.
3. U funkciji `umetni_ispred` (fajl `dvostruka_lista.h`) zamenite redosled
   poslednje dve dodele pokazivača. Testovi padaju na proveri invarijante:
   nacrtajte na papiru šta se desilo sa vezama.
4. Napišite funkciju `obrni()` za klasu `Lista`, koja obrće redosled
   elemenata u vremenu $O(n)$, bez alokacije novih čvorova.

## Zašto nema `.cpp` fajlova za klase

`Lista` i `DvostrukaLista` su šabloni klasa, pa se, kao i `DinamickiNiz` iz
glave 3, čitava implementacija nalazi u zaglavlju. Objašnjenje je u
`README.md` za glavu 3.
