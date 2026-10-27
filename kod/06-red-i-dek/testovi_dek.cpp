// Testovi klase Dek. Posle svake promene proverava se i invarijanta
// reprezentacije (funkcija invarijanta_vazi).
//
// Testovi se oslanjaju na assert, pa program ne treba prevoditi
// sa definisanim makroom NDEBUG.

#include "dek.h"

#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

template <typename T, std::size_t S>
bool isti_sadrzaj(const Dek<T, S>& dek, const std::deque<T>& uzor)
{
    if (dek.velicina() != uzor.size())
        return false;
    for (std::size_t i = 0; i < uzor.size(); ++i)
        if (dek[i] != uzor[i])
            return false;
    return true;
}

void test_prazan_dek()
{
    Dek<int, 4> dek;
    assert(dek.prazan());
    assert(dek.velicina() == 0);
    assert(dek.invarijanta_vazi());

    dek.dodaj_na_kraj(1);
    assert(dek.prednji() == 1 && dek.zadnji() == 1);
    dek.izbaci_sa_pocetka();
    assert(dek.prazan());
    assert(dek.invarijanta_vazi());

    dek.dodaj_na_pocetak(2);
    assert(dek.prednji() == 2 && dek.zadnji() == 2);
    dek.izbaci_sa_kraja();
    assert(dek.prazan());
    assert(dek.invarijanta_vazi());
}

void test_primer_sa_slike()
{
    // Dek sa 12 elemenata u cetiri segmenta kapaciteta 4, kao na slici u
    // odeljku o segmentima: prvi element upisuje se na mesto S / 2 = 2, pa je
    // prvi segment popunjen u intervalu [2,4), a poslednji u [0,2).
    Dek<int, 4> dek;
    for (int x = 0; x < 12; ++x)
        dek.dodaj_na_kraj(x);
    assert(dek.invarijanta_vazi());
    assert(dek.velicina() == 12);

    for (std::size_t i = 0; i < 12; ++i)
        assert(dek[i] == static_cast<int>(i));
    assert(dek.prednji() == 0);
    assert(dek.zadnji() == 11);

    dek[7] = 100;                       // operator[] vraca referencu
    assert(dek[7] == 100);

    const Dek<int, 4>& samo_citanje = dek;
    assert(samo_citanje[7] == 100);
    assert(samo_citanje.prednji() == 0 && samo_citanje.zadnji() == 11);
}

void test_poredjenje_sa_uzorom()
{
    Dek<int, 4> dek;
    std::deque<int> uzor;
    unsigned seme = 2026;

    for (int korak = 0; korak < 200000; ++korak) {
        seme = seme * 1103515245u + 12345u;
        unsigned izbor = (seme >> 16) % 5;

        // Dodavanja su nesto cesca, pa dek povremeno raste.
        if (izbor == 0 || izbor == 1) {
            dek.dodaj_na_kraj(korak);
            uzor.push_back(korak);
        } else if (izbor == 2) {
            dek.dodaj_na_pocetak(korak);
            uzor.push_front(korak);
        } else if (!uzor.empty() && izbor == 3) {
            dek.izbaci_sa_pocetka();
            uzor.pop_front();
        } else if (!uzor.empty()) {
            dek.izbaci_sa_kraja();
            uzor.pop_back();
        }

        assert(dek.invarijanta_vazi());
        if (korak % 1000 == 0)
            assert(isti_sadrzaj(dek, uzor));
    }
    assert(isti_sadrzaj(dek, uzor));
}

void test_rast_na_jednu_stranu()
{
    Dek<int, 4> na_kraj;
    Dek<int, 4> na_pocetak;
    const int n = 100000;
    for (int i = 0; i < n; ++i) {
        na_kraj.dodaj_na_kraj(i);
        na_pocetak.dodaj_na_pocetak(i);
    }
    assert(na_kraj.invarijanta_vazi());
    assert(na_pocetak.invarijanta_vazi());

    for (int i = 0; i < n; ++i) {
        assert(na_kraj[i] == i);
        assert(na_pocetak[i] == n - 1 - i);
    }

    while (!na_kraj.prazan())
        na_kraj.izbaci_sa_kraja();
    while (!na_pocetak.prazan())
        na_pocetak.izbaci_sa_pocetka();
    assert(na_kraj.invarijanta_vazi());
    assert(na_pocetak.invarijanta_vazi());
}

void test_adrese_ostaju_iste()
{
    Dek<int, 4> dek;
    for (int i = 0; i < 10; ++i)
        dek.dodaj_na_kraj(i);

    std::vector<int*> adrese;
    for (std::size_t i = 0; i < dek.velicina(); ++i)
        adrese.push_back(&dek[i]);

    // Mnogo dodavanja na oba kraja izaziva vise preuredjivanja niza
    // pokazivaca, ali ne i premestanje elemenata.
    const std::size_t dodato_na_pocetak = 5000;
    for (std::size_t k = 0; k < dodato_na_pocetak; ++k) {
        dek.dodaj_na_pocetak(-1);
        dek.dodaj_na_kraj(-1);
    }
    assert(dek.mesta_u_nizu_pokazivaca() > 8);

    for (std::size_t i = 0; i < adrese.size(); ++i) {
        assert(&dek[dodato_na_pocetak + i] == adrese[i]);
        assert(*adrese[i] == static_cast<int>(i));
    }
}

void test_dek_putuje()
{
    // Stalno dodavanje na kraj i uklanjanje sa pocetka: broj elemenata se ne
    // menja, pa se niz pokazivaca samo ponovo centrira i ne raste.
    Dek<int, 4> dek;
    for (int i = 0; i < 10; ++i)
        dek.dodaj_na_kraj(i);

    for (int i = 10; i < 1000000; ++i) {
        dek.dodaj_na_kraj(i);
        dek.izbaci_sa_pocetka();
        assert(dek.prednji() == i - 9);
    }
    assert(dek.invarijanta_vazi());
    assert(dek.velicina() == 10);
    assert(dek.mesta_u_nizu_pokazivaca() == 8);
}

void test_stringovi()
{
    Dek<std::string> dek;   // podrazumevani kapacitet segmenta, 32
    dek.dodaj_na_kraj("sredina");
    dek.dodaj_na_pocetak("pocetak");
    dek.dodaj_na_kraj("kraj");
    assert(dek.prednji() == "pocetak");
    assert(dek[1] == "sredina");
    assert(dek.zadnji() == "kraj");
    assert(dek.invarijanta_vazi());
}

int main()
{
    test_prazan_dek();
    test_primer_sa_slike();
    test_poredjenje_sa_uzorom();
    test_rast_na_jednu_stranu();
    test_adrese_ostaju_iste();
    test_dek_putuje();
    test_stringovi();

    std::cout << "Svi testovi su prosli.\n";
    return 0;
}
