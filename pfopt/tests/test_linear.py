# -*- coding: utf-8 -*-
"""
Created on 2021-01-05

@author: cheng.li
"""

import numpy as np
import pytest
from pfopt.linear import (
    LpOptimizer,
    L1LpOptimizer
)


def test_lpoptimizer():
    n = 10
    lower_bound = np.ones(n) * 0.0
    upper_bound = np.ones(n) * 0.2
    constraints = np.zeros((1, n + 2))
    cost = np.zeros(n)

    for i in range(n):
        cost[i] = float(i)
        constraints[0, i] = 1.
    constraints[0, n] = 1.
    constraints[0, n + 1] = 1.0

    optimizer = LpOptimizer(cost, constraints, lower_bound, upper_bound)
    x, f_eval, status = optimizer.solve()

    pytest.approx(f_eval, 2., 1e-8)
    for i in range(n):
        if i < 5:
            pytest.approx(x[i], 0.2, 1e-8)
        else:
            pytest.approx(x[i], 0.0, 1e-8)


def test_l1lpoptimizer():
    n = 300
    cost = np.random.randn(n)
    risk_exp = np.random.randn(n, 30)
    risk_exp = np.concatenate([risk_exp, np.ones((n, 1))], axis=1)
    bm = np.random.randint(100, size=n).astype(float)

    bm = bm / bm.sum()
    turn_over_target = 0.1

    risk_lbound = bm @ risk_exp
    risk_ubound = bm @ risk_exp

    risk_tolerance = 0.01 * np.abs(risk_lbound[:-1])

    risk_lbound[:-1] = risk_lbound[:-1] - risk_tolerance
    risk_ubound[:-1] = risk_ubound[:-1] + risk_tolerance
    constraints = np.concatenate([risk_exp, risk_lbound.reshape((1, -1)), risk_ubound.reshape((1, -1))]).T

    optimizer = L1LpOptimizer(cost,
                              benchmark=bm,
                              l1norm=turn_over_target,
                              cons_matrix=constraints,
                              lower_bound=0.00,
                              upper_bound=0.01)

    x, f_eval, status = optimizer.solve()
    assert status == "optimal"
    pytest.approx(np.sum(x), 1., 1e-8)
    assert np.all(x <= 0.01 + 1e-8)
    assert np.all(x >= -1e-8)
    assert np.sum(np.abs(x - bm)) <= turn_over_target + 1e-8
    calc_risk = (x - bm) @ risk_exp
    assert np.all(calc_risk[:-1] <= risk_tolerance)
    assert np.all(calc_risk[-1] <= 1e-8)


