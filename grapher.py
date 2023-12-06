import os, sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    benchmark_name = sys.argv[1]
    result_file_name = os.path.join("results", benchmark_name + ".txt")

    f_result = open(result_file_name, 'r')
    nextline = f_result.readline()
    results = []

    # Record Data
    while nextline != "":
        next_segment = nextline.split(' ')
        if next_segment[0] == "Time":
            if len(results) == 0:
                raise ValueError("Incorrect result file format")
            results[-1].append(int(next_segment[3][:-5]))
        else:
            results.append([])
        nextline = f_result.readline()

    f_result.close()

    for i in range(len(results)):
        results[i] = sum(results[i]) / len(results[i])
        print(f"i = {i}, time = {results[i]}")
    
    x = [i for i in range(len(results))]
    plt.plot(x, results)
    plt.show()