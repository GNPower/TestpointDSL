import json
import numpy as np
import matplotlib.pyplot as plt
from alive_progress import alive_bar

from signals.constant import Constant
from signals.sinewave import SineWave
from signals.pwmwave import PWMWave
from signals.sawtoothwave import SawtoothWave
from signals.trianglewave import TriangleWave
from signals.trapezoidwave import TrapezoidWave
from signals.exponentialpulse import ExponentialPulse, DoubleExponentialPulse, InvertingExponentialPulse, SpikePulse

from similarity.errors import MeanSquaredError, RootMeanSquaredError, MeanAbsoluteError
from similarity.distances import FrechetDist, EuclidianDistance, DynamicTimeWarping
from similarity.algos import CrossCorrelation, Pipe, PointToPoint, RectangularTolerances

from interactive_plot import interactive_plot


def CompareConstant():
    func = Constant(1)
    
    exact = func.getf_Exact()
    # spike_height = np.linspace(1, 5, 20).tolist()
    spike_height = 3
    std_list = np.linspace(0.05, 2, 100).tolist()

    custom_std_list = [[], []]
    # spike1 = []
    # spike10 = []
    custom_std_names = ["spike1", "spike10"]
    for i in range(len(std_list)):
        custom_std_list[0].append(func.getf_SpikeNoise(relative_height=spike_height, duration=1, std=std_list[i]))
        custom_std_list[1].append(func.getf_SpikeNoise(relative_height=spike_height, duration=10, std=std_list[i]))

    # Get generic results
    # results = CompareGeneric(func)
    results = CompareGeneric(func, std_list, std_func_lists = custom_std_list, std_func_names = custom_std_names)
    results["spike_height"] = spike_height

    # Write results
    SaveGeneric("ConstantResults", results)


def PlotConstantComparison(filename: str):
    data = LoadGeneric(filename)
    std_list, y_std_list, y_std_label, xy_std_list, xy_std_label = PlotGeneric(data)

    fig, ax, leg = interactive_plot(std_list, y_std_list, y_std_label)
    plt.title("Normalized Comparison Error for Y Noisy Constants")
    plt.show()

    fig1, ax1, leg1 = interactive_plot(std_list, xy_std_list, xy_std_label)
    plt.title("Normalized Comparison Error for X&Y Noisy Constants")
    plt.show()

    spike_list = [
        NormalizeGeneric(data["mse_spike1"]),
        NormalizeGeneric(data["rmse_spike1"]),
        NormalizeGeneric(data["mae_spike1"]),
        NormalizeGeneric(data["frec_spike1"]),
        NormalizeGeneric(data["euclid_spike1"]),
        NormalizeGeneric(data["dtw_spike1"]),
        NormalizeGeneric(data["crocs_spike1"]),
        NormalizeGeneric(data["pipe_spike1"]),
        NormalizeGeneric(data["ptp_spike1"]),
        NormalizeGeneric(data["rtol_spike1"]),

        NormalizeGeneric(data["mse_spike10"]),
        NormalizeGeneric(data["rmse_spike10"]),
        NormalizeGeneric(data["mae_spike10"]),
        NormalizeGeneric(data["frec_spike10"]),
        NormalizeGeneric(data["euclid_spike10"]),
        NormalizeGeneric(data["dtw_spike10"]),
        NormalizeGeneric(data["crocs_spike10"]),
        NormalizeGeneric(data["pipe_spike10"]),
        NormalizeGeneric(data["ptp_spike10"]),
        NormalizeGeneric(data["rtol_spike10"]),
    ]

    spike_name_list = [
        "mse_spike1",
        "rmse_spike1",
        "mae_spike1",
        "frec_spike1",
        "euclid_spike1",
        "dtw_spike1",
        "crocs_spike1",
        "pipe_spike1",
        "ptp_spike1",
        "rtol_spike1",

        "mse_spike10",
        "rmse_spike10",
        "mae_spike10",
        "frec_spike10",
        "euclid_spike10",
        "dtw_spike10",
        "crocs_spike10",
        "pipe_spike10",
        "ptp_spike10",
        "rtol_spike10",
    ]

    fig2, ax2, leg2 = interactive_plot(std_list, spike_list, spike_name_list)
    plt.title("Normalized Comparison Error for Spiked Constants")
    plt.show()



