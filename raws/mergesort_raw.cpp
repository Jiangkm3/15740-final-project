#include <iostream>
#include <chrono>
#include <math.h>
#include <vector>
#define THRESHOLD 256

using namespace std;

/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(vector<int> & arr, int l, int m, int r);

// Utility function to find minimum of two integers
int min(int x, int y) {
    return (x < y) ? x : y;
}

/* Iterative mergesort function to sort arr[0...n-1] */
void mergeSort(vector<int> & arr, int n) {
    int curr_size;
    int left_start;
    int mid;
    int REPEAT = n * ((int) log(n) + 1);
    
    // Only start prefetching after curr_size reaches certain THRESHOLD
    for (curr_size = 1; curr_size < THRESHOLD; curr_size = 2 * curr_size) {
        // Pick starting point of different subarrays of current size
        for (left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
            // Find ending point of left subarray. mid+1 is starting 
            // point of right
            int mid = min(left_start + curr_size - 1, n - 1);

            int right_end = min(left_start + 2 * curr_size - 1, n - 1);

            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
            merge(arr, left_start, mid, right_end);
        }
    }

    [[ LOOP REPEAT ]]

    [[ ADDR left_start curr_size mid ]]
    if (curr_size < n) {
        left_start += 2 * curr_size;
        if (left_start >= n - 1) {
            curr_size = 2 * curr_size;
            left_start = 0;
        }
        mid = min(left_start + curr_size - 1, n - 1) + 1;
    }

    [[ PREFETCH arr left_start ]]
    [[ PREFETCH arr mid ]]

    [[ OTHER ]]
    if (curr_size < n) {
        int right_end = min(left_start + 2 * curr_size - 1, n - 1);
        merge(arr, left_start, mid - 1, right_end);
    }

    [[ ENDLOOP ]]
}

/* Function to merge the two halves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(vector<int> & arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

int main(int args, char *argv[]) {
    int n = stoi(argv[1]);
    vector<int> arr;
    for (int i = 0; i < n; i++) {
        arr.push_back(n - i);
    }

    // Record Runtime
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    mergeSort(arr, n);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[us]" << endl;

    return 0;
}
