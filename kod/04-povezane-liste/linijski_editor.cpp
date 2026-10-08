// Linijski editor (glava 4, odeljak o listama u standardnoj biblioteci).
//
// Linija teksta cuva se u dvostruko povezanoj listi, a kursor je pozicija u
// listi: pokazuje na karakter desno od kursora, odnosno na kraj liste kada
// je kursor na kraju linije. Komande:
//     iX   umetanje karaktera X
//     <    pomeranje kursora ulevo
//     >    pomeranje kursora udesno
//     b    brisanje karaktera levo od kursora (backspace)
//     d    brisanje karaktera desno od kursora (delete)
//
// Ista funkcija napisana je dva puta: nad std::list, kao u knjizi, i nad
// klasom DvostrukaLista, radi poredjenja. Iterator iz prve verzije u drugoj
// je zamenjen pokazivacem na cvor.
//
// Primer pokretanja:
//     echo "iAiB<biC" | ./linijski_editor

#include "dvostruka_lista.h"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

std::string uredi(const std::string& komande)
{
    std::list<char> linija;
    auto kursor = linija.end();

    for (std::size_t i = 0; i < komande.size(); ++i) {
        char k = komande[i];
        if (k == 'i') {
            ++i;
            linija.insert(kursor, komande[i]);
        } else if (k == '<') {
            if (kursor != linija.begin())
                --kursor;
        } else if (k == '>') {
            if (kursor != linija.end())
                ++kursor;
        } else if (k == 'b') {
            if (kursor != linija.begin())
                linija.erase(std::prev(kursor));
        } else if (k == 'd') {
            if (kursor != linija.end())
                kursor = linija.erase(kursor);
        }
    }
    return std::string(linija.begin(), linija.end());
}

std::string uredi_dvostrukom_listom(const std::string& komande)
{
    DvostrukaLista<char> linija;
    auto kursor = linija.kraj();

    for (std::size_t i = 0; i < komande.size(); ++i) {
        char k = komande[i];
        if (k == 'i') {
            ++i;
            linija.umetni_ispred(kursor, komande[i]);
        } else if (k == '<') {
            if (kursor != linija.prvi())
                kursor = kursor->prethodni;
        } else if (k == '>') {
            if (kursor != linija.kraj())
                kursor = kursor->sledeci;
        } else if (k == 'b') {
            if (kursor != linija.prvi())
                linija.obrisi(kursor->prethodni);
        } else if (k == 'd') {
            if (kursor != linija.kraj()) {
                // Pre brisanja pamtimo sledbenika, jer posle delete
                // kursor->sledeci vise ne sme da se cita.
                auto sledeci = kursor->sledeci;
                linija.obrisi(kursor);
                kursor = sledeci;
            }
        }
    }

    std::string rezultat;
    for (char c : linija)
        rezultat += c;
    return rezultat;
}

int main()
{
    std::string komande;
    if (!std::getline(std::cin, komande))
        komande = "iAiB<biC";

    std::cout << "Komande:              " << komande << '\n';
    std::cout << "std::list:            " << uredi(komande) << '\n';
    std::cout << "DvostrukaLista:       " << uredi_dvostrukom_listom(komande) << '\n';
    return 0;
}
