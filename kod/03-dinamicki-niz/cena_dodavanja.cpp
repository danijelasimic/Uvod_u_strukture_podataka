// Eksperimentalna provera analize iz odeljka o ceni dodavanja.
//
// Program broji upise u memoriju (upise novih elemenata i prenose pri
// realokaciji) za dve strategije rasta:
//   - aritmeticku: kapacitet se uvecava za k mesta;
//   - geometrijsku: kapacitet se udvostrucava.
// Za geometrijsku strategiju broj se, pored simulacije, meri i na klasi
// DinamickiNiz, pomocu tipa koji broji koliko je puta pozvan njegov
// operator dodele.

#include "dinamicki_niz.h"

#include <cassert>
#include <initializer_list>
#include <iomanip>
#include <iostream>

// Broj poziva operatora dodele tipa Brojac.
std::size_t broj_dodela = 0;

struct Brojac {
    int vrednost;

    Brojac() : vrednost(0) {}
    Brojac(int v) : vrednost(v) {}
    Brojac(const Brojac&) = default;

    Brojac& operator=(const Brojac& drugi)
    {
        ++broj_dodela;
        vrednost = drugi.vrednost;
        return *this;
    }
};

// Broj upisa pri dodavanju n elemenata, kada kapacitet raste za k.
std::size_t upisi_aritmeticki(std::size_t n, std::size_t k)
{
    std::size_t velicina = 0;
    std::size_t kapacitet = k;
    std::size_t upisi = 0;

    for (std::size_t i = 0; i < n; ++i) {
        if (velicina == kapacitet) {
            upisi += velicina;       // prenos postojecih elemenata
            kapacitet += k;
        }
        upisi += 1;                  // upis novog elementa
        ++velicina;
    }
    return upisi;
}

// Broj upisa pri dodavanju n elemenata, kada se kapacitet udvostrucava.
std::size_t upisi_geometrijski(std::size_t n, std::size_t pocetni_kapacitet)
{
    std::size_t velicina = 0;
    std::size_t kapacitet = pocetni_kapacitet;
    std::size_t upisi = 0;

    for (std::size_t i = 0; i < n; ++i) {
        if (velicina == kapacitet) {
            upisi += velicina;
            kapacitet *= 2;
        }
        upisi += 1;
        ++velicina;
    }
    return upisi;
}

// Broj upisa izmeren na klasi DinamickiNiz.
std::size_t upisi_izmereno(std::size_t n)
{
    broj_dodela = 0;

    DinamickiNiz<Brojac> niz(4);
    for (std::size_t i = 0; i < n; ++i)
        niz.dodaj(Brojac(static_cast<int>(i)));

    return broj_dodela;
}

int main()
{
    const std::size_t k = 100;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(10) << "n"
              << std::setw(16) << "aritm. (k=100)"
              << std::setw(12) << "po elem."
              << std::setw(14) << "geometrijski"
              << std::setw(12) << "po elem." << '\n';

    for (std::size_t n = 1000; n <= 1000000; n *= 10) {
        std::size_t a = upisi_aritmeticki(n, k);
        std::size_t g = upisi_geometrijski(n, 4);

        // Simulacija i klasa moraju da daju isti broj upisa.
        assert(g == upisi_izmereno(n));

        std::cout << std::setw(10) << n
                  << std::setw(16) << a
                  << std::setw(12) << static_cast<double>(a) / static_cast<double>(n)
                  << std::setw(14) << g
                  << std::setw(12) << static_cast<double>(g) / static_cast<double>(n) << '\n';
    }

    // Broj upisa po elementu kod geometrijske strategije zavisi od toga
    // koliko je blok popunjen: najmanji je kada je niz upravo popunjen,
    // a najveci neposredno posle realokacije.
    std::cout << "\nGeometrijska strategija, oko granice kapaciteta 1024:\n";
    for (std::size_t n : {std::size_t(1024), std::size_t(1025)}) {
        std::size_t g = upisi_geometrijski(n, 4);
        std::cout << "  n = " << std::setw(4) << n << ": " << g
                  << " upisa, " << static_cast<double>(g) / static_cast<double>(n)
                  << " po elementu\n";
    }

    return 0;
}
