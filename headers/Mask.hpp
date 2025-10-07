#pragma once

#include "./numc_types.hpp"
#include <vector>

namespace SamH::NumC
{

struct Mask 
{
    std::vector<bool> data;

    Mask() {}
    Mask(arg_type size) : data(size > 0 ? size : 0) {}

    std::vector<bool>::reference operator[](arg_type index) { return data[index]; }

    bool operator[](arg_type index) const { return data[index]; }

    arg_type size() const { return static_cast<arg_type>(data.size()); }

    void push_back(bool elem) { data.push_back(elem); }
};

}
