#ifndef LISTA_H
#define LISTA_H

#include <cassert>
#include <cstddef>

// Jednostruko povezana lista iz glave 4.
//
// Lista je vlasnik svojih cvorova: alocira ih sa new, a oslobadja ih
// destruktor. Kopiranje je zabranjeno; duboko kopiranje prikazano je samo
// u glavi 3, na primeru dinamickog niza.
//
// Tip T mora da podrzava kopiranje. Funkcije pronadji i obrisi(x) dodatno
// zahtevaju operatore == i !=, a umetni_sortirano i ucesljaj operatore
// < i <=.
//
// Invarijanta:
//   Ako se krene od pocetak_ i prate pokazivaci sledeci, posle tacno
//   velicina_ cvorova stize se do nullptr. Svaki cvor na tom putu alocirala
//   je lista i nijedan se ne pojavljuje dva puta.

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

    void ucesljaj(Lista& druga);

private:
    Cvor<T>* pocetak_;
    std::size_t velicina_;
};


// -----------------------------------------------------------------------------
// Definicije funkcija clanica
// -----------------------------------------------------------------------------

template <typename T>
Lista<T>::~Lista()
{
    isprazni();
}

template <typename T>
void Lista<T>::dodaj_na_pocetak(const T& x)
{
    Cvor<T>* novi = new Cvor<T>(x, pocetak_);
    pocetak_ = novi;
    ++velicina_;
}

// c mora da pokazuje na cvor ove liste.
template <typename T>
void Lista<T>::umetni_iza(Cvor<T>* c, const T& x)
{
    assert(c != nullptr);
    Cvor<T>* novi = new Cvor<T>(x, c->sledeci);
    c->sledeci = novi;
    ++velicina_;
}

// Novi cvor se umece iza c, a podaci se rasporede tako da redosled bude
// kao da je umetnut ispred c. Posle poziva c pokazuje na novi element.
template <typename T>
void Lista<T>::umetni_ispred(Cvor<T>* c, const T& x)
{
    assert(c != nullptr);
    umetni_iza(c, c->podatak);
    c->podatak = x;
}

// Ako je lista bila sortirana, ostaje sortirana.
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
    pocetak_ = stari->sledeci;   // pre delete, dok je stari jos ispravan
    delete stari;
    --velicina_;
}

template <typename T>
void Lista<T>::obrisi_iza(Cvor<T>* c)
{
    assert(c != nullptr);
    Cvor<T>* stari = c->sledeci;
    assert(stari != nullptr);
    c->sledeci = stari->sledeci;
    delete stari;
    --velicina_;
}

// Brise prvo pojavljivanje x. Vraca false ako x nije u listi.
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
Cvor<T>* Lista<T>::pronadji(const T& x) const
{
    for (Cvor<T>* p = pocetak_; p != nullptr; p = p->sledeci)
        if (p->podatak == x)
            return p;
    return nullptr;
}

// Ucesljava dve sortirane liste. Posle poziva ova lista sadrzi sve
// elemente, a druga je prazna. Cvorovi se ne kopiraju, samo se drugacije
// povezuju.
template <typename T>
void Lista<T>::ucesljaj(Lista& druga)
{
    Cvor<T>* a = pocetak_;
    Cvor<T>* b = druga.pocetak_;
    Cvor<T>* poslednji = nullptr;   // poslednji cvor rezultata

    while (a != nullptr && b != nullptr) {
        Cvor<T>* manji;
        if (a->podatak <= b->podatak) {
            manji = a;
            a = a->sledeci;
        } else {
            manji = b;
            b = b->sledeci;
        }

        // Prvi cvor rezultata nema prethodnika, pa se menja pocetak_.
        if (poslednji == nullptr)
            pocetak_ = manji;
        else
            poslednji->sledeci = manji;
        poslednji = manji;
    }

    Cvor<T>* ostatak = (a != nullptr) ? a : b;
    if (poslednji == nullptr)
        pocetak_ = ostatak;
    else
        poslednji->sledeci = ostatak;

    // Svaki cvor sme da pripada samo jednoj listi, inace bi ga oba
    // destruktora oslobodila.
    velicina_ += druga.velicina_;
    druga.pocetak_ = nullptr;
    druga.velicina_ = 0;
}

#endif
