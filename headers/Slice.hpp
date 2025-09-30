#pragma once

#include "./numc_types.hpp"
#include <stdexcept>

namespace SamH::numc 
{
class Slice 
{
public:
    Slice(arg_type begin, arg_type end, arg_type step = 1) 
        : s_begin(begin)
        , s_end(end)
        , s_step(valid_step(step))
    {}

    void normalize(arg_type array_len) {
        // Handle negative begin/end
        if (s_begin < 0) s_begin += array_len;
        if (s_end   < 0) s_end   += array_len;

        // Clamp to bounds
        if (s_begin < 0) s_begin = 0;
        if (s_end   < 0) s_end   = 0;
        if (s_begin > array_len) s_begin = array_len;
        if (s_end   > array_len) s_end   = array_len;
    }

    arg_type operator[](arg_type idx) const {
        if (idx >= size()) {
            throw std::out_of_range("Slice::Index out of range");
        }
        return s_begin + idx * s_step;
    }

    arg_type size() const {
        if (s_step > 0) {
            if (s_begin >= s_end) return 0;
            return (s_end - s_begin + s_step - 1) / s_step;
        } else {
            if (s_begin <= s_end) return 0;
            return (s_begin - s_end - s_step - 1) / (-s_step);
        }
    }

private:
    arg_type valid_step(arg_type step) const {
        if (step == 0) throw std::invalid_argument("Slice::Step cannot be 0");
        return step;
    }

private:
    arg_type s_begin;
    arg_type s_end;
    arg_type s_step;
};
}
