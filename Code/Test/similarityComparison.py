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


def CompareConstant():
    func = Constant(1)
    
    exact = func.getf_Exact()
    spike_height = np.linspace(1, 5, 20).tolist()

    spike1_list = []
    spikenoise1_list = []
    spike10_list = []
    spikenoise10_list = []
    spike_Std = 0.2
    for i in spike_height:
        spike1_list.append(func.getf_Spike(relative_height=i, duration=1))
        spikenoise1_list.append(func.getf_SpikeNoise(relative_height=i, duration=1, std = spike_Std))
        spike10_list.append(func.getf_Spike(relative_height=i, duration=10))
        spikenoise10_list.append(func.getf_SpikeNoise(relative_height=i, duration=10, std = spike_Std))

    mse_spike1 = []
    mse_spikenoise1 = []
    mse_spike10 = []
    mse_spikenoise10 = []
    print(f"Spike Range: {len(spike_height)}")
    for i in range(len(spike_height)):
        print(i)
        mse_spike1.append(MeanSquaredError(exact, spike1_list[i]))
        mse_spikenoise1.append(MeanSquaredError(exact, spikenoise1_list[i]))
        mse_spike10.append(MeanSquaredError(exact, spike10_list[i]))
        mse_spikenoise10.append(MeanSquaredError(exact, spikenoise10_list[i]))

    # Get generic results
    results = CompareGeneric(func)
    # Add results specific to Constant Values
    results["spike_std"] = spike_Std
    results["mse_spike1"] = mse_spike1
    results["mse_spikenoise1"] = mse_spikenoise1
    results["mse_spike10"] = mse_spike10
    results["mse_spikenoise10"] = mse_spikenoise10

    # Write results
    SaveGeneric("ConstantResults", results)


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




def CompareGeneric(func):
    # Get the x values, exact y values
    x = func.get_x().tolist()
    exact = func.getf_Exact()
    # a range of std to generate outputs for
    std_list = np.linspace(0.05, 2, 100).tolist()

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
    with alive_bar(20*len(std_list)) as bar:
        for i in range(len(std_list)):
            print(i)
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
    print("Generic Results Computation Completed!")

    return results


def SaveGeneric(filename: str, results: dict):
    print(f"Saving All Results To results/{filename}.json ...")
    jobj = json.dumps(results, indent=4)
    with open(f"results/{filename}.json", "w") as f:
        f.write(jobj)