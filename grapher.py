import os, sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    benchmark_name = sys.argv[1]
    result_file_name = os.path.join("results", benchmark_name + "_prefetch.txt")
    graph_file_name = os.path.join("results", benchmark_name + ".png")

    # Results w/ prefetch
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

    # Results w/ no prefetch
    result_file_name = os.path.join("results", benchmark_name + "_no_prefetch.txt")
    f_result = open(result_file_name, 'r')
    nextline = f_result.readline()
    results_no_prefetch = []
    # Record Data
    while nextline != "":
        next_segment = nextline.split(' ')
        if next_segment[0] == "Time":
            if len(results_no_prefetch) == 0:
                raise ValueError("Incorrect result file format")
            results_no_prefetch[-1].append(int(next_segment[3][:-5]))
        else:
            results_no_prefetch.append([])
        nextline = f_result.readline()
    f_result.close()

    for i in range(len(results)):
        results[i] = sum(results[i]) / len(results[i])
        results_no_prefetch[i] = sum(results_no_prefetch[i]) / len(results_no_prefetch[i])
    
    baseline = results[0]
    results = results[1:]
    results_no_prefetch = results_no_prefetch[1:]
    f = plt.figure() 
    f.set_figwidth(10) 
    f.set_figheight(5)

    x = [int(i) for i in labels[1:]]
    plt.plot(x, results, label = "w/ prefetch")
    plt.plot(x, results_no_prefetch, label = "w/o prefetch")
    plt.hlines(baseline, xmin=0, xmax=x[-1], colors="red", linestyles="dashed", label = "baseline")
    plt.xlabel("Prefetch Distance (number of iterations in advance)")
    plt.ylabel("Program Runtime (ms)") 

    plt.legend(loc='upper left')
    plt.savefig(graph_file_name)