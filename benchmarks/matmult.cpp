#include <iostream>
#include <chrono>
#include <vector>
// #include <mmintrin.h>
// #include <xmmintrin.h>

using namespace std;

#define mat(A, x, y, Y) A[x * Y + y]

vector<int> mulMat(int X, int Y, int Z, vector<int> A, vector<int> B) {
    vector<int> C(X * Z);
 
    for (int i = 0; i < X; i++) {
        for (int k = 0; k < Z; k++) {
            mat(C, i, k, Z) = 0;
            for (int j = 0; j < Y; j++) {
                mat(C, i, k, Z) += mat(A, i, j, Y) * mat(B, j, k, Z);
            }
        }
    }
    return C;
}

// Matrix size is X x Y and Y x Z
int main(int args, char *argv[]) {
    int X = stoi(argv[1]);
    int Y = stoi(argv[2]);
    int Z = stoi(argv[3]);

    // Matrix initialization
    // Store matrices as 1D array to ensure their entries are stored together in the memory
    vector<int> A(X * Y);
    vector<int> B(Y * Z);
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            mat(A, i, j, Y) = i;
        }
    }
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < Z; j++) {
            mat(B, i, j, Z) = i;
        }
    }

    // Record Runtime
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<int> C = mulMat(X, Y, Z, A, B);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]" << endl;

    // Print the size of the output so the compiler does not eliminate everything
    cout << "Output = " << C.size() << endl;
 
    return 0;
}