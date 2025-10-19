#pragma once

#include "./numc_types.hpp"
#include <vector>
#include <cmath>

namespace SamH::NumC {
    template <typename T>
    class Array;
}

namespace SamH::NumC::Global
{
    // ----------------- MATH -----------------
    namespace Math 
    {
        template <typename T>
        T det(const Array<T>& arr);
    }

    struct Bitwise
    {
    private:
        Bitwise() default;
        
        // --- STATIC UNARY HELPER ---
        template <typename T, typename Container, typename Op>
        static Container 
        bitwise_unary_op(const Container& c, Op op)
        {
            Container res(c.size());
            // Simple heuristic for allocating a new container from an existing one
            if constexpr (std::is_same_v<Container, Array<T>> || std::is_same_v<Container, Mask>) {
                // Assuming Mask/Array can be copied via assignment or copy constructor
                res = c;
            } else {
                // Allocate memory if Container is not copyable via assignment
                res = Container(c.size()); 
            }

            for (decltype(c.size()) i = 0; i < c.size(); ++i) { 
                res[i] = op(c[i]); 
            }
            return res;
        }

        // --- STATIC BINARY HELPER ---
        template <typename T, typename Container, typename Op>
        static Container 
        bitwise_binary_op(const Container& c1, const Container& c2, Op op)
        {
            if (c1.size() != c2.size()) {
                throw std::invalid_argument("bitwise_op: size mismatch.");
            }

            // Only check shape for Array<T>
            if constexpr (std::is_same_v<Container, Array<T>>) {
                if (c1.shape() != c2.shape()) {
                    throw std::invalid_argument("bitwise_op: shape mismatch for Array.");
                }
            }

            Container res(c1.size());
            for (decltype(c1.size()) i = 0; i < c1.size(); ++i) { 
                res[i] = op(c1[i], c2[i]);
            }
            return res;
        }
        
    public:
        // Bitwise AND
        template <typename T> static std::enable_if_t<std::is_integral_v<T>, T> bitwise_and(T n1, T n2);
        template <typename T> static Array<T> bitwise_and(const Array<T>& arr1, const Array<T>& arr2);
        static Mask bitwise_and(const Mask& mask1, const Mask& mask2);

        // Bitwise OR
        template <typename T> static std::enable_if_t<std::is_integral_v<T>, T> bitwise_or(T n1, T n2);
        template <typename T> static Array<T> bitwise_or(const Array<T>& arr1, const Array<T>& arr2);
        static Mask bitwise_or(const Mask& mask1, const Mask& mask2);

        // Bitwise XOR
        template <typename T> static std::enable_if_t<std::is_integral_v<T>, T> bitwise_xor(T n1, T n2);
        template <typename T> static Array<T> bitwise_xor(const Array<T>& arr1, const Array<T>& arr2);
        static Mask bitwise_xor(const Mask& mask1, const Mask& mask2);
        
        // Bitwise NOT
        template <typename T> static std::enable_if_t<std::is_integral_v<T>, T> bitwise_not(T n);
        template <typename T> static Array<T> bitwise_not(const Array<T>& arr);
        static Mask bitwise_not(const Mask& mask);

        // Invert
        template <typename T> static std::enable_if_t<std::is_integral_v<T>, T> invert(T n);
        template <typename T> static Array<T> invert(const Array<T>& arr);
        static Mask invert(const Mask& mask); 
    };

    template <typename T>
    Array<T> concatenate(const Array<T>& arr1, const Array<T>& arr2, arg_type axis = 0);
    
    template <typename T>
    Array<T> zeros(const std::vector<arg_type>& dims);

    template <typename T>
    Array<T> zeros_like(const Array<T>& arr);

    template <typename T>
    Array<T> ones(const std::vector<arg_type>& dims);

    template <typename T>
    Array<T> ones_like(const Array<T>& arr);

    template <typename T>
    Array<T> identity(arg_type size);

    template <typename T>
    Array<T> eye(arg_type n, arg_type m, arg_type k = 0, bool order = true);



}

#include "../templates/global_methods.ipp"