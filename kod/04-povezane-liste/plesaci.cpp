// Plesaci u krugu (glava 4, odeljak o kruznoj listi).
//
// U krugu stoji n plesaca, oznacenih brojevima od 1 do n. Brojanje pocinje
// od prvog plesaca, a svaki m-ti izlazi iz kruga. Brojanje se nastavlja od
// sledeceg plesaca, sve dok u krugu ne ostane samo jedan.
//
// Krug je jednostruko povezana kruzna lista: poslednji cvor pokazuje na
// prvi. Program pamti cvor ispred onoga do koga se broji, pa izlazak
// plesaca brise cvor iza njega u vremenu O(1). Ukupna slozenost je
// O(n * m).
//
// Primer pokretanja:
//     echo "5 3" | ./plesaci
// ispisuje redosled izlaska 3 1 5 2 i poslednjeg plesaca 4.

#include "lista.h"   // zbog strukture Cvor

#include <iostream>

int poslednji_plesac(int n, int m)
{
    // Pravimo obicnu listu 1, 2, ..., n, pa je zatvaramo u krug.
    Cvor<int>* prvi = new Cvor<int>(1, nullptr);
    Cvor<int>* poslednji = prvi;
    for (int i = 2; i <= n; ++i) {
        poslednji->sledeci = new Cvor<int>(i, nullptr);
        poslednji = poslednji->sledeci;
    }
    poslednji->sledeci = prvi;

    // Brojanje krece od prvog plesaca, pa je na pocetku "ispred" njega
    // poslednji.
    Cvor<int>* prethodni = poslednji;
    std::cout << "Redosled izlaska:";

    for (int preostalo = n; preostalo > 1; --preostalo) {
        for (int i = 0; i < m - 1; ++i)
            prethodni = prethodni->sledeci;

        Cvor<int>* izlazi = prethodni->sledeci;
        std::cout << ' ' << izlazi->podatak;
        prethodni->sledeci = izlazi->sledeci;
        delete izlazi;
    }
    std::cout << '\n';

    // U krugu je ostao jedan cvor, koji pokazuje sam na sebe.
    int rezultat = prethodni->podatak;
    delete prethodni;
    return rezultat;
}

int main()
{
    int n;
    int m;
    if (!(std::cin >> n >> m) || n < 1 || m < 1) {
        std::cerr << "Unesite dva pozitivna cela broja n i m.\n";
        return 1;
    }

    int rezultat = poslednji_plesac(n, m);
    std::cout << "Poslednji plesac: " << rezultat << '\n';
    return 0;
}
