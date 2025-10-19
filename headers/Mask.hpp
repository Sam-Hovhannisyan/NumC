#pragma once

#include "./numc_types.hpp"
#include <vector>

namespace SamH::NumC
{

struct Mask 
{
    std::vector<bool> data;
    using iterator = typename std::vector<bool>::iterator;
    using const_iterator = typename std::vector<bool>::const_iterator;

    // Begin / End for non-const Array
    iterator begin() { return data.begin(); }
    iterator end()   { return data.end(); }

    // Begin / End for const Array
    const_iterator begin() const { return data.begin(); }
    const_iterator end()   const { return data.end(); }

    const_iterator cbegin() const { return data.cbegin(); }
    const_iterator cend()   const { return data.cend(); }

    Mask() {}
    Mask(arg_type size) : data(size > 0 ? size : 0) {}
    Mask(const std::vector<bool>& d) : data(d) {}

    std::vector<bool>::reference operator[](arg_type index) { return data[index]; }

    bool operator[](arg_type index) const { return data[index]; }

    arg_type size() const { return static_cast<arg_type>(data.size()); }

    void push_back(bool elem) { data.push_back(elem); }
};

}
