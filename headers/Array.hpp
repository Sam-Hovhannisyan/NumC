#pragma once

#include "./numc_types.hpp"
#include "./Mask.hpp"
#include "./Viewer.hpp"
#include "./random.hpp"
#include "./global_methods.hpp"
#include <vector>
#include <cstdlib>
#include <stdexcept>

namespace SamH::NumC
{

template <typename T>
class Array 
{
public:
friend struct Viewer<T>;
template <typename U>
friend U Global::Math::det(const Array<U>& arr);

struct Slice 
{
public:
    Slice(arg_type begin, arg_type end, arg_type step = 1);

    void normalize(arg_type array_len); 

    arg_type operator[](arg_type idx) const;

    arg_type size() const;

private:
    arg_type valid_step(arg_type step) const;

private:
    arg_type s_begin;
    arg_type s_end;
    arg_type s_step;
};

public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Begin / End for non-const Array
    iterator begin() { return n_data.begin(); }
    iterator end()   { return n_data.end(); }

    // Begin / End for const Array
    const_iterator begin() const { return n_data.begin(); }
    const_iterator end()   const { return n_data.end(); }

    const_iterator cbegin() const { return n_data.cbegin(); }
    const_iterator cend()   const { return n_data.cend(); }

public:
    Array(arg_type size = 0, const T& elem = T());
    Array(const T* arr, const arg_type len);
    Array(const std::vector<T>& vector);
    Array(const T* from, const T* to);
    Array(const Array& rhv);
    Array(const std::initializer_list<T>& init);
    inline Array(const Viewer<T>& view);
    
    Array& operator=(const Array& rhv);
    
    void push_back(const T& rhv);
    void pop_back();
    
    const T& get_value(const std::vector<arg_type>& args) const;
    T& get_value(const std::vector<arg_type>& args);
    Viewer<T> operator()(const std::vector<Slice>& slices);
    Array<T>  operator()(const std::vector<Slice>& slices) const;

    Array<T> clip(arg_type min_val, arg_type max_val) const;
    void reshape(const std::vector<arg_type>& new_shape);
    
    Array<T> unique() const;
    Array<T> unique_sorted() const;
    Array<arg_type> unique_indices() const;
    Array<arg_type> unique_inverse() const;
    Array<arg_type> unique_counts() const;

    // Inside class Array<T>
    template <typename U>
    static Array<U> where(const Mask& condition,
                          const U& x,
                          const Array<U>& y);

    template <typename U>
    static Array<U> where(const Mask& condition,
                          const Array<U>& x,
                          const U& y);

    template <typename U>
    static Array<U> where(const Mask& condition,
                          const Array<U>& x,
                          const Array<U>& y);

    template <typename U>
    static Array<U> where(const Mask& condition,
                          const U& x,
                          const U& y);

    template <typename U>
    static U dot(const Array<U>& x, const Array<U>& y);

    T sum() const;
    T prod() const;
    T mean() const;
    T var() const;
    T std() const;

    T min() const;
    T max() const;
    arg_type argmin() const;
    arg_type argmax() const;

    Array<T> operator+(const Array<T>& rhv) const;
    Array<T> operator-(const Array<T>& rhv) const;
    Array<T> operator*(const Array<T>& rhv) const;
    Array<T> operator/(const Array<T>& rhv) const;
    
    Mask operator> (const Array<T>& rhv) const;
    Mask operator< (const Array<T>& rhv) const;
    Mask operator>=(const Array<T>& rhv) const;
    Mask operator<=(const Array<T>& rhv) const;
    Mask operator==(const Array<T>& rhv) const;
    Mask operator!=(const Array<T>& rhv) const;

    Mask operator< (const T& rhv) const;
    Mask operator> (const T& rhv) const;
    Mask operator>=(const T& rhv) const;
    Mask operator<=(const T& rhv) const;
    Mask operator==(const T& rhv) const;
    Mask operator!=(const T& rhv) const;

    // Non-bool types - non-const
    template <typename U = T>
    typename std::enable_if<!std::is_same<U, bool>::value, U&>::type
    operator[](arg_type index);

    // Non-bool types - const
    template <typename U = T>
    typename std::enable_if<!std::is_same<U, bool>::value, const U&>::type
    operator[](arg_type index) const;

    // Bool type - non-const (vector<bool> proxy reference)
    template <typename U = T>
    typename std::enable_if<std::is_same<U, bool>::value, typename std::vector<bool>::reference>::type
    operator[](arg_type index);

    // Bool type - const (returns by value)
    template <typename U = T>
    typename std::enable_if<std::is_same<U, bool>::value, bool>::type
    operator[](arg_type index) const;

    Array<T> operator[](const Mask& rhv) const;
    Array<T> operator[](const std::vector<bool>& rhv) const;

    arg_type size() const;
    const std::vector<arg_type>& shape() const;
    template <typename U> Array<U> cast() const;
    void print_data() const;
    void print_dims() const;

private:
    enum Broadcast
    {
        NONE,
        FIRST,
        SECOND,
        BOTH,
        INVALID
    };

    enum Sign 
    {
        SUM,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };

private:
    template <typename U, typename List>
    friend Array<U> make_array_impl(const List& init);

    inline static Broadcast can_broadcast(const Array<T>& first, const Array<T>& second);
    inline static Array<T> broadcast(const Array<T>& arr, const std::vector<arg_type>& dims);
    inline static Array<T> calculate(const Array<T>& first, const Array<T>& second, Sign sign);

private:
    std::vector<T> n_data;
    std::vector<arg_type> n_dims;
};

inline Mask logical_and(const Mask& x, const Mask& y);
inline Mask logical_or (const Mask& x, const Mask& y);
inline Mask logical_not(const Mask& arr);

}

#include "../templates/Array.ipp"
#include "./make_array.hpp"
#include "./Slice_impl.hpp"
