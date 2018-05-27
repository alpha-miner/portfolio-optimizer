//
// Created by wegamekinglc on 18-5-25.
//

#include "gtest/gtest.h"
#include <pfopt/utilities.hpp>
#include <iostream>

using pfopt::calculate_grad;


TEST(TestUtilities, TestCalcGrad) {
    VectorXd w(3);
    w << 0.33, 0.33, 0.33;
    MatrixXd factorLoading(3, 2);
    MatrixXd factorVarMatrix(2, 2);
    VectorXd idynsc(3);

    factorLoading << 0.8, 0.2, 0.5, 0.5, 0.2, 0.8;
    factorVarMatrix << 0.5, -0.3, -0.3, 0.7;
    idynsc << 0.0, 0.0, 0.0;

    VectorXd res = calculate_grad(w, factorLoading, factorVarMatrix, idynsc);
    double x_expected[] = {0.1188, 0.1485, 0.1782};
    for(size_t i=0; i != 3; ++i) {
        ASSERT_DOUBLE_EQ(res(i), x_expected[i]);
    }
}