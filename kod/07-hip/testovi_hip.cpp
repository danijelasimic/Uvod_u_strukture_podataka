// Testovi klase Hip, funkcije formiraj_hip i hipsorta.
//
// Testovi se oslanjaju na assert, pa program ne treba prevoditi
// sa definisanim makroom NDEBUG.

#include "hip.h"
#include "hipsort.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <vector>

// Proverava invarijantu maks-hipa nad prvih n elemenata niza.
template <typename T>
bool je_hip(const std::vector<T>& a, std::size_t n)
{
    for (std::size_t i = 1; i < n; ++i)
        if (a[(i - 1) / 2] < a[i])
            return false;
    return true;
}

// Jednostavan generator pseudoslucajnih brojeva, isti na svim sistemima.
unsigned seme = 12345;

unsigned slucajan()
{
    seme = seme * 1103515245u + 12345u;
    return seme >> 16;
}

void test_osnovne_operacije()
{
    Hip<int> hip;
    assert(hip.prazan());
    assert(hip.velicina() == 0);

    // Primer iz odeljka "Osnovne operacije".
    hip.dodaj(4);
    hip.dodaj(9);
    hip.dodaj(2);
    hip.dodaj(7);
    assert(hip.velicina() == 4);
    assert(hip.vrh() == 9);

    hip.ukloni_vrh();
    assert(hip.vrh() == 7);

    hip.dodaj(10);
    assert(hip.vrh() == 10);
    assert(hip.invarijanta_vazi());

    while (!hip.prazan())
        hip.ukloni_vrh();
    assert(hip.velicina() == 0);

    // Hip mora ponovo da radi posle praznjenja.
    hip.dodaj(5);
    assert(hip.vrh() == 5);
    hip.ukloni_vrh();
    assert(hip.prazan());
}

void test_ponavljanja()
{
    Hip<int> hip;
    for (int x : {3, 5, 5, 1, 5, 3})
        hip.dodaj(x);

    // ukloni_vrh uklanja samo jedno pojavljivanje najveceg elementa.
    std::vector<int> izvadjeno;
    while (!hip.prazan()) {
        izvadjeno.push_back(hip.vrh());
        hip.ukloni_vrh();
    }
    assert((izvadjeno == std::vector<int>{5, 5, 5, 3, 3, 1}));
}

// Nasumican niz operacija; std::priority_queue sluzi kao uzor.
void test_poredjenje_sa_uzorom()
{
    Hip<int> hip;
    std::priority_queue<int> uzor;

    for (int korak = 0; korak < 200000; ++korak) {
        bool dodavanje = slucajan() % 3 != 0;   // dve trecine dodavanja

        if (dodavanje || uzor.empty()) {
            int x = static_cast<int>(slucajan() % 1000);
            hip.dodaj(x);
            uzor.push(x);
        } else {
            hip.ukloni_vrh();
            uzor.pop();
        }

        assert(hip.velicina() == uzor.size());
        if (!uzor.empty())
            assert(hip.vrh() == uzor.top());
        if (korak % 1000 == 0)
            assert(hip.invarijanta_vazi());
    }
}

void test_primeri_iz_glave()
{
    // Uklanjanje vrha i spustanje.
    std::vector<int> a = {9, 7, 8, 3, 4, 6, 5, 1, 2};
    a.front() = a.back();
    a.pop_back();
    spusti(a, 0, a.size());
    assert((a == std::vector<int>{8, 7, 6, 3, 4, 2, 5, 1}));

    // Formiranje hipa odozdo navise.
    std::vector<int> b = {7, 2, 8, 9, 4, 6, 5, 1, 3};
    formiraj_hip(b);
    assert((b == std::vector<int>{9, 7, 8, 3, 4, 6, 5, 1, 2}));

    // Formiranje odozgo nanize: elementi 1..5 dodaju se redom.
    Hip<int> c(std::vector<int>{1, 2, 3, 4, 5});
    assert(c.invarijanta_vazi());
    assert(c.vrh() == 5);
}

void test_formiranje()
{
    for (std::size_t n = 0; n <= 200; ++n) {
        std::vector<int> a(n);
        for (int& x : a)
            x = static_cast<int>(slucajan() % 50);

        std::vector<int> b = a;
        formiraj_hip(b);
        assert(je_hip(b, b.size()));

        // Formiranje samo preuredjuje elemente.
        std::vector<int> s1 = a, s2 = b;
        std::sort(s1.begin(), s1.end());
        std::sort(s2.begin(), s2.end());
        assert(s1 == s2);

        Hip<int> hip(a);
        assert(hip.velicina() == n);
        assert(hip.invarijanta_vazi());
        if (n > 0)
            assert(hip.vrh() == s1.back());
    }
}

void test_hipsort()
{
    std::vector<int> prazan;
    hipsort(prazan);
    assert(prazan.empty());

    std::vector<int> jedan = {42};
    hipsort(jedan);
    assert(jedan == std::vector<int>{42});

    std::vector<int> a = {7, 2, 8, 9, 4, 6, 5, 1, 3};
    hipsort(a);
    assert((a == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}));

    for (std::size_t n = 0; n <= 300; ++n) {
        std::vector<int> b(n);
        for (int& x : b)
            x = static_cast<int>(slucajan() % 20);   // mnogo ponavljanja

        std::vector<int> ocekivano = b;
        std::sort(ocekivano.begin(), ocekivano.end());
        hipsort(b);
        assert(b == ocekivano);
    }

    // Vec sortiran i obrnuto sortiran niz.
    std::vector<int> rastuci(1000), opadajuci(1000);
    for (int i = 0; i < 1000; ++i) {
        rastuci[i] = i;
        opadajuci[i] = 1000 - i;
    }
    hipsort(rastuci);
    hipsort(opadajuci);
    assert(std::is_sorted(rastuci.begin(), rastuci.end()));
    assert(std::is_sorted(opadajuci.begin(), opadajuci.end()));
}

// Podatak sa kljucem i oznakom; porede se samo kljucevi.
struct Podatak {
    int kljuc;
    char oznaka;

    bool operator<(const Podatak& drugi) const
    {
        return kljuc < drugi.kljuc;
    }
};

// Primer iz glave: hipsort nije stabilan.
void test_nestabilnost()
{
    std::vector<Podatak> a = {{2, 'A'}, {2, 'B'}, {1, 'C'}};
    hipsort(a);
    assert(a[0].kljuc == 1);
    assert(a[1].oznaka == 'B' && a[2].oznaka == 'A');
}

int main()
{
    test_osnovne_operacije();
    test_ponavljanja();
    test_poredjenje_sa_uzorom();
    test_primeri_iz_glave();
    test_formiranje();
    test_hipsort();
    test_nestabilnost();

    std::cout << "Svi testovi su prosli.\n";
    return 0;
}
