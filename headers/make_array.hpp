#pragma once

#include "./Array.hpp"

namespace SamH::numc 
{

// ----------------- Type alias -----------------
template<typename U>
using init_list = std::initializer_list<U>;

// ----------------- Recursive implementation -----------------

template <typename U, typename List>
Array<U> 
make_array_impl(const List& init) 
{
    Array<U> result;

    // check if the value type of List is T (1D)
    if constexpr (std::is_same_v<typename List::value_type, U>) {
        result.n_data.assign(init.begin(), init.end());
        result.n_dims = { static_cast<arg_type>(init.size()) };
    } else {
        // N-D recursive case
        std::vector<U> flat;
        std::vector<arg_type> dims;

        for (auto& sublist : init) {
            auto subarray = make_array_impl<U>(sublist); // recurse
            flat.insert(flat.end(), subarray.n_data.begin(), subarray.n_data.end());
            if (dims.empty()) dims = subarray.n_dims;
        }

        result.n_data = std::move(flat);    
        result.n_dims = { static_cast<arg_type>(init.size()) };
        result.n_dims.insert(result.n_dims.end(), dims.begin(), dims.end());
    }

    return result;
}

// ----------------- Macro to generate N-D function -----------------

// Helper macros to repeat init_list N times
#define MAKE_ARRAY_REPEAT(U, N) MAKE_ARRAY_REPEAT_##N(U)
#define MAKE_ARRAY_REPEAT_1(U)  U
#define MAKE_ARRAY_REPEAT_2(U)  init_list<MAKE_ARRAY_REPEAT_1(U)>
#define MAKE_ARRAY_REPEAT_3(U)  init_list<MAKE_ARRAY_REPEAT_2(U)>
#define MAKE_ARRAY_REPEAT_4(U)  init_list<MAKE_ARRAY_REPEAT_3(U)>
#define MAKE_ARRAY_REPEAT_5(U)  init_list<MAKE_ARRAY_REPEAT_4(U)>
#define MAKE_ARRAY_REPEAT_6(U)  init_list<MAKE_ARRAY_REPEAT_5(U)>
#define MAKE_ARRAY_REPEAT_7(U)  init_list<MAKE_ARRAY_REPEAT_6(U)>
#define MAKE_ARRAY_REPEAT_8(U)  init_list<MAKE_ARRAY_REPEAT_7(U)>
#define MAKE_ARRAY_REPEAT_9(U)  init_list<MAKE_ARRAY_REPEAT_8(U)>
#define MAKE_ARRAY_REPEAT_10(U) init_list<MAKE_ARRAY_REPEAT_9(U)>
#define MAKE_ARRAY_REPEAT_11(U) init_list<MAKE_ARRAY_REPEAT_10(U)>
#define MAKE_ARRAY_REPEAT_12(U) init_list<MAKE_ARRAY_REPEAT_11(U)>
#define MAKE_ARRAY_REPEAT_13(U) init_list<MAKE_ARRAY_REPEAT_12(U)>
#define MAKE_ARRAY_REPEAT_14(U) init_list<MAKE_ARRAY_REPEAT_13(U)>
#define MAKE_ARRAY_REPEAT_15(U) init_list<MAKE_ARRAY_REPEAT_14(U)>
#define MAKE_ARRAY_REPEAT_16(U) init_list<MAKE_ARRAY_REPEAT_15(U)>

// Macro to generate the make_array function for N-D
#define MAKE_ARRAY_FUNC(N) \
template<typename U> \
Array<U> make_array(init_list<MAKE_ARRAY_REPEAT(U, N)> init) { \
    return make_array_impl<U>(init); \
}

// ----------------- Generate all N-D functions -----------------
MAKE_ARRAY_FUNC(1)
MAKE_ARRAY_FUNC(2)
MAKE_ARRAY_FUNC(3)
MAKE_ARRAY_FUNC(4)
MAKE_ARRAY_FUNC(5)
MAKE_ARRAY_FUNC(6)
MAKE_ARRAY_FUNC(7)
MAKE_ARRAY_FUNC(8)
MAKE_ARRAY_FUNC(9)
MAKE_ARRAY_FUNC(10)
MAKE_ARRAY_FUNC(11)
MAKE_ARRAY_FUNC(12)
MAKE_ARRAY_FUNC(13)
MAKE_ARRAY_FUNC(14)
MAKE_ARRAY_FUNC(15)
MAKE_ARRAY_FUNC(16)

// ----------------- Cleanup macros -----------------
#undef MAKE_ARRAY_REPEAT
#undef MAKE_ARRAY_REPEAT_1
#undef MAKE_ARRAY_REPEAT_2
#undef MAKE_ARRAY_REPEAT_3
#undef MAKE_ARRAY_REPEAT_4
#undef MAKE_ARRAY_REPEAT_5
#undef MAKE_ARRAY_REPEAT_6
#undef MAKE_ARRAY_REPEAT_7
#undef MAKE_ARRAY_REPEAT_8
#undef MAKE_ARRAY_REPEAT_9
#undef MAKE_ARRAY_REPEAT_10
#undef MAKE_ARRAY_REPEAT_11
#undef MAKE_ARRAY_REPEAT_12
#undef MAKE_ARRAY_REPEAT_13
#undef MAKE_ARRAY_REPEAT_14
#undef MAKE_ARRAY_REPEAT_15
#undef MAKE_ARRAY_REPEAT_16
#undef MAKE_ARRAY_FUNC

}