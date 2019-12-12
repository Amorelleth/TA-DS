#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

int fill_vector_from_binary(int** data, char* binary, long rank, int count_proc, int& data_size) {
    ifstream bin_file(binary, ios::in | ios::binary);
    bin_file.seekg(0, ios::end);

    const long int count_all_bytes = bin_file.tellg();
    data_size                      = (count_all_bytes / sizeof(int) / count_proc);

    if(data_size < 1) {
        cout << "The amount of processes is higher than the amount of values. In this case not every process will become values." << endl;
        return EXIT_FAILURE;
    }
    else if( (count_all_bytes / sizeof(int)) % count_proc != 0  ) {
        cout << "The amount of values have to be even on every process. Otherwise the sorting would be incorrect." << endl;
        return EXIT_FAILURE;
    }

    *data = new int[data_size];

    bin_file.seekg(rank * sizeof(int) * data_size, ios::beg);
    bin_file.read(reinterpret_cast<char *>(*data), sizeof(int) * data_size);

    return EXIT_SUCCESS;
}

void print_array(int *data, int rank, int data_size) {
    cout << "rank " << rank << " : ";
    for(auto i = 0; i < data_size; i++) {
        cout << data[i] << " ";
    }
    cout << endl;
}

int findPartner(int phase, int rank) {
    int partner;

    /* if it's an even phase */
    if (phase % 2 == 0) {
        /* if we are an even process */
        if (rank % 2 == 0) {
            partner = rank + 1;
        } else {
            partner = rank - 1;
        }
    } else {
        /* it's an odd phase - do the opposite */
        if (rank % 2 == 0) {
            partner = rank - 1;
        } else {
            partner = rank + 1;
        }
    }
    return partner;
}

int compare (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void parallel_sort(int* data, int rank, int count_proc, int data_size) {
    auto concat_data_size = data_size * 2;

    auto *other      = new int[data_size];
    auto *concat_data = new int[concat_data_size];

    for (int i = 0; i < count_proc; i++) {
        int partner = findPartner(i, rank);
        if (partner < 0 || partner >= count_proc) continue;

        if (rank % 2 == 0) {
            MPI_Send(data, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
            MPI_Recv(other, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(other, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(data, (int)data_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
        }

        merge(data, data  + data_size, other, other + data_size, concat_data);
        qsort(concat_data, concat_data_size, sizeof(int), compare);

        auto pos_half_concat_data = concat_data + data_size;
        if (rank < partner) {
            copy(concat_data, pos_half_concat_data, data);
        } else {
            copy(pos_half_concat_data, concat_data + concat_data_size, data);
        }
    }
}

int main(int argCount, char** argValues) {
    int rank, count_proc;

    MPI_Init(&argCount, &argValues);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count_proc);

    int *data;
    int data_size;
    int status = fill_vector_from_binary(&data, argValues[1], rank, count_proc, data_size);

    if (status == EXIT_FAILURE) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    parallel_sort(data, rank, count_proc, data_size);

    if(argCount > 2 && strcmp(argValues[2], "y") == 0) {
        print_array(data, rank, data_size);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
