#pragma once

#include "./Slice.hpp"
#include <vector>

namespace SamH::numc
{

template <typename T>
class Array;

template <typename T>
struct Viewer
{
    T* data_begin;
    T* data_end;
    arg_type* dim_begin;
    arg_type* dim_end;
    std::vector<Slice> views; 

    Viewer(T* dt_b = nullptr
         , T* dt_e = nullptr
         , arg_type* dm_b = nullptr
         , arg_type* dm_e = nullptr) 
        : data_begin(dt_b)
        , data_end  (dt_e)
        , dim_begin (dm_b)
        , dim_end   (dm_e)
    {}   

    T operator()(const std::vector<arg_type>& coords) const {
        arg_type dim_size = dim_end - dim_begin;
        if (static_cast<arg_type>(coords.size()) != dim_size) {
            throw std::invalid_argument("Incorrect number of coordinates provided.");
        }

        arg_type elem_count = data_end - data_begin;
        arg_type index = 0;
        for (arg_type x = 0; x < dim_size; ++x) {
            elem_count /= *(dim_begin + x);
            index += views[x][coords[x]] * elem_count;
        }
        return *(data_begin + index);
    }

    T& operator()(const std::vector<arg_type>& coords) {
        arg_type dim_size = dim_end - dim_begin;
        if (static_cast<arg_type>(coords.size()) != dim_size) {
            throw std::invalid_argument("Incorrect number of coordinates provided.");
        }
        
        arg_type elem_count = data_end - data_begin;
        arg_type index = 0;
        for (arg_type x = 0; x < dim_size; ++x) {
            elem_count /= *(dim_begin + x);
            index += views[x][coords[x]] * elem_count;
        }
        return *(data_begin + index);
    }

    // Assignment from a vector of data
    void operator=(const std::vector<T>& data) {
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
                coords[j]++;
                if (coords[j] < view_shape[j]) {
                    break;
                }
                coords[j] = 0;
            }
        }
    }

    // Assignment from a scalar (broadcasts the value)
    void operator=(const T& scalar_value) {
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
                coords[j]++;
                if (coords[j] < view_shape[j]) {
                    break;
                }
                coords[j] = 0;
            }
        }
    }

    operator Array<T>() const
    {
        return Array<T>(*this);
    }

};
}
