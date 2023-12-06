#define QUEUE 10
// #include <mmintrin.h>
// #include <xmmintrin.h>

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
    int i = 1;
    int j = 0;

    int _NEXT_ACCESS = 0;
    int _NEXT_AVAIL = QUEUE - 2;
    int j_QUEUE[QUEUE];
    for (int _IT = 0; _IT < QUEUE - 1; _IT++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        // _mm_prefetch((const char *)&a[j], _MM_HINT_T0);
        j_QUEUE[_IT] = j;
    }

    for (int _IT = 0; _IT < n-1 - QUEUE; _IT++) {
        j = j_QUEUE[_NEXT_AVAIL];
        // bit reversal
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        // _mm_prefetch((const char *)&a[j], _MM_HINT_T0);
        _NEXT_AVAIL = (_NEXT_AVAIL + 1) % QUEUE;
        j_QUEUE[_NEXT_AVAIL] = j;

        j = j_QUEUE[_NEXT_ACCESS];
        _NEXT_ACCESS = (_NEXT_ACCESS + 1) % QUEUE;
        if (i < j) {
            swap(a[i], a[j]);
        }
        i = i + 1;
    }

    for (int _IT = 0; _IT < QUEUE; _IT++) {
        j = j_QUEUE[_NEXT_ACCESS];
        _NEXT_ACCESS = (_NEXT_ACCESS + 1) % QUEUE;
        if (i < j) {
            swap(a[i], a[j]);
        }
        i = i + 1;
    }

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
    cout << "Output = " << endl;
    for (int k = 0; k < c.size(); k++) {
        cout << c[k] << ", ";
    }
    cout << endl;
    return 0;
}