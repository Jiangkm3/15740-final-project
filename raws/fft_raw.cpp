#include <iostream>
#include <chrono>
#include <complex>
#include <vector>

using namespace std;

using cd = complex<double>;
const double PI = acos(-1);

// Iterative implementation of FFT
void fft(vector<cd> & a, bool invert) {
    int n = a.size();
    int i = 0;
    int j = 0;

    [[ LOOP n-1 ]]

    [[ ADDR j ]]
    int bit = n >> 1;
    for (; j & bit; bit >>= 1)
        j ^= bit;
    j ^= bit;

    [[ PREFETCH a j ]]

    [[ OTHER ]]
    i = i + 1;
    if (i < j) {
        swap(a[i], a[j]);
    }

    [[ ENDLOOP ]]

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i+j], v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd & x : a)
            x /= n;
    }
}

vector<int> multiply(vector<int> const& a, vector<int> const& b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size()) 
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) {
        fa[i] *= fb[i];
    }
    fft(fa, true);

    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = round(fa[i].real());
    }
    return result;
}

// Polynomial is of size `SIZE`
int main(int args, char *argv[]) {
    int SIZE = stoi(argv[1]);
    vector<int> a(SIZE);
    vector<int> b(SIZE);
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i;
    }

    // Record Runtime
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<int> c = multiply(a, b);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "[ms]" << endl;

    // Print the size of the output so the compiler does not eliminate everything
    cout << "Output = " << c.size() << endl;
    return 0;
}