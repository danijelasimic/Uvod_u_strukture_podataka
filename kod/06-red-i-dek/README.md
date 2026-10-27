# Glava 6 — Red i dek

Kod koji prati glavu 6 knjige *Uvod u strukture podataka*.

| Fajl | Sadržaj |
|---|---|
| [`red.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/red.h) | Šablon klase `Red<T>` — red nad kružnim baferom fiksnog kapaciteta, kao u glavi. |
| [`red_lista.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/red_lista.h) | Šablon klase `RedLista<T>` — red nad jednostruko povezanom listom, sa pokazivačima na prvi i poslednji čvor. |
| [`red_dva_steka.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/red_dva_steka.h) | Šablon klase `RedDvaSteka<T>` — red pomoću ulaznog i izlaznog steka. |
| [`dek.h`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/dek.h) | Šablon klase `Dek<T, S>` — dek kao niz pokazivača na segmente, sa svim operacijama na oba kraja. |
| [`primer_red.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/primer_red.cpp) | Niz operacija iz tabele u glavi, izvršen nad sve tri implementacije reda. |
| [`pretraga_u_sirinu.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/pretraga_u_sirinu.cpp) | Pretraga u širinu na mapi iz glave 5, sa rekonstrukcijom najkraćeg puta preko prethodnika. |
| [`primer_dek.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/primer_dek.cpp) | Primer upotrebe deka: operacije na oba kraja, pristup po indeksu, stabilnost adresa, provera palindroma. |
| [`testovi_red.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/testovi_red.cpp) | Testovi sve tri implementacije reda, uključujući kružno omotavanje i poređenje sa `std::deque`. |
| [`testovi_dek.cpp`](https://github.com/danijelasimic/Uvod_u_strukture_podataka/blob/main/kod/06-red-i-dek/testovi_dek.cpp) | Testovi klase `Dek`; posle svake promene proverava se invarijanta. |

Fajlovi `dinamicki_niz.h`, `stek.h` i `lista.h` prepisani su iz glava 3, 5 i
4, da bi folder bio samostalan. `RedDvaSteka` koristi klasu `Stek`, a
`RedLista` samo strukturu `Cvor<T>`.

## Imena klasa

U glavi se i red nad kružnim baferom i red nad listom zovu `Red`, jer se
nikada ne pojavljuju zajedno. Ovde su u istom folderu, pa red nad listom
nosi ime `RedLista`, po uzoru na `StekLista` iz glave 5.

## Operacije koje nisu u knjizi

Klasa `Red` ima operacije iz glave i, pored njih:

| Funkcija | Značenje | Složenost |
|---|---|---|
| `pun()` | da li je dostignut kapacitet, pa dodavanje nije dozvoljeno | $O(1)$ |
| `prazan()`, `velicina()` | provera praznog reda i broj elemenata | $O(1)$ |

U glavi su za `RedLista` prikazane samo operacije `dodaj` i `izbaci`. Ostale
(`prednji`, `prazan`, `velicina` i destruktor) urađene su onako kako ih tekst
opisuje.

Klasa `Dek` u glavi ima kompletnu deklaraciju, pristup po indeksu,
konstruktor, destruktor, dodavanje na kraj i preuređivanje niza pokazivača.
Ovde su dopisane i operacije koje su u tekstu samo opisane:

| Funkcija | Značenje | Složenost | `std::deque` |
|---|---|---|---|
| `dodaj_na_pocetak(x)` | simetrično dodavanju na kraj; novi segment počinje intervalom $[S, S)$ | $O(1)$ amortizovano | `push_front(x)` |
| `izbaci_sa_pocetka()` | prazan prvi segment se oslobađa | $O(1)$ | `pop_front()` |
| `izbaci_sa_kraja()` | prazan poslednji segment se oslobađa | $O(1)$ | `pop_back()` |
| `const` verzije `operator[]`, `prednji`, `zadnji` | čitanje iz konstantnog deka | $O(1)$ | iste |

Dve funkcije služe samo testovima i nisu deo interfejsa deka:
`invarijanta_vazi()` proverava invarijantu iz glave, a
`mesta_u_nizu_pokazivaca()` vraća kapacitet niza pokazivača, pa se može
proveriti da niz ne raste kada dek „putuje“.

Kapacitet segmenta `S` mora biti bar 2: prvi element praznog deka upisuje se
na mesto $S/2$, a sa jednim mestom u segmentu dodavanje na početak ne bi
imalo gde da počne.

## Prevođenje

Potreban je prevodilac koji podržava C++17, na primer:

```
g++ -std=c++17 -Wall -Wextra -pedantic testovi_red.cpp -o testovi_red
g++ -std=c++17 -Wall -Wextra -pedantic testovi_dek.cpp -o testovi_dek
g++ -std=c++17 -Wall -Wextra -pedantic primer_red.cpp -o primer_red
g++ -std=c++17 -Wall -Wextra -pedantic primer_dek.cpp -o primer_dek
g++ -std=c++17 -Wall -Wextra -pedantic pretraga_u_sirinu.cpp -o pretraga_u_sirinu
```

Program `pretraga_u_sirinu` čita mapu u istom obliku kao `lavirint` iz
glave 5, pa se dva programa mogu pokrenuti nad istim ulazom. Na mapi
$3 \times 3$ bez prepreka pretraga u širinu nalazi put od 4 koraka, a
pretraga u dubinu prolazi kroz svih 9 polja.

Testovi se oslanjaju na `assert`, pa se treba biti pažljiv ako se **prevodi
sa `-DNDEBUG`** — tada se sve provere uklanjaju i program „prolazi“ bez
ikakve provere.

Greške u radu sa memorijom mogu se otkriti prevođenjem sa sanitizerom (GCC i
Clang):

```
g++ -std=c++17 -g -fsanitize=address,undefined testovi_dek.cpp -o testovi_dek
```

## Vežbe

1. U funkciji `izbaci` klase `RedLista` (fajl `red_lista.h`) izbrišite
   naredbu kojom se `poslednji` postavlja na `nullptr`. Testovi i dalje
   prolaze, jer `dodaj` prazan red prepoznaje po pokazivaču `prvi`.
   Zamenite zatim u funkciji `dodaj` uslov `prvi == nullptr` uslovom
   `poslednji == nullptr` i prevedite testove sa `-fsanitize=address`:
   posle uklanjanja jedinog elementa `poslednji` je viseći pokazivač, a
   sledeće dodavanje upisuje u oslobođenu memoriju. Greška je, dakle,
   postojala i ranije, samo nije bila vidljiva.
2. U funkciji `pripremi` klase `RedDvaSteka` uklonite proveru
   `if (izlazni.prazan())`, tako da se prebacivanje obavlja uvek. Testovi
   padaju već na osnovnim operacijama: novi elementi dospevaju iznad
   starijih i preteknu ih.
3. Proširite klasu `Red` tako da pri dodavanju u pun red udvostruči
   kapacitet. Pazite na redosled: elementi se u novi niz prepisuju
   redosledom kojim su u redu, od pozicije `pocetak`, a ne redosledom
   fizičkih pozicija.
4. U destruktoru klase `Dek` zamenite redosled: najpre `delete[] segmenti_`,
   pa petlja. Šta prijavljuje sanitizer?
5. U klasi `RedDvaSteka` brojite operacije nad stekovima i proverite da
   posle $m$ operacija nad redom, koji je na početku prazan, njihov broj
   nije veći od $4m$.

## Zašto nema `.cpp` fajlova za klase

Sve klase u ovom folderu su šabloni, pa se, kao i `DinamickiNiz` iz glave 3,
čitava implementacija nalazi u zaglavlju. Objašnjenje je u `README.md` za
glavu 3.
