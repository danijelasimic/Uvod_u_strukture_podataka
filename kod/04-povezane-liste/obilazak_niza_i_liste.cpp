// Keš lokalnost u praksi (glava 4, odeljak o keš lokalnosti).
//
// Program sabira iste brojeve na tri nacina i meri vreme:
//   1. obilaskom std::vector (elementi su jedan do drugog u memoriji);
//   2. obilaskom liste ciji cvorovi leze redom u memoriji, onako kako su
//      alocirani;
//   3. obilaskom liste ciji su cvorovi povezani nasumicnim redom, kao u
//      listi posle mnogo umetanja i brisanja.
// Sva tri obilaska imaju slozenost O(n). Razlika u vremenu potice samo od
// toga koliko dobro obilazak koristi keš memoriju.
//
// Rezultat zavisi od racunara. Prevodite sa optimizacijom:
//     g++ -std=c++17 -O2 obilazak_niza_i_liste.cpp -o obilazak_niza_i_liste

#include "lista.h"   // zbog strukture Cvor

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

// Meri vreme izvrsavanja funkcije f u milisekundama.
template <typename F>
double izmeri(F f)
{
    auto pocetak = std::chrono::steady_clock::now();
    f();
    auto kraj = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(kraj - pocetak).count();
}

long long saberi_listu(Cvor<int>* pocetak)
{
    long long zbir = 0;
    for (Cvor<int>* p = pocetak; p != nullptr; p = p->sledeci)
        zbir += p->podatak;
    return zbir;
}

// Povezuje cvorove redom kojim su navedeni u nizu i vraca pocetak liste.
Cvor<int>* povezi(const std::vector<Cvor<int>*>& cvorovi)
{
    for (std::size_t i = 0; i + 1 < cvorovi.size(); ++i)
        cvorovi[i]->sledeci = cvorovi[i + 1];
    cvorovi.back()->sledeci = nullptr;
    return cvorovi.front();
}

int main()
{
    const int n = 10000000;

    std::vector<int> niz(n);
    std::vector<Cvor<int>*> cvorovi(n);
    for (int i = 0; i < n; ++i) {
        niz[i] = i % 100;
        cvorovi[i] = new Cvor<int>(i % 100, nullptr);
    }

    long long z1 = 0;
    long long z2 = 0;
    long long z3 = 0;

    double t1 = izmeri([&] {
        for (int x : niz)
            z1 += x;
    });

    Cvor<int>* lista = povezi(cvorovi);
    double t2 = izmeri([&] { z2 = saberi_listu(lista); });

    // Isti cvorovi, isti podaci, ali nasumican redosled povezivanja.
    // Mesanjem se ne menja zbir, jer se sabiraju isti brojevi.
    std::vector<Cvor<int>*> izmesani = cvorovi;
    std::mt19937 generator(2026);
    std::shuffle(izmesani.begin(), izmesani.end(), generator);
    lista = povezi(izmesani);
    double t3 = izmeri([&] { z3 = saberi_listu(lista); });

    std::cout << "Broj elemenata: " << n << "\n\n";
    std::cout << "std::vector:                " << t1 << " ms\n";
    std::cout << "lista, cvorovi redom:       " << t2 << " ms\n";
    std::cout << "lista, cvorovi nasumicno:   " << t3 << " ms\n";

    // Ispis zbirova sprecava prevodioca da izbaci petlje kao nepotrebne.
    std::cout << "\nZbirovi: " << z1 << ' ' << z2 << ' ' << z3 << '\n';

    for (Cvor<int>* c : cvorovi)
        delete c;
    return 0;
}
