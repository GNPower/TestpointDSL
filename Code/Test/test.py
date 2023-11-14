import matplotlib.pyplot as plt
import numpy as np
from math import exp, log, pow, sqrt, pi
from scipy.ndimage import shift

from signals.constant import PlotVariousConstants
from signals.sinewave import PlotVariousSineWave
from signals.pwmwave import PlotVariousPWMWave
from signals.sawtoothwave import PlotVariousSawtoothWave
from signals.trianglewave import PlotVariousTriangleWave
from signals.trapezoidwave import PlotVariousTrapezoidWave
from signals.exponentialpulse import PlotVariousExponentialPulse, PlotVariousDoubleExponentialPulse, PlotVariousInvertingExponentialPulse, PlotVariousSpikePulse

from interactive_plot import interactive_plot

from similarityComparison import CompareConstant, PlotConstantComparison, CompareSineWave, PlotSineWaveComparison, ComparePWMWave, PlotPWMWaveComparison, CompareSawtoothWave, PlotSawtoothWaveComparison, CompareTriangleWave, PlotTriangleWaveComparison, CompareTrapezoidWave, PlotTrapezoidWaveComparison, CompareExponentialPulse, PlotExponentialPulseComparison

from similarity.errors import MeanSquaredError, RootMeanSquaredError, MeanAbsoluteError
from similarity.distances import FrechetDist, EuclidianDistance, DynamicTimeWarping
from similarity.algos import CrossCorrelation, RectangularTolerances, PointToPoint, Pipe
from signals.sinewave import SineWave
from signals.constant import Constant
from signals.pwmwave import PWMWave
from signals.sawtoothwave import SawtoothWave
from signals.trianglewave import TriangleWave
from signals.trapezoidwave import TrapezoidWave
from signals.exponentialpulse import ExponentialPulse, DoubleExponentialPulse, InvertingExponentialPulse, SpikePulse

from similarity.algos import CrossCorrelation
from similarity.distances import DynamicTimeWarping
from similarityComparison import NormalizeGeneric

import json

###################################################
#
# PLOT VARIOUS SIGNALS:
#
# Uncomment one to view an interactive graph
#
###################################################

# PlotVariousConstants(interactive_plot)
# PlotVariousSineWave(interactive_plot)
# PlotVariousPWMWave(interactive_plot)
# PlotVariousSawtoothWave(interactive_plot)
# PlotVariousTriangleWave(interactive_plot)
# PlotVariousTrapezoidWave(interactive_plot)
# PlotVariousExponentialPulse(interactive_plot, is_neg=False)
# PlotVariousExponentialPulse(interactive_plot, is_neg=True)
# PlotVariousDoubleExponentialPulse(interactive_plot, is_neg=False)
# PlotVariousDoubleExponentialPulse(interactive_plot, is_neg=True)
# PlotVariousInvertingExponentialPulse(interactive_plot, is_neg=FalsWe)
# PlotVariousInvertingExponentialPulse(interactive_plot, is_neg=True)
# PlotVariousSpikePulse(interactive_plot, is_neg=False)
# PlotVariousSpikePulse(interactive_plot, is_neg=True)

###################################################
#
# COMPARE VARIOUS SIGNALS:
#
# Uncomment one to generate comparison results for
# a signal. NOTE: these comparisons can take 
# upwards of an hour.
#
###################################################

# CompareConstant()
# CompareSineWave()
# ComparePWMWave()
# CompareSawtoothWave()
# CompareTriangleWave()
# CompareTrapezoidWave()
# CompareExponentialPulse()

###################################################
#
# PLOT VARIOUS COMPARISONS:
#
# Uncomment one to view an interactive graph
#
###################################################

# PlotConstantComparison("ConstantResults")
# PlotSineWaveComparison("SineWaveResults")
# PlotPWMWaveComparison("PWMWaveResults")
# PlotSawtoothWaveComparison("SawtoothWaveResults")
# PlotTriangleWaveComparison("TriangleWaveResults")
# PlotTrapezoidWaveComparison("TrapezoidWaveResults")
# PlotExponentialPulseComparison("ExponentialPulseResults")
