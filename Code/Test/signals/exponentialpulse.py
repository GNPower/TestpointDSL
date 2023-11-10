import matplotlib.pyplot as plt
from math import exp, log, sqrt, pi

from . import Signal


def PlotVariousExponentialPulse(
        plot_call_func, 
        is_neg: bool = False,
        g_std: float = 1e-7, 
        g_delay: int = 5, 
        g_early: int = 5,
):
    func = ExponentialPulse(is_neg=is_neg)

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




class ExponentialPulse(Signal):

    def __init__(
            self, 
            is_neg: bool = False,
            E0: float = 10,
            alpha: float = 0.3,
            beta_over_alpha: float = 1.001,
            length: int = 100, 
            stride: float = 0.1
        ) -> None:
        super().__init__(length, stride)
        self.is_neg = is_neg
        self.E0 = E0
        self.alpha = alpha
        self.beta_over_alpha = beta_over_alpha
        self.beta = alpha * beta_over_alpha
        self.k = exp(-alpha * ( (log(alpha) - log(self.beta)) / (alpha - self.beta) )) - exp(-self.beta * ((log(alpha) - log(self.beta)) / (alpha - self.beta) ))

    def evaluate(self, x: float):
        val = self.E0*self.k*(exp(-self.alpha*x) - exp(-self.beta*x))
        if self.is_neg:
            return 0 - val
        return val
    




def PlotVariousDoubleExponentialPulse(
        plot_call_func,
        is_neg: bool = False,
        g_std: float = 1e-7,
        g_delay: int = 5, 
        g_early: int = 5,
):
    func = DoubleExponentialPulse(is_neg=is_neg)

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




class DoubleExponentialPulse(Signal):

    def __init__(
            self, 
            is_neg: bool = False,
            E0: float = 10,
            alpha: float = 0.3,
            beta_over_alpha: float = 1.001,
            pulse2_delay = 10,
            pulse2_rel_E0 = 0.5,
            length: int = 100, 
            stride: float = 0.1
        ) -> None:
        super().__init__(length, stride)
        self.is_neg = is_neg
        self.E0 = E0
        self.alpha = alpha
        self.beta_over_alpha = beta_over_alpha
        self.beta = alpha * beta_over_alpha
        self.pulse2_delay = pulse2_delay
        self.pulse2_rel_E0 = pulse2_rel_E0
        self.k = exp(-alpha * ( (log(alpha) - log(self.beta)) / (alpha - self.beta) )) - exp(-self.beta * ((log(alpha) - log(self.beta)) / (alpha - self.beta) ))

    def evaluate(self, x: float):
        val = (
            self.E0*self.k*(exp(-self.alpha*x) - exp(-self.beta*x))
            + (
                ( 1 / (1 + exp(-0.75*(x-self.pulse2_delay))) )
                *self.pulse2_rel_E0*self.E0*self.k*(exp(-self.alpha*(x-self.pulse2_delay)) - exp(-self.beta*(x-self.pulse2_delay)))
            )
        )
        if self.is_neg:
            return 0 - val
        return val
    





def PlotVariousInvertingExponentialPulse(
        plot_call_func,
        is_neg: bool = False,
        g_std: float = 1e-7,
        g_delay: int = 5, 
        g_early: int = 5,
):
    func = InvertingExponentialPulse(is_neg=is_neg)

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




class InvertingExponentialPulse(Signal):

    def __init__(
            self, 
            is_neg: bool = False,
            E0: float = 10,
            alpha: float = 0.3,
            beta_over_alpha: float = 1.001,
            pulse2_delay = 10,
            pulse2_rel_E0 = 0.5,
            length: int = 100, 
            stride: float = 0.1
        ) -> None:
        super().__init__(length, stride)
        self.is_neg = is_neg
        self.E0 = E0
        self.alpha = alpha
        self.beta_over_alpha = beta_over_alpha
        self.beta = alpha * beta_over_alpha
        self.pulse2_delay = pulse2_delay
        self.pulse2_rel_E0 = pulse2_rel_E0
        self.k = exp(-alpha * ( (log(alpha) - log(self.beta)) / (alpha - self.beta) )) - exp(-self.beta * ((log(alpha) - log(self.beta)) / (alpha - self.beta) ))

    def evaluate(self, x: float):
        val = (
            self.E0*self.k*(exp(-self.alpha*x) - exp(-self.beta*x))
            - (
                ( 1 / (1 + exp(-0.75*(x-self.pulse2_delay))) )
                *2*self.pulse2_rel_E0*self.E0*self.k*(exp(-self.alpha*(x-self.pulse2_delay)) - exp(-self.beta*(x-self.pulse2_delay)))
            )
        )
        if self.is_neg:
            return 0 - val
        return val
    




def PlotVariousSpikePulse(
        plot_call_func,
        is_neg: bool = False,
        g_std: float = 5e-2,
        g_delay: int = 5, 
        g_early: int = 5,
):
    func = SpikePulse(is_neg=is_neg)

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


class SpikePulse(Signal):
    
    def __init__(
            self, 
            is_neg: bool = False,
            E0: float = 10,
            alpha: float = 1,
            beta_over_alpha: float = 4,
            pulse2_delay = 10,
            pulse2_rel_E0 = 0.5,
            length: int = 100, 
            stride: float = 0.1
        ) -> None:
        super().__init__(length, stride)
        self.is_neg = is_neg
        self.E0 = E0
        self.alpha = alpha
        self.beta_over_alpha = beta_over_alpha
        self.beta = alpha * beta_over_alpha
        self.pulse_delay = pulse2_delay
        self.k = exp(-alpha * ( (log(alpha) - log(self.beta)) / (alpha - self.beta) )) - exp(-self.beta * ((log(alpha) - log(self.beta)) / (alpha - self.beta) ))

    def evaluate(self, x: float):
        val = self.E0*self.k*(1/sqrt(self.alpha*self.beta*2*pi))*exp(-0.5*((x-self.pulse_delay)**2)/(self.alpha*self.beta))
        if self.is_neg:
            return 0 - val
        return val