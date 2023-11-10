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
    results = CompareGeneric(func, std_list, custom_std_list, custom_std_names)

    # Write results
    SaveGeneric("ConstantResults", results)


def PlotConstant(filename: str):
    data = LoadGeneric(filename)
    std_list, y_std_list, y_std_label, xy_std_list, xy_std_label = PlotGeneric(data)

    fig, ax, leg = interactive_plot(std_list, y_std_list, y_std_label)
    plt.show()

    fig1, ax1, leg1 = interactive_plot(std_list, xy_std_list, xy_std_label)
    plt.show()




def CompareSineWave():
    func = SineWave()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("SineWaveResults", results)


def ComparePWMWave():
    func = PWMWave()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("PWMWaveResults", results)


def CompareSawtoothWave():
    func = SawtoothWave()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("SawtoothWaveResults", results)


def CompareTriangleWave():
    func = TriangleWave()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("TriangleWaveResults", results)


def CompareTrapezoidWave():
    func = TrapezoidWave()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("TrapezoidWaveResults", results)


def CompareExponentialPulse():
    func = ExponentialPulse()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("ExponentialPulseResults", results)


def CompareDoubleExponentialPulse():
    func = DoubleExponentialPulse()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("DoubleExponentialPulseResults", results)


def CompareInvertingExponentialPulse():
    func = InvertingExponentialPulse()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("InvertingExponentialPulseResults", results)


def CompareSpikePulse():
    func = SpikePulse()

    # Get generic results
    results = CompareGeneric(func)
    # TODO: Add custom error functions

    # Write results
    SaveGeneric("SpikePulseResults", results)




def CompareGeneric(func, std_list, std_func_lists, std_func_names):
    # Get the x values, exact y values
    x = func.get_x().tolist()
    exact = func.getf_Exact()
    # a range of std to generate outputs for
    # std_list = np.linspace(0.05, 2, 100).tolist()

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
            crocs_y_std.append(CrossCorrelation(exact, gausy_list[i]))
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
            crocs_xy_std.append(CrossCorrelation(exact, gausxy_list[i]))
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
                custom_stds[j][f"crocs_{std_func_names[j]}"].append(CrossCorrelation(exact, std_func_lists[j][i]))
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
    
def PlotGeneric(data: dict):
    std_list = data["std_list"]

    y_std_list = [
        [ i / max(data["mse_y_std"]) for i in data["mse_y_std"]],
        [ i / max(data["rmse_y_std"]) for i in data["rmse_y_std"]],
        [ i / max(data["mae_y_std"]) for i in data["mae_y_std"]],
        [ i / max(data["frec_y_std"]) for i in data["frec_y_std"]],
        [ i / max(data["euclid_y_std"]) for i in data["euclid_y_std"]],
        [ i / max(data["dtw_y_std"]) for i in data["dtw_y_std"]],
        [ i / max(data["crocs_y_std"]) for i in data["crocs_y_std"]],
        [ 1 - (i / max(data["pipe_y_std"])) for i in data["pipe_y_std"]],
        [ 1 - (i / max(data["ptp_y_std"])) for i in data["ptp_y_std"]],
        [ 1 - (i / max(data["rtol_y_std"])) for i in data["rtol_y_std"]],
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
        [ i / max(data["mse_xy_std"]) for i in data["mse_xy_std"]],
        [ i / max(data["rmse_xy_std"]) for i in data["rmse_xy_std"]],
        [ i / max(data["mae_xy_std"]) for i in data["mae_xy_std"]],
        [ i / max(data["frec_xy_std"]) for i in data["frec_xy_std"]],
        [ i / max(data["euclid_xy_std"]) for i in data["euclid_xy_std"]],
        [ i / max(data["dtw_xy_std"]) for i in data["dtw_xy_std"]],
        [ i / max(data["crocs_xy_std"]) for i in data["crocs_xy_std"]],
        [ 1 - (i / max(data["pipe_xy_std"])) for i in data["pipe_xy_std"]],
        [ 1 - (i / max(data["ptp_xy_std"])) for i in data["ptp_xy_std"]],
        [ 1 - (i / max(data["rtol_xy_std"])) for i in data["rtol_xy_std"]],
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