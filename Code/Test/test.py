import matplotlib.pyplot as plt
import numpy as np
from math import exp, log, pow, sqrt, pi

from signals.constant import PlotVariousConstants
from signals.sinewave import PlotVariousSineWave
from signals.pwmwave import PlotVariousPWMWave
from signals.sawtoothwave import PlotVariousSawtoothWave
from signals.trianglewave import PlotVariousTriangleWave
from signals.trapezoidwave import PlotVariousTrapezoidWave
from signals.exponentialpulse import PlotVariousExponentialPulse, PlotVariousDoubleExponentialPulse, PlotVariousInvertingExponentialPulse, PlotVariousSpikePulse

from interactive_plot import interactive_plot

from similarityComparison import CompareConstant


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
# PlotVariousInvertingExponentialPulse(interactive_plot, is_neg=False)
# PlotVariousInvertingExponentialPulse(interactive_plot, is_neg=True)
# PlotVariousSpikePulse(interactive_plot, is_neg=False)
# PlotVariousSpikePulse(interactive_plot, is_neg=True)

CompareConstant()