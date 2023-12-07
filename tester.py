import os, sys
import converter

INDENT = "   "
REPEAT = 20

if __name__ == "__main__":
    benchmark_name = sys.argv[1]
    # gap is a STRING!
    max_gap = int(sys.argv[2])
    # PROGRAM INPUT, a STRING
    INPUT = int(sys.argv[3])
    
    result_file_name = os.path.join("results", benchmark_name + ".txt")
    tester_file_name = os.path.join("testers", benchmark_name + ".cpp")
    binary_file_name = os.path.join("testers", benchmark_name + ".exe")
    error_file_name = os.path.join("results", "log.txt")

    no_prefetch_file_name = os.path.join("raws", benchmark_name + "_no_prefetch.cpp")
    no_prefetch_binary_name = os.path.join("raws", benchmark_name + ".exe")
    os.system(f"break > {result_file_name}")

    # Test Baseline
    os.system(f"g++ {no_prefetch_file_name} -o {no_prefetch_binary_name} 2> {error_file_name}")
    os.system(f"echo BASELINE >> {result_file_name}")
    for i in range(REPEAT):
        os.system(f"{no_prefetch_binary_name} {INPUT} >> {result_file_name}")

    for gap in range(8, max_gap, 8):
        os.system(f"echo {gap} >> {result_file_name}")
        converter.converter(benchmark_name, str(gap))
        os.system(f"g++ {tester_file_name} -o {binary_file_name} 2> {error_file_name}")
        for i in range(REPEAT):
            os.system(f"{binary_file_name} {INPUT} >> {result_file_name}")