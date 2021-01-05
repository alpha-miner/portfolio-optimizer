# -*- coding: utf-8 -*-
"""
Created on 2021-01-05

@author: cheng.li
"""

from typing import Union
import cvxpy as cp
import numpy as np
from simpleutils.asserts import require


class LpOptimizer:

    def __init__(self,
                 expectation: np.ndarray,
                 cons_matrix: np.ndarray = None,
                 lower_bound: Union[float, np.ndarray] = None,
                 upper_bound: Union[float, np.ndarray] = None):

        self._n = len(expectation)
        self._cons_matrix = cons_matrix
        self._lower_bound = lower_bound
        self._upper_bound = upper_bound
        self._expectation = expectation

    def solve(self, solver: str = "CBC"):
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

        prob = cp.Problem(cp.Minimize(x @ self._expectation), constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value
