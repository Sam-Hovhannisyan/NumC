#include "../headers/Array.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <type_traits>

namespace SamH::numc
{

template <typename T>
Array<T>::Array(arg_type size, const T& elem)
    : n_data(size, elem)
{
    n_dims.push_back(size);
}

template <typename T>
Array<T>::Array(const T* arr, const arg_type len)
{
    for (arg_type i = 0; i < len; ++i) { n_data.push_back(arr[i]); }
    n_dims.push_back(len);
}

template <typename T>
Array<T>::Array(const std::vector<T>& vector) 
    : n_data(vector)
{
    n_dims.push_back(vector.size());
}

template <typename T>
Array<T>::Array(const T* from, const T* to)
{
    arg_type size = 0;
    for (T* i = from; i != to; ++i, ++size) { n_data.push_back(*i); }
    n_dims.push_back(size);
}

template <typename T>
Array<T>::Array(const Array& rhv)
    : n_data(rhv.n_data)
    , n_dims(rhv.n_dims)
{}

template <typename T>
Array<T>::Array(const std::initializer_list<T> &init)
{
    n_data.insert(n_data.end(), init.begin(), init.end());
    n_dims.push_back(init.size());
}

template <typename T>
Array<T>::Array(const Viewer<T>& view)
{
    arg_type total_size = 1;
    for (const auto& slice : view.views) {
        arg_type current_dim_size = slice.size();
        
        if (current_dim_size == 0) {
            total_size = 0;
            break;
        }
        total_size *= current_dim_size;
        this->n_dims.push_back(current_dim_size);
    }

    if (total_size == 0) {
        return;
    }

    this->n_data.reserve(total_size);

    std::vector<arg_type> coords(n_dims.size(), 0);
    for (arg_type i = 0; i < total_size; ++i) {
        this->n_data.push_back(view(coords));

        for (int j = n_dims.size() - 1; j >= 0; --j) {
            coords[j]++;
            if (coords[j] < n_dims[j]) {
                break;
            }
            coords[j] = 0;
        }
    }
}

// NON-CONST slicing operator (returns a read/write proxy)
template <typename T>
Viewer<T>
Array<T>::operator()(const std::vector<Slice>& slices) {
    Viewer<T> view(n_data.data(), n_data.data() + n_data.size(), 
                   n_dims.data(), n_dims.data() + n_dims.size());
    for (std::size_t d = 0; d < slices.size(); ++d) {
        Slice s = slices[d];
        s.normalize(n_dims[d]);   // adjust negatives relative to this dimension
        view.views.push_back(s);
    }
    return view;
}

// CONST slicing operator (returns a new Array, read-only)
template <typename T>
Array<T> 
Array<T>::operator()(const std::vector<Slice>& slices) const {
    Viewer<T> view(const_cast<T*>(n_data.data()), 
                   const_cast<T*>(n_data.data() + n_data.size()), 
                   const_cast<arg_type*>(n_dims.data()), 
                   const_cast<arg_type*>(n_dims.data() + n_dims.size()));
    for (std::size_t d = 0; d < slices.size(); ++d) {
        Slice s = slices[d];
        s.normalize(n_dims[d]);   // adjust negatives relative to this dimension
        view.views.push_back(s);
    }
    return Array<T>(view); // Uses the constructor we defined above
}

template <typename T>
Array<T>&
Array<T>::operator=(const Array &rhv)
{
    if (this != &rhv) {
        n_data = rhv.n_data;
        n_dims = rhv.n_dims;
    }
    return *this;
}

template <typename T>
void Array<T>::push_back(const T& rhv)
{
    n_data.push_back(rhv);
}

template <typename T>
void Array<T>::pop_back()
{
    n_data.pop_back();
}

template <typename T>
const T&
Array<T>::operator()(const std::vector<arg_type>& args) const
{
    if (args.size() > n_dims.size()) {
        throw std::invalid_argument("Arguments are out of dimention!");
    }

    arg_type elem_count = n_data.size();

    arg_type index = 0;
    for (size_t x = 0; x < n_dims.size(); ++x) {
        elem_count /= n_dims[x];
        index += args[x] * elem_count;
    }

    return n_data[index];
}

template <typename T>
T&
Array<T>::operator()(const std::vector<arg_type>& args)
{
    if (args.size() > n_dims.size()) {
        throw std::invalid_argument("Arguments are out of dimention!");
    }

    arg_type elem_count = n_data.size();

    arg_type index = 0;
    for (size_t x = 0; x < n_dims.size(); ++x) {
        elem_count /= n_dims[x];
        index += args[x] * elem_count;
    }

    return n_data[index];
}

template <typename T>
template <typename U>
U 
Array<T>::det(const Array<U>& arr)
{
    if (arr.n_dims.size() != 2 || arr.n_dims[0] != arr.n_dims[1]) {
        throw std::invalid_argument("Determinant cannot be calculated.");
    }
    auto mat = arr;
    double det = 1.0;
    auto n = arr.n_dims[0];
    for (arg_type i = 0; i < n; ++i) {
        // Pivot
        arg_type pivot = i;
        for (arg_type j = i + 1; j < n; ++j) {
            if (fabs(mat[j * n + i]) > fabs(mat[pivot * n + i])) pivot = j;
        }
        if (fabs(mat[pivot * n + i]) < 1e-12) return 0.0; // singular
        
        if (pivot != i) {
            for (arg_type k = 0; k < n; ++k) {
                std::swap(mat[i * n + k], mat[pivot * n + k]);
            }
            det = -det; // row swap flips sign
        }
        
        det *= mat[i * n + i];
        
        // Eliminate
        for (arg_type j = i + 1; j < n; ++j) {
            double factor = static_cast<double>(mat[j * n + i]) / mat[i * n + i];
            for (arg_type k = i; k < n; ++k) {
                mat[j * n + k] -= factor * mat[i * n + k];
            }
        }
    }

    return static_cast<U>(det);
}

// (cond, scalar, array)
template <typename T>
template <typename U>
Array<U> 
Array<T>::where(const Mask& condition,
                const U& x,
                const Array<U>& y)
{
    assert(condition.size() == y.size());
    Array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x : y[i]);
    }
    return result;
}

