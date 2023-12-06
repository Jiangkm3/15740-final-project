#include <iostream> 
#include <chrono>
#include <vector>
#include <mmintrin.h>
#include <xmmintrin.h>

using namespace std;
#define SIZE 160000
  
void merge(int *array, int const left, int const mid, int const right) { 
    int const subArrayOne = mid - left + 1; 
    int const subArrayTwo = right - mid; 
  
    // Create temp arrays 
    int leftArray[subArrayOne];
    int rightArray[subArrayTwo];
  
    for (int i = 0; i < subArrayOne; i++) 
        leftArray[i] = array[left + i]; 
    for (int j = 0; j < subArrayTwo; j++) 
        rightArray[j] = array[mid + 1 + j]; 
  
    // Initial index of first sub-array 
    // Initial index of second sub-array 
    int indexOfSubArrayOne = 0;
    int indexOfSubArrayTwo = 0;  
  
    // Initial index of merged array 
    int indexOfMergedArray = left;  
  
    // Merge the temp arrays back into  
    // array[left..right] 
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) { 
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) { 
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne]; 
            indexOfSubArrayOne++; 
        } else { 
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo]; 
            indexOfSubArrayTwo++; 
        } 
        indexOfMergedArray++; 
    } 
      
    // Copy the remaining elements of 
    // left[], if there are any 
    while (indexOfSubArrayOne < subArrayOne) { 
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne]; 
        indexOfSubArrayOne++; 
        indexOfMergedArray++; 
    } 
  
    // Copy the remaining elements of 
    // right[], if there are any 
    while (indexOfSubArrayTwo < subArrayTwo) { 
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo]; 
        indexOfSubArrayTwo++; 
        indexOfMergedArray++; 
    } 
} 

void mergeSort(int* array, int const begin, int const end) { 
    // Returns recursively 
    if (begin >= end) 
        return;  
  
    int mid = begin + (end - begin) / 2; 
    mergeSort(array, begin, mid); 
    mergeSort(array, mid + 1, end); 
    merge(array, begin, mid, end); 
} 

// Array is of size `SIZE`
int main(int args, char *argv[]) {
    int k = stoi(argv[1]);
    int a[SIZE];
    for (int i = 0; i < SIZE; i++) {
        a[i] = SIZE - i;
    }

    // Record Runtime
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    mergeSort(a, 0, SIZE - 1);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]" << endl;
    
    cout << "Output = " << a[k] << endl;
    return 0; 
} 
