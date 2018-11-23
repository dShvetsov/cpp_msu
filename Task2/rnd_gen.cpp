#include <any>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils.h"
#include "zip.h"

using PySTL::zip;

class TRandomNumberGenerator {
public:
    virtual ~TRandomNumberGenerator() = default;

    /// Generate number
    virtual double Generate() const = 0;

    // Get theoretical mean number
    virtual double Mean() const = 0;
protected:
    double uniform_rand() const {
        return double(std::rand()) / RAND_MAX;
    }
};

using TRandomNumberGeneratorPtr = std::shared_ptr<TRandomNumberGenerator>;
using TRandomNumberGeneratorUniquePtr = std::unique_ptr<TRandomNumberGenerator>;

class Factory {
public:
    template<class... Args>
    TRandomNumberGeneratorUniquePtr Create(const std::string& type, Args&&... args) {
        std::vector<std::any> params{std::forward<Args>(args)...};
        try {
            return funcs.at(type)(params);
        } catch(std::out_of_range& oor) {
            std::cerr << "Such distrution doesn't exist: " << type << std::endl;
        } catch (std::invalid_argument& ia) {
            std::cerr << "Invald argument: " << ia.what() << std::endl;
        } catch (std::bad_any_cast& bac) {
            std::cerr << "This arguments have not satisfy type" << std::endl;
        }
        return nullptr;
    }

    using GenerateFunc = std::function<TRandomNumberGeneratorUniquePtr(std::vector<std::any>&)>;

    void AddDistribution(std::string name, GenerateFunc f) {
        funcs.insert({name, f});
    }

private:
    std::unordered_map<std::string, GenerateFunc> funcs;
};

// Singleton
Factory* getFactory() {
    static Factory f;
    return &f;
}

class PoisonRandomGenerator : public TRandomNumberGenerator {
public:
    PoisonRandomGenerator(double lambda) : m_lambda(lambda) {
         CHECK_ARG(lambda > 0);
    }
    double Generate() const override {
        double L = std::exp(-m_lambda);
        double p = 1.0;
        int k = 0;
        do {
            k++;
            p *= uniform_rand();
        } while (p > L);
        return k - 1;
    }

    double Mean() const override {
        return m_lambda;
    }

    static void RegisterInFactory() {
        auto f = getFactory();
        f->AddDistribution("poison", [](std::vector<std::any>& v) {
            CHECK_ARG(v.size() == 1);
            double lambda = std::any_cast<double>(v[0]);
            return std::make_unique<PoisonRandomGenerator>(lambda);
        });
    }

private:
    const double m_lambda;
};

class BernoulliRandomGenerator : public TRandomNumberGenerator {
public:
    BernoulliRandomGenerator(double p) : m_p(p)
    {
        CHECK_ARG(p >= 0);
        CHECK_ARG(p <= 1);
    }

    double Generate() const override {
        return uniform_rand() > m_p ? 0.0 : 1.0;
    }

    double Mean() const override {
        return m_p;
    }

    static void RegisterInFactory() {
        auto f = getFactory();
        f->AddDistribution("bernoulli", [](std::vector<std::any>& v) {
            CHECK_ARG(v.size() == 1);
            double p = std::any_cast<double>(v[0]);
            return std::make_unique<BernoulliRandomGenerator>(p);
        });
    }
private:
    const double m_p;
};

class GeometricRandomGenerator : public TRandomNumberGenerator {
public:
    GeometricRandomGenerator(double p) : m_p(p)
    {
        CHECK_ARG(p > 0 && p <= 1);
    }

    double Generate() const override {
        //return std::floor( log(1 - m_p, uniform_rand() / m_p) );
        return std::floor( log(1 - m_p, 1 - uniform_rand()) );
    }

    double Mean() const override {
        return (1 - m_p) / m_p;
    }

    static void RegisterInFactory() {
        auto f = getFactory();
        f->AddDistribution("geometric", [](std::vector<std::any>& v) {
            CHECK_ARG(v.size() == 1);
            double p = std::any_cast<double>(v[0]);
            return std::make_unique<GeometricRandomGenerator>(p);
        });
    }

private:
    const double m_p;
};

