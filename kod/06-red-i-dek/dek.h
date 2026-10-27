// Dek kao niz pokazivaca na segmente (odeljak "Osnovna ideja implementacije
// deka").
//
// Elementi se cuvaju u segmentima, nizovima fiksne duzine S, koji se
// alociraju zasebno. Niz pokazivaca segmenti_ cuva samo adrese segmenata.
// Kada niz pokazivaca treba prosiriti ili ponovo centrirati, prepisuju se
// samo pokazivaci, pa elementi ostaju na istim adresama: reference i
// pokazivaci na elemente ostaju ispravni posle dodavanja na bilo koji kraj.
//
// Niz T elementi[S] pravi svih S elemenata pri alokaciji segmenta, pa tip T
// mora imati podrazumevani konstruktor i operator dodele. Kopiranje deka je
// zabranjeno; duboko kopiranje prikazano je u glavi 3.
//
// Invarijanta:
//   U svakom segmentu 0 <= pocetak <= kraj <= S.
//   Ako dek nije prazan, segmenti postoje tacno na mestima prvi_seg_, ...,
//   poslednji_seg_ niza pokazivaca, svaki sadrzi bar jedan element, a ostala
//   mesta sadrze nullptr. Ako segmenata ima vise, prvi ima kraj == S,
//   poslednji pocetak == 0, a svi izmedju njih su puni.
//   velicina_ je ukupan broj elemenata.
//   Prazan dek nema nijedan segment i prvi_seg_ == poslednji_seg_ ==
//   kapacitet_ / 2.
#ifndef DEK_H
#define DEK_H

#include <cassert>
#include <cstddef>

template <typename T, std::size_t S = 32>
class Dek {
    // Prvi element praznog deka upisuje se na sredinu segmenta, na mesto
    // S / 2, sa slobodnim mestima na obe strane.
    static_assert(S >= 2, "segment mora imati bar dva mesta");

public:
    Dek();
    ~Dek();

    Dek(const Dek&) = delete;
    Dek& operator=(const Dek&) = delete;

    void dodaj_na_pocetak(const T& x);
    void dodaj_na_kraj(const T& x);
    void izbaci_sa_pocetka();
    void izbaci_sa_kraja();

    T& operator[](std::size_t i);
    const T& operator[](std::size_t i) const;

    T& prednji() { return (*this)[0]; }
    const T& prednji() const { return (*this)[0]; }
    T& zadnji() { return (*this)[velicina_ - 1]; }
    const T& zadnji() const { return (*this)[velicina_ - 1]; }

    bool prazan() const { return velicina_ == 0; }
    std::size_t velicina() const { return velicina_; }

    // Pomocne funkcije za testove; nisu deo interfejsa deka.
    std::size_t mesta_u_nizu_pokazivaca() const { return kapacitet_; }
    bool invarijanta_vazi() const;

private:
    struct Segment {
        T elementi[S];
        std::size_t pocetak;  // prvo zauzeto mesto
        std::size_t kraj;     // mesto iza poslednjeg zauzetog

        Segment(std::size_t p) : pocetak(p), kraj(p) {}
    };

    Segment** segmenti_;
    std::size_t kapacitet_;
    std::size_t prvi_seg_;
    std::size_t poslednji_seg_;
    std::size_t velicina_;

    void preuredi_pokazivace();
};

template <typename T, std::size_t S>
Dek<T, S>::Dek()
    : segmenti_(new Segment*[8]()),
      kapacitet_(8), prvi_seg_(4),
      poslednji_seg_(4), velicina_(0)
{
}

template <typename T, std::size_t S>
Dek<T, S>::~Dek()
{
    // Prvo segmenti, pa niz pokazivaca: obrnutim redom bismo izgubili
    // adrese segmenata. delete nad nullptr nema efekta.
    for (std::size_t j = 0; j < kapacitet_; ++j)
        delete segmenti_[j];
    delete[] segmenti_;
}

template <typename T, std::size_t S>
T& Dek<T, S>::operator[](std::size_t i)
{
    assert(i < velicina_);
    std::size_t pomeraj = segmenti_[prvi_seg_]->pocetak + i;
    Segment* segment = segmenti_[prvi_seg_ + pomeraj / S];
    return segment->elementi[pomeraj % S];
}

template <typename T, std::size_t S>
const T& Dek<T, S>::operator[](std::size_t i) const
{
    assert(i < velicina_);
    std::size_t pomeraj = segmenti_[prvi_seg_]->pocetak + i;
    const Segment* segment = segmenti_[prvi_seg_ + pomeraj / S];
    return segment->elementi[pomeraj % S];
}

