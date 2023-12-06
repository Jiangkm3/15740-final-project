#include <iostream>
#include <chrono>
#include <mmintrin.h>
#include <xmmintrin.h>

using namespace std;
#define SIZE 50000000

// Matrix size is X x X
int main(int args, char *argv[]) {
    int k = stoi(argv[1]);
    int gap = stoi(argv[2]);

    int* A = (int*) malloc(SIZE * sizeof(int));
    int s = 0;

    for (int i = 0; i < SIZE; i++) {
        A[i] = i + 1;
    }

    // Record Runtime
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (int i = SIZE - 1; i != 0; i--) {
        if (i > gap) {
            // _mm_prefetch((const char *)&A[i - gap], _MM_HINT_T0);
            _mm_prefetch((const char *)&A[0], _MM_HINT_T0);
        }
        for (int j = 0; j != 100; j++) {
            s += A[i];
        }
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "[ms]" << endl;

    // Print something out so the compiler does not eliminate everything
    cout << "Output = " << s << endl;
 
    return 0;
}