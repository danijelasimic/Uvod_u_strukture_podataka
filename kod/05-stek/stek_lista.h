// Stek nad jednostruko povezanom listom (odeljak "Stek nad povezanom listom").
// Vrh steka je prvi cvor liste, jer se samo na tom kraju umetanje i brisanje
// obavljaju u vremenu O(1). Klasa se zove StekLista da bi mogla da postoji
// u istom programu kao i Stek iz stek.h.
#ifndef STEK_LISTA_H
#define STEK_LISTA_H

#include <cassert>
#include <cstddef>

#include "lista.h"

template <typename T>
class StekLista {
public:
    StekLista() = default;

    StekLista(const StekLista&) = delete;
    StekLista& operator=(const StekLista&) = delete;

    void ubaci(const T& vrednost) { podaci.dodaj_na_pocetak(vrednost); }

    void izbaci()
    {
        assert(!prazan());
        podaci.obrisi_sa_pocetka();
    }

    T& vrh()
    {
        assert(!prazan());
        return podaci.pocetak()->podatak;
    }

    const T& vrh() const
    {
        assert(!prazan());
        return podaci.pocetak()->podatak;
    }

    bool prazan() const { return podaci.prazna(); }
    std::size_t velicina() const { return podaci.velicina(); }

private:
    Lista<T> podaci;
};

#endif
