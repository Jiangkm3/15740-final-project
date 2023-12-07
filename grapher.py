import os, sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    benchmark_name = sys.argv[1]
    result_file_name = os.path.join("results", benchmark_name + "_old.txt")
    graph_file_name = os.path.join("results", benchmark_name + ".png")

    f_result = open(result_file_name, 'r')
    nextline = f_result.readline()
    labels = []
    results = []

    # Record Data
    while nextline != "":
        next_segment = nextline.split(' ')
        if next_segment[0] == "Time":
            if len(results) == 0:
                raise ValueError("Incorrect result file format")
            results[-1].append(int(next_segment[3][:-5]))
        else:
            labels.append(next_segment[0])
            results.append([])
        nextline = f_result.readline()

    f_result.close()

    for i in range(len(results)):
        results[i] = sum(results[i]) / len(results[i])
        print(f"i = {i}, time = {results[i]}")
    
    baseline = results[0]
    results = results[1:]

    x = [int(i) for i in labels[1:]]
    plt.plot(x, results)
    plt.savefig(graph_file_name)