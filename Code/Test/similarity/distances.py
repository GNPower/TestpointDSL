from math import dist
import numpy as np

from .frechet import linear_frechet, frechet_euclidean


def FrechetDist(x: list, y1: list, y2: list) -> float:
    points_y1 = [[x[i], y1[i]] for i in range(len(x))]
    points_y2 = [[x[i], y2[i]] for i in range(len(x))]
    return linear_frechet(np.asarray(points_y1), np.asarray(points_y2), frechet_euclidean)
    

def EuclidianDistance(x: list, y1: list, y2: list) -> float:
    points_y1 = [[x[i], y1[i]] for i in range(len(x))]
    points_y2 = [[x[i], y2[i]] for i in range(len(x))]
    ed = 0
    for i in range(len(x)):
        ed += dist(points_y1[i], points_y2[i])
    return ed


def DynamicTimeWarping(y1: list, y2: list) -> float:
    # Fill the matrix
    l_s_1, l_s_2 = len(y1), len(y2)
    cost_matrix = np.zeros((l_s_1+1, l_s_2+1))
    for i in range(l_s_1+1):
        for j in range(l_s_2+1):
            cost_matrix[i, j] = np.inf
    cost_matrix[0, 0] = 0

    # print("Initial Cost")
    # print(cost_matrix)

    for i in range(1, l_s_1+1):
        for j in range(1, l_s_2+1):
            cost = abs(y1[i-1] - y2[j-1])
            #take last min from the window
            prev_min = np.min([cost_matrix[i-1, j], cost_matrix[i, j-1], cost_matrix[i-1, j-1]])
            cost_matrix[i, j] = cost + prev_min

    # print("Populated Cost")
    # print(cost_matrix)

    # Walk the cost matrix
    walk = []
    l_i, l_j = len(y1), len(y2)
    while(l_i > 0 and l_j > 0):
        walk.append(cost_matrix[l_i, l_j])
        mi = cost_matrix[l_i-1, l_j] if l_i > 0 else np.inf
        mj = cost_matrix[l_i, l_j-1] if l_j > 0 else np.inf
        mij = cost_matrix[l_i-1, l_j-1] if (l_i > 0 and l_j > 0) else np.inf
        if mi <= mj and mi <= mij:
            l_i -= 1
        elif mj <= mi and mj <= mij:
            l_j -= 1
        else:
            l_i -= 1
            l_j -= 1
    walk.append(cost_matrix[l_i, l_j])

    # Return the sum
    return sum(walk)
