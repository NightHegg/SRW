from itertools import product, chain
from scipy.spatial import Delaunay
import numpy as np

splitCoef = 5
dimTask = 2
amntSubdomains = [2, 1]
coefOverlap = 0.3
E = 70e+9
nyu = 0.34
coef_u = 1000
coef_sigma = 1e-3

bounds = np.array([[0.01, 0], [0.02, 0], [0.02, 0.01], [0.01, 0.01], [0.01, 0]])
x_list, y_list = np.linspace(bounds[0, 0], bounds[1, 0], splitCoef), np.linspace(bounds[0, 1], bounds[2, 1], splitCoef)
points = np.array(list(product(x_list,y_list)))
tri = Delaunay(points)

dirichlet_conditions = [[0, 1], [1, 0], [3, 0]]
neumann_conditions = [[2, 0, -2e+7]]

with open("test_1.dat","w") as f:
    f.write(str(dimTask)+'\n')
    f.write("{:g}\n".format(len(bounds)))
    for point in bounds:
        f.write("{:g} {:g}\n".format(point[0], point[1]))
    f.write("{:d}\n".format(splitCoef))
    f.write("{:g}\n".format(coefOverlap))
    f.write("{:d} {:d}\n".format(amntSubdomains[0], amntSubdomains[1]))
    f.write("{:g} {:g}\n".format(E, nyu))
    f.write("{:g} {:g}\n".format(coef_u, coef_sigma))
    f.write("{:g}\n".format(len(dirichlet_conditions)))
    for cond in dirichlet_conditions:
        f.write("{:g} {:g}\n".format(cond[0], cond[1]))
    f.write("{:g}\n".format(len(neumann_conditions)))
    for cond in neumann_conditions:
        f.write("{:g} {:g} {:g}\n".format(cond[0], cond[1], cond[2]))
    f.write("{:d}\n".format(len(points)))
    for point in points:
        f.write("{:g} {:g}\n".format(point[0], point[1]))
    f.write("{:d}\n".format(len(tri.simplices)))
    for element in tri.simplices:
        f.write("{:g} {:g} {:g}\n".format(element[0], element[1], element[2]))