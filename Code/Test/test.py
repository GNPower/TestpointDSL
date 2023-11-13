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

from similarityComparison import CompareConstant, PlotConstantComparison, CompareSineWave, PlotSineWaveComparison, ComparePWMWave, PlotPWMWaveComparison, CompareSawtoothWave, PlotSawtoothWaveComparison, CompareTriangleWave, PlotTriangleWaveComparison, CompareTrapezoidWave, PlotTrapezoidWaveComparison

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

# CompareConstant()
# CompareSineWave()
# ComparePWMWave()
# CompareSawtoothWave()
# CompareTriangleWave()
CompareTrapezoidWave()

# PlotConstantComparison("ConstantResults")
# PlotSineWaveComparison("SineWaveResults")
# PlotPWMWaveComparison("PWMWaveResults")
# PlotSawtoothWaveComparison("SawtoothWaveResults")
# PlotTriangleWaveComparison("TriangleWaveResults")
# PlotTrapezoidWaveComparison("TrapezoidWaveResults")






# pulse_list = []
# epulse_list = []
# for i in range(10):
#     e = ExponentialPulse(alpha=(10-i)/10, beta_over_alpha=10.001)
#     epulse_list.append(e.getf_Exact())
#     d = DoubleExponentialPulse(alpha=0.1, beta_over_alpha=10.001, pulse2_delay=20, pulse2_rel_E0=i*0.1)
#     pulse_list.append(d.getf_Exact())

# x = pulse_list[0].get_x()

# fig2, ax2, leg2 = interactive_plot(x, pulse_list, ["ds1", "ds2", "ds3", "ds4", "ds5", "ds6", "ds7", "ds8", "ds9", "ds10"])
# plt.show()

# es_mse = [MeanSquaredError(epulse_list[0], epulse_list[i]) for i in range(10)]
# es_dtw = [DynamicTimeWarping(epulse_list[0], epulse_list[i]) for i in range(10)]

# t2 = range(10)
# ds_mse = [MeanAbsoluteError(pulse_list[0], pulse_list[i]) for i in range(10)]
# ds_dtw = [DynamicTimeWarping(pulse_list[0], pulse_list[i]) for i in range(10)]


# fig3, ax3, leg3 = interactive_plot(t2, [es_mse, es_dtw], ["es_mse", "es_dtw"])
# plt.show()

# fig4, ax4, leg4 = interactive_plot(t2, [ds_mse, ds_dtw], ["ds_mse", "ds_dtw"])
# plt.show()

# fig5, ax5, leg5 = interactive_plot(t2, [es_mse, ds_mse], ["es_mse", "ds_mse"])
# plt.show()

# fig4, ax4, leg4 = interactive_plot(t2, [es_dtw, ds_dtw], ["es_dtw", "ds_dtw"])
# plt.show()
