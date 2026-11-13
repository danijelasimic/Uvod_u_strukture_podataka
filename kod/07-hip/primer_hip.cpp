// Primeri iz glave 7: osnovne operacije reda sa prioritetom, red poslova
// uredjen po hitnosti, formiranje hipa odozdo navise i min-hip iz
// standardne biblioteke.
//
// Prevodjenje:  g++ -std=c++17 -Wall -Wextra primer_hip.cpp -o primer_hip
// Izlaz:
//   vrh = 9, posle uklanjanja vrh = 7, posle dodavanja 10 vrh = 10
//   [7 2 8 9 4 6 5 1 3] -> [9 7 8 3 4 6 5 1 2]
//   poslovi: 3 rezervna kopija, 2 slanje izvestaja, 1 ciscenje logova
//   min-hip: 1 2 4 7 9
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "hip.h"

// Posao se poredi samo po hitnosti; opis ne utice na redosled.
struct Posao {
    int hitnost;
    std::string opis;

    bool operator<(const Posao& drugi) const
    {
        return hitnost < drugi.hitnost;
    }
};

void ispisi(const std::vector<int>& a)
{
    std::cout << '[';
    for (std::size_t i = 0; i < a.size(); ++i)
        std::cout << (i > 0 ? " " : "") << a[i];
    std::cout << ']';
}

int main()
{
    // Primer iz odeljka "Osnovne operacije".
    Hip<int> red;
    red.dodaj(4);
    red.dodaj(9);
    red.dodaj(2);
    red.dodaj(7);
    std::cout << "vrh = " << red.vrh();
    red.ukloni_vrh();
    std::cout << ", posle uklanjanja vrh = " << red.vrh();
    red.dodaj(10);
    std::cout << ", posle dodavanja 10 vrh = " << red.vrh() << '\n';

    // Primer formiranja hipa odozdo navise.
    std::vector<int> a = {7, 2, 8, 9, 4, 6, 5, 1, 3};
    ispisi(a);
    formiraj_hip(a);
    std::cout << " -> ";
    ispisi(a);
    std::cout << '\n';

    // Poslovi se obradjuju po hitnosti, a ne po redosledu dolaska.
    Hip<Posao> poslovi;
    poslovi.dodaj(Posao{1, "ciscenje logova"});
    poslovi.dodaj(Posao{3, "rezervna kopija"});
    poslovi.dodaj(Posao{2, "slanje izvestaja"});

    std::cout << "poslovi: ";
    bool prvi = true;
    while (!poslovi.prazan()) {
        const Posao& p = poslovi.vrh();
        if (!prvi)
            std::cout << ", ";
        std::cout << p.hitnost << ' ' << p.opis;
        prvi = false;
        poslovi.ukloni_vrh();
    }
    std::cout << '\n';

    // Min-hip iz standardne biblioteke: kriterijum poredjenja je std::greater.
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_hip;
    for (int x : {4, 9, 2, 7, 1})
        min_hip.push(x);

    std::cout << "min-hip:";
    while (!min_hip.empty()) {
        std::cout << ' ' << min_hip.top();
        min_hip.pop();
    }
    std::cout << '\n';

    return 0;
}
