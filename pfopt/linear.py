# -*- coding: utf-8 -*-
"""
Created on 2021-01-05

@author: cheng.li
"""

import abc
from typing import Union
import cvxpy as cp
import numpy as np
from simpleutils.asserts import require


class _ILpOptimizer(abc.ABC):

    def __init__(self,
                 cost: np.ndarray,
                 cons_matrix: np.ndarray = None,
                 lower_bound: Union[float, np.ndarray] = None,
                 upper_bound: Union[float, np.ndarray] = None):
        self._n = len(cost)
        self._cons_matrix = cons_matrix
        self._lower_bound = lower_bound
        self._upper_bound = upper_bound
        self._cost = cost

    def _prepare(self):
        x = cp.Variable(self._n)
        constraints = []
        if self._lower_bound is not None:
            require(isinstance(self._lower_bound, float) or len(self._lower_bound) == self._n,
                    ValueError,
                    "lower bounds must be a single value or an array with same size as x")
            constraints.append(x >= self._lower_bound)

        if self._upper_bound is not None:
            require(isinstance(self._upper_bound, float) or len(self._upper_bound) == self._n,
                    ValueError,
                    "upper bounds must be a single value or an array with same size as x")
            constraints.append(x <= self._upper_bound)

        if self._cons_matrix is not None:
            require(self._cons_matrix.shape[1] == self._n + 2,
                    ValueError,
                    "constraints must be a matrix with size as x + 2")
            constraints.append(self._cons_matrix[:, :self._n] @ x >= self._cons_matrix[:, self._n])
            constraints.append(self._cons_matrix[:, :self._n] @ x <= self._cons_matrix[:, self._n + 1])

        return x, constraints


class LpOptimizer(_ILpOptimizer):

    def __init__(self,
                 cost: np.ndarray,
                 cons_matrix: np.ndarray = None,
                 lower_bound: Union[float, np.ndarray] = None,
                 upper_bound: Union[float, np.ndarray] = None):
        super().__init__(cost, cons_matrix, lower_bound, upper_bound)

    def solve(self, solver: str = "CBC"):
        x, constraints = self._prepare()
        prob = cp.Problem(cp.Minimize(x @ self._cost), constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value, prob.status


class L1LpOptimizer(_ILpOptimizer):

    def __init__(self,
                 cost: np.ndarray,
                 benchmark: np.ndarray,
                 l1norm: float,
                 cons_matrix: np.ndarray = None,
                 lower_bound: Union[float, np.ndarray] = None,
                 upper_bound: Union[float, np.ndarray] = None):
        super().__init__(cost, cons_matrix, lower_bound, upper_bound)
        self._benchmark = benchmark
        self._l1norm = l1norm

    def solve(self, solver: str = "ECOS"):
        x, constraints = self._prepare()
        constraints.append(
            cp.pnorm(x - self._benchmark, 1) <= self._l1norm
        )
        prob = cp.Problem(cp.Minimize(x @ self._cost), constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value, prob.status
