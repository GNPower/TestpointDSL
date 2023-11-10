from math import sqrt


def MeanSquaredError(y1: list, y2: list) -> float:
    sum = 0
    for i in range(len(y1)):
        sum += (y1[i] - y2[i])**2
    return sum / len(y1)


def RootMeanSquaredError(y1: list, y2: list) -> float:
    sum = 0
    for i in range(len(y1)):
        sum += (y1[i] - y2[i])**2
    return sqrt(sum / len(y1))


def MeanAbsoluteError(y1: list, y2: list) -> float:
    sum = 0
    for i in range(len(y1)):
        sum += abs(y1[i] - y2[i])
    return sum / len(y1)
