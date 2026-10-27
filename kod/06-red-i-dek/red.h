// Red nad kruznim baferom fiksnog kapaciteta (odeljak "Implementacija
// kruznim baferom").
//
// Memorija za niz zauzima se u konstruktoru i ne prosiruje se: dodavanje u
// pun red nije dozvoljeno. Kopiranje je zabranjeno; duboko kopiranje
// prikazano je u glavi 3, na primeru dinamickog niza.
//
// Izraz new T[kapacitet] pravi sve elemente unapred, pa tip T mora imati
// podrazumevani konstruktor i operator dodele.
//
// Invarijanta:
//   kapacitet > 0, 0 <= pocetak < kapacitet, 0 <= broj <= kapacitet.
//   Element sa logickom pozicijom i u redu (0 <= i < broj) nalazi se u nizu
//   na poziciji (pocetak + i) % kapacitet.
#ifndef RED_H
#define RED_H

#include <cassert>
#include <cstddef>

template <typename T>
class Red {
public:
    Red(std::size_t pocetni_kapacitet)
        : elementi(new T[pocetni_kapacitet]),
          kapacitet(pocetni_kapacitet)
    {
        assert(pocetni_kapacitet > 0);
    }

    ~Red() { delete[] elementi; }

    Red(const Red&) = delete;
    Red& operator=(const Red&) = delete;

    void dodaj(const T& vrednost)
    {
        assert(broj < kapacitet);
        const std::size_t kraj = (pocetak + broj) % kapacitet;
        elementi[kraj] = vrednost;
        ++broj;
    }

    // Stara vrednost ostaje u nizu, ali vise ne pripada redu.
    void izbaci()
    {
        assert(broj > 0);
        pocetak = (pocetak + 1) % kapacitet;
        --broj;
    }

    T& prednji()
    {
        assert(broj > 0);
        return elementi[pocetak];
    }

    const T& prednji() const
    {
        assert(broj > 0);
        return elementi[pocetak];
    }

    bool prazan() const { return broj == 0; }
    bool pun() const { return broj == kapacitet; }
    std::size_t velicina() const { return broj; }

private:
    T* elementi;
    std::size_t pocetak = 0;
    std::size_t broj = 0;
    std::size_t kapacitet;
};

#endif
