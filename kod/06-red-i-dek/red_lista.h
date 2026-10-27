// Red nad jednostruko povezanom listom (odeljak "Implementacija pomocu
// povezane liste"). Elementi se dodaju iza poslednjeg cvora, a uklanjaju sa
// pocetka liste.
//
// Klasa Lista iz glave 4 ne cuva pokazivac na poslednji cvor, pa red sam
// upravlja cvorovima tipa Cvor<T>. Klasa se zove RedLista da bi mogla da
// postoji u istom programu kao i Red iz red.h.
//
// Invarijanta:
//   Ako je red prazan, prvi == nullptr, poslednji == nullptr i broj == 0.
//   Inace prvi pokazuje na prvi, a poslednji na poslednji cvor liste,
//   poslednji->sledeci == nullptr, a broj je jednak broju cvorova.
//   Ako red ima jedan element, prvi i poslednji pokazuju na isti cvor.
#ifndef RED_LISTA_H
#define RED_LISTA_H

#include <cassert>
#include <cstddef>

#include "lista.h"

template <typename T>
class RedLista {
public:
    RedLista() = default;
    ~RedLista();

    RedLista(const RedLista&) = delete;
    RedLista& operator=(const RedLista&) = delete;

    void dodaj(const T& vrednost);
    void izbaci();

    T& prednji()
    {
        assert(prvi != nullptr);
        return prvi->podatak;
    }

    const T& prednji() const
    {
        assert(prvi != nullptr);
        return prvi->podatak;
    }

    bool prazan() const { return broj == 0; }
    std::size_t velicina() const { return broj; }

private:
    Cvor<T>* prvi = nullptr;
    Cvor<T>* poslednji = nullptr;
    std::size_t broj = 0;
};

template <typename T>
RedLista<T>::~RedLista()
{
    while (prvi != nullptr) {
        Cvor<T>* stari = prvi;
        prvi = prvi->sledeci;
        delete stari;
    }
}

template <typename T>
void RedLista<T>::dodaj(const T& vrednost)
{
    Cvor<T>* novi = new Cvor<T>(vrednost, nullptr);

    if (prvi == nullptr)
        prvi = novi;
    else
        poslednji->sledeci = novi;

    poslednji = novi;
    ++broj;
}

template <typename T>
void RedLista<T>::izbaci()
{
    assert(prvi != nullptr);

    Cvor<T>* stari = prvi;
    prvi = prvi->sledeci;
    // Posle uklanjanja jedinog cvora poslednji bi ostao viseci pokazivac.
    if (prvi == nullptr)
        poslednji = nullptr;

    delete stari;
    --broj;
}

#endif
