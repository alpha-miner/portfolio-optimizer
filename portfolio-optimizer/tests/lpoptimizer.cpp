#include "gtest/gtest.h"
#include <pfopt/lpoptimizer.hpp>


using namespace std;
using namespace pfopt;


TEST(LpOptimizerTest, SimpleCase) {
    constexpr size_t n = 10;
    double consMat[n+2];
    double lower[n];
    double upper[n];
    double objective[n];

    for(size_t i=0; i != n; ++i) {
        consMat[i] = 1.;
        lower[i] = 0.;
        upper[i] = 0.2;
        objective[i] = static_cast<double>(i);
    }

    consMat[n] = 1.;
    consMat[n+1] = 1.;

    LpOptimizer optimizer(n, 1, consMat, lower, upper, objective);

    auto x = optimizer.xValue();
    auto f_val = optimizer.feval();

    for(size_t i=0; i != n; ++i) {
        if(i < 5)
            ASSERT_DOUBLE_EQ(x[i], 0.2);
        else
            ASSERT_DOUBLE_EQ(x[i], 0.);
    }

    ASSERT_DOUBLE_EQ(f_val, 2.);
}