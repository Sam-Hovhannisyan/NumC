#include "../headers/numc.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <type_traits>
#include "numc.hpp"

namespace SamH::numc
{

template <typename T>
array<T>::array(size_t size, const T& elem)
    : n_data(size, elem)
{}

template <typename T>
array<T>::array(const T* arr, const size_t len)
{
    for (size_t i = 0; i < len; ++i) { n_data.push_back(arr[i]); }
}

template <typename T>
array<T>::array(const std::vector<T>& vector) 
    : n_data(vector)
{}

template <typename T>
array<T>::array(const T* from, const T* to)
{
    for (T* i = from; i != to; ++i) { n_data.push_back(*i); }
}

template <typename T>
array<T>::array(const array& rhv)
    : n_data(rhv.n_data)
{}

template <typename T>
array<T>::array(const std::initializer_list<T> &init)
{
    n_data.insert(n_data.end(), init.begin(), init.end());
    n_dims.push_back(init.size());
}

template <typename T>
array<T>& 
array<T>::operator=(const array& rhv)
{
    if (this != &rhv) {
        n_data = rhv.n_data;
    }
    return *this;
}

template <typename T>
void array<T>::push_back(const T& rhv)
{
    n_data.push_back(rhv);
}

template <typename T>
void array<T>::pop_back()
{
    n_data.pop_back();
}

template <typename T>
array<T>
array<T>::operator()(arg_type begin, arg_type end, arg_type step) const
{
    assert(step != 0);
    arg_type n = size();

    if (end == 0) { end = n; }

    if (begin < 0) { begin += n; }
    if (end   < 0) { end   += n; }

    if (begin < 0) { begin = 0; }
    if (end > n) { end = n; }

    array<T> result;
    if (step > 0) {
        for (arg_type i = begin; i < end; i += step) {
            result.n_data.push_back(n_data[i]);
        }
    } else {
        for (arg_type i = begin; i > end; i += step) {
            result.n_data.push_back(n_data[i]);
        }
    }
    return result;
}

// (cond, scalar, array)
template <typename T>
template <typename U>
array<U> 
array<T>::where(const array<bool>& condition,
                const U& x,
                const array<U>& y)
{
    assert(condition.size() == y.size());
    array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x : y[i]);
    }
    return result;
}

// (cond, array, scalar)
template <typename T>
template <typename U>
array<U> 
array<T>::where(const array<bool>& condition,
                const array<U>& x,
                const U& y)
{
    assert(condition.size() == x.size());
    array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x[i] : y);
    }
    return result;
}

// (cond, array, array)
template <typename T>
template <typename U>
array<U> 
array<T>::where(const array<bool>& condition,
                const array<U>& x,
                const array<U>& y)
{
    assert(condition.size() == x.size() && x.size() == y.size());
    array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x[i] : y[i]);
    }
    return result;
}

// (cond, scalar, scalar)
template <typename T>
template <typename U>
array<U> 
array<T>::where(const array<bool>& condition,
                const U& x,
                const U& y)
{
    array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x : y);
    }
    return result;
}

template <typename T>
template <typename U>
U 
array<T>::dot(const array<U>& x, const array<U>& y)
{
    assert(x.size() == y.size());
    U res = 0;
    for (arg_type i = 0; i < x.size(); ++i) { res += (x[i] * y[i]); }
    return res;
}

template <typename T>
array<T>
array<T>::clip(arg_type min_val, arg_type max_val) const
{
    assert(min <= max);
    array<T> result;
    for (const auto& val : n_data) {
        if (val < min_val) result.push_back(min_val);
        else if (val > max_val) result.push_back(max_val);
        else result.push_back(val);
    }
    return result;
}

template <typename T>
array<T> 
array<T>::unique() const
{
    array<T> result;
    std::unordered_set<T> seen;

    for (const auto& val : n_data) {
        if (seen.find(val) == seen.end()) {
            result.push_back(val);
            seen.insert(val);
        }
    }
    return result;
}

template <typename T>
array<T> 
array<T>::unique_sorted() const
{
    std::vector<T> temp = n_data;
    std::sort(temp.begin(), temp.end());
    array<T> result;

    if (!temp.empty()) {
        result.push_back(temp[0]);
        for (size_t i = 1; i < temp.size(); ++i) {
            if (temp[i] != temp[i - 1])
                result.push_back(temp[i]);
        }
    }
    return result;
}

template <typename T>
array<typename array<T>::arg_type>
array<T>::unique_indices() const
{
    array<arg_type> indices;
    std::unordered_set<T> seen;

    for (arg_type i = 0; i < size(); ++i) {
        if (seen.find(n_data[i]) == seen.end()) {
            indices.push_back(i);
            seen.insert(n_data[i]);
        }
    }
    return indices;
}

