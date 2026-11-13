// Maks-hip nad vektorom (odeljci "Binarni hip", "Implementacija hipa" i
// "Formiranje hipa").
//
// Hip je niz posmatran po nivoima: element na poziciji i ima decu na
// pozicijama 2i+1 i 2i+2, a roditelja na poziciji (i-1)/2. Elementi se
// porede operatorom <, pa izraz a[i] < a[j] znaci da element na poziciji j
// ima visi prioritet.
//
// Invarijanta maks-hipa:
//   za svako i > 0 vazi !(a[(i-1)/2] < a[i]),
//   odnosno svaki element osim vrha nije veci od svog roditelja.
//
// Memoriju vodi std::vector, pa klasi ne trebaju ni destruktor ni
// zabrana kopiranja: kopija hipa je ispravan, nezavisan hip.
#ifndef HIP_H
#define HIP_H

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

// Spustanje elementa sa pozicije i u hipu koji cine prvih n elemenata
// vektora a. Pretpostavlja se da je narusen samo odnos elementa na poziciji
// i prema njegovoj deci. Parametar n ne sme biti veci od a.size(); hipsort
// ga smanjuje, pa se zato ne koristi a.size().
template <typename T>
void spusti(std::vector<T>& a, std::size_t i, std::size_t n)
{
    while (i < n / 2) {
        std::size_t dete = 2 * i + 1;

        if (dete + 1 < n && a[dete] < a[dete + 1])
            ++dete;

        if (!(a[i] < a[dete]))
            break;

        std::swap(a[i], a[dete]);
        i = dete;
    }
}

// Formiranje hipa odozdo navise, u vremenu O(n). Pozicije od a.size()/2 do
// kraja nemaju decu, pa se obradjuju samo pozicije a.size()/2 - 1, ..., 0.
template <typename T>
void formiraj_hip(std::vector<T>& a)
{
    std::size_t i = a.size() / 2;
    while (i > 0) {
        --i;
        spusti(a, i, a.size());
    }
}

template <typename T>
class Hip {
private:
    std::vector<T> elementi_;

    void podigni(std::size_t i);

public:
    Hip() = default;
    Hip(const std::vector<T>& elementi);

    bool prazan() const { return elementi_.empty(); }
    std::size_t velicina() const { return elementi_.size(); }

    const T& vrh() const
    {
        assert(!prazan());
        return elementi_.front();
    }

    void dodaj(const T& x);
    void ukloni_vrh();

    // Sluzi samo testovima: proverava invarijantu maks-hipa.
    bool invarijanta_vazi() const;
};

// Formiranje odozgo nanize, kao u knjizi: pre poziva podigni(i) elementi na
// pozicijama 0..i-1 cine hip, a posle poziva hip cine elementi 0..i.
// U najgorem slucaju (rastuci ulaz) traje O(n log n); formiraj_hip bi isti
// posao obavio u vremenu O(n).
template <typename T>
Hip<T>::Hip(const std::vector<T>& elementi)
    : elementi_(elementi)
{
    for (std::size_t i = 1; i < elementi_.size(); ++i)
        podigni(i);
}

template <typename T>
void Hip<T>::podigni(std::size_t i)
{
    while (i > 0) {
        std::size_t roditelj = (i - 1) / 2;
        if (!(elementi_[roditelj] < elementi_[i]))
            break;

        std::swap(elementi_[roditelj], elementi_[i]);
        i = roditelj;
    }
}

template <typename T>
void Hip<T>::dodaj(const T& x)
{
    elementi_.push_back(x);
    podigni(elementi_.size() - 1);
}

template <typename T>
void Hip<T>::ukloni_vrh()
{
    assert(!prazan());
    elementi_.front() = elementi_.back();
    elementi_.pop_back();
    spusti(elementi_, 0, elementi_.size());
}

template <typename T>
bool Hip<T>::invarijanta_vazi() const
{
    for (std::size_t i = 1; i < elementi_.size(); ++i)
        if (elementi_[(i - 1) / 2] < elementi_[i])
            return false;
    return true;
}

#endif