def CompareSineWave():
    func = SineWave()
    CompareWaveGeneric(func, "SineWave", num_xaxis=100, shift_std=0.1)


def PlotSineWaveComparison(filename: str):
    PlotComparisonGeneric(filename, "Sine Wave")


def ComparePWMWave():
    func = PWMWave()
    CompareWaveGeneric(func, "PWMWave", num_xaxis=100, shift_std=0.1)


def PlotPWMWaveComparison(filename: str):
    PlotComparisonGeneric(filename, "PWM Wave")


def CompareSawtoothWave():
    func = SawtoothWave()
    CompareWaveGeneric(func, "SawtoothWave", num_xaxis=100, shift_std=0.1)


def PlotSawtoothWaveComparison(filename: str):
    PlotComparisonGeneric(filename, "Sawtooth Wave")


def CompareTriangleWave():
    func = TriangleWave()
    CompareWaveGeneric(func, "TriangleWave", num_xaxis=100, shift_std=0.1)


def PlotTriangleWaveComparison(filename: str):
    PlotComparisonGeneric(filename, "Triangle Wave")


def CompareTrapezoidWave():
    func = TrapezoidWave()
    CompareWaveGeneric(func, "TrapezoidWave", num_xaxis=100, shift_std=0.1)


def PlotTrapezoidWaveComparison(filename: str):
    PlotComparisonGeneric(filename, "Trapezoid Wave")


def CompareExponentialPulse():
    func = ExponentialPulse()
    # TODO: Fix this
    CompareWaveGeneric(func, "ExponentialPulse", num_xaxis=100, shift_std=0.1)


def PlotExponentialPulseComparison(filename: str):
    PlotComparisonGeneric(filename, "Exponential Pulse")


# def CompareDoubleExponentialPulse():
#     func = DoubleExponentialPulse()

#     # Get generic results
#     results = CompareGeneric(func)
#     # TODO: Add custom error functions

#     # Write results
#     SaveGeneric("DoubleExponentialPulseResults", results)


# def CompareInvertingExponentialPulse():
#     func = InvertingExponentialPulse()

#     # Get generic results
#     results = CompareGeneric(func)
#     # TODO: Add custom error functions

#     # Write results
#     SaveGeneric("InvertingExponentialPulseResults", results)


# def CompareSpikePulse():
#     func = SpikePulse()

#     # Get generic results
#     results = CompareGeneric(func)
#     # TODO: Add custom error functions

#     # Write results
#     SaveGeneric("SpikePulseResults", results)








def GenShiftGeneric(func, std: float, num_xaxis: int):
    shift_list = np.linspace(1, 100, num_xaxis).tolist()

    custom_shift_list = [[], [], [], []]
    custom_shift_names = ["early", "early_noise", "delayed", "delayed_noise"]
    for i in range(len(shift_list)):
        custom_shift_list[0].append(func.getf_Early(i))
        custom_shift_list[1].append(func.getf_EarlyNoise(i, std))
        custom_shift_list[2].append(func.getf_Delayed(i))
        custom_shift_list[3].append(func.getf_DelayedNoise(i, std))
    
    return (shift_list, custom_shift_list, custom_shift_names)

def CompareWaveGeneric(func, funcname: str, num_xaxis: int = 100, shift_std: float = 0.1):
    std_list = np.linspace(0.05, 2, num_xaxis).tolist()
    
    shift_list, custom_shift_list, custom_shift_names = GenShiftGeneric(func, shift_std, num_xaxis)

    # Get generic results
    results = CompareGeneric(func, std_list, std_func_lists = custom_shift_list, std_func_names = custom_shift_names)
    # TODO: Add custom error functions
    results["shift_std"] = shift_std
    results["shift_list"] = shift_list

    # Write results
    SaveGeneric(f"{funcname}Results", results)