template <typename T, std::size_t S>
void Dek<T, S>::dodaj_na_kraj(const T& x)
{
    if (prazan()) {
        segmenti_[poslednji_seg_] = new Segment(S / 2);
    } else if (segmenti_[poslednji_seg_]->kraj == S) {
        // poslednji segment je pun, potreban je novi
        if (poslednji_seg_ == kapacitet_ - 1)
            preuredi_pokazivace();
        ++poslednji_seg_;
        segmenti_[poslednji_seg_] = new Segment(0);
    }

    Segment* poslednji = segmenti_[poslednji_seg_];
    poslednji->elementi[poslednji->kraj] = x;
    ++poslednji->kraj;
    ++velicina_;
}

template <typename T, std::size_t S>
void Dek<T, S>::dodaj_na_pocetak(const T& x)
{
    if (prazan()) {
        segmenti_[prvi_seg_] = new Segment(S / 2);
    } else if (segmenti_[prvi_seg_]->pocetak == 0) {
        // prvi segment nema mesta ispred prvog elementa, potreban je novi
        if (prvi_seg_ == 0)
            preuredi_pokazivace();
        --prvi_seg_;
        segmenti_[prvi_seg_] = new Segment(S);
    }

    Segment* prvi = segmenti_[prvi_seg_];
    --prvi->pocetak;
    prvi->elementi[prvi->pocetak] = x;
    ++velicina_;
}

template <typename T, std::size_t S>
void Dek<T, S>::izbaci_sa_kraja()
{
    assert(!prazan());

    Segment* poslednji = segmenti_[poslednji_seg_];
    --poslednji->kraj;
    --velicina_;

    if (poslednji->pocetak == poslednji->kraj) {
        delete poslednji;
        segmenti_[poslednji_seg_] = nullptr;
        if (prazan())
            prvi_seg_ = poslednji_seg_ = kapacitet_ / 2;
        else
            --poslednji_seg_;
    }
}

template <typename T, std::size_t S>
void Dek<T, S>::izbaci_sa_pocetka()
{
    assert(!prazan());

    Segment* prvi = segmenti_[prvi_seg_];
    ++prvi->pocetak;
    --velicina_;

    if (prvi->pocetak == prvi->kraj) {
        delete prvi;
        segmenti_[prvi_seg_] = nullptr;
        if (prazan())
            prvi_seg_ = poslednji_seg_ = kapacitet_ / 2;
        else
            ++prvi_seg_;
    }
}

// Pravi novi niz pokazivaca i u njega, oko sredine, prepisuje pokazivace
// na postojece segmente. Niz se udvostrucava samo ako su segmenti zauzimali
// vise od polovine starog niza; inace se zauzeti deo samo ponovo centrira.
// Posle preuredjivanja sa obe strane zauzetog dela ostaje slobodnih mesta.
template <typename T, std::size_t S>
void Dek<T, S>::preuredi_pokazivace()
{
    std::size_t broj = poslednji_seg_ - prvi_seg_ + 1;
    std::size_t novi_kapacitet = kapacitet_;
    if (broj > kapacitet_ / 2)
        novi_kapacitet = 2 * kapacitet_;

    Segment** nova = new Segment*[novi_kapacitet]();
    std::size_t novi_prvi = (novi_kapacitet - broj) / 2;
    for (std::size_t j = 0; j < broj; ++j)
        nova[novi_prvi + j] = segmenti_[prvi_seg_ + j];

    delete[] segmenti_;  // oslobadja samo stari niz pokazivaca
    segmenti_ = nova;
    kapacitet_ = novi_kapacitet;
    prvi_seg_ = novi_prvi;
    poslednji_seg_ = novi_prvi + broj - 1;
}

template <typename T, std::size_t S>
bool Dek<T, S>::invarijanta_vazi() const
{
    if (prazan()) {
        if (prvi_seg_ != kapacitet_ / 2 || poslednji_seg_ != kapacitet_ / 2)
            return false;
        for (std::size_t j = 0; j < kapacitet_; ++j)
            if (segmenti_[j] != nullptr)
                return false;
        return true;
    }

    if (prvi_seg_ > poslednji_seg_ || poslednji_seg_ >= kapacitet_)
        return false;

    std::size_t ukupno = 0;
    for (std::size_t j = 0; j < kapacitet_; ++j) {
        const Segment* segment = segmenti_[j];
        bool zauzeto = prvi_seg_ <= j && j <= poslednji_seg_;
        if (!zauzeto) {
            if (segment != nullptr)
                return false;
            continue;
        }

        if (segment == nullptr)
            return false;
        if (segment->pocetak >= segment->kraj || segment->kraj > S)
            return false;
        if (j > prvi_seg_ && segment->pocetak != 0)
            return false;
        if (j < poslednji_seg_ && segment->kraj != S)
            return false;
        ukupno += segment->kraj - segment->pocetak;
    }
    return ukupno == velicina_;
}

#endif
