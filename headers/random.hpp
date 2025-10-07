#pragma once

#include <vector>
#include "./numc_types.hpp"

namespace SamH::NumC {
    template <typename T>
    class Array;
}

namespace SamH::NumC::Random
{
    // ----------------- RANDINT -----------------
    Array<int> randint(const std::vector<arg_type>& low,
                       const std::vector<arg_type>& high,
                       const std::vector<arg_type>& size,
                       bool endpoint = false);

    Array<int> randint(arg_type low,
                       arg_type high,
                       arg_type size,
                       bool endpoint = false);

    Array<int> randint(arg_type low,
                       arg_type high,
                       const std::vector<arg_type>& size,
                       bool endpoint = false);

    Array<int> randint(const std::vector<arg_type>& low,
                       const std::vector<arg_type>& high,
                       arg_type size,
                       bool endpoint = false);

    // ----------------- RANDOM -----------------
    double random();
    Array<double> random(arg_type size);
    Array<double> random(const std::vector<arg_type>& size);

    // ----------------- UNIFORM -----------------
    Array<double> uniform(const std::vector<double>& low,
                          const std::vector<double>& high,
                          const std::vector<arg_type>& size);

    Array<double> uniform(double low, double high, const std::vector<arg_type>& size);
    Array<double> uniform(double low, double high, arg_type size);

    // ----------------- NORMAL -----------------
    Array<double> normal(const std::vector<double>& mean,
                         const std::vector<double>& stddev,
                         const std::vector<arg_type>& size);

    Array<double> normal(double mean, double stddev, const std::vector<arg_type>& size);
    Array<double> normal(double mean, double stddev, arg_type size);

    // ----------------- CHOICE / SHUFFLE -----------------
    Array<int> choice(const std::vector<int>& a,
                      const std::vector<arg_type>& size,
                      bool replace = true);

    void shuffle(Array<int>& arr);
    Array<int> permutation(const Array<int>& arr);

    // ----------------- OTHER DISTRIBUTIONS -----------------
    Array<int> binomial(int n, double p, arg_type size);
    Array<int> binomial(int n, double p, const std::vector<arg_type>& size);
    Array<int> binomial(const std::vector<int>& n,
                        const std::vector<double>& p,
                        const std::vector<arg_type>& size);

    Array<int> poisson(double lam, arg_type size);
    Array<int> poisson(double lam, const std::vector<arg_type>& size);
    Array<int> poisson(const std::vector<double>& lam,
                       const std::vector<arg_type>& size);

    Array<double> beta(double a, double b, arg_type size);
    Array<double> beta(double a, double b, const std::vector<arg_type>& size);
    Array<double> beta(const std::vector<double>& a,
                       const std::vector<double>& b,
                       const std::vector<arg_type>& size);

    Array<int> geometric(double p, arg_type size);
    Array<int> geometric(double p, const std::vector<arg_type>& size);
    Array<int> geometric(const std::vector<double>& p,
                         const std::vector<arg_type>& size);

    Array<double> triangular(double left, double mode, double right, arg_type size);
    Array<double> triangular(double left, double mode, double right, const std::vector<arg_type>& size);
    Array<double> triangular(const std::vector<double>& left,
                             const std::vector<double>& mode,
                             const std::vector<double>& right,
                             const std::vector<arg_type>& size);

    Array<double> exponential(double scale, arg_type size);
    Array<double> exponential(double scale, const std::vector<arg_type>& size);
    Array<double> exponential(const std::vector<double>& scale,
                              const std::vector<arg_type>& size);

}
