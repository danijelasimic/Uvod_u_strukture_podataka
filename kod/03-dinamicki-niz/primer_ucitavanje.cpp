// Primer iz uvoda glave: program ucitava cele brojeve dok ne naidje na
// kraj ulaza, a da unapred ne zna koliko ce ih biti.
//
// Primer pokretanja:
//     ./primer_ucitavanje < brojevi.txt
// ili unos sa tastature, uz Ctrl+D (Linux, macOS) odnosno Ctrl+Z i Enter
// (Windows) na kraju.

#include "dinamicki_niz.h"

#include <iostream>

int main()
{
    DinamickiNiz<int> brojevi;

    int x;
    while (std::cin >> x)
        brojevi.dodaj(x);

    std::cout << "Broj ucitanih elemenata: " << brojevi.velicina()
              << " (kapacitet: " << brojevi.kapacitet() << ")\n";

    if (brojevi.prazan())
        return 0;

    std::cout << "Brojevi u obrnutom redosledu:";
    for (std::size_t i = brojevi.velicina(); i > 0; --i)
        std::cout << ' ' << brojevi[i - 1];
    std::cout << '\n';

    return 0;
}
