#include <stdexcept>
#include <type_traits>
#include <functional>

namespace SamH::NumC {
    template <typename T>
    class Array;
}

namespace SamH::NumC::Global 
{
// ----------------- MATH -----------------
namespace Math {

    namespace detail {
        // ----------------- Unary elementwise operation -----------------
        template <typename T, typename F>
        Array<T> elementwise_op(const Array<T>& arr, F func) {
            Array<T> result(arr);
            for (arg_type i = 0; i < arr.size(); ++i)
                result[i] = static_cast<T>(func(static_cast<double>(arr[i])));
            return result;
        }

        // ----------------- Binary elementwise operation -----------------
        template <typename T, typename F>
        Array<T> elementwise_op(const Array<T>& a, const Array<T>& b, F func) {
            if (a.shape() != b.shape())
                throw std::invalid_argument("Arrays must have the same shape");
            Array<T> result(a);
            for (arg_type i = 0; i < a.size(); ++i)
                result[i] = static_cast<T>(func(static_cast<double>(a[i]), static_cast<double>(b[i])));
            return result;
        }

        // ---- Overload resolvers for ambiguous cmath functions ----
        inline double (*resolve_ldexp())(double, int) { return static_cast<double(*)(double, int)>(&std::ldexp); }
        inline double (*resolve_scalbn())(double, int) { return static_cast<double(*)(double, int)>(&std::scalbn); }
        inline double (*resolve_scalbln())(double, long) { return static_cast<double(*)(double, long)>(&std::scalbln); }
        inline double (*resolve_nexttoward())(double, long double) { return static_cast<double(*)(double, long double)>(&std::nexttoward); }

        inline long (*resolve_lround())(double) { return static_cast<long(*)(double)>(&std::lround); }
        inline long long (*resolve_llround())(double) { return static_cast<long long(*)(double)>(&std::llround); }
    }

    // ----------------- Macros -----------------

