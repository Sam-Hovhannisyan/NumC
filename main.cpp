#include <iostream>
#include "./headers/numc.hpp"

using namespace SamH::numc;

int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    array<int> numpy(v);
    array<int> testSize(5);
    array<int> sliced1 = numpy(1, 3);
    array<int> sliced2 = numpy(2, 4);
    auto sliced3 = numpy(-1, -4, -1);
    array<int> result = sliced1 + sliced2;
    // result.print_data();
    result *= sliced1;
    testSize[4] = 5;
    testSize[-2] = 4;
    testSize.print_data();
    sliced1.print_data();
    sliced2.print_data();
    sliced3.print_data();


    array<int> temp1(std::vector<int>({1, 4, 9, 16, 25, 36, 49, 64, 81}));
    temp1.print_data();
    temp1 = temp1.sqrt();
    temp1.print_data();

    // array<double> arr({2.3, 53.8, 7.1, 25.5, 8.4, 5.6});
    // arr.printData();
    // arr.ceil().printData();
    // arr.floor().printData();
    // arr.round().printData();

    array<int> arr = {5, 2, 9, 1, 7};
    arr.print_dims();

    std::cout << "Min: " << arr.min() << "\n";      
    std::cout << "Max: " << arr.max() << "\n";      
    std::cout << "Argmin: " << arr.argmin() << "\n";
    std::cout << "Argmax: " << arr.argmax() << "\n";

    array<int> a({1, 3, 0});
    array<int> b({0, 3, 2});
    a[a >= b].print_data();
    a[a <= b].print_data();

    a[logical_and(a >= b, a <= b)].print_data();
    a[logical_or (a >= b, a <= b)].print_data();

    b[b > 1].print_data();

    array<int> c = {1, -1, 2, -2, 3, -3, 4, -4};

    array<int>::where(c > 0, 0, c).print_data();

    auto test = make_array({{{1}, {2}}, {{2}, {3}}, {{3}, {4}}});
    test.print_data();
    test.print_dims();

    auto d = make_array({{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}, {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}});
    d.print_data();
    d.print_dims();

    return 0;
}
