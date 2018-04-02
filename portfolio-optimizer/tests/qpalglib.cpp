//
// Created by wegamekinglc on 18-3-29.
//

#include <gtest/gtest.h>
#include <pfopt/qpalglib.hpp>

using namespace pfopt;


TEST(QPAlglibTest, SimpleCase) {
    constexpr int n = 3;
    double er[] = {0.01, 0.02, 0.03};
    double cov[] = {0.05, 0.01, 0.02, 0.01, 0.06, 0.03, 0.02, 0.03, 0.07};
    double lb[] = {0., 0., 0.};
    double ub[] = {0.5, 0.5, 0.5};

    QPAlglib opt(n, er, cov, lb, ub);

    auto x_values = opt.xValue();

    double x_expected[] = {0.027397, 0.150685, 0.356164};

    for(size_t i=0; i!=n; ++i) {
        ASSERT_NEAR(x_expected[i], x_values[i], 1e-6);
    }
}