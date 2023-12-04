#include <iostream>
#include <chrono>
#include <complex>
#include <vector>

// #include <mmintrin.h>
// #include <xmmintrin.h>

using cd = std::complex<double>;
const double PI = acos(-1);

// Iterative implementation of FFT
void fft(std::vector<cd> & a, bool invert) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++) {
        // bit reversal
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
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

std::vector<int> multiply(std::vector<int> const& a, std::vector<int> const& b) {
    std::vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size()) 
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    fft(fa, true);

    std::vector<int> result(n);
    for (int i = 0; i < n; i++)
        result[i] = round(fa[i].real());
    return result;
}

int main(int args, char *argv[]) {
    int size = std::stoi(argv[1]);
    std::vector<int> a;
    std::vector<int> b;
    for (int i = 0; i < size; i++) {
        a.push_back(i);
        b.push_back(i);
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::vector<int> c = multiply(a, b);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    // for (int i = 0; i < c.size(); i++) {
        // std::cout << c[i] << " ";
    // }
    // std::cout << std::endl;
    std::cout << c.size() << std::endl;
    return 0;
}