def CompareGeneric(func, std_list, std_func_lists = [], std_func_names = []):
    # Get the x values, exact y values
    x = func.get_x().tolist()
    exact = func.getf_Exact()
    # a range of std to generate outputs for

    # Generate a list of functions with only Y noise and XY noise
    gausy_list = []
    gausxy_list = []
    for i in std_list:
        gausy_list.append(func.getf_GaussianNoise_y(std = i))
        gausxy_list.append(func.getf_GaussianNoise_xy(std = i))
    print(len(gausy_list))
    print(len(gausy_list[0]))

    # List for comparison outputs at all std's
    mse_y_std = []
    rmse_y_std = []
    mae_y_std = []
    frec_y_std = []
    euclid_y_std = []
    dtw_y_std = []
    crocs_y_std = []
    pipe_y_std = []
    ptp_y_std = []
    rtol_y_std = []
    
    mse_xy_std = []
    rmse_xy_std = []
    mae_xy_std = []
    frec_xy_std = []
    euclid_xy_std = []
    dtw_xy_std = []
    crocs_xy_std = []
    pipe_xy_std = []
    ptp_xy_std = []
    rtol_xy_std = []

    # Calaulate comparison outputs for all std's
    print(f"Std Range: {len(std_list)}")
    custom_stds = [
        {
            f"mse_{std_func_names[i]}": [],
            f"rmse_{std_func_names[i]}": [],
            f"mae_{std_func_names[i]}": [],
            f"frec_{std_func_names[i]}": [],
            f"euclid_{std_func_names[i]}": [],
            f"dtw_{std_func_names[i]}": [],
            f"crocs_{std_func_names[i]}": [],
            f"pipe_{std_func_names[i]}": [],
            f"ptp_{std_func_names[i]}": [],
            f"rtol_{std_func_names[i]}": [],
        }
        for i in range(len(std_func_lists))
    ]
    num_bars = ( (10 * len(std_func_lists)) + 20) * len(std_list)
    with alive_bar(num_bars) as bar:
        for i in range(len(std_list)):
            print(i)
            # Y Std
            mse_y_std.append(MeanSquaredError(exact, gausy_list[i]))
            bar()
            rmse_y_std.append(RootMeanSquaredError(exact, gausy_list[i]))
            bar()
            mae_y_std.append(MeanAbsoluteError(exact, gausy_list[i]))
            bar()
            frec_y_std.append(FrechetDist(x, exact, gausy_list[i]))
            bar()
            euclid_y_std.append(EuclidianDistance(x, exact, gausy_list[i]))
            bar()
            dtw_y_std.append(DynamicTimeWarping(exact, gausy_list[i]))
            bar()
            crocs_y_std.append(CrossCorrelation(x, exact, gausy_list[i]))
            bar()
            pipe_y_std.append(Pipe(exact, gausy_list[i], 0.5, False))
            bar()
            ptp_y_std.append(PointToPoint(exact, gausy_list[i], 10, 0.5))
            bar()
            rtol_y_std.append(RectangularTolerances(exact, gausy_list[i], 5, 5, 0.5, 0.5))
            bar()

            # XY Std
            mse_xy_std.append(MeanSquaredError(exact, gausxy_list[i]))
            bar()
            rmse_xy_std.append(RootMeanSquaredError(exact, gausxy_list[i]))
            bar()
            mae_xy_std.append(MeanAbsoluteError(exact, gausxy_list[i]))
            bar()
            frec_xy_std.append(FrechetDist(x, exact, gausxy_list[i]))
            bar()
            euclid_xy_std.append(EuclidianDistance(x, exact, gausxy_list[i]))
            bar()
            dtw_xy_std.append(DynamicTimeWarping(exact, gausxy_list[i]))
            bar()
            crocs_xy_std.append(CrossCorrelation(x, exact, gausxy_list[i]))
            bar()
            pipe_xy_std.append(Pipe(exact, gausxy_list[i], 0.5, False))
            bar()
            ptp_xy_std.append(PointToPoint(exact, gausxy_list[i], 10, 0.5))
            bar()
            rtol_xy_std.append(RectangularTolerances(exact, gausxy_list[i], 5, 5, 0.5, 0.5))
            bar()

            # Custom Std
            for j in range(len(std_func_lists)):
                custom_stds[j][f"mse_{std_func_names[j]}"].append(MeanSquaredError(exact, std_func_lists[j][i]))
                bar()
                custom_stds[j][f"rmse_{std_func_names[j]}"].append(RootMeanSquaredError(exact, std_func_lists[j][i]))
                bar()
                custom_stds[j][f"mae_{std_func_names[j]}"].append(MeanAbsoluteError(exact, std_func_lists[j][i]))
                bar()
                custom_stds[j][f"frec_{std_func_names[j]}"].append(FrechetDist(x, exact, std_func_lists[j][i]))
                bar()
                custom_stds[j][f"euclid_{std_func_names[j]}"].append(EuclidianDistance(x, exact, std_func_lists[j][i]))
                bar()
                custom_stds[j][f"dtw_{std_func_names[j]}"].append(DynamicTimeWarping(exact, std_func_lists[j][i]))
                bar()
                custom_stds[j][f"crocs_{std_func_names[j]}"].append(CrossCorrelation(x, exact, std_func_lists[j][i]))
                bar()
                custom_stds[j][f"pipe_{std_func_names[j]}"].append(Pipe(exact, std_func_lists[j][i], 0.5, False))
                bar()
                custom_stds[j][f"ptp_{std_func_names[j]}"].append(PointToPoint(exact, std_func_lists[j][i], 10, 0.5))
                bar()
                custom_stds[j][f"rtol_{std_func_names[j]}"].append(RectangularTolerances(exact, std_func_lists[j][i], 5, 5, 0.5, 0.5))
                bar()

            

    results = {
        "x": x,
        "exact": exact,
        "std_list": std_list,
        "gausy_list": gausy_list,
        "gausxy_list": gausxy_list,

        "mse_y_std": mse_y_std,
        "rmse_y_std": rmse_y_std,
        "mae_y_std": mae_y_std,
        "frec_y_std": frec_y_std,
        "euclid_y_std": euclid_y_std,
        "dtw_y_std": dtw_y_std,
        "crocs_y_std": crocs_y_std,
        "pipe_y_std": pipe_y_std,
        "ptp_y_std": ptp_y_std,
        "rtol_y_std": rtol_y_std,

        "mse_xy_std": mse_xy_std,
        "rmse_xy_std": rmse_xy_std,
        "mae_xy_std": mae_xy_std,
        "frec_xy_std": frec_xy_std,
        "euclid_xy_std": euclid_xy_std,
        "dtw_xy_std": dtw_xy_std,
        "crocs_xy_std": crocs_xy_std,
        "pipe_xy_std": pipe_xy_std,
        "ptp_xy_std": ptp_xy_std,
        "rtol_xy_std": rtol_xy_std,
    }

    for j in range(len(std_func_lists)):
        results[f"mse_{std_func_names[j]}"] = custom_stds[j][f"mse_{std_func_names[j]}"]
        results[f"rmse_{std_func_names[j]}"] = custom_stds[j][f"rmse_{std_func_names[j]}"]
        results[f"mae_{std_func_names[j]}"] = custom_stds[j][f"mae_{std_func_names[j]}"]
        results[f"frec_{std_func_names[j]}"] = custom_stds[j][f"frec_{std_func_names[j]}"]
        results[f"euclid_{std_func_names[j]}"] = custom_stds[j][f"euclid_{std_func_names[j]}"]
        results[f"dtw_{std_func_names[j]}"] = custom_stds[j][f"dtw_{std_func_names[j]}"]
        results[f"crocs_{std_func_names[j]}"] = custom_stds[j][f"crocs_{std_func_names[j]}"]
        results[f"pipe_{std_func_names[j]}"] = custom_stds[j][f"pipe_{std_func_names[j]}"]
        results[f"ptp_{std_func_names[j]}"] = custom_stds[j][f"ptp_{std_func_names[j]}"]
        results[f"rtol_{std_func_names[j]}"] = custom_stds[j][f"rtol_{std_func_names[j]}"]

    print("Generic Results Computation Completed!")

    return results


