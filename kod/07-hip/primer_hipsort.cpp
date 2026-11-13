// Hipsort korak po korak, na primeru iz odeljka "Hipsort". Posle svake
// iteracije ispisuje se niz, a uspravna crta odvaja hip od sortiranog dela.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra primer_hipsort.cpp -o primer_hipsort
// Izlaz:
//   pocetni niz:            [7 2 8 9 4 6 5 1 3]
//   posle formiranja hipa:  [9 7 8 3 4 6 5 1 2]
//   [8 7 6 3 4 2 5 1 | 9]
//   [7 4 6 3 1 2 5 | 8 9]
//   ...
//   [1 | 2 3 4 5 6 7 8 9]
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

#include "hip.h"

void ispisi(const std::vector<int>& a, std::size_t k)
{
    std::cout << '[';
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (i > 0)
            std::cout << ' ';
        if (i == k)
            std::cout << "| ";
        std::cout << a[i];
    }
    std::cout << "]\n";
}

int main()
{
    std::vector<int> a = {7, 2, 8, 9, 4, 6, 5, 1, 3};
    std::cout << "pocetni niz:            ";
    ispisi(a, a.size());

    // Isti koraci kao u funkciji hipsort iz hipsort.h, uz ispis stanja.
    formiraj_hip(a);
    std::cout << "posle formiranja hipa:  ";
    ispisi(a, a.size());

    std::size_t k = a.size();
    while (k > 1) {
        --k;
        std::swap(a[0], a[k]);
        spusti(a, 0, k);
        ispisi(a, k);
    }

    return 0;
}
