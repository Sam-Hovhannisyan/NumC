#include <stdexcept>
#include <type_traits>

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
    DEFINE_UNARY_FUNC(exp)
    DEFINE_UNARY_FUNC(exp2)
    DEFINE_UNARY_FUNC(expm1)
    DEFINE_UNARY_FUNC(log)
    DEFINE_UNARY_FUNC(log2)
    DEFINE_UNARY_FUNC(log10)
    DEFINE_UNARY_FUNC(log1p)
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
    DEFINE_UNARY_FUNC(sqrt)
    DEFINE_UNARY_FUNC(abs)
    DEFINE_UNARY_FUNC(ceil)
    DEFINE_UNARY_FUNC(floor)
    DEFINE_UNARY_FUNC(round)
    DEFINE_UNARY_FUNC(trunc)
    DEFINE_UNARY_FUNC(nearbyint)
    DEFINE_UNARY_FUNC(rint)

    // int-returning
    DEFINE_UNARY_INT_FUNC(lround, long, resolve_lround)
    DEFINE_UNARY_INT_FUNC(llround, long long, resolve_llround)

    // ----------------- Binary functions -----------------
    DEFINE_BINARY_FUNC(pow)
    DEFINE_BINARY_FUNC(hypot)
    DEFINE_BINARY_FUNC(fmod)
    DEFINE_BINARY_FUNC(remainder)
    DEFINE_BINARY_FUNC(fmin)
    DEFINE_BINARY_FUNC(fmax)
    DEFINE_BINARY_FUNC(fdim)
    DEFINE_BINARY_FUNC(atan2)
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
        if (arr.n_dims.size() != 2 || arr.n_dims[0] != arr.n_dims[1])
            throw std::invalid_argument("Determinant requires a square matrix");

        auto mat = arr;
        double det = 1.0;
        auto n = arr.n_dims[0];
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
} // namespace Global
