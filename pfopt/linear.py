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

    def solve(self, solver: str = None):
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

        prob = cp.Problem(cp.Maximize(x @ self._expectation), constraints=constraints)
        prob.solve(solver=solver)
        return x.value, prob.value


if __name__ == "__main__":
    import datetime as dt

    def time_function(py_callable, n):
        start = dt.datetime.now()
        result = py_callable(n)
        elapsed = (dt.datetime.now() - start).total_seconds()
        return elapsed, result


    def cvxpy_lp(n):

        lower_bound = np.zeros(n)
        upper_bound = 0.01 * np.ones(n)
        risk_constraints1 = np.ones((n + 2, 1))
        risk_constraints2 = np.zeros((n + 2, 1))
        risk_constraints2[0][0] = 1.
        risk_constraints2[1][0] = 1.

        risk_constraints1[n][0] = 1.
        risk_constraints1[n+1][0] = 1.
        risk_constraints2[n][0] = 0.015
        risk_constraints2[n + 1][0] = 0.015
        constraints = np.concatenate((risk_constraints1, risk_constraints2), axis=1).T

        np.random.seed(1)
        er = np.random.randn(n)

        optimizer = LpOptimizer(er, constraints, lower_bound, upper_bound)
        return optimizer.solve(solver=cp.CBC)


    print("{0:<8}{1:>12}{2:>12}{3:>12}{4:>12}{5:>12}{6:>15}".format('Scale(n)', 'time(ms)', 'feval', 'min(x)', 'max(x)',
                                                                    'sum(x)', 'x(0) + x(1)'))

    for n in range(200, 3200, 200):
        elapsed, result = time_function(cvxpy_lp, n)
        s = np.array(result[0]).flatten()
        print("{0:<8}{1:>12.2f}{2:>12.2f}{3:>12f}{4:>12f}{5:>12f}{6:>15}".format(n, elapsed * 1000, result[1],
                                                                                 s.min(), s.max(), s.sum(),
                                                                                 s[0] + s[1]))