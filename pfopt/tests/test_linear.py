# -*- coding: utf-8 -*-
"""
Created on 2021-01-05

@author: cheng.li
"""

import numpy as np
import pytest
from pfopt.linear import LpOptimizer


def test_lpoptimizer():
    n = 10
    lower_bound = np.ones(n) * 0.0
    upper_bound = np.ones(n) * 0.2
    constraints = np.zeros((1, n + 2))
    expectation = np.zeros(n)

    for i in range(n):
        expectation[i] = float(i)
        constraints[0, i] = 1.
    constraints[0, n] = 1.
    constraints[0, n + 1] = 1.0

    optimizer = LpOptimizer(expectation, constraints, lower_bound, upper_bound)
    x, f_eval = optimizer.solve()

    pytest.approx(f_eval, 2., 1e-8)
    for i in range(n):
        if i < 5:
            pytest.approx(x[i], 0.2, 1e-8)
        else:
            pytest.approx(x[i], 0.0, 1e-8)
