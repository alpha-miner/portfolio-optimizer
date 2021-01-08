# -*- coding: utf-8 -*-
"""
Created on 2021-01-06

@author: cheng.li
"""

from typing import Union
import cvxpy as cp
import numpy as np
from pfopt.base import _IOptimizer
from simpleutils.asserts import require


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
        risk = cp.sum_squares(cp.multiply(np.sqrt(self._factor_special), x)) \
               + cp.quad_form((x.T @ self._factor_load).T, self._factor_var)
        prob = cp.Problem(cp.Minimize(x @ self._cost + 0.5 * self._penalty * risk),
                          constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value, prob.status


class TargetVarianceOptimizer(_IOptimizer):

    def __init__(self,
                 cost: np.ndarray,
                 variance_target: float,
                 factor_var: np.ndarray = None,
                 factor_load: np.ndarray = None,
                 factor_special: np.ndarray = None,
                 variance: np.ndarray = None,
                 cons_matrix: np.ndarray = None,
                 lower_bound: Union[float, np.ndarray] = None,
                 upper_bound: Union[float, np.ndarray] = None):
        super().__init__(cost, cons_matrix, lower_bound, upper_bound)
        require(factor_var is not None or variance is not None,
                ValueError,
                "factor var or total var should not all be empty")
        if factor_var is not None:
            self._factor_var = factor_var
            self._factor_load = factor_load
            self._factor_special = factor_special
            self._use_factor = True
        else:
            self._variance = variance
            self._use_factor = False
        require(variance_target >= 0, ValueError, "variance target can't be negative")
        self._var_target = variance_target

    def solve(self, solver: str = "ECOS"):
        x, constraints = self._prepare()
        if self._use_factor:
            risk = cp.sum_squares(cp.multiply(np.sqrt(self._factor_special), x)) \
                   + cp.quad_form((x.T @ self._factor_load).T, self._factor_var)
        else:
            risk = cp.quad_form(x, self._variance)
        constraints.append(risk <= self._var_target)
        prob = cp.Problem(cp.Minimize(x @ self._cost),
                          constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value, prob.status
