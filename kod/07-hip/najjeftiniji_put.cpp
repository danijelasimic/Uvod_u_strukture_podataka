// Najjeftiniji put kroz lavirint (odeljak "Najjeftiniji put kroz
// lavirint"). Okvir je isti kao kod pretrage u sirinu iz glave 6, ali
// redosled obrade odredjuje minimalni red sa prioritetom: sledece se
// obradjuje polje sa najmanjom ukupnom cenom od pocetka. Ovo je Dijkstrin
// algoritam.
//
// Funkcija iz knjige vraca samo cenu puta. Ovde se pri svakom poboljsanju
// cene pamti i prethodnik polja, pa se na kraju, od cilja ka pocetku,
// rekonstruise sam put.
//
// Ulaz:   m n, a zatim m redova sa po n celih brojeva: -1 je prepreka, a
//         nenegativan broj cena ulaska u polje. Pocetno polje je (0,0), a
//         ciljno (m-1,n-1); oba moraju biti prolazna. Cena pocetnog polja
//         se ne naplacuje.
// Izlaz:  cena najjeftinijeg puta, broj koraka, polja puta od pocetka do
//         cilja i mapa na kojoj je put oznacen sa *.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra najjeftiniji_put.cpp -o najjeftiniji_put
//
// Primer:    ulaz           izlaz
//            5 3            Najjeftiniji put kosta 42 i ima 10 koraka.
//            1  1  1        Put: (0,0) (0,1) (0,2) (1,2) (2,2) (2,1) (2,0)
//            9 -1  1             (3,0) (4,0) (4,1) (4,2)
//            9  1  1        Mapa (# prepreka, * put):
//            9 -1 -1        * * *
//            9  9  1        . # *
//                           * * *
//                           * # #
//                           * * *
//
// Put sa najmanje koraka ide niz prvu kolonu i ima 6 koraka, ali kosta 46.
// Najjeftiniji put zaobilazi skupa polja i ima 10 koraka.
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

struct Polje {
    int vrsta, kolona;

    bool operator==(const Polje& drugo) const
    {
        return vrsta == drugo.vrsta && kolona == drugo.kolona;
    }
};

const int smer[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Unos u redu sa prioritetom: polje i cena puta kojim se do njega stiglo.
struct Zapis {
    long long cena;
    Polje polje;

    bool operator>(const Zapis& drugi) const
    {
        return cena > drugi.cena;
    }
};

// Vraca cenu najjeftinijeg puta, ili -1 ako put ne postoji. Ako put
// postoji, prethodnik[v][k] sadrzi polje iz kojeg se najjeftinije stize do
// polja (v,k), za svako polje na tom putu osim pocetnog.
long long najjeftiniji_put(const std::vector<std::vector<int>>& cena,
                           std::vector<std::vector<Polje>>& prethodnik)
{
    int m = static_cast<int>(cena.size());
    int n = static_cast<int>(cena[0].size());
    const long long BESKONACNO = std::numeric_limits<long long>::max();

    std::vector<std::vector<long long>> udaljenost(
        m, std::vector<long long>(n, BESKONACNO));
    prethodnik.assign(m, std::vector<Polje>(n, Polje{-1, -1}));
    std::priority_queue<Zapis, std::vector<Zapis>, std::greater<Zapis>> red;

    Polje pocetak{0, 0};
    Polje cilj{m - 1, n - 1};

    udaljenost[pocetak.vrsta][pocetak.kolona] = 0;
    red.push(Zapis{0, pocetak});

    while (!red.empty()) {
        Zapis zapis = red.top();
        red.pop();
        Polje polje = zapis.polje;

        if (zapis.cena != udaljenost[polje.vrsta][polje.kolona])
            continue;  // zastareo unos
        if (polje == cilj)
            return zapis.cena;

        for (int i = 0; i < 4; ++i) {
            Polje sused{polje.vrsta + smer[i][0], polje.kolona + smer[i][1]};
            int v = sused.vrsta;
            int k = sused.kolona;
            if (v < 0 || v >= m || k < 0 || k >= n || cena[v][k] == -1)
                continue;

            long long nova_cena = zapis.cena + cena[v][k];
            if (nova_cena < udaljenost[v][k]) {
                udaljenost[v][k] = nova_cena;
                prethodnik[v][k] = polje;
                red.push(Zapis{nova_cena, sused});
            }
        }
    }
    return -1;
}

int main()
{
    int m, n;
    if (!(std::cin >> m >> n) || m <= 0 || n <= 0) {
        std::cerr << "Neispravne dimenzije mape.\n";
        return 1;
    }

    std::vector<std::vector<int>> cena(m, std::vector<int>(n, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(std::cin >> cena[i][j]) || cena[i][j] < -1) {
                std::cerr << "Neispravan unos mape.\n";
                return 1;
            }
        }
    }

    if (cena[0][0] == -1 || cena[m - 1][n - 1] == -1) {
        std::cerr << "Pocetno i ciljno polje moraju biti prolazna.\n";
        return 1;
    }

    std::vector<std::vector<Polje>> prethodnik;
    long long ukupno = najjeftiniji_put(cena, prethodnik);
    if (ukupno == -1) {
        std::cout << "Ne postoji put.\n";
        return 0;
    }

    // Prateci prethodnike dobijamo polja od cilja ka pocetku, pa ih na
    // kraju ispisujemo obrnutim redom.
    Polje pocetak{0, 0};
    std::vector<Polje> put;
    Polje polje{m - 1, n - 1};
    put.push_back(polje);
    while (!(polje == pocetak)) {
        polje = prethodnik[polje.vrsta][polje.kolona];
        put.push_back(polje);
    }

    std::vector<std::string> mapa(m, std::string(n, '.'));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (cena[i][j] == -1)
                mapa[i][j] = '#';

    std::cout << "Najjeftiniji put kosta " << ukupno << " i ima "
              << put.size() - 1 << " koraka.\nPut:";
    for (std::size_t i = put.size(); i > 0; --i) {
        const Polje& p = put[i - 1];
        mapa[p.vrsta][p.kolona] = '*';
        std::cout << " (" << p.vrsta << ',' << p.kolona << ')';
    }
    std::cout << "\nMapa (# prepreka, * put):\n";
    for (const std::string& vrsta : mapa) {
        for (std::size_t j = 0; j < vrsta.size(); ++j)
            std::cout << vrsta[j] << (j + 1 < vrsta.size() ? " " : "");
        std::cout << '\n';
    }

    return 0;
}
