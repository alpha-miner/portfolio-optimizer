#include "gtest/gtest.h"
#include <pfopt/lpoptimizer.hpp>


using namespace std;
using namespace pfopt;


TEST(LpOptimizerTest, SimpleCase) {
    size_t n = 10;
    vector<double> consMat(n+2, 1.);
    vector<double> lower(n, 0.);
    vector<double> upper(n, 0.2);
    vector<double> objective(n);

    for(size_t i=0; i != n; ++i) {
        objective[i] = static_cast<double>(i);
    }

    LpOptimizer optimizer(consMat, lower, upper, objective);

    auto x = optimizer.xValue();
    auto f_val = optimizer.feval();

    for(size_t i=0; i!=x.size(); ++i) {
        if(i < 5)
            ASSERT_DOUBLE_EQ(x[i], 0.2);
        else
            ASSERT_DOUBLE_EQ(x[i], 0.);
    }

    ASSERT_DOUBLE_EQ(f_val, 2.);
}