#include <iostream>
#include "./headers/Array.hpp"

using namespace SamH::NumC;

int main()
{
    /*
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    Array<int> numpy(v);
    Array<int> testSize(5);
    Array<int> sliced1 = numpy({{1, 3}});
    Array<int> sliced2 = numpy({{2, 4}});
    Array<int> result = sliced1 + sliced2;
    Array<int> sliced3 = numpy({{-1, -4, -1}});
    testSize[4] = 5;
    testSize[-2] = 4;
    testSize.print_data();
    std::cout << "------------------------" << std::endl;
    sliced1.print_data();
    sliced2.print_data();
    result.print_data();
    sliced3.print_data();

    Array<int> temp1(std::vector<int>({1, 4, 9, 16, 25, 36, 49, 64, 81}));
    temp1.print_data();
    temp1 = Global::Math::sqrt(temp1);
    temp1.print_data();

    std::cout << "------------------------" << std::endl;
    Array<double> arr({2.3, 53.8, 7.1, 25.5, 8.4, 5.6});
    arr.print_data();
    Global::Math::ceil(arr).print_data();
    Global::Math::floor(arr).print_data();
    Global::Math::round(arr).print_data();
    std::cout << "------------------------" << std::endl;

    Array<int> arr2 = {5, 2, 9, 1, 7};
    arr2.print_dims();

    std::cout << "Min: " << arr2.min() << "\n";      
    std::cout << "Max: " << arr2.max() << "\n";      
    std::cout << "Argmin: " << arr2.argmin() << "\n";
    std::cout << "Argmax: " << arr2.argmax() << "\n";

    Array<int> a({1, 3, 0});
    Array<int> b({0, 3, 2});
    a[a >= b].print_data();
    a[a <= b].print_data();

    a[logical_and(a >= b, a <= b)].print_data();
    a[logical_or (a >= b, a <= b)].print_data();

    b[b > 1].print_data();

    Array<int> c = {1, -1, 2, -2, 3, -3, 4, -4};

    Array<int>::where(c > 0, 0, c).print_data();

    auto test = make_array({{{1}, {2}}, 
                            {{2}, {3}}, 
                            {{3}, {4}}});
    test.print_data();
    test.print_dims();

    auto d = make_array({{{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}, {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}}});
    d.print_data();
    d.print_dims();

    Array<int> e1 = {1, 2, 3};
    Global::Math::pow(e1, std::vector<int>({1, 2, 3})).print_data();
    Array<int> e2 = make_array({{10,20,30}, 
                                {40,50,60}, 
                                {70,80,90}});
    
    Array<double> e4 = make_array({{10.0,20.0,30.0}, 
                                   {40.0,50.0,60.0}, 
                                   {70.0,80.0,90.0}});

    std::cout << "Determinant: " << Global::Math::det(e2) << std::endl;
    Global::concatenate(e1, e2, 0).print_data();
    Global::concatenate(e1, e2, 0).print_dims();
    Global::concatenate(e1.reshape({1, 3}), e2, 0).print_data();
    Global::concatenate(e1.reshape({1, 3}), e2, 0).print_dims();
    Global::concatenate(e1.reshape({3, 1}), e2, 1).print_data();
    Global::concatenate(e1.reshape({3, 1}), e2, 1).print_dims();
    Global::concatenate(e2, e2, 1).print_data();


    Global::Math::log10(e4).print_data();

    e1.print_data();
    e2.print_data();

    auto sum = e1 + e2;
    auto sub = e1 - e2;

    auto rs = e1({{0, 2}}) + e2({{0, 2}, {0, 2}});
    rs.print_data();

    sum.print_data();
    sub.print_data();

    e2({{0, 2}, {0, 2}}) = {100, 200, 400, 500};
    auto e3 = e2({{0, 2}, {0, 2}});
    e2.print_data();

    Array<int> e5 = {1, 2, 3, 4, 5, 6, 7, 8};
    e5.reshape({2, 4});
    e5.print_dims();

    Array<int> randint = Random::randint({0}, {10}, {10}, true);
    randint.print_data();
    randint.print_dims();

    auto random = Random::random({3, 3});
    random.print_data();
    random.print_dims();

    Array<double> lt = {1, E, Global::Math::exp(2.5), Global::Math::exp<double>(3)};
    Global::Math::log(lt).print_data();
    */
    
    Array<int> id = Global::identity<int>(4);
    id.print_data();
    id.print_dims();

    Array<int> z = Global::zeros<int>({4, 4});
    z.print_data();
    z.print_dims();

    Array<int> e = Global::eye<int>(4, 8, 4, false);
    e.print_data();
    e.print_dims();

    Mask a({true, false, true});
    Mask b({true, true, true});

    Mask res = Global::Bitwise::bitwise_or(a, b);
    
    for (auto i : res) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
0 0 0 1 
