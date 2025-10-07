
namespace SamH::NumC::Global
{
// ----------------- MATH -----------------
namespace Math 
{
    namespace detail {
        template <typename T, typename F>
        Array<T> elementwise_op(const Array<T>& arr, F func) {
            return apply(arr, [func](T x){ return static_cast<T>(func(static_cast<double>(x))); });
        }
    }

    // Macro with explicit cast for overloaded std:: functions
    #define DEFINE_MATH_FUNC(name) \
    template <typename T> \
    inline T name(const T& x) { \
        return static_cast<T>(std::name(static_cast<double>(x))); \
    } \
    \
    template <typename T> \
    inline Array<T> name(const Array<T>& arr) { \
        return detail::elementwise_op(arr, static_cast<double(*)(double)>(&std::name)); \
    }

    #define DEFINE_MATH_FUNC_INT_RET(name) \
    template <typename T> \
    Array<T> name(const Array<T>& arr) { \
        return detail::elementwise_op(arr, [](T x){ return static_cast<T>(std::name(static_cast<double>(x))); }); \
    }


    template <typename T, typename Func>
    Array<T> apply(const Array<T>& arr, Func f)
    {
        Array<T> result(arr);
        for (arg_type i = 0; i < arr.size(); ++i)
            result[i] = f(arr[i]);
        return result;
    }

    template <typename T>
    T 
    det(const Array<T>& arr)
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

        return static_cast<T>(det);
    }

    DEFINE_MATH_FUNC(exp)
    DEFINE_MATH_FUNC(exp2)
    DEFINE_MATH_FUNC(expm1)

    DEFINE_MATH_FUNC(log)
    DEFINE_MATH_FUNC(log2)
    DEFINE_MATH_FUNC(log10)
    DEFINE_MATH_FUNC(log1p)

    DEFINE_MATH_FUNC(sin)
    DEFINE_MATH_FUNC(cos)
    DEFINE_MATH_FUNC(tan)
    DEFINE_MATH_FUNC(asin)
    DEFINE_MATH_FUNC(acos)
    DEFINE_MATH_FUNC(atan)
    DEFINE_MATH_FUNC(sinh)
    DEFINE_MATH_FUNC(cosh)
    DEFINE_MATH_FUNC(tanh)
    DEFINE_MATH_FUNC(asinh)
    DEFINE_MATH_FUNC(acosh)
    DEFINE_MATH_FUNC(atanh)
    
    DEFINE_MATH_FUNC(sqrt)
    DEFINE_MATH_FUNC(abs)

    DEFINE_MATH_FUNC(ceil)
    DEFINE_MATH_FUNC(floor)
    DEFINE_MATH_FUNC(round)
    DEFINE_MATH_FUNC(trunc)
    DEFINE_MATH_FUNC_INT_RET(lround)
    DEFINE_MATH_FUNC_INT_RET(llround)
    DEFINE_MATH_FUNC_INT_RET(nearbyint)
    DEFINE_MATH_FUNC_INT_RET(rint)

    #undef DEFINE_MATH_FUNC
    #undef DEFINE_MATH_FUNC_INT_RET
}

}
