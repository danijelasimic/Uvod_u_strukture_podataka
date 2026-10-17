// Osnovne operacije nad stekom, nad nizom i nad listom.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra primer_stek.cpp -o primer_stek
// Izlaz:        2 7 4
//               2 7 4
#include <iostream>

#include "stek.h"
#include "stek_lista.h"

template <typename S>
void ispisi_i_isprazni(S& stek)
{
    while (!stek.prazan()) {
        std::cout << stek.vrh() << ' ';
        stek.izbaci();
    }
    std::cout << '\n';
}

int main()
{
    Stek<int> niz;
    StekLista<int> lista;

    for (int x : {4, 7, 2}) {
        niz.ubaci(x);
        lista.ubaci(x);
    }

    ispisi_i_isprazni(niz);
    ispisi_i_isprazni(lista);

    return 0;
}
