// Kojim cinilacem raste kapacitet klase std::vector?
//
// Standard cinilac ne propisuje, pa rezultat zavisi od implementacije
// standardne biblioteke uz koju je program preveden.

#include <iomanip>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v;
    std::size_t prethodni = v.capacity();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(10) << "velicina"
              << std::setw(12) << "kapacitet"
              << std::setw(10) << "odnos" << '\n';

    for (int i = 0; i < 1000; ++i) {
        v.push_back(i);

        if (v.capacity() != prethodni) {
            std::cout << std::setw(10) << v.size()
                      << std::setw(12) << v.capacity();
            if (prethodni > 0)
                std::cout << std::setw(10)
                          << static_cast<double>(v.capacity())
                                 / static_cast<double>(prethodni);
            std::cout << '\n';
            prethodni = v.capacity();
        }
    }

    return 0;
}
