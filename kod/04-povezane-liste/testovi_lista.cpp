// Testovi klase Lista.
//
// Testovi se oslanjaju na assert, pa program ne treba prevoditi
// sa definisanim makroom NDEBUG.

#include "lista.h"

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <string>

template <typename T>
void ispisi(const Lista<T>& lista)
{
    std::cout << "[";
    for (Cvor<T>* p = lista.pocetak(); p != nullptr; p = p->sledeci) {
        if (p != lista.pocetak())
            std::cout << ", ";
        std::cout << p->podatak;
    }
    std::cout << "]  velicina = " << lista.velicina() << '\n';
}

// Proverava sadrzaj liste i, usput, invarijantu: posle tacno velicina()
// cvorova stize se do nullptr.
template <typename T>
bool sadrzi_redom(const Lista<T>& lista, std::initializer_list<T> ocekivano)
{
    if (lista.velicina() != ocekivano.size())
        return false;

    Cvor<T>* p = lista.pocetak();
    for (const T& x : ocekivano) {
        if (p == nullptr || p->podatak != x)
            return false;
        p = p->sledeci;
    }
    return p == nullptr;
}

void test_prazna_lista()
{
    Lista<int> l;

    assert(l.prazna());
    assert(l.velicina() == 0);
    assert(l.pocetak() == nullptr);
    assert(l.pronadji(5) == nullptr);
    assert(!l.obrisi(5));

    l.dodaj_na_pocetak(7);
    assert(!l.prazna());
    assert(sadrzi_redom(l, {7}));

    l.obrisi_sa_pocetka();
    assert(l.prazna());
}

void test_umetanja()
{
    Lista<int> l;
    l.dodaj_na_pocetak(3);
    l.dodaj_na_pocetak(2);
    l.dodaj_na_pocetak(1);                 // 1 2 3
    assert(sadrzi_redom(l, {1, 2, 3}));

    l.umetni_iza(l.pronadji(2), 25);        // 1 2 25 3
    assert(sadrzi_redom(l, {1, 2, 25, 3}));

    // Umetanje iza poslednjeg cvora.
    l.umetni_iza(l.pronadji(3), 4);         // 1 2 25 3 4
    assert(sadrzi_redom(l, {1, 2, 25, 3, 4}));

    // Umetanje ispred: posle poziva c pokazuje na novi element.
    Cvor<int>* c = l.pronadji(1);
    l.umetni_ispred(c, 0);                  // 0 1 2 25 3 4
    assert(sadrzi_redom(l, {0, 1, 2, 25, 3, 4}));
    assert(c->podatak == 0);

    std::cout << "Posle umetanja:\n";
    ispisi(l);
}

void test_brisanja()
{
    Lista<int> l;
    for (int x = 5; x >= 1; --x)
        l.dodaj_na_pocetak(x);              // 1 2 3 4 5

    l.obrisi_iza(l.pronadji(2));            // 1 2 4 5
    assert(sadrzi_redom(l, {1, 2, 4, 5}));

    l.obrisi_sa_pocetka();                  // 2 4 5
    assert(sadrzi_redom(l, {2, 4, 5}));

    assert(l.obrisi(5));                    // poslednji: 2 4
    assert(l.obrisi(2));                    // prvi: 4
    assert(!l.obrisi(42));                  // nema ga
    assert(sadrzi_redom(l, {4}));

    assert(l.obrisi(4));
    assert(l.prazna());

    // Lista posle praznjenja moze dalje da se koristi.
    l.dodaj_na_pocetak(1);
    l.dodaj_na_pocetak(1);
    l.isprazni();
    assert(l.prazna() && l.velicina() == 0);
    l.dodaj_na_pocetak(9);
    assert(sadrzi_redom(l, {9}));
}

void test_pretrage()
{
    Lista<int> l;
    l.dodaj_na_pocetak(8);
    l.dodaj_na_pocetak(15);
    l.dodaj_na_pocetak(8);
    l.dodaj_na_pocetak(4);                  // 4 8 15 8

    Cvor<int>* p = l.pronadji(8);
    assert(p != nullptr && p == l.pocetak()->sledeci);   // prvo pojavljivanje
    assert(l.pronadji(100) == nullptr);

    // Preko pronadjenog cvora podatak moze da se menja.
    p->podatak = 16;
    assert(sadrzi_redom(l, {4, 16, 15, 8}));
}

void test_sortirane_liste()
{
    Lista<int> l;
    for (int x : {5, 1, 9, 3, 7, 3})
        l.umetni_sortirano(x);
    assert(sadrzi_redom(l, {1, 3, 3, 5, 7, 9}));

    Lista<int> a;
    Lista<int> b;
    for (int x : {1, 4, 7})
        a.umetni_sortirano(x);
    for (int x : {2, 3, 8, 10})
        b.umetni_sortirano(x);

    a.ucesljaj(b);
    assert(sadrzi_redom(a, {1, 2, 3, 4, 7, 8, 10}));
    assert(b.prazna() && b.velicina() == 0);

    // Granicni slucajevi: prazna lista sa bilo koje strane.
    Lista<int> prazna;
    a.ucesljaj(prazna);
    assert(a.velicina() == 7);
    prazna.ucesljaj(a);
    assert(prazna.velicina() == 7 && a.prazna());

    std::cout << "Posle ucesljavanja:\n";
    ispisi(prazna);
}

void test_duge_liste()
{
    // Sve operacije su iterativne, pa ni veoma duga lista ne
    // prepunjava programski stek.
    const int n = 1000000;
    Lista<int> a;
    Lista<int> b;
    for (int i = n; i > 0; --i) {
        a.dodaj_na_pocetak(2 * i);
        b.dodaj_na_pocetak(2 * i + 1);
    }

    a.ucesljaj(b);
    assert(a.velicina() == 2 * static_cast<std::size_t>(n));

    int prethodni = 0;
    for (Cvor<int>* p = a.pocetak(); p != nullptr; p = p->sledeci) {
        assert(prethodni <= p->podatak);
        prethodni = p->podatak;
    }
}

void test_sa_drugim_tipom()
{
    Lista<std::string> reci;
    for (const char* r : {"liste", "nizovi", "stabla", "hipovi"})
        reci.umetni_sortirano(r);

    assert(sadrzi_redom<std::string>(reci, {"hipovi", "liste", "nizovi", "stabla"}));

    // Lista cuva sopstvenu kopiju podatka.
    std::string s = "grafovi";
    reci.dodaj_na_pocetak(s);
    s[0] = 'G';
    assert(reci.pocetak()->podatak == "grafovi");

    std::cout << "\nIsta klasa sa tipom std::string:\n";
    ispisi(reci);
}

int main()
{
    test_prazna_lista();
    test_umetanja();
    test_brisanja();
    test_pretrage();
    test_sortirane_liste();
    test_duge_liste();
    test_sa_drugim_tipom();

    std::cout << "\nSvi testovi su uspesno prosli.\n";
    return 0;
}
