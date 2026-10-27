// Obrnuti ispis liste: rekurzijom i eksplicitnim stekom.
//
// Program ucitava broj elemenata n i izbor postupka (0 -- eksplicitni stek,
// 1 -- rekurzija), pravi listu 1, 2, ..., n i ispisuje je od kraja ka pocetku.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra obrnuti_ispis.cpp -o obrnuti_ispis
//
// Primeri:      echo "5 0" | ./obrnuti_ispis              ispisuje 5 4 3 2 1
//               echo "1000000 0" | ./obrnuti_ispis > /dev/null    radi
//               echo "1000000 1" | ./obrnuti_ispis > /dev/null    prekoracenje steka
//
// Pri velikom n izlaz preusmeriti u datoteku ili na /dev/null.
#include <cstddef>
#include <iostream>

#include "lista.h"
#include "stek.h"

void ispisi_obrnuto_rekurzivno(Cvor<int>* p)
{
    if (p == nullptr)
        return;
    ispisi_obrnuto_rekurzivno(p->sledeci);
    std::cout << p->podatak << ' ';
}

void ispisi_obrnuto(const Lista<int>& lista)
{
    Stek<Cvor<int>*> stek;
    for (Cvor<int>* p = lista.pocetak(); p != nullptr; p = p->sledeci)
        stek.ubaci(p);

    while (!stek.prazan()) {
        std::cout << stek.vrh()->podatak << ' ';
        stek.izbaci();
    }
}

int main()
{
    std::size_t n;
    int izbor;
    if (!(std::cin >> n >> izbor) || (izbor != 0 && izbor != 1)) {
        std::cerr << "Ocekuje se: broj elemenata i izbor (0 ili 1).\n";
        return 1;
    }

    Lista<int> lista;
    for (std::size_t i = n; i >= 1; --i)
        lista.dodaj_na_pocetak(static_cast<int>(i));

    if (izbor == 1)
        ispisi_obrnuto_rekurzivno(lista.pocetak());
    else
        ispisi_obrnuto(lista);
    std::cout << '\n';

    return 0;
}