// (cond, array, scalar)
template <typename T>
template <typename U>
Array<U> 
Array<T>::where(const Mask& condition,
                const Array<U>& x,
                const U& y)
{
    assert(condition.size() == x.size());
    Array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x[i] : y);
    }
    return result;
}

// (cond, array, array)
template <typename T>
template <typename U>
Array<U> 
Array<T>::where(const Mask& condition,
                const Array<U>& x,
                const Array<U>& y)
{
    assert(condition.size() == x.size() && x.size() == y.size());
    Array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x[i] : y[i]);
    }
    return result;
}

// (cond, scalar, scalar)
template <typename T>
template <typename U>
Array<U> 
Array<T>::where(const Mask& condition,
                const U& x,
                const U& y)
{
    Array<U> result;
    result.n_data.reserve(condition.size());
    for (arg_type i = 0; i < condition.size(); ++i) {
        result.n_data.push_back(condition[i] ? x : y);
    }
    return result;
}

template <typename T>
template <typename U>
U 
Array<T>::dot(const Array<U>& x, const Array<U>& y)
{
    if(x.n_dims.size() != y.n_dims.size() && x.size() != y.size()) {
        throw std::invalid_argument("Invalid input for dot product.");
    }
    U res = 0;
    for (arg_type i = 0; i < x.size(); ++i) { res += (x[i] * y[i]); }
    return res;
}

template <typename T>
Array<T>
Array<T>::clip(arg_type min_val, arg_type max_val) const
{
    assert(min <= max);
    Array<T> result;
    for (const auto& val : n_data) {
        if (val < min_val) result.push_back(min_val);
        else if (val > max_val) result.push_back(max_val);
        else result.push_back(val);
    }
    return result;
}

