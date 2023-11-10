import matplotlib.pyplot as plt

from . import Signal


def PlotVariousSawtoothWave(
        plot_call_func, 
        g_std: float = 0.2, 
        g_delay: int = 5, 
        g_early: int = 5, 
        g_rel_height: float = 2, 
        g_duration: int = 2
):
    func = SawtoothWave()

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
    ]

    fig, ax, leg = plot_call_func(x, y_list, label_list)
    plt.show()




class SawtoothWave(Signal):

    def __init__(
            self, 
            amplitude: float = 1, 
            period: int = 10,
            duty_cycle: float = 0.5,
            length: int = 100, 
            stride: float = 0.1
        ) -> None:
        super().__init__(length, stride)
        self.amplitude = amplitude
        self.period = period
        self.duty_cycle = duty_cycle

    def evaluate(self, x: float):
        return self.amplitude*(x % self.period)/(self.period*self.duty_cycle)*(x % self.period < self.period * self.duty_cycle)