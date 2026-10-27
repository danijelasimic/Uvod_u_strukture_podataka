// Red pomocu dva steka (odeljak "Red pomocu dva steka").
//
// Novi elementi stavljaju se na ulazni stek, a uklanjaju se sa izlaznog.
// Kada je izlazni stek prazan, na njega se prebacuju svi elementi ulaznog
// steka; prebacivanje obrce redosled, pa najranije dodat element dolazi na
// vrh. Svaki element se prebacuje najvise jednom, pa je amortizovana
// slozenost svake operacije O(1).
//
// Klasa ne zauzima memoriju sama, pa joj nije potreban destruktor: o
// elementima brinu stekovi.
//
// Invarijanta:
//   Ako je izlazni stek <x_0, ..., x_{p-1}>, a ulazni <y_0, ..., y_{q-1}>
//   (oba od dna ka vrhu), red ima p + q elemenata i njegovo stanje je
//   <x_{p-1}, ..., x_0, y_0, ..., y_{q-1}>.
#ifndef RED_DVA_STEKA_H
#define RED_DVA_STEKA_H

#include <cassert>
#include <cstddef>

#include "stek.h"

template <typename T>
class RedDvaSteka {
public:
    RedDvaSteka() = default;

    RedDvaSteka(const RedDvaSteka&) = delete;
    RedDvaSteka& operator=(const RedDvaSteka&) = delete;

    void dodaj(const T& vrednost) { ulazni.ubaci(vrednost); }

    void izbaci()
    {
        assert(!prazan());
        pripremi();
        izlazni.izbaci();
    }

    // Nije const: priprema moze da prebaci elemente sa steka na stek.
    T& prednji()
    {
        assert(!prazan());
        pripremi();
        return izlazni.vrh();
    }

    bool prazan() const { return ulazni.prazan() && izlazni.prazan(); }

    std::size_t velicina() const
    {
        return ulazni.velicina() + izlazni.velicina();
    }

private:
    // Prebacuje elemente samo kada je izlazni stek prazan.
    void pripremi()
    {
        if (izlazni.prazan()) {
            while (!ulazni.prazan()) {
                izlazni.ubaci(ulazni.vrh());
                ulazni.izbaci();
            }
        }
    }

    Stek<T> ulazni;
    Stek<T> izlazni;
};

#endif
