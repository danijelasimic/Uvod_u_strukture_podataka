// Jednostruko povezana lista iz glave 4 (kod/04-povezane-liste/), prepisana
// ovde da bi folder 05-stek bio samostalan.
#ifndef LISTA_H
#define LISTA_H

#include <cassert>
#include <cstddef>

template <typename T>
struct Cvor {
    T podatak;
    Cvor* sledeci;

    Cvor(const T& p, Cvor* s) : podatak(p), sledeci(s) {}
};

template <typename T>
class Lista {
public:
    Lista() : pocetak_(nullptr), velicina_(0) {}
    ~Lista();

    Lista(const Lista&) = delete;
    Lista& operator=(const Lista&) = delete;

    bool prazna() const { return pocetak_ == nullptr; }
    std::size_t velicina() const { return velicina_; }
    Cvor<T>* pocetak() const { return pocetak_; }

    void dodaj_na_pocetak(const T& x);
    void umetni_iza(Cvor<T>* c, const T& x);
    void umetni_ispred(Cvor<T>* c, const T& x);
    void umetni_sortirano(const T& x);

    void obrisi_sa_pocetka();
    void obrisi_iza(Cvor<T>* c);
    bool obrisi(const T& x);
    void isprazni();

    Cvor<T>* pronadji(const T& x) const;

private:
    Cvor<T>* pocetak_;
    std::size_t velicina_;
};

template <typename T>
void Lista<T>::dodaj_na_pocetak(const T& x)
{
    Cvor<T>* novi = new Cvor<T>(x, pocetak_);
    pocetak_ = novi;
    ++velicina_;
}

template <typename T>
void Lista<T>::umetni_iza(Cvor<T>* c, const T& x)
{
    Cvor<T>* novi = new Cvor<T>(x, c->sledeci);
    c->sledeci = novi;
    ++velicina_;
}

template <typename T>
void Lista<T>::umetni_ispred(Cvor<T>* c, const T& x)
{
    umetni_iza(c, c->podatak);
    c->podatak = x;
}

template <typename T>
void Lista<T>::umetni_sortirano(const T& x)
{
    if (prazna() || x <= pocetak_->podatak) {
        dodaj_na_pocetak(x);
        return;
    }

    Cvor<T>* prethodni = pocetak_;
    while (prethodni->sledeci != nullptr && prethodni->sledeci->podatak < x)
        prethodni = prethodni->sledeci;
    umetni_iza(prethodni, x);
}

template <typename T>
void Lista<T>::obrisi_sa_pocetka()
{
    assert(!prazna());
    Cvor<T>* stari = pocetak_;
    pocetak_ = stari->sledeci;
    delete stari;
    --velicina_;
}

template <typename T>
void Lista<T>::obrisi_iza(Cvor<T>* c)
{
    Cvor<T>* stari = c->sledeci;
    assert(stari != nullptr);
    c->sledeci = stari->sledeci;
    delete stari;
    --velicina_;
}

template <typename T>
bool Lista<T>::obrisi(const T& x)
{
    if (prazna())
        return false;
    if (pocetak_->podatak == x) {
        obrisi_sa_pocetka();
        return true;
    }

    Cvor<T>* prethodni = pocetak_;
    while (prethodni->sledeci != nullptr && prethodni->sledeci->podatak != x)
        prethodni = prethodni->sledeci;

    if (prethodni->sledeci == nullptr)
        return false;
    obrisi_iza(prethodni);
    return true;
}

template <typename T>
void Lista<T>::isprazni()
{
    while (!prazna())
        obrisi_sa_pocetka();
}

template <typename T>
Lista<T>::~Lista()
{
    isprazni();
}

template <typename T>
Cvor<T>* Lista<T>::pronadji(const T& x) const
{
    for (Cvor<T>* p = pocetak_; p != nullptr; p = p->sledeci)
        if (p->podatak == x)
            return p;
    return nullptr;
}

#endif