template <typename T>
array<typename array<T>::arg_type>
array<T>::unique_inverse() const
{
    array<T> u = unique(); 
    std::unordered_map<T, arg_type> mapping;
    for (arg_type i = 0; i < u.size(); ++i)
        mapping[u[i]] = i;

    array<arg_type> inverse;
    for (const auto& val : n_data)
        inverse.push_back(mapping[val]);

    return inverse;
}

template <typename T>
array<typename array<T>::arg_type>
array<T>::unique_counts() const
{
    std::unordered_map<T, arg_type> counts;
    for (const auto& val : n_data)
        counts[val]++;

    array<T> u = unique(); 
    array<arg_type> result;
    for (const auto& val : u)
        result.push_back(counts[val]);

    return result;
}

template <typename T>
array<T>
array<T>::sqrt() const
{
    array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::sqrt(val));
    }
    return temp;
}

template <typename T>
array<T> 
array<T>::floor() const
{
    array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::floor(val));
    }
    return temp;
}

template <typename T>
array<T> 
array<T>::ceil() const
{
    array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::ceil(val));
    }
    return temp;
}

template <typename T>
array<T> 
array<T>::round() const
{
    array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::round(val));
    }
    return temp;
}

template <typename T>
T 
array<T>::sum() const
{
    T res = 0;
    for (const auto& val : n_data) {
        res += val;
    }

    return res;    
}

template <typename T>
T 
array<T>::prod() const
{
    T res = 1;
    for (const auto& val : n_data) {
        res *= val;
    }

    return res;    
}

template <typename T>
T 
array<T>::mean() const
{
    assert(size() > 0);
    T sum = T();
    for (const auto& val : n_data) {
        sum += val;
    }
    return sum / static_cast<T>(size());
}

template <typename T>
T 
array<T>::var() const
{
    assert(size() > 0);
    const T m = mean();
    T sum = T();
    for (const auto& val : n_data) {
        T diff = val - m;
        sum += diff * diff;
    }
    return sum / static_cast<T>(size());
}

template <typename T>
T 
array<T>::std() const
{
    return static_cast<T>(std::sqrt(var()));
}

template <typename T>
T 
array<T>::min() const
{
    assert(!n_data.empty());
    return *std::min_element(n_data.begin(), n_data.end());
}

template <typename T>
T 
array<T>::max() const
{
    assert(!n_data.empty());
    return *std::max_element(n_data.begin(), n_data.end());
}

template <typename T>
typename array<T>::arg_type
array<T>::argmin() const
{
    assert(!n_data.empty());
    return std::distance(n_data.begin(), std::min_element(n_data.begin(), n_data.end()));
}

template <typename T>
typename array<T>::arg_type
array<T>::argmax() const
{
    assert(!n_data.empty());
    return std::distance(n_data.begin(), std::max_element(n_data.begin(), n_data.end()));
}

template <typename T>
array<T>
array<T>::operator+(const array<T> &rhv) const
{
    assert(size() == rhv.size());
    array<T> temp(size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        temp[i] = n_data[i] + rhv.n_data[i];
    }
    return temp;
}

template <typename T>
const array<T>&
array<T>::operator+=(const array<T>& rhv)
{
    assert(size() == rhv.size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        n_data[i] += rhv.n_data[i];
    }
    return *this;
}

template <typename T>
array<T> 
array<T>::operator-(const array<T>& rhv) const
{
    assert(size() == rhv.size());
    array<T> temp(size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        temp[i] = n_data[i] - rhv.n_data[i];
    }
    return temp;
}

template <typename T>
const array<T>&
array<T>::operator-=(const array<T>& rhv)
{
    assert(size() == rhv.size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        n_data[i] -= rhv.n_data[i];
    }
    return *this;
}

template <typename T>
array<T> 
array<T>::operator*(const array<T>& rhv) const
{
    assert(size() == rhv.size());
    array<T> temp(size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        temp[i] = n_data[i] * rhv.n_data[i];
    }
    return temp;
}

template <typename T>
const array<T>&
array<T>::operator*=(const array<T>& rhv)
{
    assert(size() == rhv.size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        n_data[i] *= rhv.n_data[i];
    }
    return *this;
}

template <typename T>
array<T> 
array<T>::operator/(const array<T>& rhv) const
{
    assert(size() == rhv.size());
    array<T> temp(size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        temp[i] = n_data[i] / rhv.n_data[i];
    }
    return temp;
}