class FiniteRandomGenerator : public TRandomNumberGenerator {
public:
    FiniteRandomGenerator(std::vector<double>& values, std::vector<double>& probs)
    {
        CHECK_ARG(values.size() == probs.size());
        CHECK_ARG(equal(sum(probs), 1));
        double glob_p = 0;
        for (auto [v, p] : zip(values, probs)) {
            glob_p += p;
            m_values[glob_p] = v;
        }
    }

    double Generate() const override {
        return m_values.upper_bound(uniform_rand())->second;
    }

    double Mean() const override {
        double mean = 0;
        double last = 0;
        for (auto [p, v] : m_values) {
            mean += ((p - last)  * v);
            last = p;
        }
        return mean;
    }

    static void RegisterInFactory() {
        auto f = getFactory();
        f->AddDistribution("finite", [](std::vector<std::any>& v) {
            CHECK_ARG(v.size() == 2);
            auto values = std::any_cast<std::vector<double>>(v[0]);
            auto probs  = std::any_cast<std::vector<double>>(v[1]);
            return std::make_unique<FiniteRandomGenerator>(values, probs);
        });
    }

private:
    std::map<double, double> m_values;
};

double empirical_mean(TRandomNumberGeneratorPtr generator, size_t count=10000000)
{
    CHECK_ARG(generator != nullptr);
    double emp_mean = 0;
    for (size_t i = 0; i < count; i++) {
        emp_mean += (generator->Generate() / count);
    }
    return emp_mean;
}

void compare_empirical_and_theoretical_mean(TRandomNumberGeneratorPtr generator, double eps = 0.005)
{
    double emp_mean = empirical_mean(generator);
    double theor_mean = generator->Mean();
    double diff = std::abs(emp_mean - theor_mean);
    terminal::settings color = (diff < eps) ? terminal::settings::green : terminal::settings::red;
    std::cout << terminal::setup{color}
              << "  Emperical mean  : " << emp_mean << std::endl
              << "  Theoretical mean: " << theor_mean << std:: endl
              << "  Difference is   : " << diff << std::endl
              << std::endl;
}

int main() {
    PoisonRandomGenerator::RegisterInFactory();
    BernoulliRandomGenerator::RegisterInFactory();
    GeometricRandomGenerator::RegisterInFactory();
    FiniteRandomGenerator::RegisterInFactory();

    std::cout << "Test Poison distribution" << std::endl;
    TRandomNumberGeneratorPtr distr;
    for (double lambda : {1, 2, 3, 4, 5}) {
        distr = getFactory()->Create("poison", lambda);
        compare_empirical_and_theoretical_mean(distr);
    }

    std::cout << "Test Bernoulli distribution" << std::endl;
    for (double p : {0.1, 0.2, 0.5, 0.8, 0.99}) {
        distr = getFactory()->Create("bernoulli", p);
        compare_empirical_and_theoretical_mean(distr);
    }

    std::cout << "Test Geometric distribution" << std::endl;
    for (double p : {0.1, 0.2, 0.5, 0.8, 0.99}) {
        distr = getFactory()->Create("geometric", p);
        compare_empirical_and_theoretical_mean(distr);
    }

    std::cout << "Test Finite distribution" << std::endl;
    {
    std::vector<double> values{1,2,3,4,5};
    std::vector<double> probs{0.2,0.2,0.2,0.2,0.2};
    distr = getFactory()->Create("finite", values, probs);
    compare_empirical_and_theoretical_mean(distr);
    }
    {
    std::vector<double> values{1,2,3,4,5};
    std::vector<double> probs{0.1,0.3,0.1,0.3,0.2};
    distr = getFactory()->Create("finite", values, probs);
    compare_empirical_and_theoretical_mean(distr);
    }
    {
    std::vector<double> values{1,2,3,4,5};
    std::vector<double> probs{0.0,0.0,0.0,0.0,1.0};
    distr = getFactory()->Create("finite", values, probs);
    compare_empirical_and_theoretical_mean(distr);
    }
    {
    std::vector<double> values{1,2};
    std::vector<double> probs{0.5,0.5};
    distr = getFactory()->Create("finite", values, probs);
    compare_empirical_and_theoretical_mean(distr);
    }
    return 0;
}

