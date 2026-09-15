#ifndef DINAMICKI_NIZ_H
#define DINAMICKI_NIZ_H

#include <cassert>
#include <cstddef>

// Dinamicki niz iz glave 3.
//
// Implementacija namerno koristi new[]/delete[] i sama upravlja kopiranjem
// (destruktor, konstruktor kopije, operator dodele), kao sto je opisano u
// glavi. U aplikativnom kodu umesto ove klase koristi se std::vector.
//
// Zbog nacina na koji se alocira memorija, tip T mora da podrzava:
//   1. podrazumevano konstruisanje (new T[n]);
//   2. operator dodele.
// Funkcija pronadji dodatno zahteva operator ==.
//
// Invarijanta:
//   0 <= velicina_ <= kapacitet_, elementi ukazuje na blok od kapacitet_
//   objekata tipa T, a pozicije 0, ..., velicina_ - 1 sadrze elemente niza.
//   Pored toga, kapacitet_ > 0, jer rast udvostrucava kapacitet.
template <typename T>
class DinamickiNiz {
private:
    T* elementi;
    std::size_t velicina_;
    std::size_t kapacitet_;

    void povecaj_kapacitet();

public:
    DinamickiNiz(std::size_t pocetni_kapacitet = 4);
    ~DinamickiNiz();

    DinamickiNiz(const DinamickiNiz& drugi);
    DinamickiNiz& operator=(const DinamickiNiz& drugi);

    std::size_t velicina() const;
    std::size_t kapacitet() const;
    bool prazan() const;

    T& operator[](std::size_t indeks);
    const T& operator[](std::size_t indeks) const;

    T& poslednji();
    const T& poslednji() const;

    void dodaj(const T& vrednost);
    void umetni(std::size_t indeks, const T& vrednost);
    void obrisi(std::size_t indeks);
    void obrisi_poslednji();

    std::size_t pronadji(const T& vrednost) const;
};


// -----------------------------------------------------------------------------
// Definicije funkcija clanica
//
// Definicije funkcija sablona moraju biti vidljive prevodiocu na mestu gde se
// sablon instancira, pa se nalaze u zaglavlju, a ne u posebnom .cpp fajlu.
// -----------------------------------------------------------------------------

template <typename T>
DinamickiNiz<T>::DinamickiNiz(std::size_t pocetni_kapacitet)
    : elementi(new T[pocetni_kapacitet]),
      velicina_(0),
      kapacitet_(pocetni_kapacitet)
{
    // Pri kapacitetu 0 udvostrucavanje bi dalo ponovo 0, pa bi dodaj
    // pisalo izvan bloka.
    assert(pocetni_kapacitet > 0);
}

template <typename T>
DinamickiNiz<T>::~DinamickiNiz()
{
    delete[] elementi;
}

template <typename T>
DinamickiNiz<T>::DinamickiNiz(const DinamickiNiz<T>& drugi)
    : elementi(new T[drugi.kapacitet_]),
      velicina_(drugi.velicina_),
      kapacitet_(drugi.kapacitet_)
{
    for (std::size_t i = 0; i < velicina_; ++i)
        elementi[i] = drugi.elementi[i];
}

template <typename T>
DinamickiNiz<T>& DinamickiNiz<T>::operator=(const DinamickiNiz<T>& drugi)
{
    if (this == &drugi)
        return *this;

    // Novi blok se obezbedjuje i popunjava pre oslobadjanja starog.
    T* novi = new T[drugi.kapacitet_];

    for (std::size_t i = 0; i < drugi.velicina_; ++i)
        novi[i] = drugi.elementi[i];

    delete[] elementi;

    elementi = novi;
    velicina_ = drugi.velicina_;
    kapacitet_ = drugi.kapacitet_;

    return *this;
}

template <typename T>
std::size_t DinamickiNiz<T>::velicina() const
{
    return velicina_;
}

template <typename T>
std::size_t DinamickiNiz<T>::kapacitet() const
{
    return kapacitet_;
}

template <typename T>
bool DinamickiNiz<T>::prazan() const
{
    return velicina_ == 0;
}

template <typename T>
T& DinamickiNiz<T>::operator[](std::size_t indeks)
{
    assert(indeks < velicina_);
    return elementi[indeks];
}

template <typename T>
const T& DinamickiNiz<T>::operator[](std::size_t indeks) const
{
    assert(indeks < velicina_);
    return elementi[indeks];
}

template <typename T>
T& DinamickiNiz<T>::poslednji()
{
    // Posle provere oduzimanje ne moze da ode ispod nule.
    assert(velicina_ > 0);
    return elementi[velicina_ - 1];
}

template <typename T>
const T& DinamickiNiz<T>::poslednji() const
{
    assert(velicina_ > 0);
    return elementi[velicina_ - 1];
}

template <typename T>
void DinamickiNiz<T>::povecaj_kapacitet()
{
    std::size_t novi_kapacitet = 2 * kapacitet_;

    T* novi = new T[novi_kapacitet];

    for (std::size_t i = 0; i < velicina_; ++i)
        novi[i] = elementi[i];

    delete[] elementi;

    elementi = novi;
    kapacitet_ = novi_kapacitet;
}

template <typename T>
void DinamickiNiz<T>::dodaj(const T& vrednost)
{
    if (velicina_ == kapacitet_)
        povecaj_kapacitet();

    elementi[velicina_] = vrednost;
    ++velicina_;
}

template <typename T>
void DinamickiNiz<T>::umetni(std::size_t indeks, const T& vrednost)
{
    assert(indeks <= velicina_);

    if (velicina_ == kapacitet_)
        povecaj_kapacitet();

    // Pomeranje ide zdesna nalevo, da se ne bi prepisali elementi
    // koji tek treba da budu pomereni.
    for (std::size_t i = velicina_; i > indeks; --i)
        elementi[i] = elementi[i - 1];

    elementi[indeks] = vrednost;
    ++velicina_;
}

template <typename T>
void DinamickiNiz<T>::obrisi(std::size_t indeks)
{
    assert(indeks < velicina_);

    for (std::size_t i = indeks; i + 1 < velicina_; ++i)
        elementi[i] = elementi[i + 1];

    --velicina_;
}

template <typename T>
void DinamickiNiz<T>::obrisi_poslednji()
{
    assert(velicina_ > 0);
    --velicina_;
}

// Vraca indeks prvog elementa jednakog vrednosti, odnosno velicina()
// ako takvog elementa nema (po uzoru na std::find, koji tada vraca end()).
template <typename T>
std::size_t DinamickiNiz<T>::pronadji(const T& vrednost) const
{
    for (std::size_t i = 0; i < velicina_; ++i)
        if (elementi[i] == vrednost)
            return i;

    return velicina_;
}

#endif
