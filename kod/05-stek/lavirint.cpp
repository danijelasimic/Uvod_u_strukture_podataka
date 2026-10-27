// Pretraga u dubinu na mapi zadatoj matricom, pomocu steka (odeljak
// "Primene steka"). Stek u svakom trenutku sadrzi jedan put od pocetnog
// polja do tekuceg polja; kada se dostigne cilj, na steku je trazeni put.
//
// Ulaz:   m n, a zatim m redova sa po n brojeva: 0 je prolazno polje, 1 je
//         prepreka. Pocetno polje je (0,0), a ciljno (m-1,n-1); oba moraju
//         biti prolazna.
// Izlaz:  da li put postoji i, ako postoji, polja puta od pocetka do cilja i
//         mapa na kojoj je put oznacen sa *.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra lavirint.cpp -o lavirint
//
// Primer 1:  ulaz              izlaz
//            3 3               Postoji put.
//            0 0 0             Put: (0,0) (1,0) (1,1) (2,1) (2,2)
//            0 0 1             Mapa (# prepreka, * put):
//            1 0 0             * . .
//                              * * #
//                              # * *
//
// Primer 2:  3 3 / 0 0 0 / 0 1 1 / 0 1 0   ispisuje   Ne postoji put.
#include <iostream>
#include <string>
#include <vector>

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

bool postoji_put(const std::vector<std::vector<int>>& prepreke,
                 Stek<Polje>& put)
{
    int m = static_cast<int>(prepreke.size());
    int n = static_cast<int>(prepreke[0].size());
    std::vector<std::vector<bool>> poseceno(m, std::vector<bool>(n, false));

    Polje pocetak{0, 0};
    Polje cilj{m - 1, n - 1};

    poseceno[pocetak.vrsta][pocetak.kolona] = true;
    put.ubaci(pocetak);

    while (!put.prazan()) {
        Polje polje = put.vrh();
        if (polje == cilj)
            return true;

        bool napredovali = false;
        for (int i = 0; i < 4; ++i) {
            Polje sused{polje.vrsta + smer[i][0], polje.kolona + smer[i][1]};
            if (moze_se_preci_na(prepreke, poseceno, sused)) {
                poseceno[sused.vrsta][sused.kolona] = true;
                put.ubaci(sused);
                napredovali = true;
                break;
            }
        }
        if (!napredovali)
            put.izbaci();
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

    Stek<Polje> put;
    if (!postoji_put(prepreke, put)) {
        std::cout << "Ne postoji put.\n";
        return 0;
    }

    // Stek vraca polja od cilja ka pocetku. Izbacujemo ih na pomocni stek,
    // koji redosled jos jednom obrce, pa se polja ispisuju od pocetka ka cilju.
    std::vector<std::string> mapa(m, std::string(n, '.'));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (prepreke[i][j] == 1)
                mapa[i][j] = '#';

    Stek<Polje> obrnuto;
    while (!put.prazan()) {
        Polje polje = put.vrh();
        mapa[polje.vrsta][polje.kolona] = '*';
        obrnuto.ubaci(polje);
        put.izbaci();
    }

    std::cout << "Postoji put.\nPut:";
    while (!obrnuto.prazan()) {
        std::cout << " (" << obrnuto.vrh().vrsta << ','
                  << obrnuto.vrh().kolona << ')';
        obrnuto.izbaci();
    }
    std::cout << "\nMapa (# prepreka, * put):\n";
    for (const std::string& red : mapa) {
        for (std::size_t j = 0; j < red.size(); ++j)
            std::cout << red[j] << (j + 1 < red.size() ? " " : "");
        std::cout << '\n';
    }

    return 0;
}
