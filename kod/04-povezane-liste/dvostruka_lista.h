#ifndef DVOSTRUKA_LISTA_H
#define DVOSTRUKA_LISTA_H

#include <cassert>
#include <cstddef>

// Dvostruko povezana lista sa sentinel-cvorom iz glave 4.
//
// Sentinel-cvor stoji izmedju poslednjeg i prvog elementa, pa cvorovi
// cine krug. U praznoj listi sentinel pokazuje sam na sebe. Zahvaljujuci
// tome nijedan pokazivac nije nullptr i umetanje i brisanje nemaju
// posebnih slucajeva.
//
// Pored operacija iz knjige, klasa ima i nekoliko koje se iz njih lako
// izvode, kao i iterator (vidi odeljak o iteratorima u glavi i README).
//
// Tip T mora da podrzava kopiranje i podrazumevano konstruisanje (zbog
// polja podatak u sentinel-cvoru). Funkcija pronadji zahteva operator ==.
//
// Invarijanta:
//   Za svaki cvor c, ukljucujuci i sentinel, vazi
//   c->sledeci->prethodni == c i c->prethodni->sledeci == c.
//   Izmedju dva prolaska kroz sentinel nalazi se tacno velicina_ cvorova.

template <typename T>
class DvostrukaLista {
public:
    struct Cvor {
        T podatak;
        Cvor* prethodni;
        Cvor* sledeci;

        Cvor() : podatak(), prethodni(this), sledeci(this) {}
        Cvor(const T& p, Cvor* pre, Cvor* sl)
            : podatak(p), prethodni(pre), sledeci(sl) {}
    };

    class Iterator;

    DvostrukaLista() : sentinel(new Cvor()), velicina_(0) {}

    ~DvostrukaLista()
    {
        isprazni();
        delete sentinel;
    }

    DvostrukaLista(const DvostrukaLista&) = delete;
    DvostrukaLista& operator=(const DvostrukaLista&) = delete;

    bool prazna() const { return sentinel->sledeci == sentinel; }
    std::size_t velicina() const { return velicina_; }

    Cvor* prvi() const { return sentinel->sledeci; }
    Cvor* poslednji() const { return sentinel->prethodni; }
    Cvor* kraj() const { return sentinel; }

    void umetni_ispred(Cvor* c, const T& x);
    void obrisi(Cvor* c);

    void dodaj_na_pocetak(const T& x) { umetni_ispred(prvi(), x); }
    void dodaj_na_kraj(const T& x) { umetni_ispred(kraj(), x); }
    void umetni_iza(Cvor* c, const T& x) { umetni_ispred(c->sledeci, x); }

    void obrisi_sa_pocetka() { obrisi(prvi()); }
    void obrisi_sa_kraja() { obrisi(poslednji()); }
    void isprazni();

    Cvor* pronadji(const T& x) const;

    Iterator begin() const { return Iterator(prvi()); }
    Iterator end() const { return Iterator(kraj()); }

private:
    Cvor* sentinel;
    std::size_t velicina_;
};


// -----------------------------------------------------------------------------
// Iterator
//
// Iterator cuva pokazivac na cvor, ali ga ne otkriva korisniku: dozvoljava
// samo pristup podatku i prelazak na susedni cvor. Zahvaljujuci funkcijama
// begin() i end(), lista moze da se obilazi i petljom
//     for (int x : lista) ...
// Kao i kod std::list, end() je pozicija sentinel-cvora.
// -----------------------------------------------------------------------------

template <typename T>
class DvostrukaLista<T>::Iterator {
public:
    explicit Iterator(Cvor* c) : trenutni(c) {}

    T& operator*() const { return trenutni->podatak; }

    Iterator& operator++()
    {
        trenutni = trenutni->sledeci;
        return *this;
    }

    Iterator& operator--()
    {
        trenutni = trenutni->prethodni;
        return *this;
    }

    bool operator==(const Iterator& drugi) const { return trenutni == drugi.trenutni; }
    bool operator!=(const Iterator& drugi) const { return trenutni != drugi.trenutni; }

private:
    Cvor* trenutni;
};


// -----------------------------------------------------------------------------
// Definicije funkcija clanica
// -----------------------------------------------------------------------------

template <typename T>
void DvostrukaLista<T>::umetni_ispred(Cvor* c, const T& x)
{
    Cvor* novi = new Cvor(x, c->prethodni, c);
    c->prethodni->sledeci = novi;   // jos se koristi stara vrednost c->prethodni
    c->prethodni = novi;
    ++velicina_;
}

template <typename T>
void DvostrukaLista<T>::obrisi(Cvor* c)
{
    assert(c != sentinel);
    c->prethodni->sledeci = c->sledeci;
    c->sledeci->prethodni = c->prethodni;
    delete c;
    --velicina_;
}

template <typename T>
void DvostrukaLista<T>::isprazni()
{
    while (!prazna())
        obrisi(prvi());
}

// Cvor je tip definisan unutar sablona, pa se van klase njegovo ime pise
// kao typename DvostrukaLista<T>::Cvor. Rec typename kaze prevodiocu da je
// to ime tipa.
template <typename T>
typename DvostrukaLista<T>::Cvor* DvostrukaLista<T>::pronadji(const T& x) const
{
    for (Cvor* p = prvi(); p != kraj(); p = p->sledeci)
        if (p->podatak == x)
            return p;
    return nullptr;
}

#endif
