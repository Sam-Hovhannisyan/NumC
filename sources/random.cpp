#include "../headers/random.hpp"
#include "../headers/Array.hpp"
#include <random>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace SamH::NumC::Random
{
    // ---------------------- ENGINE ----------------------
    inline std::mt19937& get_engine() {
        static std::random_device rd;
        static std::mt19937 eng(rd());
        return eng;
    }

    inline arg_type compute_total(const std::vector<arg_type>& size) {
        arg_type total = 1;
        for (auto s : size) total *= s;
        return total;
    }

    // ---------------------- GENERIC FILLER ----------------------
    template <typename T, typename Dist>
    Array<T> generate_array(Dist dist, const std::vector<arg_type>& size) {
        arg_type total = compute_total(size);
        Array<T> result(total);
        result.reshape(size);

        std::mt19937& eng = get_engine();
        for (arg_type i = 0; i < total; ++i)
            result[i] = dist(eng);

        return result;
    }

    template <typename T, typename Dist>
    Array<T> generate_array(Dist dist, arg_type size) {
        Array<T> result(size);
        result.reshape({size});
        std::mt19937& eng = get_engine();
        for (arg_type i = 0; i < size; ++i)
            result[i] = dist(eng);
        return result;
    }

    template <typename T, typename Dist>
    Array<T> generate_array_broadcast(Dist make_dist, const std::vector<arg_type>& size) {
        arg_type total = compute_total(size);
        Array<T> result(total);
        result.reshape(size);

        std::mt19937& eng = get_engine();
        for (arg_type i = 0; i < total; ++i)
            result[i] = make_dist(i)(eng);

        return result;
    }

    // ---------------------- RANDINT ----------------------
    Array<int> randint(arg_type low, arg_type high, arg_type size, bool endpoint) {
        return generate_array<int>(std::uniform_int_distribution<arg_type>(low, high - !endpoint), size);
    }

    Array<int> randint(arg_type low, arg_type high, const std::vector<arg_type>& size, bool endpoint) {
        return generate_array<int>(std::uniform_int_distribution<arg_type>(low, high - !endpoint), size);
    }

    Array<int> randint(const std::vector<arg_type>& low, const std::vector<arg_type>& high,
                       arg_type size, bool endpoint) 
    {
        auto make_dist = [&](arg_type i) {
            return std::uniform_int_distribution<arg_type>(
                low[i % low.size()],
                high[i % high.size()] - !endpoint
            );
        };
        return generate_array_broadcast<int>(make_dist, {size});
    }

    Array<int> randint(const std::vector<arg_type>& low, const std::vector<arg_type>& high,
                       const std::vector<arg_type>& size, bool endpoint) 
    {
        auto make_dist = [&](arg_type i) {
            return std::uniform_int_distribution<arg_type>(
                low[i % low.size()],
                high[i % high.size()] - !endpoint
            );
        };
        return generate_array_broadcast<int>(make_dist, size);
    }

    // ---------------------- RANDOM ----------------------
    double random() {
        return std::uniform_real_distribution<double>(0.0, 1.0)(get_engine());
    }

    Array<double> random(arg_type size) {
        return generate_array<double>(std::uniform_real_distribution<double>(0.0, 1.0), size);
    }

    Array<double> random(const std::vector<arg_type>& size) {
        return generate_array<double>(std::uniform_real_distribution<double>(0.0, 1.0), size);
    }

    // ---------------------- UNIFORM ----------------------
    Array<double> uniform(double low, double high, arg_type size) {
        return generate_array<double>(std::uniform_real_distribution<double>(low, high), size);
    }

    Array<double> uniform(double low, double high, const std::vector<arg_type>& size) {
        return generate_array<double>(std::uniform_real_distribution<double>(low, high), size);
    }

    Array<double> uniform(const std::vector<double>& low, const std::vector<double>& high,
                          const std::vector<arg_type>& size)
    {
        auto make_dist = [&](arg_type i) {
            return std::uniform_real_distribution<double>(
                low[i % low.size()],
                high[i % high.size()]
            );
        };
        return generate_array_broadcast<double>(make_dist, size);
    }

    // ---------------------- NORMAL ----------------------
    Array<double> normal(double mean, double stddev, arg_type size) {
        return generate_array<double>(std::normal_distribution<double>(mean, stddev), size);
    }

    Array<double> normal(double mean, double stddev, const std::vector<arg_type>& size) {
        return generate_array<double>(std::normal_distribution<double>(mean, stddev), size);
    }

    Array<double> normal(const std::vector<double>& mean,
                         const std::vector<double>& stddev,
                         const std::vector<arg_type>& size)
    {
        auto make_dist = [&](arg_type i) {
            return std::normal_distribution<double>(
                mean[i % mean.size()],
                stddev[i % stddev.size()]
            );
        };
        return generate_array_broadcast<double>(make_dist, size);
    }

    // ---------------------- BINOMIAL ----------------------
    Array<int> binomial(int n, double p, arg_type size) {
        return generate_array<int>(std::binomial_distribution<int>(n, p), size);
    }

    Array<int> binomial(int n, double p, const std::vector<arg_type>& size) {
        return generate_array<int>(std::binomial_distribution<int>(n, p), size);
    }

    Array<int> binomial(const std::vector<int>& n,
                        const std::vector<double>& p,
                        const std::vector<arg_type>& size)
    {
        auto make_dist = [&](arg_type i) {
            return std::binomial_distribution<int>(
                n[i % n.size()],
                p[i % p.size()]
            );
        };
        return generate_array_broadcast<int>(make_dist, size);
    }

    // ---------------------- POISSON ----------------------
    Array<int> poisson(double lam, arg_type size) {
        return generate_array<int>(std::poisson_distribution<int>(lam), size);
    }

    Array<int> poisson(double lam, const std::vector<arg_type>& size) {
        return generate_array<int>(std::poisson_distribution<int>(lam), size);
    }

    Array<int> poisson(const std::vector<double>& lam,
                       const std::vector<arg_type>& size)
    {
        auto make_dist = [&](arg_type i) {
            return std::poisson_distribution<int>(lam[i % lam.size()]);
        };
        return generate_array_broadcast<int>(make_dist, size);
    }

    // ---------------------- BETA ----------------------
    Array<double> beta(double a, double b, arg_type size) {
        Array<double> result(size);
        result.reshape({size});
        std::mt19937& eng = get_engine();
        std::gamma_distribution<double> dist_a(a, 1.0);
        std::gamma_distribution<double> dist_b(b, 1.0);

        for (arg_type i = 0; i < size; ++i) {
            double sample_a = dist_a(eng);
            double sample_b = dist_b(eng);
            result[i] = sample_a / (sample_a + sample_b);
        }

        return result;
    }

    Array<double> beta(double a, double b, const std::vector<arg_type>& size) {
        arg_type total = compute_total(size);
        Array<double> result(total);
        result.reshape(size);
        std::mt19937& eng = get_engine();
        std::gamma_distribution<double> dist_a(a, 1.0);
        std::gamma_distribution<double> dist_b(b, 1.0);

        for (arg_type i = 0; i < total; ++i) {
            double sample_a = dist_a(eng);
            double sample_b = dist_b(eng);
            result[i] = sample_a / (sample_a + sample_b);
        }

        return result;
    }

    Array<double> beta(const std::vector<double>& a,
                    const std::vector<double>& b,
                    const std::vector<arg_type>& size)
    {
        arg_type total = compute_total(size);
        Array<double> result(total);
        result.reshape(size);
        std::mt19937& eng = get_engine();

        for (arg_type i = 0; i < total; ++i) {
            double alpha = a[i % a.size()];
            double beta_val = b[i % b.size()];

            std::gamma_distribution<double> dist_a(alpha, 1.0);
            std::gamma_distribution<double> dist_b(beta_val, 1.0);

            double sample_a = dist_a(eng);
            double sample_b = dist_b(eng);
            result[i] = sample_a / (sample_a + sample_b);
        }

        return result;
    }


    // ---------------------- GEOMETRIC ----------------------
    Array<int> geometric(double p, arg_type size) {
        return generate_array<int>(std::geometric_distribution<int>(p), size);
    }

    Array<int> geometric(double p, const std::vector<arg_type>& size) {
        return generate_array<int>(std::geometric_distribution<int>(p), size);
    }

    Array<int> geometric(const std::vector<double>& p, const std::vector<arg_type>& size) {
        auto make_dist = [&](arg_type i) {
            return std::geometric_distribution<int>(p[i % p.size()]);
        };
        return generate_array_broadcast<int>(make_dist, size);
    }

    // ---------------------- TRIANGULAR ----------------------
    Array<double> triangular(double left, double mode, double right, arg_type size) {
        arg_type total = size;
        Array<double> result(size);
        result.reshape({size});

        std::mt19937& eng = get_engine();
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (arg_type i = 0; i < total; ++i) {
            double u = dist(eng);
            if (u < (mode - left) / (right - left))
                result[i] = left + std::sqrt(u * (right - left) * (mode - left));
            else
                result[i] = right - std::sqrt((1 - u) * (right - left) * (right - mode));
        }

        return result;
    }

    Array<double> triangular(double left, double mode, double right, const std::vector<arg_type>& size) {
        arg_type total = compute_total(size);
        Array<double> result(total);
        result.reshape(size);

        std::mt19937& eng = get_engine();
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (arg_type i = 0; i < total; ++i) {
            double u = dist(eng);
            if (u < (mode - left) / (right - left))
                result[i] = left + std::sqrt(u * (right - left) * (mode - left));
            else
                result[i] = right - std::sqrt((1 - u) * (right - left) * (right - mode));
        }

        return result;
    }

    Array<double> triangular(const std::vector<double>& left,
                             const std::vector<double>& mode,
                             const std::vector<double>& right,
                             const std::vector<arg_type>& size)
    {
        arg_type total = compute_total(size);
        Array<double> result(total);
        result.reshape(size);

        std::mt19937& eng = get_engine();
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (arg_type i = 0; i < total; ++i) {
            double l = left[i % left.size()];
            double m = mode[i % mode.size()];
            double r = right[i % right.size()];
            double u = dist(eng);

            if (u < (m - l) / (r - l))
                result[i] = l + std::sqrt(u * (r - l) * (m - l));
            else
                result[i] = r - std::sqrt((1 - u) * (r - l) * (r - m));
        }

        return result;
    }

    // ---------------------- EXPONENTIAL ----------------------
    Array<double> exponential(double scale, arg_type size) {
        return generate_array<double>(std::exponential_distribution<double>(1.0 / scale), size);
    }

    Array<double> exponential(double scale, const std::vector<arg_type>& size) {
        return generate_array<double>(std::exponential_distribution<double>(1.0 / scale), size);
    }

    Array<double> exponential(const std::vector<double>& scale, const std::vector<arg_type>& size) {
        auto make_dist = [&](arg_type i) {
            return std::exponential_distribution<double>(1.0 / scale[i % scale.size()]);
        };
        return generate_array_broadcast<double>(make_dist, size);
    }

    // ---------------------- CHOICE ----------------------
    Array<int> choice(const std::vector<int>& a,
                      const std::vector<arg_type>& size,
                      bool replace)
    {
        arg_type total = compute_total(size);
        Array<int> result(total);
        result.reshape(size);

        std::mt19937& eng = get_engine();

        if (replace) {
            std::uniform_int_distribution<arg_type> dist(0, a.size() - 1);
            for (arg_type i = 0; i < total; ++i)
                result[i] = a[dist(eng)];
        } else {
            std::vector<int> pool = a;
            std::shuffle(pool.begin(), pool.end(), eng);
            for (arg_type i = 0; i < total; ++i)
                result[i] = pool[i % pool.size()];
        }

        return result;
    }

    // ---------------------- SHUFFLE ----------------------
    void shuffle(Array<int>& arr) {
        std::mt19937& eng = get_engine();
        for (arg_type i = arr.size() - 1; i > 0; --i) {
            std::uniform_int_distribution<arg_type> dist(0, i);
            arg_type j = dist(eng);
            std::swap(arr[i], arr[j]);
        }
    }


    // ---------------------- PERMUTATION ----------------------
    Array<int> permutation(const Array<int>& arr) {
        Array<int> result = arr;
        shuffle(result);
        return result;
    }
}
