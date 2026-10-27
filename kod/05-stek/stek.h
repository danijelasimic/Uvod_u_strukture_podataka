// Stek nad dinamickim nizom (odeljak "Stek nad dinamickim nizom").
// Vrh steka je poslednji element niza.
#ifndef STEK_H
#define STEK_H

#include <cassert>
#include <cstddef>

#include "dinamicki_niz.h"

template <typename T>
class Stek {
public:
    Stek() = default;

    Stek(const Stek&) = delete;
    Stek& operator=(const Stek&) = delete;

    void ubaci(const T& vrednost) { podaci.dodaj(vrednost); }

    void izbaci()
    {
        assert(!prazan());
        podaci.obrisi(podaci.velicina() - 1);
    }

    T& vrh()
    {
        assert(!prazan());
        return podaci[podaci.velicina() - 1];
    }

    const T& vrh() const
    {
        assert(!prazan());
        return podaci[podaci.velicina() - 1];
    }

    bool prazan() const { return podaci.velicina() == 0; }
    std::size_t velicina() const { return podaci.velicina(); }

private:
    DinamickiNiz<T> podaci;
};

#endif
