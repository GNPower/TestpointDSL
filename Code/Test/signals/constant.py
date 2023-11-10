import numpy as np
import matplotlib.pyplot as plt
import random

from . import Signal

def PlotVariousConstants(
        plot_call_func, 
        g_std: float = 0.2, 
        g_delay: int = 5, 
        g_early: int = 5, 
        g_rel_height: float = 2, 
        g_duration: int = 2
):
    func = Constant(1)

    x = func.get_x()
    exact = func.getf_Exact()
    # Common Errored Functions
    gausy = func.getf_GaussianNoise_y(std = g_std)
    gausx = func.getf_GaussianNoise_x(std = g_std)
    gausxy = func.getf_GaussianNoise_xy(std = g_std)
    delay = func.getf_Delayed(delay = g_delay)
    early = func.getf_Early(early = g_early)
    delaynoise = func.getf_DelayedNoise(delay = g_delay, std = g_std)
    earlynoise = func.getf_EarlyNoise(early = g_early, std = g_std)
    # Custom Errored Function
    spike = func.getf_Spike(relative_height = g_rel_height, duration = g_duration)
    spikenoise = func.getf_SpikeNoise(relative_height = g_rel_height, duration = g_duration, std = g_std)

    y_list = [
        exact,
        # Common Errored Functions
        gausy,
        gausx,
        gausxy,
        delay,
        early,
        delaynoise,
        earlynoise,
        # Custom Errored Functions
        spike,
        spikenoise,
    ]

    label_list = [
        "exact",
        # Common Errored Functions
        "Gaus Y",
        "Gaus X",
        "Gaus XY",
        "Delay",
        "Early",
        "Delay Gaus XY",
        "Early Gaus XY",
        # Custom Errored Functions
        "Spike",
        "Spike Gause XY",
    ]

    fig, ax, leg = plot_call_func(x, y_list, label_list)
    plt.show()

    


class Constant(Signal):

    def __init__(self, value: float, length: int = 100, stride: float = 0.1) -> None:
        super().__init__(length, stride)
        self.value = value

    def evaluate(self, x: float):
        return self.value

    def getf_Spike(self, relative_height: float = 2, duration: int = 1):
        data = self.getf_Exact()
        start = random.randint(0, self.get_x_len()-1)
        for i in range(duration):
            data[i+start] = relative_height*data[i+start]
        return data
    
    def getf_SpikeNoise(self, relative_height: float = 2, duration: int = 1, std: float = 1):
        data = self.getf_GaussianNoise_xy(std)
        start = random.randint(0, self.get_x_len()-1-duration)
        for i in range(duration):
            data[i+start] = relative_height*data[i+start]
        return data