def SaveGeneric(filename: str, results: dict):
    print(f"Saving All Results To results/{filename}.json ...")

    jobj = json.dumps(results, indent=4)
    with open(f"results/{filename}.json", "w") as f:
        f.write(jobj)


def LoadGeneric(filename: str) -> dict:
    with open(f"results/{filename}.json", "r") as f:
        data = json.load(f)
        return data
    
def NormalizeGeneric(data: list) -> list:
    mval = max(data)
    if mval == 0:
        mval = 1
    return [ i / mval for i in data]

def NormalizeGenericInverted(data: list) -> list:
    mval = max(data)
    if mval == 0:
        mval = 1
    return [ 1 - (i / mval) for i in data]


def PlotComparisonGeneric(filename: str, funcname: str):
    data = LoadGeneric(filename)

    std_list, y_std_list, y_std_label, xy_std_list, xy_std_label = PlotGeneric(data)

    fig, ax, leg = interactive_plot(std_list, y_std_list, y_std_label)
    plt.title(f"Normalized Comparison Error for Y Noisy {funcname}")
    plt.show()

    fig1, ax1, leg1 = interactive_plot(std_list, xy_std_list, xy_std_label)
    plt.title(f"Normalized Comparison Error for X&Y Noisy {funcname}")
    plt.show()

    shift_list, early_list, early_name, delay_list, delay_name = PlotShiftedGeneric(data)

    fig2, ax2, leg2 = interactive_plot(shift_list, early_list, early_name)
    plt.title(f"Normalized Comparison Error for Early Phase {funcname}")
    plt.show()

    fig3, ax3, leg3 = interactive_plot(shift_list, delay_list, delay_name)
    plt.title(f"Normalized Comparison Error for Delayed Phase {funcname}")
    plt.show()


