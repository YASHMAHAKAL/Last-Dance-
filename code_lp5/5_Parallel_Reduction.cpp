#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>

using namespace std;

int main()
{
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    // Sequential Operations
    double start_seq = omp_get_wtime();

    int seq_sum = 0;
    int seq_min = INT_MAX;
    int seq_max = INT_MIN;

    for (int i = 0; i < n; i++)
    {
        seq_sum += arr[i];

        if (arr[i] < seq_min)
            seq_min = arr[i];

        if (arr[i] > seq_max)
            seq_max = arr[i];
    }

    double seq_avg = (double)seq_sum / n;

    double end_seq = omp_get_wtime();

    // Parallel Reduction Operations
    int par_sum = 0;
    int par_min = INT_MAX;
    int par_max = INT_MIN;

    double start_par = omp_get_wtime();

    #pragma omp parallel for reduction(+:par_sum) reduction(min:par_min) reduction(max:par_max)
    for (int i = 0; i < n; i++)
    {
        par_sum += arr[i];

        if (arr[i] < par_min)
            par_min = arr[i];

        if (arr[i] > par_max)
            par_max = arr[i];
    }

    double par_avg = (double)par_sum / n;

    double end_par = omp_get_wtime();

    // Results
    cout << "\n----- Sequential Results -----\n";
    cout << "Sum     : " << seq_sum << endl;
    cout << "Minimum : " << seq_min << endl;
    cout << "Maximum : " << seq_max << endl;
    cout << "Average : " << seq_avg << endl;
    cout << "Execution Time : " << (end_seq - start_seq) << " seconds\n";

    cout << "\n----- Parallel Results -----\n";
    cout << "Sum     : " << par_sum << endl;
    cout << "Minimum : " << par_min << endl;
    cout << "Maximum : " << par_max << endl;
    cout << "Average : " << par_avg << endl;
    cout << "Execution Time : " << (end_par - start_par) << " seconds\n";

    return 0;
}