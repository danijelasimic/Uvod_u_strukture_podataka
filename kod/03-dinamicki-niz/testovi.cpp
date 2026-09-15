// Testovi klase DinamickiNiz.
//
// Testovi se oslanjaju na assert, pa program ne treba prevoditi
// sa definisanim makroom NDEBUG.

#include "dinamicki_niz.h"

#include <cassert>
#include <iostream>
#include <string>

template <typename T>
void ispisi(const DinamickiNiz<T>& niz)
{
    std::cout << "[";
    for (std::size_t i = 0; i < niz.velicina(); ++i) {
        if (i != 0)
            std::cout << ", ";
        std::cout << niz[i];
    }
    std::cout << "]  velicina = " << niz.velicina()
              << ", kapacitet = " << niz.kapacitet() << '\n';
}

void test_prazan_niz()
{
    DinamickiNiz<int> a;

    assert(a.prazan());
    assert(a.velicina() == 0);
    assert(a.kapacitet() == 4);

    a.dodaj(7);
    assert(!a.prazan());
    assert(a.poslednji() == 7);

    a.obrisi_poslednji();
    assert(a.prazan());
    assert(a.kapacitet() == 4);   // brisanje ne smanjuje kapacitet
}

void test_dodavanja_i_rasta()
{
    DinamickiNiz<int> a(4);

    for (int i = 1; i <= 4; ++i)
        a.dodaj(10 * i);

    assert(a.velicina() == 4);
    assert(a.kapacitet() == 4);

    // Peto dodavanje izaziva realokaciju: 4 -> 8.
    a.dodaj(50);
    assert(a.velicina() == 5);
    assert(a.kapacitet() == 8);

    // Posle realokacije elementi su sacuvani.
    for (std::size_t i = 0; i < a.velicina(); ++i)
        assert(a[i] == 10 * static_cast<int>(i + 1));

    // Vise uzastopnih realokacija: 8 -> 16 -> ... -> 1024.
    DinamickiNiz<int> b(1);
    for (int i = 0; i < 1000; ++i)
        b.dodaj(i);

    assert(b.velicina() == 1000);
    assert(b.kapacitet() == 1024);
    for (std::size_t i = 0; i < b.velicina(); ++i)
        assert(b[i] == static_cast<int>(i));

    std::cout << "Posle dodavanja:\n";
    ispisi(a);
}

void test_umetanja()
{
    DinamickiNiz<int> a(4);
    a.dodaj(10);
    a.dodaj(20);
    a.dodaj(30);
    a.dodaj(40);

    // Umetanje u sredinu punog niza: pomeranje i realokacija zajedno.
    a.umetni(2, 25);
    assert(a.velicina() == 5);
    assert(a.kapacitet() == 8);
    assert(a[0] == 10 && a[1] == 20 && a[2] == 25 && a[3] == 30 && a[4] == 40);

    // Umetanje na pocetak pomera sve elemente.
    a.umetni(0, 5);
    assert(a[0] == 5 && a[1] == 10 && a[5] == 40);

    // Umetanje na poziciju velicina() svodi se na dodavanje na kraj.
    a.umetni(a.velicina(), 60);
    assert(a.velicina() == 7);
    assert(a.poslednji() == 60);

    // Umetanje u prazan niz.
    DinamickiNiz<int> b;
    b.umetni(0, 1);
    assert(b.velicina() == 1 && b[0] == 1);

    std::cout << "Posle umetanja:\n";
    ispisi(a);
}

