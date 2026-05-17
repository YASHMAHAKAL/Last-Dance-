#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Quicksort Function
void quicksort(vector<int>& arr, int low, int high)
{
    if (low < high)
    {
        int pivot = arr[high];

        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (arr[j] < pivot)
            {
                i++;
                swap(arr[i], arr[j]);
            }
        }

        swap(arr[i + 1], arr[high]);

        int pi = i + 1;

        quicksort(arr, low, pi - 1);

        quicksort(arr, pi + 1, high);
    }
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 100000;

    vector<int> data;

    int chunk_size = N / size;

    vector<int> local_data(chunk_size);

    // Root Process Generates Random Data
    if (rank == 0)
    {
        srand(time(0));

        data.resize(N);

        for (int i = 0; i < N; i++)
        {
            data[i] = rand() % 1000000;
        }
    }

    double start = MPI_Wtime();

    // Scatter Data
    MPI_Scatter(
        data.data(),
        chunk_size,
        MPI_INT,
        local_data.data(),
        chunk_size,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    // Local Sort
    quicksort(local_data, 0, chunk_size - 1);

    // Gather Sorted Chunks
    vector<int> gathered;

    if (rank == 0)
    {
        gathered.resize(N);
    }

    MPI_Gather(
        local_data.data(),
        chunk_size,
        MPI_INT,
        gathered.data(),
        chunk_size,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    // Final Merge at Root
    if (rank == 0)
    {
        sort(gathered.begin(), gathered.end());

        double end = MPI_Wtime();

        cout << "First 20 Sorted Elements:\n";

        for (int i = 0; i < 20; i++)
        {
            cout << gathered[i] << " ";
        }

        cout << "\n\nExecution Time: "
             << end - start
             << " seconds\n";
    }

    MPI_Finalize();

    return 0;
}