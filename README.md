# 15740-final-project

## Tester
Unroll loops in a custom C++ program with annotations for cache prefetch. Automatically identify the optimal number of iterations to unroll.

Test using `python3 converter.py <benchmark_name> <max_unroll> <input>`, where
* `<benchmark_name>` is the name of the benchmark, currently supporting `fft` and `matmult`
* `<max_unroll>` is the maximum number of iterations the tester should unroll
* `<input>` is the input of the benchmark. In both benchmarks we want `<max_unroll> < <input>`