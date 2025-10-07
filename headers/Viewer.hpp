#pragma once

#include "./numc_types.hpp"
#include <vector>

namespace SamH::NumC
{

template <typename T>
class Array;

template <typename T>
struct Viewer
{
    using Slice = typename Array<T>::Slice;

    T* data_begin;
    T* data_end;
    std::vector<arg_type> dims;
    std::vector<Slice> views; 

    Viewer(T* dt_b = nullptr
         , T* dt_e = nullptr
         , const std::vector<arg_type>& shape = {}
         , const std::vector<Slice>& slices = {});
    
    T operator()(const std::vector<arg_type>& coords) const;
    T& operator()(const std::vector<arg_type>& coords);

    void operator=(const std::vector<T>& data);
    void operator=(const T& scalar_value);

    Array<T> operator+(const Viewer<T>& rhv) const;
    Array<T> operator-(const Viewer<T>& rhv) const;
    Array<T> operator*(const Viewer<T>& rhv) const;
    Array<T> operator/(const Viewer<T>& rhv) const;

    operator Array<T>() const
    {
        return Array<T>(*this);
    }

};
}

#include "../templates/Viewer.cpp"
