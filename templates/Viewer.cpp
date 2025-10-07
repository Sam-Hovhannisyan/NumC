#include "../headers/Viewer.hpp"
#include <cassert>
#include <iostream>
#include <limits>

namespace SamH::NumC
{
template <typename T>
Viewer<T>::Viewer(T* dt_b, T* dt_e, const std::vector<arg_type>& shape, const std::vector<Slice>& slices)
    : data_begin(dt_b)
    , data_end(dt_e)
    , dims(shape)
    , views(slices)
{}

template <typename T>
T 
Viewer<T>::operator()(const std::vector<arg_type>& coords) const {
    arg_type dim_size = dims.size();
    if (static_cast<arg_type>(coords.size()) != dim_size) {
        throw std::invalid_argument("Incorrect number of coordinates provided.");
    }

    std::vector<arg_type> strides(dim_size);
    strides[dim_size - 1] = 1;
    for (int i = dim_size - 2; i >= 0; --i)
        strides[i] = strides[i + 1] * dims[i + 1];

    arg_type index = 0;
    for (arg_type x = 0; x < dim_size; ++x)
        index += views[x][coords[x]] * strides[x];

    return *(data_begin + index);
}

template <typename T>
T& 
Viewer<T>::operator()(const std::vector<arg_type>& coords) {
    arg_type dim_size = dims.size();
    if (static_cast<arg_type>(coords.size()) != dim_size) {
        throw std::invalid_argument("Incorrect number of coordinates provided.");
    }
    
    std::vector<arg_type> strides(dim_size);
    strides[dim_size - 1] = 1;
    for (int i = dim_size - 2; i >= 0; --i)
        strides[i] = strides[i + 1] * dims[i + 1];

    arg_type index = 0;
    for (arg_type x = 0; x < dim_size; ++x)
        index += views[x][coords[x]] * strides[x];

    return *(data_begin + index);
}

template <typename T>
void Viewer<T>::operator=(const std::vector<T>& data)
{
    arg_type total_size = 1;
    std::vector<arg_type> view_shape;
    view_shape.reserve(views.size());

    for (auto& slice : views) {
        arg_type current_dim_size = slice.size();
        if (current_dim_size == 0) {
            total_size = 0;
            break;
        }
        total_size *= current_dim_size;
        view_shape.push_back(current_dim_size);
    }

    if (static_cast<arg_type>(data.size()) != total_size) {
        throw std::invalid_argument("Input data size does not match the view's size.");
    }
    
    if (total_size == 0) {
        return;
    }

    std::vector<arg_type> coords(views.size(), 0);
    for (arg_type i = 0; i < total_size; ++i) {
        (*this)(coords) = data[i];
        
        for (int j = view_shape.size() - 1; j >= 0; --j) {
            ++coords[j];
            if (coords[j] < view_shape[j]) {
                break;
            }
            coords[j] = 0;
        }
    }
}

template <typename T>
void Viewer<T>::operator=(const T& scalar_value)
{
    arg_type total_size = 1;
    std::vector<arg_type> view_shape;
    view_shape.reserve(views.size());

    for (auto& slice : views) {
        const arg_type current_dim_size = slice.size();
        if (current_dim_size == 0) {
            total_size = 0;
            break;
        }
        total_size *= current_dim_size;
        view_shape.push_back(current_dim_size);
    }

    if (total_size == 0) {
        return;
    }

    std::vector<arg_type> coords(views.size(), 0);
    for (arg_type i = 0; i < total_size; ++i) {
        (*this)(coords) = scalar_value;
        for (int j = view_shape.size() - 1; j >= 0; --j) {
            ++coords[j];
            if (coords[j] < view_shape[j]) {
                break;
            }
            coords[j] = 0;
        }
    }
}

template <typename T>
Array<T> 
Viewer<T>::operator+(const Viewer<T>& rhv) const
{
    Array<T> arr1 = Array<T>(*this);
    Array<T> arr2 = Array<T>(rhv);

    return arr1 + arr2;
}

template <typename T>
Array<T> 
Viewer<T>::operator-(const Viewer<T>& rhv) const
{
    Array<T> arr1 = Array<T>(*this);
    Array<T> arr2 = Array<T>(rhv);

    return arr1 - arr2;
}

template <typename T>
Array<T> 
Viewer<T>::operator*(const Viewer<T>& rhv) const
{
    Array<T> arr1 = Array<T>(*this);
    Array<T> arr2 = Array<T>(rhv);

    return arr1 * arr2;
}

template <typename T>
Array<T> 
Viewer<T>::operator/(const Viewer<T>& rhv) const
{
    Array<T> arr1 = Array<T>(*this);
    Array<T> arr2 = Array<T>(rhv);

    return arr1 / arr2;
}

}
