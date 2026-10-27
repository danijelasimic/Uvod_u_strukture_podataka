// Niz operacija iz tabele u odeljku "Red i njegove osnovne operacije",
// izvrsen nad sve tri implementacije reda.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra primer_red.cpp -o primer_red
// Izlaz:        kruzni bafer:  prednji = 4, prednji = 7, preostalo: 7 2
//               lista:         prednji = 4, prednji = 7, preostalo: 7 2
//               dva steka:     prednji = 4, prednji = 7, preostalo: 7 2
#include <iostream>
#include <string>

#include "red.h"
#include "red_dva_steka.h"
#include "red_lista.h"

template <typename R>
void izvrsi(const std::string& naziv, R& red)
{
    std::cout << naziv;

    red.dodaj(4);
    red.dodaj(7);
    std::cout << "prednji = " << red.prednji() << ", ";
    red.dodaj(2);
    red.izbaci();
    std::cout << "prednji = " << red.prednji() << ", ";

    std::cout << "preostalo:";
    while (!red.prazan()) {
        std::cout << ' ' << red.prednji();
        red.izbaci();
    }
    std::cout << '\n';
}

int main()
{
    Red<int> bafer(4);
    RedLista<int> lista;
    RedDvaSteka<int> stekovi;

    izvrsi("kruzni bafer:  ", bafer);
    izvrsi("lista:         ", lista);
    izvrsi("dva steka:     ", stekovi);

    return 0;
}
