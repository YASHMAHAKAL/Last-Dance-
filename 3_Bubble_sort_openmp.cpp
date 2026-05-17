#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>

using namespace std;

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {

    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {

        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort using OpenMP
void parallelBubbleSort(vector<int>& arr) {

    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Print array
void printArray(vector<int>& arr) {

    for (int num : arr) {
        cout << num << " ";
    }

    cout << endl;
}

int main() {

    int n;

    cout << "===== Parallel Bubble Sort using OpenMP =====" << endl;

    cout << "\nEnter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "\nEnter elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Create copies
    vector<int> seqArr = arr;
    vector<int> parArr = arr;

    // Sequential timing
    double startSeq = omp_get_wtime();

    sequentialBubbleSort(seqArr);

    double endSeq = omp_get_wtime();

    double startPar = omp_get_wtime();

    parallelBubbleSort(parArr);

    double endPar = omp_get_wtime();

    // Output
    cout << "\nSequential Bubble Sort Result:\n";
    printArray(seqArr);

    cout << "\nParallel Bubble Sort Result:\n";
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
/*Time Complexity

Bubble Sort Complexity:

O(n
2
)*/