template <typename T>
Array<T> 
Array<T>::unique() const
{
    Array<T> result;
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
Array<T> 
Array<T>::unique_sorted() const
{
    std::vector<T> temp = n_data;
    std::sort(temp.begin(), temp.end());
    Array<T> result;

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
Array<arg_type>
Array<T>::unique_indices() const
{
    Array<arg_type> indices;
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
Array<arg_type>
Array<T>::unique_inverse() const
{
    Array<T> u = unique(); 
    std::unordered_map<T, arg_type> mapping;
    for (arg_type i = 0; i < u.size(); ++i)
        mapping[u[i]] = i;

    Array<arg_type> inverse;
    for (const auto& val : n_data)
        inverse.push_back(mapping[val]);

    return inverse;
}

template <typename T>
Array<arg_type>
Array<T>::unique_counts() const
{
    std::unordered_map<T, arg_type> counts;
    for (const auto& val : n_data)
        counts[val]++;

    Array<T> u = unique(); 
    Array<arg_type> result;
    for (const auto& val : u)
        result.push_back(counts[val]);

    return result;
}

template <typename T>
Array<T>
Array<T>::sqrt() const
{
    Array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::sqrt(val));
    }
    return temp;
}

template <typename T>
Array<T> 
Array<T>::floor() const
{
    Array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::floor(val));
    }
    return temp;
}

template <typename T>
Array<T> 
Array<T>::ceil() const
{
    Array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::ceil(val));
    }
    return temp;
}

template <typename T>
Array<T> 
Array<T>::round() const
{
    Array<T> temp(n_data);
    for (auto& val : temp.n_data) {
        val = static_cast<T>(std::round(val));
    }
    return temp;
}

template <typename T>
T 
Array<T>::sum() const
{
    T res = 0;
    for (const auto& val : n_data) {
        res += val;
    }

    return res;    
}

template <typename T>
T 
Array<T>::prod() const
{
    T res = 1;
    for (const auto& val : n_data) {
        res *= val;
    }

    return res;    
}

template <typename T>
T 
Array<T>::mean() const
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
Array<T>::var() const
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
Array<T>::std() const
{
    return static_cast<T>(std::sqrt(var()));
}

template <typename T>
T 
Array<T>::min() const
{
    assert(!n_data.empty());
    return *std::min_element(n_data.begin(), n_data.end());
}

template <typename T>
T 
Array<T>::max() const
{
    assert(!n_data.empty());
    return *std::max_element(n_data.begin(), n_data.end());
}

template <typename T>
arg_type
Array<T>::argmin() const
{
    assert(!n_data.empty());
    return std::distance(n_data.begin(), std::min_element(n_data.begin(), n_data.end()));
}

template <typename T>
arg_type
Array<T>::argmax() const
{
    assert(!n_data.empty());
    return std::distance(n_data.begin(), std::max_element(n_data.begin(), n_data.end()));
}

template <typename T>
Array<T>
Array<T>::operator+(const Array<T>& rhv) const
{
    Broadcast flag = can_broadcast(*this, rhv);
    if (flag == Broadcast::FIRST) { return calculate(broadcast(*this, rhv.n_dims),   rhv, Sign::SUM); }
    else if (flag == Broadcast::SECOND) { return calculate(broadcast(rhv, n_dims), *this, Sign::SUM); }
    else if (flag == Broadcast::NONE)   { return calculate(                   *this, rhv, Sign::SUM); }
  
    return Array<T>();
}

template <typename T>
Array<T> 
Array<T>::operator-(const Array<T>& rhv) const
{
    Broadcast flag = can_broadcast(*this, rhv);
    if (flag == Broadcast::FIRST) { return calculate(broadcast(*this, rhv.n_dims), rhv,   Sign::SUBTRACT); }
    else if (flag == Broadcast::SECOND) { return calculate(broadcast(rhv, n_dims), *this, Sign::SUBTRACT); }
    else if (flag == Broadcast::NONE)   { return calculate(                   *this, rhv, Sign::SUBTRACT); }

    return Array<T>();
}

template <typename T>
Array<T> 
Array<T>::operator*(const Array<T>& rhv) const
{
    Broadcast flag = can_broadcast(*this, rhv);
    if (flag == Broadcast::FIRST) { return calculate(broadcast(*this, rhv.n_dims), rhv,   Sign::MULTIPLY); }
    else if (flag == Broadcast::SECOND) { return calculate(broadcast(rhv, n_dims), *this, Sign::MULTIPLY); }
    else if (flag == Broadcast::NONE)   { return calculate(                   *this, rhv, Sign::MULTIPLY); }

    return Array<T>();
}

