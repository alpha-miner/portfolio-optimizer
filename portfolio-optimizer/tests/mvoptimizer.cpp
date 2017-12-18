#include "gtest/gtest.h"
#include <pfopt/mvoptimizer.hpp>

using namespace pfopt;


TEST(MVOptimizerTest, SimpleCase) {
    constexpr int n = 3;
    double er[] = {0.1, 0.2, 0.3};
    double cov[] = {0.05, 0.01, 0.02, 0.01, 0.06, 0.03, 0.02, 0.03, 0.07};
    double lb[] = {0., 0., 0.};
    double ub[] = {0.5, 0.5, 0.5};

    constexpr int n_cons = 1;
    double cons_matrix[] = {1., 1., 1.};

    double clb[] = {1.};
    double cub[] = {1.};

    MVOptimizer opt(n, er, cov, lb, ub, n_cons, cons_matrix, clb, cub);

    auto x_values = opt.xValue();
    auto y_value = opt.feval();

    double x_expected[] = {0., 0.5, 0.5};
    double y_expected = -0.22625;


    for(size_t i=0; i!=n; ++i) {
        ASSERT_DOUBLE_EQ(x_values[i], x_expected[i]);
    }

    ASSERT_NEAR(y_value, y_expected, 1e-8);
}

TEST(MVOptimizerTest, SimpleCaseWithDefaultValue) {
    constexpr int n = 3;
    double er[] = {-0.02, 0.01, 0.03};
    double cov[] = {1., 0., 0., 0., 1., 0., 0., 0., 1.};
    double lb[] = {-1e10, -1e10, -1e10};
    double ub[] = {1e10, 1e10, 1e10};

    constexpr int n_cons = 1;

    MVOptimizer opt(n, er, cov, lb, ub, n_cons);

    auto x_values = opt.xValue();
    auto y_value = opt.feval();
    auto y_expected = -0.0007;

    double x_expected[] = {-0.02, 0.01, 0.03};

    for(size_t i=0; i!=n; ++i) {
        ASSERT_DOUBLE_EQ(x_values[i], x_expected[i]);
    }

    ASSERT_NEAR(y_value, y_expected, 1e-8);
}