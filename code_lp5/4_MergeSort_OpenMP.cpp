#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Merge Function
void merge(vector<int>& arr, int left, int mid, int right) {

    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    // Copy data
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Merge temp arrays
    while (i < n1 && j < n2) {

        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }

        k++;
    }

    // Remaining elements
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    sequentialMergeSort(arr, left, mid);
    sequentialMergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right, int depth) {

    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    // Limit parallel recursion depth
    if (depth <= 0) {

        sequentialMergeSort(arr, left, right);
        return;
    }

    // Parallel sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            parallelMergeSort(arr, left, mid, depth - 1);
        }

        #pragma omp section
        {
            parallelMergeSort(arr, mid + 1, right, depth - 1);
        }
    }

    merge(arr, left, mid, right);
}

// Print Array
void printArray(vector<int>& arr) {

    for (int num : arr) {
        cout << num << " ";
    }

    cout << endl;
}

int main() {

    int n;

    cout << "===== Parallel Merge Sort using OpenMP =====" << endl;

    cout << "\nEnter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "\nEnter elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Copies
    vector<int> seqArr = arr;
    vector<int> parArr = arr;

    // Sequential Timing
    double startSeq = omp_get_wtime();

    sequentialMergeSort(seqArr, 0, n - 1);

    double endSeq = omp_get_wtime();

    // Parallel Timing
    double startPar = omp_get_wtime();

    parallelMergeSort(parArr, 0, n - 1, 4);

    double endPar = omp_get_wtime();

    // Results
    cout << "\nSequential Merge Sort Result:\n";
    printArray(seqArr);

    cout << "\nParallel Merge Sort Result:\n";
    printArray(parArr);

    // Performance
    cout << "\n===== Performance Analysis =====" << endl;

    cout << "\nSequential Time: "
         << (endSeq - startSeq)
         << " seconds";

    cout << "\nParallel Time: "
         << (endPar - startPar)
         << " seconds";

    return 0;
}

/*Merge Sort Complexity

Merge Sort Complexity:

O(nlogn)*/
