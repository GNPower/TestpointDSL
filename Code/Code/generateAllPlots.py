import matplotlib.pyplot as plt

ACCURACY_OUTPUT_FILE_NAME = "plotGen/accuracy_results"
TIMING_OUTPUT_FILE_NAME = "plotGen/timing_results"


def main():
    # Create the Execution Time Plot
    print("\n\nCreating Execution Time Plot...\n\n")
    clocks_per_sec = 1    
    with open(f"{TIMING_OUTPUT_FILE_NAME}.txt", "r") as f:
        clocks_per_sec = int(f.readline().split(":")[1])
        implementation_names = f.readline().split(",")[1:-1]
        vector_sizes = []
        timing_values = [[] for i in range(len(implementation_names))]
        line = f.readline().split(",")
        while line != ['']:
            vector_sizes.append(line[0])
            for i in range(1,len(line)-1):
                timing_values[i-1].append(float(line[i]))
            line = f.readline().split(",")
            print(line)

    print(clocks_per_sec)
    print(len(implementation_names))
    print(implementation_names)
    print(vector_sizes)
    print(timing_values)

    plt.figure()
    for i in range(len(timing_values)):
        plt.plot(vector_sizes, timing_values[i], label=implementation_names[i])
    plt.legend()
    plt.xlabel("Vector Size")
    plt.ylabel(f"Execution Time (CPU Clocks @{clocks_per_sec} clks/sec)")
    figure = plt.gcf()  # get current figure
    figure.set_size_inches(16, 9) # set figure's size manually to your full screen (32x18)
    plt.savefig(f"{TIMING_OUTPUT_FILE_NAME}.png", dpi=100)


    # Create the Accuracy Plot
    print("\n\nCreating Implementation Accuracy Plot...\n\n")
    with open(f"{ACCURACY_OUTPUT_FILE_NAME}.txt", "r") as f:
        implementation_names = f.readline().split(",")[:-1]
        accuracy_values = [float(i) for i in f.readline().split(",")[:-1]]
    plt.figure()
    plt.bar(implementation_names, accuracy_values)
    plt.ylabel("Error Rate (Percent)")
    plt.xlabel("DTW Implementation")
    figure = plt.gcf()  # get current figure
    figure.set_size_inches(16, 9) # set figure's size manually to your full screen (32x18)
    plt.savefig(f"{ACCURACY_OUTPUT_FILE_NAME}.png", dpi=100)

    print(implementation_names)
    print(accuracy_values)


if __name__ == '__main__':
    main()
