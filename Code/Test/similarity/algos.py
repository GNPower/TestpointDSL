import numpy as np


def CrossCorrelation(y1: list, y2: list) -> float:
    corr = np.correlate(y1, y2)
    return np.max(corr)


def Pipe(y1: list, y2: list, pipe_range: float = 0.1, pipe_type: bool = False) -> bool:
    for i in range(len(y1)):
        if pipe_type: # Absolute Pipe Range
            max = y1[i] + pipe_range
            min = y1[i] - pipe_range
        else:
            max = y1[i] + (y1[i]*pipe_range)
            min = y1[i] - (y1[i]*pipe_range)
        if y2[i] > max or y2[i] < min:
                return False
    return True


def PointToPoint(y1: list, y2: list, t_tol: int, s_tol: float) -> bool: # WCOMP
    for i in range(len(y1)):
        i_min = i - t_tol if i >= t_tol else 0
        i_max = i + t_tol if i + t_tol < len(y1) else len(y1)-1
        s_min = y1[i] - s_tol
        s_max = y1[i] + s_tol
        passed = False
        for j in range(i_min, i_max+1):
            if y2[j] >= s_min and y2[j] <= s_max:
                passed = True
                break
        if passed == False:
            return False
    return True


def RectangularTolerances(y1: list, y2: list, tneg_tol: int, tpos_tol: int, sneg_tol: float, spos_tol: float) -> float: # Wiechewski et. al.
    for i in range(len(y1)):
        i_min = i - tneg_tol if i >= tneg_tol else 0
        i_max = i + tpos_tol if i + tpos_tol < len(y1) else len(y1)-1
        s_min = y1[i] - sneg_tol
        s_max = y1[i] + spos_tol
        passed = False
        for j in range(i_min, i_max+1):
            if y2[j] >= s_min and y2[j] <= s_max:
                passed = True
                break
        if passed == False:
            return False
    return True
