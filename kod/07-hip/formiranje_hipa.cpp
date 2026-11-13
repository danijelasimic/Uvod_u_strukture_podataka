// Eksperimentalna provera analize iz odeljka "Formiranje hipa".
//
// Hip se formira od rastuceg niza 0, 1, ..., n-1, sto je najgori slucaj za
// formiranje odozgo nanize: svaki novi element podize se do vrha. Program
// broji poredjenja na dva nacina:
//   - odozgo nanize, konstruktorom klase Hip (uzastopno podizanje);
//   - odozdo navise, funkcijom formiraj_hip (spustanje od sredine ka pocetku).
// Poredjenja se broje pomocu tipa Brojac, ciji operator < uvecava globalni
// brojac.
//
// Prevodjenje:  g++ -std=c++17 -O2 -Wall -Wextra formiranje_hipa.cpp -o formiranje_hipa
//
// Izlaz:
//            n   odozgo nanize    po el.   odozdo navise    po el.
//         1000            7987      7.99            1982      1.98
//        10000          113631     11.36           19982      2.00
//       100000         1468946     14.69          199978      2.00
//      1000000        17951445     17.95         1999974      2.00
//
// Broj poredjenja po elementu odozgo nanize raste priblizno kao log2 n, a
// odozdo navise ostaje konstantan.
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

#include "hip.h"

std::size_t broj_poredjenja = 0;

struct Brojac {
    int vrednost;

    bool operator<(const Brojac& drugi) const
    {
        ++broj_poredjenja;
        return vrednost < drugi.vrednost;
    }
};

int main()
{
    std::cout << std::setw(10) << "n"
              << std::setw(16) << "odozgo nanize"
              << std::setw(10) << "po el."
              << std::setw(16) << "odozdo navise"
              << std::setw(10) << "po el." << '\n';

    for (std::size_t n = 1000; n <= 1000000; n *= 10) {
        std::vector<Brojac> a(n);
        for (std::size_t i = 0; i < n; ++i)
            a[i].vrednost = static_cast<int>(i);

        broj_poredjenja = 0;
        Hip<Brojac> hip(a);
        std::size_t odozgo = broj_poredjenja;

        broj_poredjenja = 0;
        formiraj_hip(a);
        std::size_t odozdo = broj_poredjenja;

        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(10) << n
                  << std::setw(16) << odozgo
                  << std::setw(10) << static_cast<double>(odozgo) / n
                  << std::setw(16) << odozdo
                  << std::setw(10) << static_cast<double>(odozdo) / n << '\n';
    }

    return 0;
}