template <typename T>
Array<T> 
Array<T>::operator/(const Array<T>& rhv) const
{
    Broadcast flag = can_broadcast(*this, rhv);
    if (flag == Broadcast::FIRST) { return calculate(broadcast(*this, rhv.n_dims), rhv,   Sign::DIVIDE); }
    else if (flag == Broadcast::SECOND) { return calculate(broadcast(rhv, n_dims), *this, Sign::DIVIDE); }
    else if (flag == Broadcast::NONE)   { return calculate(                   *this, rhv, Sign::DIVIDE); }

    return Array<T>();
}

// Comparison operators

template <typename T>
Mask 
Array<T>::operator>(const Array<T>& rhv) const {
    assert(size() == rhv.size());
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] > rhv.n_data[i];
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator<(const Array<T>& rhv) const {
    assert(size() == rhv.size());
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] < rhv.n_data[i];
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator>=(const Array<T>& rhv) const {
    assert(size() == rhv.size());
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] >= rhv.n_data[i];
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator<=(const Array<T>& rhv) const {
    assert(size() == rhv.size());
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] <= rhv.n_data[i];
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator==(const Array<T>& rhv) const {
    assert(size() == rhv.size());
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] == rhv.n_data[i];
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator!=(const Array<T>& rhv) const {
    assert(size() == rhv.size());
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] != rhv.n_data[i];
    }
    return result;
}

// Compare by value

template <typename T>
Mask 
Array<T>::operator>(const T& rhv) const {
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] > rhv;
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator<(const T& rhv) const {
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] < rhv;
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator>=(const T& rhv) const {
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] >= rhv;
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator<=(const T& rhv) const {
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] <= rhv;
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator==(const T& rhv) const {
    Mask result(size());
    for (arg_type i = 0; i < size(); ++i) {
        result[i] = n_data[i] == rhv;
    }
    return result;
}

