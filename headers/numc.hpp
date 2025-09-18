#ifndef NUMC_HPP
#define NUMC_HPP

#include <vector>
#include <cstdlib>

namespace SamH::numc
{
template <typename T>
class array {
public:
    using arg_type = int64_t;

public:
    array(arg_type size = 0, const T& elem = T());
    array(const T* arr, const arg_type len);
    array(const std::vector<T>& vector);
    array(const T* from, const T* to);
    array(const array& rhv);
    array(const std::initializer_list<T>& init);
    
    array& operator=(const array& rhv);

    void push_back(const T& rhv);
    void pop_back();

    array<T> operator()(arg_type begin = 0, arg_type end = 0, arg_type step = 1) const;
    array<T> clip(arg_type min_val, arg_type max_val) const;
    
    array<T> unique() const;
    array<T> unique_sorted() const;
    array<arg_type> unique_indices() const;
    array<arg_type> unique_inverse() const;
    array<arg_type> unique_counts() const;

    array<T> sqrt() const;
    array<T> floor() const;
    array<T> ceil() const;
    array<T> round() const;

    // inside class array<T>
    template <typename U>
    static array<U> where(const array<bool>& condition,
                        const U& x,
                        const array<U>& y);

    template <typename U>
    static array<U> where(const array<bool>& condition,
                        const array<U>& x,
                        const U& y);

    template <typename U>
    static array<U> where(const array<bool>& condition,
                        const array<U>& x,
                        const array<U>& y);

    template <typename U>
    static array<U> where(const array<bool>& condition,
                        const U& x,
                        const U& y);

    template <typename U>
    static U dot(const array<U>& x, const array<U>& y);

    T sum() const;
    T prod() const;
    T mean() const;
    T var() const;
    T std() const;

    T min() const;
    T max() const;
    arg_type argmin() const;
    arg_type argmax() const;

    array<T> operator+(const array<T>& rhv) const;
    array<T> operator-(const array<T>& rhv) const;
    array<T> operator*(const array<T>& rhv) const;
    array<T> operator/(const array<T>& rhv) const;
    
    array<bool> operator> (const array<T>& rhv) const;
    array<bool> operator< (const array<T>& rhv) const;
    array<bool> operator>=(const array<T>& rhv) const;
    array<bool> operator<=(const array<T>& rhv) const;
    array<bool> operator==(const array<T>& rhv) const;
    array<bool> operator!=(const array<T>& rhv) const;

    array<bool> operator< (const T& rhv) const;
    array<bool> operator> (const T& rhv) const;
    array<bool> operator>=(const T& rhv) const;
    array<bool> operator<=(const T& rhv) const;
    array<bool> operator==(const T& rhv) const;
    array<bool> operator!=(const T& rhv) const;

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

    array<T> operator[](const array<bool>& rhv) const;
    array<T> operator[](const std::vector<bool>& rhv) const;

    arg_type size() const;
    const std::vector<arg_type>& shape() const;
    template <typename U> array<U> cast() const;
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
    friend array<U> make_array_impl(const List& init);

    static Broadcast can_broadcast(const array<T>& first, const array<T>& second);
    static array<T> broadcast(const array<T>& arr, const std::vector<arg_type>& dims);
    static array<T> calculate(const array<T>& first, const array<T>& second, Sign sign);

private:
    std::vector<T> n_data;
    std::vector<arg_type> n_dims;
};

array<bool> logical_and(const array<bool>& x, const array<bool>& y);
array<bool> logical_or (const array<bool>& x, const array<bool>& y);
array<bool> logical_not(const array<bool>& arr);

}

#include "../templates/numc.cpp"
#include "./make_array.hpp"

#endif
