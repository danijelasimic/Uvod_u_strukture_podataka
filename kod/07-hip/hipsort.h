// Hipsort (odeljak "Hipsort").
//
// Isti niz cuva i hip i sortirani rezultat: prvih k elemenata cine
// maks-hip, a elementi od pozicije k do kraja vec su na svojim konacnim
// mestima. U svakom koraku vrh hipa razmenjuje se sa poslednjim elementom
// hipa, hip se smanjuje za jedan, a novi vrh se spusta.
//
// Invarijanta petlje (na pocetku svake iteracije):
//   - a[0..k-1] je maks-hip;
//   - a[k..n-1] je sortiran neopadajuce;
//   - svaki element hipa nije veci ni od jednog elementa sortiranog dela.
//
// Vreme O(n log n) u najgorem slucaju, dodatni prostor O(1). Nije stabilan.
#ifndef HIPSORT_H
#define HIPSORT_H

#include <cstddef>
#include <utility>
#include <vector>

#include "hip.h"

template <typename T>
void hipsort(std::vector<T>& a)
{
    formiraj_hip(a);

    std::size_t k = a.size();
    while (k > 1) {
        --k;
        std::swap(a[0], a[k]);
        spusti(a, 0, k);
    }
}

#endif
