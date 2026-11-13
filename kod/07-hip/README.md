# Glava 7 — Hip i red sa prioritetom

Kod koji prati glavu 7 knjige *Uvod u strukture podataka*.

| Fajl | Sadržaj |
|---|---|
| [`hip.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/07-hip/hip.h) | Šablon klase `Hip<T>` (maks-hip nad `std::vector`) i funkcije `spusti` i `formiraj_hip`. |
| [`hipsort.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/07-hip/hipsort.h) | Funkcija `hipsort`, kao u glavi. |
| [`primer_hip.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/07-hip/primer_hip.cpp) | Primeri iz glave: osnovne operacije, red poslova po hitnosti, formiranje hipa i min-hip pomoću `std::priority_queue`. |
| [`primer_hipsort.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/07-hip/primer_hipsort.cpp) | Hipsort korak po korak, sa granicom između hipa i sortiranog dela. |
| [`formiranje_hipa.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/07-hip/formiranje_hipa.cpp) | Eksperimentalna provera: broj poređenja pri formiranju hipa odozgo naniže i odozdo naviše. |
| [`najjeftiniji_put.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/07-hip/najjeftiniji_put.cpp) | Najjeftiniji put kroz lavirint (Dijkstrin algoritam), sa rekonstrukcijom puta preko prethodnika. |
| [`testovi_hip.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/07-hip/testovi_hip.cpp) | Testovi klase `Hip`, formiranja hipa i hipsorta, uključujući poređenje sa `std::priority_queue` i `std::sort`. |

Za razliku od foldera za glave 5 i 6, ovde nema prepisanih klasa iz ranijih
glava: hip čuva elemente u `std::vector`, a najjeftiniji put koristi
`std::priority_queue`, kao i u tekstu.

## Funkcija `spusti`

Spuštanje se koristi na tri mesta: u operaciji `ukloni_vrh`, pri formiranju
hipa i u hipsortu. U hipsortu se granica hipa pomera, pa funkcija mora da
dobije i broj elemenata nad kojima radi, a u formiranju hipa i hipsortu radi
nad običnim vektorom, a ne nad objektom klase `Hip`. Zato je `spusti` ovde
slobodna funkcija

```cpp
template <typename T>
void spusti(std::vector<T>& a, std::size_t i, std::size_t n);
```

i klasa je poziva kao `spusti(elementi_, 0, elementi_.size())`. Funkcija
`podigni` koristi se samo u klasi, pa je ostala privatna.

## Dva načina formiranja hipa

Konstruktor `Hip(const std::vector<T>&)` gradi hip odozgo naniže, kao u
glavi, uzastopnim podizanjem. Funkcija `formiraj_hip` gradi ga odozdo
naviše. Program `formiranje_hipa` meri razliku na rastućem nizu, koji je
najgori slučaj za podizanje:

```
         n   odozgo nanize    po el.   odozdo navise    po el.
      1000            7987      7.99            1982      1.98
   1000000        17951445     17.95         1999974      2.00
```

Broj poređenja po elementu odozgo naniže raste približno kao $\log_2 n$, a
odozdo naviše ostaje konstantan, što odgovara ocenama $O(n \log n)$ i $O(n)$
iz glave.

## Operacije koje nisu u knjizi

Klasa `Hip` ima sve operacije iz glave, kao i podrazumevani konstruktor.
Funkcija `invarijanta_vazi()` služi samo testovima i nije deo interfejsa
reda sa prioritetom.

Klasa ne alocira memoriju sama, pa nema ni destruktor ni zabranu kopiranja:
kopiju hipa pravi `std::vector`, i ona je ispravan, nezavisan hip.

## Prevođenje

Potreban je prevodilac koji podržava C++17, na primer:

```
g++ -std=c++17 -Wall -Wextra -pedantic testovi_hip.cpp -o testovi_hip
g++ -std=c++17 -Wall -Wextra -pedantic primer_hip.cpp -o primer_hip
g++ -std=c++17 -Wall -Wextra -pedantic primer_hipsort.cpp -o primer_hipsort
g++ -std=c++17 -O2 -Wall -Wextra -pedantic formiranje_hipa.cpp -o formiranje_hipa
g++ -std=c++17 -Wall -Wextra -pedantic najjeftiniji_put.cpp -o najjeftiniji_put
```

Program `najjeftiniji_put` čita mapu u obliku sličnom kao `lavirint` iz
glave 5 i `pretraga_u_sirinu` iz glave 6, samo što su umesto oznaka prepreka
zadate cene: $-1$ je prepreka, a nenegativan broj cena ulaska u polje.
Primer ulaza nalazi se u komentaru na početku fajla.

Testovi se oslanjaju na `assert`, pa se treba biti pažljiv ako se **prevodi
sa `-DNDEBUG`** — tada se sve provere uklanjaju i program „prolazi“ bez
ikakve provere.

## Vežbe

1. U funkciji `spusti` uklonite naredbu `++dete`, tako da se element uvek
   razmenjuje sa levim detetom. Koji testovi padaju? Nađite najmanji niz na
   kojem `formiraj_hip` daje niz koji nije hip.
2. U konstruktoru klase `Hip` zamenite petlju sa `podigni` pozivom
   `formiraj_hip(elementi_)` i ponovo pokrenite `formiranje_hipa`. Koja
   kolona se menja?
3. Dodajte klasi `Hip` drugi parametar šablona, kriterijum poređenja, po
   uzoru na `std::priority_queue`, tako da se `Hip<int, std::greater<int>>`
   ponaša kao min-hip.
4. U programu `najjeftiniji_put` brojte koliko unosa se izvadi iz reda, a
   koliko od njih je zastarelo. Uklonite zatim proveru zastarelih unosa.
   Da li je rezultat i dalje tačan? Šta se dešava sa brojem obrađenih polja?
5. Promenite poređenje u strukturi `Zapis` tako da se pri jednakoj ceni
   prednost daje polju bližem cilju. Menja li se cena puta? Menja li se
   sam put na primeru iz komentara?

## Zašto nema `.cpp` fajlova za klase

Sve klase i funkcije u ovom folderu su šabloni, pa se, kao i `DinamickiNiz`
iz glave 3, čitava implementacija nalazi u zaglavlju. Objašnjenje je u
`README.md` za glavu 3.
