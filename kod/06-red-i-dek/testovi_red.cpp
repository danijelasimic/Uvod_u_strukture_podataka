// Testovi klasa Red (kruzni bafer), RedLista i RedDvaSteka.
//
// Testovi se oslanjaju na assert, pa program ne treba prevoditi
// sa definisanim makroom NDEBUG.

#include "red.h"
#include "red_dva_steka.h"
#include "red_lista.h"

#include <cassert>
#include <deque>
#include <iostream>
#include <string>

// Operacije zajednicke za sve tri implementacije.
template <typename R>
void test_osnovne_operacije(R& red)
{
    assert(red.prazan());
    assert(red.velicina() == 0);

    red.dodaj(4);
    assert(!red.prazan());
    assert(red.prednji() == 4);

    red.dodaj(7);
    red.dodaj(2);
    assert(red.velicina() == 3);
    assert(red.prednji() == 4);

    red.izbaci();
    assert(red.prednji() == 7);
    assert(red.velicina() == 2);

    red.prednji() = 70;           // prednji vraca referencu
    assert(red.prednji() == 70);

    red.izbaci();
    red.izbaci();
    assert(red.prazan());

    // Red mora ponovo da radi posle praznjenja.
    red.dodaj(5);
    assert(red.prednji() == 5);
    red.izbaci();
    assert(red.prazan());
}

// Nasumican niz operacija; std::deque sluzi kao uzor za ocekivani sadrzaj.
template <typename R>
void test_poredjenje_sa_uzorom(R& red, std::size_t najvise)
{
    std::deque<int> uzor;
    unsigned seme = 12345;

    for (int korak = 0; korak < 200000; ++korak) {
        seme = seme * 1103515245u + 12345u;
        bool dodavanje = (seme >> 16) % 3 != 0;   // dve trecine dodavanja

        if (dodavanje && uzor.size() < najvise) {
            red.dodaj(korak);
            uzor.push_back(korak);
        } else if (!uzor.empty()) {
            assert(red.prednji() == uzor.front());
            red.izbaci();
            uzor.pop_front();
        }
        assert(red.velicina() == uzor.size());
        if (!uzor.empty())
            assert(red.prednji() == uzor.front());
    }
}

void test_kruzni_bafer_omotavanje()
{
    // Situacija sa slike u odeljku o kruznom baferu: kapacitet 6.
    Red<char> red(6);
    for (char c : {'A', 'B', 'C', 'D'})
        red.dodaj(c);
    red.izbaci();
    red.izbaci();
    for (char c : {'E', 'F', 'G'})
        red.dodaj(c);                       // G prelazi na poziciju 0

    assert(red.velicina() == 5);
    std::string redosled;
    while (!red.prazan()) {
        redosled += red.prednji();
        red.izbaci();
    }
    assert(redosled == "CDEFG");
}

void test_kruzni_bafer_pun()
{
    Red<int> red(3);
    for (int krug = 0; krug < 10; ++krug) {
        red.dodaj(1);
        red.dodaj(2);
        red.dodaj(3);
        assert(red.pun());
        red.izbaci();
        assert(!red.pun());
        red.dodaj(4);
        assert(red.pun());

        for (int ocekivano : {2, 3, 4}) {
            assert(red.prednji() == ocekivano);
            red.izbaci();
        }
        assert(red.prazan());
    }

    Red<int> jedno_mesto(1);
    for (int x = 0; x < 5; ++x) {
        jedno_mesto.dodaj(x);
        assert(jedno_mesto.pun());
        assert(jedno_mesto.prednji() == x);
        jedno_mesto.izbaci();
    }
}

void test_konstantan_red()
{
    Red<int> red(2);
    red.dodaj(8);
    const Red<int>& samo_citanje = red;
    assert(samo_citanje.prednji() == 8);

    RedLista<int> lista;
    lista.dodaj(9);
    const RedLista<int>& lista_citanje = lista;
    assert(lista_citanje.prednji() == 9);
}

void test_stringovi()
{
    RedLista<std::string> lista;
    RedDvaSteka<std::string> stekovi;
    for (const char* rec : {"prvi", "drugi", "treci"}) {
        lista.dodaj(rec);
        stekovi.dodaj(rec);
    }
    assert(lista.prednji() == "prvi");
    assert(stekovi.prednji() == "prvi");
}

void test_dva_steka_dodavanje_posle_prebacivanja()
{
    // Novi elementi ne smeju da preteknu one koji su vec na izlaznom steku.
    RedDvaSteka<int> red;
    red.dodaj(1);
    red.dodaj(2);
    red.dodaj(3);
    assert(red.prednji() == 1);     // prebacivanje
    red.dodaj(4);
    red.dodaj(5);

    for (int ocekivano = 1; ocekivano <= 5; ++ocekivano) {
        assert(red.prednji() == ocekivano);
        red.izbaci();
    }
    assert(red.prazan());
}

void test_mnogo_elemenata()
{
    RedLista<int> lista;
    RedDvaSteka<int> stekovi;
    const int n = 1000000;
    for (int i = 0; i < n; ++i) {
        lista.dodaj(i);
        stekovi.dodaj(i);
    }
    for (int i = 0; i < n / 2; ++i) {
        assert(lista.prednji() == i);
        assert(stekovi.prednji() == i);
        lista.izbaci();
        stekovi.izbaci();
    }
    assert(lista.velicina() == n / 2);
    assert(stekovi.velicina() == n / 2);
    // Preostale cvorove liste oslobadja destruktor.
}

int main()
{
    {
        Red<int> red(5);
        test_osnovne_operacije(red);
    }
    {
        RedLista<int> red;
        test_osnovne_operacije(red);
    }
    {
        RedDvaSteka<int> red;
        test_osnovne_operacije(red);
    }
    std::cout << "osnovne operacije: u redu\n";

    {
        Red<int> red(50);
        test_poredjenje_sa_uzorom(red, 50);
    }
    {
        RedLista<int> red;
        test_poredjenje_sa_uzorom(red, 1000000);
    }
    {
        RedDvaSteka<int> red;
        test_poredjenje_sa_uzorom(red, 1000000);
    }
    std::cout << "poredjenje sa std::deque: u redu\n";

    test_kruzni_bafer_omotavanje();
    test_kruzni_bafer_pun();
    test_konstantan_red();
    test_stringovi();
    test_dva_steka_dodavanje_posle_prebacivanja();
    test_mnogo_elemenata();

    std::cout << "Svi testovi su prosli.\n";
    return 0;
}
