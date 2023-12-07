#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

#define mat(A, x, y, Y) A[x * Y + y]

void mulMat(int X, int Y, int Z, vector<int> & A, vector<int> & B, vector<int> & C) { 
    for (int i = 0; i < X; i++) {
        for (int k = 0; k < Z; k++) {
            mat(C, i, k, Z) = 0;
            int j = 0;
            int tmp = k;

            [[ LOOP Y ]]

            [[ ADDR tmp ]]
            tmp += Z;

            [[ PREFETCH B tmp ]]

            [[ OTHER ]]
            mat(C, i, k, Z) += mat(A, i, j, Y) * B[tmp];
            j = j + 1;

            [[ ENDLOOP ]]
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
    vector<int> A(X * Y);
    vector<int> B(Y * Z);
    vector<int> C(X * Z);
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