template <typename T>
const array<T>&
array<T>::operator/=(const array<T>& rhv)
{
    assert(size() == rhv.size());
    for (size_t i = 0; i < n_data.size(); ++i) {
        n_data[i] /= rhv.n_data[i];
    }
    return *this;
}

// Comparison operators

template <typename T>
array<bool> 
array<T>::operator>(const array<T>& rhv) const {
    assert(size() == rhv.size());
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] > rhv.n_data[i];
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator<(const array<T>& rhv) const {
    assert(size() == rhv.size());
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] < rhv.n_data[i];
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator>=(const array<T>& rhv) const {
    assert(size() == rhv.size());
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] >= rhv.n_data[i];
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator<=(const array<T>& rhv) const {
    assert(size() == rhv.size());
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] <= rhv.n_data[i];
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator==(const array<T>& rhv) const {
    assert(size() == rhv.size());
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] == rhv.n_data[i];
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator!=(const array<T>& rhv) const {
    assert(size() == rhv.size());
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] != rhv.n_data[i];
    }
    return result;
}

// Compare by value

template <typename T>
array<bool> 
array<T>::operator>(const T& rhv) const {
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] > rhv;
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator<(const T& rhv) const {
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] < rhv;
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator>=(const T& rhv) const {
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] >= rhv;
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator<=(const T& rhv) const {
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] <= rhv;
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator==(const T& rhv) const {
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] == rhv;
    }
    return result;
}

template <typename T>
array<bool> 
array<T>::operator!=(const T& rhv) const {
    array<bool> result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] != rhv;
    }
    return result;
}


// Access operators

// --- Non-bool, const ---
template <typename T>
template <typename U>
typename std::enable_if<!std::is_same<U, bool>::value, const U&>::type
array<T>::operator[](arg_type index) const
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

// --- Non-bool, non-const ---

template <typename T>
template <typename U>
typename std::enable_if<!std::is_same<U, bool>::value, U&>::type
array<T>::operator[](arg_type index)
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

// --- Bool, const (returns by value) ---
template <typename T>
template <typename U>
typename std::enable_if<std::is_same<U, bool>::value, bool>::type
array<T>::operator[](arg_type index) const
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

// --- Bool, non-const (returns proxy reference) ---
template <typename T>
template <typename U>
typename std::enable_if<std::is_same<U, bool>::value, typename std::vector<bool>::reference>::type
array<T>::operator[](arg_type index)
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

template <typename T>
array<T> 
array<T>::operator[](const array<bool>& rhv) const
{
    assert(size() == rhv.size());
    array<T> res;
    for (arg_type i = 0; i < size(); ++i) {
        if (rhv[i]) { res.push_back(n_data[i]); }
    }
    
    return res;
}

template <typename T>
array<T> 
array<T>::operator[](const std::vector<bool>& rhv) const
{
    assert(size() == static_cast<T>(rhv.size()));
    array<T> res;
    for (arg_type i = 0; i < size(); ++i) {
        if (rhv[i]) { res.push_back(n_data[i]); }
    }
    
    return res;
}

template <typename T>
typename array<T>::arg_type
array<T>::size() const
{ 
    return n_data.size(); 
}

template <typename T>
const std::vector<typename array<T>::arg_type> &array<T>::shape() const
{
    return n_dims;
}

template <typename T>
void array<T>::print_data() const
{
    for (const auto& val : n_data) { std::cout << val << " "; }
    std::cout << std::endl;
}

template <typename T>
void 
array<T>::print_dims() const
{
    for (auto& in : n_dims) {
        std::cout << in << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
template <typename U> 
array<U> 
array<T>::cast() const
{
    array<U> result;
    for (const auto& val : n_data) {
        result.push_back(static_cast<U>(val));
    }
    return result;
}

// Global Functions

array<bool> 
logical_and(const array<bool>& x, const array<bool>& y)
{
    assert(x.size() == y.size());
    array<bool> res;
    for (auto i = 0; i < x.size(); ++i) {
        res.push_back(x[i] && y[i]);
    }
    return res;
}

array<bool> 
logical_or(const array<bool>& x, const array<bool>& y)
{
    assert(x.size() == y.size());
    array<bool> res;
    for (auto i = 0; i < x.size(); ++i) {
        res.push_back(x[i] || y[i]);
    }
    return res;
}

array<bool> 
logical_not(const array<bool>& arr)
{
    array<bool> res;
    for (auto i = 0; i < arr.size(); ++i) {
        res.push_back(!arr[i]);
    }
    return res;
}

}
