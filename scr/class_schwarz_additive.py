import os
import sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))

from itertools import combinations
import numpy as np
from scipy.sparse import linalg

import scr.functions as base_func
from scr.class_schwarz_multiplicative import schwarz_multiplicative


class schwarz_additive(schwarz_multiplicative):
    def __init__(self, cur_task, cur_mesh, cur_amnt_subds = [2, 1], coef_convergence = 1e-3, coef_alpha = 0.5, solve_function = linalg.spsolve):
        super().__init__(cur_task, cur_mesh, cur_amnt_subds, coef_convergence, solve_function)
        
        self.name_method = "schwarz additive method"
        self.coef_alpha = coef_alpha


    def internal_init(self):
        self.u_previous = np.copy(self.u)
        self.u_current = np.copy(self.u)
        self.u_sum = np.zeros_like(self.u)


    def internal_additional(self):
        self.u_sum += (self.u_current - self.u_previous)
        self.u_current = np.copy(self.u_previous)


    def internal_condition_schwarz(self, K, F, idv, ratioPoints_GlobalLocal):
        listPoints_Schwarz = sum([list(set(self.subd_boundary_overlap_points[idv]) & set(subd)) for idx, subd in enumerate(self.subd_points) if idx != idv], [])

        for node in listPoints_Schwarz:
            for dim in range(self.dimTask):
                K, F = base_func.bound_condition_dirichlet(K, F, self.dimTask, ratioPoints_GlobalLocal[node], self.u_previous[node, dim], dim)
        
        return K, F

  
    def interal_calculate_u(self):
        self.u = np.copy(self.u_previous) + (self.coef_alpha * np.copy(self.u_sum))


if __name__ == "__main__":
    pass