def PlotGeneric(data: dict):
    std_list = data["std_list"]

    y_std_list = [
        NormalizeGeneric(data["mse_y_std"]),
        NormalizeGeneric(data["rmse_y_std"]),
        NormalizeGeneric(data["mae_y_std"]),
        NormalizeGeneric(data["frec_y_std"]),
        NormalizeGeneric(data["euclid_y_std"]),
        NormalizeGeneric(data["dtw_y_std"]),
        NormalizeGeneric(data["crocs_y_std"]),
        NormalizeGenericInverted(data["pipe_y_std"]),
        NormalizeGenericInverted(data["ptp_y_std"]),
        NormalizeGenericInverted(data["rtol_y_std"]),
    ]

    y_std_label = [
        "mse_y_std",
        "rmse_y_std",
        "mae_y_std",
        "frec_y_std",
        "euclid_y_std",
        "dtw_y_std",
        "crocs_y_std",
        "pipe_y_std",
        "ptp_y_std",
        "rtol_y_std",
    ]

    xy_std_list = [
        NormalizeGeneric(data["mse_xy_std"]),
        NormalizeGeneric(data["rmse_xy_std"]),
        NormalizeGeneric(data["mae_xy_std"]),
        NormalizeGeneric(data["frec_xy_std"]),
        NormalizeGeneric(data["euclid_xy_std"]),
        NormalizeGeneric(data["dtw_xy_std"]),
        NormalizeGeneric(data["crocs_xy_std"]),
        NormalizeGenericInverted(data["pipe_xy_std"]),
        NormalizeGenericInverted(data["ptp_xy_std"]),
        NormalizeGenericInverted(data["rtol_xy_std"]),
    ]

    xy_std_label = [
        "mse_xy_std",
        "rmse_xy_std",
        "mae_xy_std",
        "frec_xy_std",
        "euclid_xy_std",
        "dtw_xy_std",
        "crocs_xy_std",
        "pipe_xy_std",
        "ptp_xy_std",
        "rtol_xy_std",
    ]

    return (std_list, y_std_list, y_std_label, xy_std_list, xy_std_label)


