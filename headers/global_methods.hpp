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
}

#include "../templates/global_methods.ipp"