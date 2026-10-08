// Otkrivanje ciklusa u jednostruko povezanoj listi -- Flojdov algoritam
// (glava 4).
//
// Lista sa ciklusom ne postoji u ispravnom programu: ona krsi invarijantu
// klase Lista, pa bi destruktor takve liste radio beskonacno. Zato su u
// ovom primeru cvorovi napravljeni i povezani rucno, bez klase Lista, i
// rucno se oslobadjaju.

#include "lista.h"   // zbog strukture Cvor

#include <iostream>

// Vraca pokazivac na prvi cvor ciklusa, odnosno nullptr ako ciklusa nema.
// Slozenost je O(n), a dodatna memorija O(1).
template <typename T>
Cvor<T>* pocetak_ciklusa(Cvor<T>* pocetak)
{
    Cvor<T>* spori = pocetak;
    Cvor<T>* brzi = pocetak;

    while (brzi != nullptr && brzi->sledeci != nullptr) {
        spori = spori->sledeci;
        brzi = brzi->sledeci->sledeci;

        if (spori == brzi) {
            // Susret je u ciklusu. Pokazivac vracen na pocetak i pokazivac
            // iz tacke susreta, pomerani po jedan cvor, sretnu se u
            // prvom cvoru ciklusa.
            spori = pocetak;
            while (spori != brzi) {
                spori = spori->sledeci;
                brzi = brzi->sledeci;
            }
            return spori;
        }
    }
    return nullptr;
}

// Pravi listu od n cvorova sa podacima 0, 1, ..., n - 1. Ako je
// pocetak_kruga >= 0, poslednji cvor pokazuje na cvor sa tim rednim
// brojem, pa lista dobija ciklus. Cvorovi se pamte u nizu cvorovi, da bi
// mogli da se oslobode.
Cvor<int>* napravi(int n, int pocetak_kruga, Cvor<int>* cvorovi[])
{
    for (int i = n - 1; i >= 0; --i)
        cvorovi[i] = new Cvor<int>(i, i + 1 < n ? cvorovi[i + 1] : nullptr);

    if (pocetak_kruga >= 0)
        cvorovi[n - 1]->sledeci = cvorovi[pocetak_kruga];

    return cvorovi[0];
}

void oslobodi(int n, Cvor<int>* cvorovi[])
{
    for (int i = 0; i < n; ++i)
        delete cvorovi[i];
}

// n sme biti najvise 100.
void proveri(int n, int pocetak_kruga)
{
    Cvor<int>* cvorovi[100];
    Cvor<int>* lista = napravi(n, pocetak_kruga, cvorovi);

    std::cout << n << " cvorova, ";
    if (pocetak_kruga < 0)
        std::cout << "bez ciklusa: ";
    else
        std::cout << "poslednji pokazuje na cvor " << pocetak_kruga << ": ";

    Cvor<int>* c = pocetak_ciklusa(lista);
    if (c == nullptr)
        std::cout << "ciklus nije pronadjen\n";
    else
        std::cout << "ciklus pocinje u cvoru " << c->podatak << '\n';

    oslobodi(n, cvorovi);
}

int main()
{
    proveri(8, -1);    // obicna lista
    proveri(8, 3);     // primer sa slike u knjizi: a = 3, b = 5
    proveri(8, 0);     // cela lista je krug
    proveri(8, 7);     // poslednji cvor pokazuje sam na sebe
    proveri(1, -1);
    proveri(1, 0);
    return 0;
}
