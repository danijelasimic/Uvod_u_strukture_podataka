// Testovi klase DvostrukaLista.
//
// Testovi se oslanjaju na assert, pa program ne treba prevoditi
// sa definisanim makroom NDEBUG.

#include "dvostruka_lista.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
void ispisi(const DvostrukaLista<T>& lista)
{
    std::cout << "[";
    for (auto p = lista.prvi(); p != lista.kraj(); p = p->sledeci) {
        if (p != lista.prvi())
            std::cout << ", ";
        std::cout << p->podatak;
    }
    std::cout << "]  velicina = " << lista.velicina() << '\n';
}

// Proverava sadrzaj u oba smera i, usput, invarijantu: korak napred pa
// korak nazad vraca u isti cvor.
template <typename T>
bool sadrzi_redom(const DvostrukaLista<T>& lista, const std::vector<T>& ocekivano)
{
    if (lista.velicina() != ocekivano.size())
        return false;

    auto p = lista.prvi();
    for (std::size_t i = 0; i < ocekivano.size(); ++i) {
        if (p == lista.kraj() || p->podatak != ocekivano[i])
            return false;
        if (p->sledeci->prethodni != p || p->prethodni->sledeci != p)
            return false;
        p = p->sledeci;
    }
    if (p != lista.kraj())
        return false;

    p = lista.poslednji();
    for (std::size_t i = ocekivano.size(); i > 0; --i) {
        if (p->podatak != ocekivano[i - 1])
            return false;
        p = p->prethodni;
    }
    return p == lista.kraj();
}

void test_prazna_lista()
{
    DvostrukaLista<int> l;

    assert(l.prazna());
    assert(l.velicina() == 0);

    // U praznoj listi sentinel pokazuje sam na sebe.
    assert(l.prvi() == l.kraj());
    assert(l.poslednji() == l.kraj());
    assert(l.pronadji(5) == nullptr);
    assert(l.begin() == l.end());
}

void test_umetanja()
{
    DvostrukaLista<int> l;
    l.dodaj_na_kraj(2);
    l.dodaj_na_kraj(3);
    l.dodaj_na_pocetak(1);                       // 1 2 3
    assert(sadrzi_redom(l, {1, 2, 3}));

    l.umetni_ispred(l.pronadji(3), 25);           // 1 2 25 3
    l.umetni_iza(l.pronadji(3), 4);               // 1 2 25 3 4
    l.umetni_ispred(l.prvi(), 0);                 // 0 1 2 25 3 4
    assert(sadrzi_redom(l, {0, 1, 2, 25, 3, 4}));

    // Umetanje ispred sentinel-cvora je dodavanje na kraj.
    l.umetni_ispred(l.kraj(), 5);
    assert(l.poslednji()->podatak == 5);

    std::cout << "Posle umetanja:\n";
    ispisi(l);
}

void test_brisanja()
{
    DvostrukaLista<int> l;
    for (int x = 1; x <= 5; ++x)
        l.dodaj_na_kraj(x);                      // 1 2 3 4 5

    // Za brisanje je dovoljan pokazivac na sam cvor.
    l.obrisi(l.pronadji(3));                     // 1 2 4 5
    assert(sadrzi_redom(l, {1, 2, 4, 5}));

    l.obrisi_sa_pocetka();                       // 2 4 5
    l.obrisi_sa_kraja();                         // 2 4
    assert(sadrzi_redom(l, {2, 4}));

    l.obrisi(l.prvi());
    l.obrisi(l.prvi());
    assert(l.prazna() && l.prvi() == l.kraj());

    l.dodaj_na_kraj(9);
    l.dodaj_na_kraj(8);
    l.isprazni();
    assert(l.prazna() && l.velicina() == 0);
}

void test_obilaska_unazad()
{
    DvostrukaLista<int> l;
    for (int x = 1; x <= 4; ++x)
        l.dodaj_na_kraj(x);

    int ocekivano = 4;
    for (auto p = l.poslednji(); p != l.kraj(); p = p->prethodni) {
        assert(p->podatak == ocekivano);
        --ocekivano;
    }
    assert(ocekivano == 0);
}

void test_iteratora()
{
    DvostrukaLista<int> l;
    for (int x = 1; x <= 4; ++x)
        l.dodaj_na_kraj(x);

    int zbir = 0;
    for (int x : l)
        zbir += x;
    assert(zbir == 10);

    // Preko iteratora podatak moze da se menja.
    for (auto it = l.begin(); it != l.end(); ++it)
        *it *= 10;
    assert(sadrzi_redom(l, {10, 20, 30, 40}));

    // --end() je poslednji element, kao kod std::list.
    auto it = l.end();
    --it;
    assert(*it == 40);
    --it;
    assert(*it == 30);
}

void test_sa_drugim_tipom()
{
    DvostrukaLista<std::string> reci;
    reci.dodaj_na_kraj("stek");
    reci.dodaj_na_kraj("red");
    reci.dodaj_na_pocetak("lista");

    assert(sadrzi_redom<std::string>(reci, {"lista", "stek", "red"}));

    std::cout << "\nIsta klasa sa tipom std::string:\n";
    ispisi(reci);
}

int main()
{
    test_prazna_lista();
    test_umetanja();
    test_brisanja();
    test_obilaska_unazad();
    test_iteratora();
    test_sa_drugim_tipom();

    std::cout << "\nSvi testovi su uspesno prosli.\n";
    return 0;
}
