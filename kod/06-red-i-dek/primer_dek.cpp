// Primer upotrebe klase Dek: dodavanje i uklanjanje na oba kraja, pristup
// po indeksu, stabilnost adresa elemenata i provera da li je rec palindrom.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra primer_dek.cpp -o primer_dek
// Izlaz:        dek: -2 -1 0 1 2 3
//               posle uklanjanja sa oba kraja: -1 0 1 2
//               adresa elementa 1 ista posle 10000 dodavanja: da
//               ana: palindrom
//               dek: nije palindrom
//               anavolimilovana: palindrom
#include <iostream>
#include <string>

#include "dek.h"

template <typename T, std::size_t S>
void ispisi(const Dek<T, S>& dek)
{
    for (std::size_t i = 0; i < dek.velicina(); ++i)
        std::cout << ' ' << dek[i];
    std::cout << '\n';
}

// Uporedjuje i uklanja krajnje znakove dok u deku ne ostane najvise jedan.
bool palindrom(const std::string& rec)
{
    Dek<char> znakovi;
    for (char c : rec)
        znakovi.dodaj_na_kraj(c);

    while (znakovi.velicina() > 1) {
        if (znakovi.prednji() != znakovi.zadnji())
            return false;
        znakovi.izbaci_sa_pocetka();
        znakovi.izbaci_sa_kraja();
    }
    return true;
}

int main()
{
    // Mali segmenti, kao na slikama u glavi, da bi se vise puta prelazilo
    // iz segmenta u segment.
    Dek<int, 4> dek;
    for (int x = 0; x <= 3; ++x)
        dek.dodaj_na_kraj(x);
    dek.dodaj_na_pocetak(-1);
    dek.dodaj_na_pocetak(-2);

    std::cout << "dek:";
    ispisi(dek);

    dek.izbaci_sa_pocetka();
    dek.izbaci_sa_kraja();
    std::cout << "posle uklanjanja sa oba kraja:";
    ispisi(dek);

    // Rast deka premesta samo pokazivace na segmente, a ne elemente.
    int* adresa = &dek[2];
    for (int k = 0; k < 5000; ++k) {
        dek.dodaj_na_kraj(k);
        dek.dodaj_na_pocetak(-k);
    }
    std::cout << "adresa elementa " << *adresa
              << " ista posle 10000 dodavanja: "
              << (adresa == &dek[5002] ? "da" : "ne") << '\n';

    for (const std::string rec : {"ana", "dek", "anavolimilovana"})
        std::cout << rec << ": " << (palindrom(rec) ? "palindrom" : "nije palindrom")
                  << '\n';

    return 0;
}
