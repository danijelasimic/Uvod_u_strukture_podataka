// Pretraga u sirinu na mapi zadatoj matricom, pomocu reda (odeljak
// "Primene reda"). Polja se obradjuju redom kojim su otkrivena, pa se sva
// polja na udaljenosti d obrade pre polja na udaljenosti d + 1. Zato je put
// do cilja koji pretraga pronadje najkraci.
//
// Funkcija postoji_put iz knjige samo proverava da li put postoji. Ovde se
// za svako otkriveno polje pamti i njegov prethodnik, pa se na kraju, od
// cilja ka pocetku, rekonstruise najkraci put.
//
// Ulaz:   isti kao za lavirint.cpp iz glave 5: m n, a zatim m redova sa po
//         n brojeva (0 prolazno polje, 1 prepreka). Pocetno polje je (0,0),
//         a ciljno (m-1,n-1); oba moraju biti prolazna.
// Izlaz:  da li put postoji i, ako postoji, broj koraka, polja puta od
//         pocetka do cilja i mapa na kojoj je put oznacen sa *.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra pretraga_u_sirinu.cpp -o pretraga_u_sirinu
//
// Primer:    ulaz              izlaz
//            3 3               Postoji put od 4 koraka.
//            0 0 0             Put: (0,0) (1,0) (1,1) (2,1) (2,2)
//            0 0 1             Mapa (# prepreka, * put):
//            1 0 0             * . .
//                              * * #
//                              # * *
//
// Na mapi 3 x 3 bez prepreka pretraga u sirinu nalazi put od 4 koraka, dok
// pretraga u dubinu iz lavirint.cpp prolazi kroz svih 9 polja (8 koraka).
#include <iostream>
#include <string>
#include <vector>

#include "red.h"
#include "stek.h"

struct Polje {
    int vrsta, kolona;

    bool operator==(const Polje& drugo) const
    {
        return vrsta == drugo.vrsta && kolona == drugo.kolona;
    }
};

const int smer[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

bool moze_se_preci_na(const std::vector<std::vector<int>>& prepreke,
                      const std::vector<std::vector<bool>>& poseceno,
                      const Polje& polje)
{
    int m = static_cast<int>(prepreke.size());
    int n = static_cast<int>(prepreke[0].size());
    bool u_mapi = 0 <= polje.vrsta && polje.vrsta < m &&
                  0 <= polje.kolona && polje.kolona < n;
    return u_mapi && !prepreke[polje.vrsta][polje.kolona] &&
           !poseceno[polje.vrsta][polje.kolona];
}

// Vraca true ako put postoji. Tada prethodnik[v][k] sadrzi polje iz kojeg je
// polje (v,k) otkriveno, za svako otkriveno polje osim pocetnog.
bool postoji_put(const std::vector<std::vector<int>>& prepreke,
                 std::vector<std::vector<Polje>>& prethodnik)
{
    int m = static_cast<int>(prepreke.size());
    int n = static_cast<int>(prepreke[0].size());
    std::vector<std::vector<bool>> poseceno(m, std::vector<bool>(n, false));
    prethodnik.assign(m, std::vector<Polje>(n, Polje{-1, -1}));

    // Svako polje dodaje se u red najvise jednom, pa je m * n dovoljno.
    Red<Polje> red(m * n);

    Polje pocetak{0, 0};
    Polje cilj{m - 1, n - 1};

    poseceno[pocetak.vrsta][pocetak.kolona] = true;
    red.dodaj(pocetak);

    while (!red.prazan()) {
        Polje polje = red.prednji();
        red.izbaci();

        if (polje == cilj)
            return true;

        for (int i = 0; i < 4; ++i) {
            Polje sused{polje.vrsta + smer[i][0], polje.kolona + smer[i][1]};
            if (moze_se_preci_na(prepreke, poseceno, sused)) {
                poseceno[sused.vrsta][sused.kolona] = true;
                prethodnik[sused.vrsta][sused.kolona] = polje;
                red.dodaj(sused);
            }
        }
    }
    return false;
}

int main()
{
    int m, n;
    if (!(std::cin >> m >> n) || m <= 0 || n <= 0) {
        std::cerr << "Neispravne dimenzije mape.\n";
        return 1;
    }

    std::vector<std::vector<int>> prepreke(m, std::vector<int>(n, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(std::cin >> prepreke[i][j]) ||
                (prepreke[i][j] != 0 && prepreke[i][j] != 1)) {
                std::cerr << "Neispravan unos mape.\n";
                return 1;
            }
        }
    }

    if (prepreke[0][0] == 1 || prepreke[m - 1][n - 1] == 1) {
        std::cerr << "Pocetno i ciljno polje moraju biti prolazna.\n";
        return 1;
    }

    std::vector<std::vector<Polje>> prethodnik;
    if (!postoji_put(prepreke, prethodnik)) {
        std::cout << "Ne postoji put.\n";
        return 0;
    }

    // Prateci prethodnike dobijamo polja od cilja ka pocetku. Stavljamo ih na
    // stek, pa ih skidamo od pocetka ka cilju.
    std::vector<std::string> mapa(m, std::string(n, '.'));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (prepreke[i][j] == 1)
                mapa[i][j] = '#';

    Polje pocetak{0, 0};
    Stek<Polje> put;
    Polje polje{m - 1, n - 1};
    put.ubaci(polje);
    while (!(polje == pocetak)) {
        polje = prethodnik[polje.vrsta][polje.kolona];
        put.ubaci(polje);
    }

    std::cout << "Postoji put od " << put.velicina() - 1 << " koraka.\nPut:";
    while (!put.prazan()) {
        Polje p = put.vrh();
        mapa[p.vrsta][p.kolona] = '*';
        std::cout << " (" << p.vrsta << ',' << p.kolona << ')';
        put.izbaci();
    }
    std::cout << "\nMapa (# prepreka, * put):\n";
    for (const std::string& red : mapa) {
        for (std::size_t j = 0; j < red.size(); ++j)
            std::cout << red[j] << (j + 1 < red.size() ? " " : "");
        std::cout << '\n';
    }

    return 0;
}
