import numpy as np

from .errors import MeanSquaredError


def CrossCorrelation(x: list, y1: list, y2: list) -> float:
    corr = np.correlate(y1, y2, mode="full")
    cx = np.linspace(-x[-1], x[-1], 2*len(x)-1)
    ndx = np.argmax(corr)
    shift = (int)(cx[ndx]/(abs(x[1]-x[0])))
    new_y2 = y2[-shift:] + y2[:-shift]
    return MeanSquaredError(y1, new_y2)


def CrossCorrelation2(x: list, y1: list, y2: list) -> float:
    # This same as np.correlate
    clen=  len(y1)
    corr = np.zeros(2*len(y1) - 1)
    cindx = 0
    for i in range(clen-1,-clen,-1):
        dotp = 0
        for j in range(clen):
            s2 = (i+j)
            if s2 < clen and s2 >= 0: 
                dotp += y1[j]*y2[s2]
        corr[cindx] = dotp
        cindx += 1
    # Now do the rest
    cx = np.linspace(-x[-1], x[-1], 2*len(x)-1)
    ndx = np.argmax(corr)
    shift = (int)(cx[ndx]/(abs(x[1]-x[0])))
    new_y2 = y2[-shift:] + y2[:-shift]
    return MeanSquaredError(y1, new_y2)


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