def PlotShiftedGeneric(data: dict):
    shift_list = data["shift_list"]

    early_list = [
        NormalizeGeneric(data["mse_early"]),
        NormalizeGeneric(data["rmse_early"]),
        NormalizeGeneric(data["mae_early"]),
        NormalizeGeneric(data["frec_early"]),
        NormalizeGeneric(data["euclid_early"]),
        NormalizeGeneric(data["dtw_early"]),
        NormalizeGeneric(data["crocs_early"]),
        NormalizeGeneric(data["pipe_early"]),
        NormalizeGeneric(data["ptp_early"]),
        NormalizeGeneric(data["rtol_early"]),

        NormalizeGeneric(data["mse_early_noise"]),
        NormalizeGeneric(data["rmse_early_noise"]),
        NormalizeGeneric(data["mae_early_noise"]),
        NormalizeGeneric(data["frec_early_noise"]),
        NormalizeGeneric(data["euclid_early_noise"]),
        NormalizeGeneric(data["dtw_early_noise"]),
        NormalizeGeneric(data["crocs_early_noise"]),
        NormalizeGeneric(data["pipe_early_noise"]),
        NormalizeGeneric(data["ptp_early_noise"]),
        NormalizeGeneric(data["rtol_early_noise"]),
    ]

    early_name = [
        "mse_early",
        "rmse_early",
        "mase_early",
        "frec_early",
        "euclid_early",
        "dtw_early",
        "crocs_early",
        "pipe_early",
        "ptp_early",
        "rtol_early",

        "mse_early_noise",
        "rmse_early_noise",
        "mae_early_noise",
        "frec_early_noise",
        "euclid_early_noise",
        "dtw_early_noise",
        "crocs_early_noise",
        "pipe_early_noise",
        "ptp_early_noise",
        "rtol_early_noise",
    ]

    delay_list = [
        NormalizeGeneric(data["mse_delayed"]),
        NormalizeGeneric(data["rmse_delayed"]),
        NormalizeGeneric(data["mae_delayed"]),
        NormalizeGeneric(data["frec_delayed"]),
        NormalizeGeneric(data["euclid_delayed"]),
        NormalizeGeneric(data["dtw_delayed"]),
        NormalizeGeneric(data["crocs_delayed"]),
        NormalizeGeneric(data["pipe_delayed"]),
        NormalizeGeneric(data["ptp_delayed"]),
        NormalizeGeneric(data["rtol_delayed"]),

        NormalizeGeneric(data["mse_delayed_noise"]),
        NormalizeGeneric(data["rmse_delayed_noise"]),
        NormalizeGeneric(data["mae_delayed_noise"]),
        NormalizeGeneric(data["frec_delayed_noise"]),
        NormalizeGeneric(data["euclid_delayed_noise"]),
        NormalizeGeneric(data["dtw_delayed_noise"]),
        NormalizeGeneric(data["crocs_delayed_noise"]),
        NormalizeGeneric(data["pipe_delayed_noise"]),
        NormalizeGeneric(data["ptp_delayed_noise"]),
        NormalizeGeneric(data["rtol_delayed_noise"]),
    ]

    delay_name = [
        "mse_delayed",
        "rmse_delayed",
        "mae_delayed",
        "frec_delayed",
        "euclid_delayed",
        "dtw_delayed",
        "crocs_delayed",
        "pipe_delayed",
        "ptp_delayed",
        "rtol_delayed",

        "mse_delayed_noise",
        "rmse_delayed_noise",
        "mae_delayed_noise",
        "frec_delayed_noise",
        "euclid_delayed_noise",
        "dtw_delayed_noise",
        "crocs_delayed_noise",
        "pipe_delayed_noise",
        "ptp_delayed_noise",
        "rtol_delayed_noise",
    ]

    return (shift_list, early_list, early_name, delay_list, delay_name)