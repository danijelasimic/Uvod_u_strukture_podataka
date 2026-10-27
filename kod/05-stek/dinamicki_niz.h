// Dinamicki niz iz glave 3 (kod/03-dinamicki-niz/), prepisan ovde da bi
// folder 05-stek bio samostalan.
#ifndef DINAMICKI_NIZ_H
#define DINAMICKI_NIZ_H

#include <cassert>
#include <cstddef>

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

    T& operator[](std::size_t indeks);
    const T& operator[](std::size_t indeks) const;

    void dodaj(const T& vrednost);
    void umetni(std::size_t indeks, const T& vrednost);
    void obrisi(std::size_t indeks);
};

template <typename T>
DinamickiNiz<T>::DinamickiNiz(std::size_t pocetni_kapacitet)
    : elementi(new T[pocetni_kapacitet]),
      velicina_(0),
      kapacitet_(pocetni_kapacitet)
{
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

#endif
