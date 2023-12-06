#define _QUEUE 9

// #include <mmintrin.h>
// #include <xmmintrin.h>
#include <iostream>
#include <chrono>

using namespace std;

#define SIZE 170000
#define mat(A, x, y, Y) A[x * Y + y]

void mulMat(int X, int Y, int Z, int* A, int* B, int* C) { 
    for (int i = 0; i < X; i++) {
        for (int k = 0; k < Z; k++) {
            mat(C, i, k, Z) = 0;
            int j = 0;
            int tmp = k;

int tmp_QUEUE[_QUEUE];
   int _NEXT_AVAIL = _QUEUE - 2;
   int _NEXT_ACCESS = 0;
   for (int _IT = 0; _IT < _QUEUE; _IT++) {
               tmp += Z;
   // _mm_prefetch((const char *)&B[tmp], _MM_HINT_T0);
   tmp_QUEUE[_IT] = tmp;
   }
   for (int _IT = 0; _IT < Y - _QUEUE; _IT++){
   tmp = tmp_QUEUE[_NEXT_AVAIL];
               tmp += Z;
   // _mm_prefetch((const char *)&B[tmp], _MM_HINT_T0);
   _NEXT_AVAIL = (_NEXT_AVAIL + 1) % _QUEUE;
   tmp_QUEUE[_NEXT_AVAIL] = tmp;
   tmp = tmp_QUEUE[_NEXT_ACCESS];
   _NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;
               mat(C, i, k, Z) += mat(A, i, j, Y) * B[tmp];
               j = j + 1;
   }
   for (int _IT = 0; _IT < _QUEUE; _IT++){
               tmp += Z;
   tmp = tmp_QUEUE[_NEXT_ACCESS];
   _NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;
               mat(C, i, k, Z) += mat(A, i, j, Y) * B[tmp];
               j = j + 1;
   }
        }
    }
}

// Matrix size is X x X
int main(int args, char *argv[]) {
    int X = stoi(argv[1]);
    int Y = stoi(argv[1]);
    int Z = stoi(argv[1]);

    // Matrix initialization
    // Store matrices as 1D array to ensure their entries are stored together in the memory
    int A[SIZE];
    int B[SIZE];
    int C[SIZE];
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            mat(A, i, j, Y) = j;
        }
    }
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < Z; j++) {
            mat(B, i, j, Z) = j;
        }
    }

    // Record Runtime
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    mulMat(X, Y, Z, A, B, C);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "[ms]" << endl;

    // Print something out so the compiler does not eliminate everything
    // cout << "Output = " << endl;
    // for (int k = 0; k < X * Z; k++) {
        // cout << C[k] << ", ";
    // }
    // cout << endl;
 
    return 0;
}