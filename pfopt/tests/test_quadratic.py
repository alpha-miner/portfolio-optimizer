# -*- coding: utf-8 -*-
"""
Created on 2021-01-06

@author: cheng.li
"""

import numpy as np
from pfopt.quadratic import (
    QOptimizer,
    DecomposedQOptimizer,
    TargetVarianceOptimizer
)


def test_qoptimizer():
    cost = np.array([-0.01, -0.02, -0.03])
    cov = np.array([[0.05, 0.01, 0.02],
                    [0.01, 0.06, 0.03],
                    [0.02, 0.03, 0.07]])
    lower_bound = 0.0
    upper_bound = 0.5
    optimizer = QOptimizer(cost, cov, lower_bound=lower_bound, upper_bound=upper_bound)
    x, f_eval, status = optimizer.solve()
    x_expected = np.array([0.027397, 0.150685, 0.356164])
    np.testing.assert_array_almost_equal(x, x_expected, 5)


def test_decomposed_qoptimizer():
    cost = np.array([-0.1, -0.2, -0.3])
    constraints = np.array([[1., 1., 1., 1., 1.]])
    factor_var = np.array([[0.5, -0.3], [-0.3, 0.7]])
    factor_load = np.array([[0.8, 0.2], [0.5, 0.5], [0.2, 0.8]])
    factor_special = np.array([0.1, 0.3, 0.2])
    lower_bound = 0.0
    upper_bound = 1.0

    optimizer = DecomposedQOptimizer(cost,
                                     factor_var=factor_var,
                                     factor_load=factor_load,
                                     factor_special=factor_special,
                                     lower_bound=lower_bound,
                                     upper_bound=upper_bound,
                                     cons_matrix=constraints)
    x, f_eval, status = optimizer.solve()

    x_expected = np.array([0.2866857 , 0.21416417, 0.49915014])
    np.testing.assert_array_almost_equal(x, x_expected, 5)


def test_target_variance_optimizer_simple_case():
    cost = np.array([-0.1, -0.2, -0.3])
    cov = np.array([[0.05, 0.01, 0.02],
                    [0.01, 0.06, 0.03],
                    [0.02, 0.03, 0.07]])
    lower_bound = -0.3
    upper_bound = 0.5
    target_variance = 0.01

    optimizer = TargetVarianceOptimizer(cost=cost,
                                        variance=cov,
                                        lower_bound=lower_bound,
                                        upper_bound=upper_bound,
                                        variance_target=target_variance)
    x, f_eval, status = optimizer.solve()
    x_expected = np.array([0.0231776, 0.1274768, 0.30130881])
    np.testing.assert_array_almost_equal(x, x_expected, 5)


def test_target_variance_optimizer_with_linear_constraints():
    cost = np.array([-0.1, -0.2, -0.3])
    cov = np.array([[0.05, 0.01, 0.02],
                    [0.01, 0.06, 0.03],
                    [0.02, 0.03, 0.07]])
    lower_bound = -0.3
    upper_bound = 0.5
    target_variance = 0.01
    constraints = np.array([[1., 1., 1., 0., 0.]])

    optimizer = TargetVarianceOptimizer(cost=cost,
                                        variance=cov,
                                        cons_matrix=constraints,
                                        lower_bound=lower_bound,
                                        upper_bound=upper_bound,
                                        variance_target=target_variance)
    x, f_eval, status = optimizer.solve()
    x_expected = np.array([-0.3000000, -0.10919033, 0.40919033])
    np.testing.assert_array_almost_equal(x, x_expected, 5)


def test_target_variance_optimizer_with_factor_model():
    cost = np.array([-0.1, -0.2, -0.3])
    lower_bound = 0.0
    upper_bound = 1.0
    target_variance = 0.25
    constraints = np.array([[1., 1., 1., 1., 1.]])
    factor_var = np.array([[0.5, -0.3], [-0.3, 0.7]])
    factor_load = np.array([[0.8, 0.2], [0.5, 0.5], [0.2, 0.8]])
    factor_special = np.array([0.1, 0.3, 0.2])

    optimizer = TargetVarianceOptimizer(cost=cost,
                                        factor_var=factor_var,
                                        factor_load=factor_load,
                                        factor_special=factor_special,
                                        variance_target=target_variance,
                                        lower_bound=lower_bound,
                                        upper_bound=upper_bound,
                                        cons_matrix=constraints)
    x, f_eval, status = optimizer.solve()
    x_expected = np.array([0.26595552, 0.21675092, 0.51729356])
    np.testing.assert_array_almost_equal(x, x_expected, 5)