    // Unary functions (returning floating-point)
    #define DEFINE_UNARY_FUNC(NAME) \
    template <typename T> \
    inline T NAME(const T& x) { return static_cast<T>(std::NAME(static_cast<double>(x))); } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& arr) { \
        return detail::elementwise_op(arr, static_cast<double(*)(double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Viewer<T>& view) { \
        Array<T> arr(view); \
        return detail::elementwise_op(arr, static_cast<double(*)(double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const std::vector<T>& vec) { \
        Array<T> arr(vec); \
        return detail::elementwise_op(arr, static_cast<double(*)(double)>(&std::NAME)); \
    }

    // Unary functions returning integer types
    #define DEFINE_UNARY_INT_FUNC(NAME, RET_TYPE, RESOLVER) \
    template <typename T> \
    inline RET_TYPE NAME(const T& x) { return static_cast<RET_TYPE>(std::NAME(static_cast<double>(x))); } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& arr) { \
        return detail::elementwise_op(arr, detail::RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Viewer<T>& view) { \
        Array<T> arr(view); \
        return detail::elementwise_op(arr, detail::RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const std::vector<T>& vec) { \
        Array<T> arr(vec); \
        return detail::elementwise_op(arr, detail::RESOLVER()); \
    } 

    // Binary functions (returning floating-point)
    #define DEFINE_BINARY_FUNC(NAME) \
    template <typename T> \
    inline T NAME(const T& x, const T& y) { \
        return static_cast<T>(std::NAME(static_cast<double>(x), static_cast<double>(y))); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& a, const Array<T>& b) { \
        return detail::elementwise_op(a, b, static_cast<double(*)(double,double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Viewer<T>& view1, const Viewer<T>& view2) { \
        Array<T> a(view1), b(view2); \
        return detail::elementwise_op(a, b, static_cast<double(*)(double,double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const std::vector<T>& vec1, const std::vector<T>& vec2) { \
        Array<T> a(vec1), b(vec2); \
        return detail::elementwise_op(a, b, static_cast<double(*)(double,double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& a, const std::vector<T>& vec2) { \
        Array<T> b(vec2); \
        return detail::elementwise_op(a, b, static_cast<double(*)(double,double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& a, const Viewer<T>& view2) { \
        Array<T> b(view2); \
        return detail::elementwise_op(a, b, static_cast<double(*)(double,double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const std::vector<T>& vec1, const Array<T>& b) { \
        Array<T> a(vec1); \
        return detail::elementwise_op(a, b, static_cast<double(*)(double,double)>(&std::NAME)); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Viewer<T>& view, const Array<T>& b) { \
        Array<T> a(view); \
        return detail::elementwise_op(a, b, static_cast<double(*)(double,double)>(&std::NAME)); \
    } \

    // Binary functions with overload resolution
    #define DEFINE_BINARY_RESOLVED_FUNC(NAME, RESOLVER) \
    template <typename T> \
    inline T NAME(const T& x, const T& y) { \
        return static_cast<T>(RESOLVER()(static_cast<double>(x), static_cast<int>(y))); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& a, const Array<T>& b) { \
        return detail::elementwise_op(a, b, RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Viewer<T>& a, const Viewer<T>& b) { \
        Array<T> arr_a(a), arr_b(b); \
        return detail::elementwise_op(arr_a, arr_b, RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const std::vector<T>& a, const std::vector<T>& b) { \
        Array<T> arr_a(a), arr_b(b); \
        return detail::elementwise_op(arr_a, arr_b, RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& a, const std::vector<T>& vec2) { \
        Array<T> b(vec2); \
        return detail::elementwise_op(a, b, RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Array<T>& a, const Viewer<T>& view2) { \
        Array<T> b(view2); \
        return detail::elementwise_op(a, b, RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const std::vector<T>& vec1, const Array<T>& b) { \
        Array<T> a(vec1); \
        return detail::elementwise_op(a, b, RESOLVER()); \
    } \
    template <typename T> \
    inline Array<T> NAME(const Viewer<T>& view, const Array<T>& b) { \
        Array<T> a(view); \
        return detail::elementwise_op(a, b, RESOLVER()); \
    } \

    // ----------------- Unary functions -----------------
    DEFINE_UNARY_FUNC(sin)
    DEFINE_UNARY_FUNC(cos)
    DEFINE_UNARY_FUNC(tan)
    DEFINE_UNARY_FUNC(asin)
    DEFINE_UNARY_FUNC(acos)
    DEFINE_UNARY_FUNC(atan)
    DEFINE_UNARY_FUNC(sinh)
    DEFINE_UNARY_FUNC(cosh)
    DEFINE_UNARY_FUNC(tanh)
    DEFINE_UNARY_FUNC(asinh)
    DEFINE_UNARY_FUNC(acosh)
    DEFINE_UNARY_FUNC(atanh)
    DEFINE_UNARY_FUNC(round)
    DEFINE_UNARY_FUNC(rint)
    DEFINE_UNARY_FUNC(floor)
    DEFINE_UNARY_FUNC(ceil)
    DEFINE_UNARY_FUNC(trunc)
    DEFINE_UNARY_FUNC(exp)
    DEFINE_UNARY_FUNC(expm1)
    DEFINE_UNARY_FUNC(exp2)
    DEFINE_UNARY_FUNC(log)
    DEFINE_UNARY_FUNC(log10)
    DEFINE_UNARY_FUNC(log2)
    DEFINE_UNARY_FUNC(log1p)
    DEFINE_UNARY_FUNC(sqrt)
    DEFINE_UNARY_FUNC(cbrt)
    DEFINE_UNARY_FUNC(abs)
    DEFINE_UNARY_FUNC(fabs)
    DEFINE_UNARY_FUNC(nearbyint)

    // int-returning
    DEFINE_UNARY_INT_FUNC(lround, long, resolve_lround)
    DEFINE_UNARY_INT_FUNC(llround, long long, resolve_llround)

    // ----------------- Binary functions -----------------
    DEFINE_BINARY_FUNC(hypot)
    DEFINE_BINARY_FUNC(atan2)
    DEFINE_BINARY_FUNC(pow)
    DEFINE_BINARY_FUNC(fmod)
    DEFINE_BINARY_FUNC(remainder)
    DEFINE_BINARY_FUNC(fmin)
    DEFINE_BINARY_FUNC(fmax)
    DEFINE_BINARY_FUNC(fdim)
    DEFINE_BINARY_FUNC(copysign)
    DEFINE_BINARY_FUNC(nextafter)

    // binary functions needing overload resolution
    DEFINE_BINARY_RESOLVED_FUNC(ldexp, detail::resolve_ldexp)
    DEFINE_BINARY_RESOLVED_FUNC(scalbn, detail::resolve_scalbn)
    DEFINE_BINARY_RESOLVED_FUNC(scalbln, detail::resolve_scalbln)
    DEFINE_BINARY_RESOLVED_FUNC(nexttoward, detail::resolve_nexttoward)

    // cleanup macros
    #undef DEFINE_UNARY_FUNC
    #undef DEFINE_UNARY_INT_FUNC
    #undef DEFINE_BINARY_FUNC
    #undef DEFINE_BINARY_RESOLVED_FUNC

    // ----------------- Determinant -----------------
    template <typename T>
    T det(const Array<T>& arr) {
        if (arr.shape().size() != 2 || arr.shape()[0] != arr.shape()[1])
            throw std::invalid_argument("Determinant requires a square matrix");

        auto mat = arr;
        double det = 1.0;
        auto n = arr.shape()[0];
        for (arg_type i = 0; i < n; ++i) {
            arg_type pivot = i;
            for (arg_type j = i + 1; j < n; ++j)
                if (std::fabs(mat[j * n + i]) > std::fabs(mat[pivot * n + i])) pivot = j;

            if (std::fabs(mat[pivot * n + i]) < 1e-12) return static_cast<T>(0);

            if (pivot != i) {
                for (arg_type k = 0; k < n; ++k)
                    std::swap(mat[i * n + k], mat[pivot * n + k]);
                det = -det;
            }

            det *= mat[i * n + i];
            for (arg_type j = i + 1; j < n; ++j) {
                double factor = static_cast<double>(mat[j * n + i]) / mat[i * n + i];
                for (arg_type k = i; k < n; ++k)
                    mat[j * n + k] -= factor * mat[i * n + k];
            }
        }
        return static_cast<T>(det);
    }

} // namespace Math

// Bitwise operators

// --- Bitwise AND ---

// FIX 2: Signature must include std::enable_if_t matching the header declaration
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> Bitwise::bitwise_and(T n1, T n2)
{
    return n1 & n2;
}

template <typename T>
Array<T> Bitwise::bitwise_and(const Array<T>& arr1, const Array<T>& arr2)
{
    // Uses the private binary helper
    return bitwise_binary_op<T, Array<T>>(arr1, arr2, std::bit_and<T>{});
}

// FIX 1: T is no longer part of the signature, but we must deduce T for the helper call.
// Since T is not deducible from Mask arguments, we assume it can be any integral type, 
// and the container type is Mask. We use int as a dummy T for the helper.
Mask Bitwise::bitwise_and(const Mask& mask1, const Mask& mask2)
{
    return bitwise_binary_op<int, Mask>(mask1, mask2, std::bit_and<bool>{});
}

// --- Bitwise OR ---

// FIX 2: Signature must include std::enable_if_t matching the header declaration
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> Bitwise::bitwise_or(T n1, T n2)
{
    return n1 | n2;
}

template <typename T>
Array<T> Bitwise::bitwise_or(const Array<T>& arr1, const Array<T>& arr2)
{
    return bitwise_binary_op<T, Array<T>>(arr1, arr2, std::bit_or<T>{});
}

Mask Bitwise::bitwise_or(const Mask& mask1, const Mask& mask2)
{
    return bitwise_binary_op<int, Mask>(mask1, mask2, std::bit_or<bool>{});
}

// --- Bitwise XOR ---

// FIX 2: Signature must include std::enable_if_t matching the header declaration
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> Bitwise::bitwise_xor(T n1, T n2)
{
    return n1 ^ n2;
}

template <typename T>
Array<T> Bitwise::bitwise_xor(const Array<T>& arr1, const Array<T>& arr2)
{
    return bitwise_binary_op<T, Array<T>>(arr1, arr2, std::bit_xor<T>{});
}

Mask Bitwise::bitwise_xor(const Mask& mask1, const Mask& mask2)
{
    return bitwise_binary_op<int, Mask>(mask1, mask2, std::bit_xor<bool>{});
}

// --- Bitwise NOT ---

// FIX 2: Signature must include std::enable_if_t matching the header declaration
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> Bitwise::bitwise_not(T n)
{
    return ~n;
}

template <typename T>
Array<T> Bitwise::bitwise_not(const Array<T>& arr)
{
    return bitwise_unary_op<T, Array<T>>(arr, std::bit_not<T>{});
}

Mask Bitwise::bitwise_not(const Mask& mask)
{
    // The Mask logic uses logical NOT (!), and we use int as a dummy T for the helper
    return bitwise_unary_op<int, Mask>(mask, std::logical_not<bool>{});
}

// --- Invert (Delegates to bitwise_not) ---

// FIX 2: Signature must include std::enable_if_t matching the header declaration
template <typename T>
std::enable_if_t<std::is_integral_v<T>, T> Bitwise::invert(T n)
{
    return bitwise_not(n);
}

template <typename T>
Array<T> Bitwise::invert(const Array<T>& arr)
{
    return bitwise_not(arr);
}

Mask Bitwise::invert(const Mask& mask)
{
    return bitwise_not(mask);
}

// Bitwise operators end

template <typename T>
Array<T> concatenate(const Array<T>& arr1, const Array<T>& arr2, arg_type axis)
{
    // Get shapes
    std::vector<arg_type> s1 = arr1.shape();
    std::vector<arg_type> s2 = arr2.shape();

    // Promote 1D arrays when axis > 0
    if (s1.size() == 1 && axis > 0) s1.insert(s1.begin(), 1);
    if (s2.size() == 1 && axis > 0) s2.insert(s2.begin(), 1);

    // Equalize dimensions
    arg_type ndim = std::max(s1.size(), s2.size());
    std::vector<arg_type> shape1(ndim, 1), shape2(ndim, 1);
    std::copy(s1.begin(), s1.end(), shape1.end() - s1.size());
    std::copy(s2.begin(), s2.end(), shape2.end() - s2.size());

    // Validate axis
    if (axis < 0 || axis >= ndim)
        throw std::out_of_range("Concatenation::Axis out of range");

    // Validate shape compatibility
    for (arg_type i = 0; i < ndim; ++i)
        if (i != axis && shape1[i] != shape2[i])
            throw std::invalid_argument("Concatenation::Shapes do not match on non-concatenation axes");

    // Build output shape
    std::vector<arg_type> new_shape(ndim);
    for (arg_type i = 0; i < ndim; ++i)
        new_shape[i] = (i == axis) ? shape1[i] + shape2[i] : shape1[i];

    Array<T> ans(new_shape, T());

    // Compute total size
    arg_type total_size1 = 1, total_size2 = 1, total_size_out = 1;
    for (arg_type v : shape1) total_size1 *= v;
    for (arg_type v : shape2) total_size2 *= v;
    for (arg_type v : new_shape) total_size_out *= v;

    // Compute the number of "inner elements" after the concatenation axis
    arg_type inner_block = 1;
    for (arg_type i = axis + 1; i < ndim; ++i)
        inner_block *= new_shape[i];

    // Compute how many outer slices exist before the concatenation axis
    arg_type outer_blocks = 1;
    for (arg_type i = 0; i < axis; ++i)
        outer_blocks *= new_shape[i];

    arg_type axis_size1 = shape1[axis];
    arg_type axis_size2 = shape2[axis];

    // Copy values flat-wise
    arg_type index_out = 0;
    arg_type index1 = 0;
    arg_type index2 = 0;

    for (arg_type ob = 0; ob < outer_blocks; ++ob)
    {
        // Copy arr1 slice
        for (arg_type i = 0; i < axis_size1 * inner_block; ++i)
        {
            if (index1 >= arr1.size()) break;
            ans[index_out++] = arr1[index1++];
        }

        // Copy arr2 slice
        for (arg_type i = 0; i < axis_size2 * inner_block; ++i)
        {
            if (index2 >= arr2.size()) break;
            ans[index_out++] = arr2[index2++];
        }
    }

    return ans;
}

template <typename T>
Array<T>
zeros(const std::vector<arg_type>& dims)
{
    return Array<T>(dims, T(0));
}

template <typename T>
Array<T> 
zeros_like(const Array<T>& arr)
{
    return Array<T>(arr.shape(), T(0));
}

template <typename T>
Array<T>
ones(const std::vector<arg_type>& dims)
{
    return Array<T>(dims, T(1));
}

template <typename T>
Array<T> 
ones_like(const Array<T>& arr)
{
    return Array<T>(arr.shape(), T(1));
}

template <typename T>
Array<T>
identity(arg_type size)
{
    if (size < 0) throw std::invalid_argument("Invalid size argument");
    Array<T> res = Array<T>({size, size}, T(0));
    for (arg_type i = 0; i < size; ++i) {
        res.get_value({i, i}) = T(1);
    }
    return res;
}

template <typename T>
Array<T> 
eye(arg_type n, arg_type m, arg_type k, bool order)
{
    Array<T> res = Array<T>({n, m}, T(0));
    if (!order) {
        for (arg_type i = 0; i < n; ++i) {
            arg_type j = i + k;
            if (j >= 0 && j < m)
                res.get_value({i, j}) = T(1);
        }
    } else {
        for (arg_type j = 0; j < m; ++j) {
            arg_type i = j - k;
            if (i >= 0 && i < n)
                res.get_value({i, j}) = T(1);
        }
    }
    return res;
}

} // namespace Global
