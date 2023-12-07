#define _QUEUE 10

// #include <mmintrin.h>
// #include <xmmintrin.h>
#include <iostream>
#include <math.h>

using namespace std;
#define SIZE 170000

/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(int arr[], int l, int m, int r);

// Utility function to find minimum of two integers
int min(int x, int y) {
    return (x < y) ? x : y;
}

/* Iterative mergesort function to sort arr[0...n-1] */
void mergeSort(int arr[], int n) {
    int curr_size = 1;
    int left_start = -2;
    int mid;
    int REPEAT = n * ((int) log(n) + 1);
    
    int mid_QUEUE[_QUEUE];
    int curr_size_QUEUE[_QUEUE];
    int left_start_QUEUE[_QUEUE];
    int _NEXT_AVAIL = _QUEUE - 2;
    int _NEXT_ACCESS = 0;
    for (int _IT = 0; _IT < _QUEUE; _IT++) {
        if (curr_size < n) {
            left_start += 2 * curr_size;
            if (left_start >= n - 1) {
                curr_size = 2 * curr_size;
                left_start = 0;
            }
            mid = min(left_start + curr_size - 1, n - 1) + 1;
        }
        // _mm_prefetch((const char *)&arr[left_start], _MM_HINT_T0);
        // _mm_prefetch((const char *)&arr[mid], _MM_HINT_T0);
        left_start_QUEUE[_IT] = left_start;
        curr_size_QUEUE[_IT] = curr_size;
        mid_QUEUE[_IT] = mid;
    }
    for (int _IT = 0; _IT < REPEAT - _QUEUE; _IT++) {
        left_start = left_start_QUEUE[_NEXT_AVAIL];
        curr_size = curr_size_QUEUE[_NEXT_AVAIL];
        mid = mid_QUEUE[_NEXT_AVAIL];
        if (curr_size < n) {
            left_start += 2 * curr_size;
            if (left_start >= n - 1) {
                curr_size = 2 * curr_size;
                left_start = 0;
            }
            mid = min(left_start + curr_size - 1, n - 1) + 1;
        }
        // _mm_prefetch((const char *)&arr[left_start], _MM_HINT_T0);
        // _mm_prefetch((const char *)&arr[mid], _MM_HINT_T0);
        _NEXT_AVAIL = (_NEXT_AVAIL + 1) % _QUEUE;
        left_start_QUEUE[_NEXT_AVAIL] = left_start;
        left_start = left_start_QUEUE[_NEXT_ACCESS];
        curr_size_QUEUE[_NEXT_AVAIL] = curr_size;
        curr_size = curr_size_QUEUE[_NEXT_ACCESS];
        mid_QUEUE[_NEXT_AVAIL] = mid;
        mid = mid_QUEUE[_NEXT_ACCESS];
        _NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;
        if (curr_size < n) {
            int right_end = min(left_start + 2 * curr_size - 1, n - 1);
            merge(arr, left_start, mid - 1, right_end);
        }
    }
    for (int _IT = 0; _IT < _QUEUE; _IT++) {
        left_start = left_start_QUEUE[_NEXT_ACCESS];
        curr_size = curr_size_QUEUE[_NEXT_ACCESS];
        mid = mid_QUEUE[_NEXT_ACCESS];
        _NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;
        if (curr_size < n) {
            int right_end = min(left_start + 2 * curr_size - 1, n - 1);
            merge(arr, left_start, mid - 1, right_end);
        }
    }
}

/* Function to merge the two halves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(int arr[], int l, int m, int r) {
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

/* Function to print an array */
void printArray(int A[], int size) {
    int i;
    for (i = 0; i < size; i++)
        cout <<" "<< A[i];
    cout <<"\n";
}

int main(int args, char *argv[]) {
    int n = stoi(argv[1]);
    int arr[SIZE];
    for (int i = 0; i < n; i++) {
        arr[i] = SIZE - i;
    }

    cout <<"Given array is \n ";
    printArray(arr, n);

    // Record Runtime
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    mergeSort(arr, n);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "[ms]" << endl;

    cout <<"\nSorted array is \n ";
    printArray(arr, n);
    return 0;
}