void test_brisanja()
{
    DinamickiNiz<int> a;
    for (int i = 0; i < 6; ++i)
        a.dodaj(i);                        // 0 1 2 3 4 5

    a.obrisi(2);                           // 0 1 3 4 5
    assert(a.velicina() == 5);
    assert(a[1] == 1 && a[2] == 3 && a[4] == 5);

    a.obrisi(0);                           // 1 3 4 5
    assert(a[0] == 1 && a.velicina() == 4);

    a.obrisi(a.velicina() - 1);            // 1 3 4
    assert(a.poslednji() == 4);

    a.obrisi_poslednji();                  // 1 3
    assert(a.velicina() == 2);
    assert(a[0] == 1 && a[1] == 3);

    // Kapacitet se brisanjem ne menja.
    assert(a.kapacitet() == 8);

    // Oslobodjeno mesto koristi se pri narednom dodavanju.
    a.dodaj(9);
    assert(a.velicina() == 3 && a.poslednji() == 9 && a.kapacitet() == 8);

    std::cout << "Posle brisanja:\n";
    ispisi(a);
}

void test_pristupa_i_pretrage()
{
    DinamickiNiz<int> a;
    a.dodaj(4);
    a.dodaj(8);
    a.dodaj(15);
    a.dodaj(8);

    // Izmena preko nekonstantnog operator[] i poslednji().
    a[0] = 11;
    a.poslednji() = 16;
    assert(a[0] == 11 && a[3] == 16);

    // Konstantna verzija: nad const objektom dozvoljeno je samo citanje.
    const DinamickiNiz<int>& c = a;
    assert(c[1] == 8);
    assert(c.poslednji() == 16);

    assert(a.pronadji(8) == 1);            // prvo pojavljivanje
    assert(a.pronadji(15) == 2);
    assert(a.pronadji(100) == a.velicina()); // nema ga
}

void test_kopiranja()
{
    DinamickiNiz<int> a;
    a.dodaj(10);
    a.dodaj(20);
    a.dodaj(30);

    // Konstruktor kopije.
    DinamickiNiz<int> b = a;
    assert(b.velicina() == a.velicina());
    assert(b.kapacitet() == a.kapacitet());

    // Kopija je duboka: izmena kopije ne menja original.
    b[0] = 999;
    b.dodaj(40);
    assert(a[0] == 10 && b[0] == 999);
    assert(a.velicina() == 3 && b.velicina() == 4);

    // Operator dodele objektu koji vec poseduje blok.
    DinamickiNiz<int> c(20);
    c.dodaj(-1);
    c = a;
    assert(c.velicina() == 3 && c.kapacitet() == a.kapacitet());
    assert(c[0] == 10 && c[1] == 20 && c[2] == 30);

    c[1] = 777;
    assert(a[1] == 20);

    // Ulancana dodela: a = b = c.
    DinamickiNiz<int> d;
    DinamickiNiz<int> e;
    d = e = a;
    assert(d.velicina() == 3 && e.velicina() == 3 && d[2] == 30);

    // Samododela, ovde posredno preko reference, ne sme da pokvari objekat.
    DinamickiNiz<int>& isti = c;
    c = isti;
    assert(c.velicina() == 3 && c[1] == 777);

    std::cout << "\nOriginal:\n";
    ispisi(a);
    std::cout << "Kopija izmenjena nezavisno od originala:\n";
    ispisi(b);
    std::cout << "Objekat posle operatora dodele:\n";
    ispisi(c);
}

void test_sa_drugim_tipom()
{
    DinamickiNiz<std::string> reci(2);

    reci.dodaj("strukture");
    reci.dodaj("podataka");
    reci.umetni(1, "i");
    reci.dodaj("algoritmi");

    assert(reci.velicina() == 4);
    assert(reci[0] == "strukture");
    assert(reci[1] == "i");
    assert(reci[2] == "podataka");
    assert(reci[3] == "algoritmi");
    assert(reci.pronadji("podataka") == 2);

    std::cout << "\nIsta klasa sa tipom std::string:\n";
    ispisi(reci);
}

int main()
{
    test_prazan_niz();
    test_dodavanja_i_rasta();
    test_umetanja();
    test_brisanja();
    test_pristupa_i_pretrage();
    test_kopiranja();
    test_sa_drugim_tipom();

    std::cout << "\nSvi testovi su uspesno prosli.\n";
    return 0;
}