template <typename T>
Mask 
Array<T>::operator!=(const T& rhv) const {
    Mask result(size());
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
Array<T>::operator[](arg_type index) const
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

// --- Non-bool, non-const ---

template <typename T>
template <typename U>
typename std::enable_if<!std::is_same<U, bool>::value, U&>::type
Array<T>::operator[](arg_type index)
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

// --- Bool, const (returns by value) ---
template <typename T>
template <typename U>
typename std::enable_if<std::is_same<U, bool>::value, bool>::type
Array<T>::operator[](arg_type index) const
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

// --- Bool, non-const (returns proxy reference) ---
template <typename T>
template <typename U>
typename std::enable_if<std::is_same<U, bool>::value, typename std::vector<bool>::reference>::type
Array<T>::operator[](arg_type index)
{
    assert((index >= 0 && index < size()) ||
           (index < 0 && std::abs(index) <= size()));
    return n_data[index >= 0 ? index : index + size()];
}

template <typename T>
Array<T> 
Array<T>::operator[](const Mask& rhv) const
{
    assert(size() == rhv.size());
    Array<T> res;
    for (arg_type i = 0; i < size(); ++i) {
        if (rhv[i]) { res.push_back(n_data[i]); }
    }
    
    return res;
}

template <typename T>
Array<T> 
Array<T>::operator[](const std::vector<bool>& rhv) const
{
    assert(size() == static_cast<T>(rhv.size()));
    Array<T> res;
    for (arg_type i = 0; i < size(); ++i) {
        if (rhv[i]) { res.push_back(n_data[i]); }
    }
    
    return res;
}

template <typename T>
arg_type
Array<T>::size() const
{ 
    return n_data.size(); 
}

template <typename T>
const std::vector<arg_type> &Array<T>::shape() const
{
    return n_dims;
}

template <typename T>
void Array<T>::print_data() const
{
    for (const auto& val : n_data) { std::cout << val << " "; }
    std::cout << std::endl;
}

template <typename T>
void 
Array<T>::print_dims() const
{
    for (auto& in : n_dims) {
        std::cout << in << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
typename Array<T>::Broadcast
Array<T>::can_broadcast(const Array<T>& first, const Array<T>& second)
{
    const std::vector<arg_type>& s1 = first.n_dims;
    const std::vector<arg_type>& s2 = second.n_dims;

    size_t n1 = s1.size();
    size_t n2 = s2.size();
    size_t n  = std::max(n1, n2);

    bool needs_first  = false;
    bool needs_second = false;

    for (size_t i = 0; i < n; ++i) {
        arg_type dim1 = (i < n - n1) ? 1 : s1[i - (n - n1)];
        arg_type dim2 = (i < n - n2) ? 1 : s2[i - (n - n2)];

        switch (0) {
            case 0: // explicit switch for readability
                if (dim1 == dim2) break;
                if (dim1 == 1) { needs_first = true; break; }
                if (dim2 == 1) { needs_second = true; break; }
                return INVALID;
        }
    }

    switch ((needs_first ? 1 : 0) + (needs_second ? 2 : 0)) {
        case 0: return NONE;
        case 1: return FIRST;
        case 2: return SECOND;
        case 3: return BOTH;
    }
    return INVALID; // fallback (shouldn’t happen)
}

template <typename T>
Array<T>
Array<T>::broadcast(const Array<T>& arr, const std::vector<arg_type>& dims)
{
    // arr has shape arr.n_dims, we want to broadcast to dims
    assert(dims.size() >= arr.n_dims.size());

    // Check compatibility
    for (size_t i = 0; i < arr.n_dims.size(); ++i) {
        arg_type a = arr.n_dims[arr.n_dims.size() - 1 - i];
        arg_type b = dims[dims.size() - 1 - i];
        assert(a == b || a == 1); // broadcast rule
    }

    Array<T> result;
    result.n_dims = dims;

    // compute total size
    arg_type total = 1;
    for (auto d : dims) total *= d;
    result.n_data.reserve(total);

    // Flattened repeat
    std::vector<arg_type> stride(arr.n_dims.size(), 1);
    for (int i = arr.n_dims.size() - 2; i >= 0; --i) {
        stride[i] = stride[i + 1] * arr.n_dims[i + 1];
    }

    for (arg_type i = 0; i < total; ++i) {
        // map i into arr index
        arg_type idx = 0;
        arg_type tmp = i;
        for (int j = dims.size() - 1, k = arr.n_dims.size() - 1;
             j >= 0; --j, --k) {
            arg_type coord = tmp % dims[j];
            tmp /= dims[j];

            if (k >= 0) {
                arg_type aj = arr.n_dims[k];
                if (aj > 1) idx += coord * stride[k];
            }
        }
        result.n_data.push_back(arr.n_data[idx]);
    }
    return result;
}


template <typename T>
Array<T> 
Array<T>::calculate(const Array<T>& first, const Array<T>& second, Sign sign)
{
    assert(first.size() == second.size());

    Array<T> result(first.size());
    result.n_dims = first.n_dims;
    for (arg_type i = 0; i < result.size(); ++i) {
        switch (sign)
        {
        case Sign::SUM:      result[i] = first.n_data[i] + second.n_data[i]; break;
        case Sign::SUBTRACT: result[i] = first.n_data[i] - second.n_data[i]; break;
        case Sign::MULTIPLY: result[i] = first.n_data[i] * second.n_data[i]; break;
        case Sign::DIVIDE:   result[i] = first.n_data[i] / second.n_data[i]; break;
        }
    }
    return result;
}

template <typename T>
template <typename U>
Array<U>
Array<T>::cast() const
{
    Array<U> result;
    for (const auto& val : n_data) {
        result.push_back(static_cast<U>(val));
    }
    return result;
}

// Global Functions

Mask 
logical_and(const Mask& x, const Mask& y)
{
    assert(x.size() == y.size());
    Mask res;
    for (auto i = 0; i < x.size(); ++i) {
        res.push_back(x[i] && y[i]);
    }
    return res;
}

Mask 
logical_or(const Mask& x, const Mask& y)
{
    assert(x.size() == y.size());
    Mask res;
    for (auto i = 0; i < x.size(); ++i) {
        res.push_back(x[i] || y[i]);
    }
    return res;
}

Mask 
logical_not(const Mask& arr)
{
    Mask res;
    for (auto i = 0; i < arr.size(); ++i) {
        res.push_back(!arr[i]);
    }
    return res;
}

}
