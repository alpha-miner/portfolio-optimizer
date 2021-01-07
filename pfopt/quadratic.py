# -*- coding: utf-8 -*-
"""
Created on 2021-01-06

@author: cheng.li
"""

from typing import Union
import cvxpy as cp
import numpy as np
from pfopt.base import _IOptimizer


class QOptimizer(_IOptimizer):

    def __init__(self,
                 cost: np.ndarray,
                 variance: np.ndarray,
                 penalty: float = 1.0,
                 cons_matrix: np.ndarray = None,
                 lower_bound: Union[float, np.ndarray] = None,
                 upper_bound: Union[float, np.ndarray] = None):
        super().__init__(cost, cons_matrix, lower_bound, upper_bound)
        self._variance = variance
        self._penalty = penalty

    def solve(self, solver: str = "ECOS"):
        x, constraints = self._prepare()
        prob = cp.Problem(cp.Minimize(x @ self._cost + 0.5 * self._penalty * cp.quad_form(x, self._variance)),
                          constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value, prob.status


class DecomposedQOptimizer(_IOptimizer):

    def __init__(self,
                 cost: np.ndarray,
                 factor_var: np.ndarray,
                 factor_load: np.ndarray,
                 factor_special: np.ndarray,
                 penalty: float = 1.0,
                 cons_matrix: np.ndarray = None,
                 lower_bound: Union[float, np.ndarray] = None,
                 upper_bound: Union[float, np.ndarray] = None):
        super().__init__(cost, cons_matrix, lower_bound, upper_bound)
        self._factor_var = factor_var
        self._factor_load = factor_load
        self._factor_special = factor_special
        self._penalty = penalty

    def solve(self, solver: str = "ECOS"):
        x, constraints = self._prepare()
        risk = cp.sum_squares(cp.multiply(self._factor_special, x)) \
               + cp.quad_form((x.T * self._factor_load).T, self._factor_var)
        prob = cp.Problem(cp.Minimize(x @ self._cost + 0.5 * self._penalty * risk),
                          constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value, prob.status
